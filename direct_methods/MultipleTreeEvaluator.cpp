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
    //"<Tree size=\"3\">* 4 4</Tree>" -> circle
    //"<Tree size=\"7\"> + * Y Y * X X</Tree>" -> circle

    //"<Tree size=\"7\">+ * Y Y * X X</Tree>" -> sphere
    //"<Tree size=\"7\">- * 5 5 * Z Z</Tree>" -> sphere

    //"<Tree size=\"7\">+ * * X X X X</Tree>" -> hyperbola
    //"<Tree size=\"5\">+ * Y Y 1.5</Tree>" -> hyperbola

    //"<Tree size=\"11\">/ * - X 1 - X 1 * 3 3</Tree>" -> ellipse
    //"<Tree size=\"13\">- / * - Y 2 - Y 2 * 4 4 1</Tree>" -> ellipse

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> harmonic oscillator
    //"<Tree size=\"3\">* 3 X</Tree>" -> harmonic oscillator

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> nonlinear harmonic oscillator
    //"<Tree size=\"4\">* 9.8 sin X</Tree>" -> nonlinear harmonic oscillator

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


    double stdev = getFitnessFromPoints(individual, randomPoints);

    return stdev < 10E-2;

}

double MultipleTreeEvaluator::getFitnessFromPoints(IndividualP individual, vector<Point> points)
{
    vector<double> additionResults;
    vector<double> subtractionResults;
    vector<double> multiplicationResults;
    vector<double> divisionResults;

    for (int i = 0; i < points.size(); i++)
    {
        Point p = points[i];
        double result1 = getResult(individual, p, "+");
        double result2 = getResult(individual, p, "-");
        double result3 = getResult(individual, p, "*");
        double result4 = getResult(individual, p, "/");

        additionResults.push_back(result1);
        subtractionResults.push_back(result2);
        multiplicationResults.push_back(result3);
        divisionResults.push_back(result4);
    }

    double stdev1 = getStdev(additionResults);
    double stdev2 = getStdev(subtractionResults);
    double stdev3 = getStdev(multiplicationResults);
    double stdev4 = getStdev(divisionResults);

    double minimalStdev = stdev1;
    if (stdev2 < minimalStdev)
    {
        minimalStdev = stdev2;
    }
    if (stdev3 < minimalStdev)
    {
        minimalStdev = stdev3;
    }
    if (stdev4 < minimalStdev)
    {
        minimalStdev = stdev4;
    }

    return minimalStdev;
}


double MultipleTreeEvaluator::getResult(IndividualP individual, Point p, string op)
{
    //"<Tree size=\"3\">* 4 4</Tree>" -> circle
    //"<Tree size=\"7\"> + * Y Y * X X</Tree>" -> circle

    //"<Tree size=\"7\">+ * Y Y * X X</Tree>" -> sphere
    //"<Tree size=\"7\">- * 5 5 * Z Z</Tree>" -> sphere

    //"<Tree size=\"7\">+ * * X X X X</Tree>" -> hyperbola
    //"<Tree size=\"5\">+ * Y Y 1.5</Tree>" -> hyperbola

    //"<Tree size=\"11\">/ * - X 1 - X 1 * 3 3</Tree>" -> ellipse
    //"<Tree size=\"13\">- / * - Y 2 - Y 2 * 4 4 1</Tree>" -> ellipse

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> harmonic oscillator
    //"<Tree size=\"3\">* 3 X</Tree>" -> harmonic oscillator

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> nonlinear harmonic oscillator
    //"<Tree size=\"4\">* 9.8 sin X</Tree>" -> nonlinear harmonic oscillator
    Tree::Tree* first = getTreeAtIndex(individual, "", 0);
    Tree::Tree* second = getTreeAtIndex(individual, "", 1);

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
        return firstResult + secondResult;
    }
    else if (op == "-")
    {
        return firstResult - secondResult;
    }
    else if (op == "*")
    {
        return firstResult * secondResult;
    }
    else if (op == "/")
    {
        return firstResult / secondResult;
    }
    else return 0;
}
//figure out which is the best

FitnessP MultipleTreeEvaluator::evaluate(IndividualP individual)
{
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

    if (isLowStdevOnRandomValues(individual))
    {
        fitness->setValue(domainSize * domainSize * domainSize);
        return fitness;
    }

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