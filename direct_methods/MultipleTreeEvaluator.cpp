//
// Created by josipmrden on 27. 02. 2020..
//
#include "MultipleTreeEvaluator.h"

MultipleTreeEvaluator::MultipleTreeEvaluator() : AbstractEvaluateOp()
{
    this->noTrees = 2;
    this->requiresPlanes = false;
}
MultipleTreeEvaluator::MultipleTreeEvaluator(StateP state, string datasetFileName, ParetoFrontier* paretoFrontier)
: AbstractEvaluateOp(datasetFileName, paretoFrontier)
{
    this->noTrees = 2;
    this->_state = state;
    this->requiresPlanes = false;
}

bool MultipleTreeEvaluator::initialize(StateP state)
{
    this->_state = state;
    this->_initializedVariables = false;

    ifstream inputFileStream(this->_datasetFileName);
    int sampleSize;
    int varCount;
    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    for(int i = 0; i < sampleSize; i++) {
        string line;
        getline(inputFileStream, line);
        istringstream lineStream(line);

        std::vector<double> sampleVariables;
        for (int j = 0; j < varCount; j++)
        {
            string variableString;
            getline(lineStream, variableString, ' ');
            double variableValue = stod(variableString);

            sampleVariables.push_back(variableValue);
        }

        Point pt;
        pt.coordinates = sampleVariables;

        _points.push_back(pt);
    }
}

AbstractEvaluateOp *MultipleTreeEvaluator::createNew() {
    return new MultipleTreeEvaluator();
}

bool MultipleTreeEvaluator::containsAllVariables(IndividualP individual)
{
    Tree::Tree* first = (Tree::Tree*) individual->getGenotype(0).get();
    Tree::Tree* second = (Tree::Tree*) individual->getGenotype(1).get();

    string firstExpression = first->toString();
    string secondExpression = second->toString();

    for (int i = 0; i < this->_variables.size(); i++)
    {
        string substringToSearch = " " + this->_variables[i] + " ";

        if (firstExpression.find(substringToSearch) == string::npos &&
            secondExpression.find(substringToSearch) == string::npos)
        {
            return false;
        }
    }

    return true;
}

void MultipleTreeEvaluator::initializeVariables(IndividualP individual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    voidP val = tree->getParameterValue(this->_state, "terminalset");
    std::string terminals = *((std::string*) val.get());

    istringstream lineStream(terminals);

    while (true)
    {
        string variableString;
        getline(lineStream, variableString, ' ');

        if (variableString.empty())
        {
            break;
        }

        if (variableString.size() == 1 && isalpha(variableString[0]))
        {
            this->_variables.push_back(variableString);
        }
    }
}

bool MultipleTreeEvaluator::isLowStdevOnRandomValues(IndividualP individual, string op)
{
    vector<Point> randomPoints;
    for (int i = 0; i < 10; i++)
    {
        Point p;
        for (int j = 0; j < this->_variables.size(); j++)
        {
            double randomValue = this->_state->getRandomizer()->getRandomDouble();
            p.coordinates.push_back(randomValue);
        }

        randomPoints.push_back(p);
    }


    double stdev = getFitnessFromPoints(individual, randomPoints, op);

    return stdev < 10E-2;

}

double MultipleTreeEvaluator::getFitnessFromPoints(IndividualP individual, vector<Point> points)
{
    Tree::Tree* first = getTreeAtIndex(individual, "", 0);
    Tree::Tree* second = getTreeAtIndex(individual, "", 1);

    map<string, vector<double>> results;
    for (string op : _operations)
    {
        vector<double> emptyResults;
        results[op] = emptyResults;
    }

    for (int i = 0; i < points.size(); i++)
    {
        Point p = points[i];

        for (int j = 0; j < this->_variables.size(); j++)
        {
            double value = p.coordinates[j];
            first->setTerminalValue(this->_variables[j], &value);
            second->setTerminalValue(this->_variables[j], &value);
        }
        double firstResult;
        double secondResult;


        first->execute(&firstResult);
        second->execute(&secondResult);

        for (string op : _operations)
        {
            if (op == "+")
            {
                results[op].push_back(firstResult + secondResult);
            }
            else if (op == "-")
            {
                results[op].push_back(firstResult - secondResult);
            }
            else if (op == "*")
            {
                results[op].push_back(firstResult * secondResult);
            }
            else if (op == "/")
            {
                results[op].push_back(firstResult / secondResult);
            }
        }
    }

    double stdev = getStdev(results[_operations[0]]);

    for (string op : _operations)
    {
        double potentialStdev = getStdev(results[op]);
        if (potentialStdev < stdev)
        {
            stdev = potentialStdev;
        }
    }

    return stdev;
}

double MultipleTreeEvaluator::getFitnessFromPoints(IndividualP individual, vector<Point> points, string op)
{
    Tree::Tree* first = getTreeAtIndex(individual, "", 0);
    Tree::Tree* second = getTreeAtIndex(individual, "", 1);

    vector<double> results;

    for (int i = 0; i < points.size(); i++)
    {
        Point p = points[i];

        for (int j = 0; j < this->_variables.size(); j++)
        {
            double value = p.coordinates[j];
            first->setTerminalValue(this->_variables[j], &value);
            second->setTerminalValue(this->_variables[j], &value);
        }
        double firstResult;
        double secondResult;

        first->execute(&firstResult);
        second->execute(&secondResult);

        if (op == "+")
        {
            results.push_back(firstResult + secondResult);
        }
        else if (op == "-")
        {
            results.push_back(firstResult - secondResult);
        }
        else if (op == "*")
        {
            results.push_back(firstResult * secondResult);
        }
        else if (op == "/")
        {
            results.push_back(firstResult / secondResult);
        }
    }

    double stdev = getStdev(results);

    return stdev;
}

FitnessP MultipleTreeEvaluator::evaluate(IndividualP individual)
{
    this->_operations = {"+", "-" ,"*", "/"};
    Tree::Tree* first = getTreeAtIndex(individual, "", 0);
    Tree::Tree* second = getTreeAtIndex(individual, "", 1);
    vector<Tree::Tree*> allTrees = { first, second };
    FitnessP fitness (new FitnessMin);

    if (!this->_initializedVariables)
    {
        this->initializeVariables(individual);
        this->_initializedVariables = true;
    }

    double domainSize = this->_points.size();
    double punishment = domainSize * domainSize * domainSize;

    if (!containsAllVariables(individual))
    {
        fitness->setValue(punishment);
        return fitness;
    }

    vector<string> leftoverOperators;
    for (string op : _operations) {
        if (!isLowStdevOnRandomValues(individual, op)) {
            leftoverOperators.push_back(op);
        }
    }

    if (leftoverOperators.empty())
    {
        fitness->setValue(domainSize * domainSize * domainSize);
        return fitness;
    }

    _operations = leftoverOperators;

    double minimalStdev = getFitnessFromPoints(individual, this->_points);
    if (isnan(minimalStdev) || fabs(minimalStdev) > punishment)
    {
        fitness->setValue(punishment);
        return fitness;
    }

    fitness->setValue(minimalStdev);

    _paretoFrontier->updateParetoFront(allTrees, minimalStdev);

    return fitness;
}