//
// Created by josipmrden on 27. 02. 2020..
//
#include "MultipleTreeEvaluator.h"
#include "../utils/MathFunctions.h"

MultipleTreeEvaluator::MultipleTreeEvaluator(StateP state, string datasetFileName, string functionsOperator)
{
    this->_state = state;
    this->_datasetFileName = datasetFileName;
    this->_functionsOperator = functionsOperator;
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

bool MultipleTreeEvaluator::isLowStdevOnRandomValues(IndividualP individual)
{
    vector<double> results;
    for (int i = 0; i < 10; i++)
    {
        Point p;
        for (int j = 0; j < this->_variables.size(); j++)
        {
            double randomValue = this->_state->getRandomizer()->getRandomDouble();
            p.coordinates.push_back(randomValue);
        }

        double result = getResult(individual, p);
        results.push_back(result);
    }

    double stdev = getStdev(results);

    return stdev < 10E-2;

}

double MultipleTreeEvaluator::getResult(IndividualP individual, Point p)
{
    Tree::Tree* first = (Tree::Tree*) individual->getGenotype(0).get();
    Tree::Tree* second = (Tree::Tree*) individual->getGenotype(1).get();

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

    if (this->_functionsOperator == "+")
    {
        return firstResult + secondResult;
    }
    else if (this->_functionsOperator == "-")
    {
        return firstResult - secondResult;
    }
    else if (this->_functionsOperator == "*")
    {
        return firstResult * secondResult;
    }
    else if (this->_functionsOperator == "/")
    {
        return firstResult / secondResult;
    }
    else return 0;
}


FitnessP MultipleTreeEvaluator::evaluate(IndividualP individual)
{
    FitnessP fitness (new FitnessMin);

    if (!this->_initializedVariables)
    {
        this->initializeVariables(individual);
        this->_initializedVariables = true;
    }

    if (!containsAllVariables(individual))
    {
        int domainSize = this->_points.size();
        fitness->setValue(domainSize * domainSize * domainSize);
        return fitness;
    }

    if (isLowStdevOnRandomValues(individual))
    {
        int domainSize = this->_points.size();
        fitness->setValue(domainSize * domainSize * domainSize);
        return fitness;
    }

    vector<double> results;
    for (int i = 0; i < this->_points.size(); i++)
    {
        Point p = this->_points[i];
        double result = getResult(individual, p);
        results.push_back(result);
    }

    double stdev = getStdev(results);

    fitness->setValue(stdev);
    return fitness;
}