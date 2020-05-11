//
// Created by josipmrden on 27. 02. 2020..
//
#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>
#include "SimpleStdevEvaluator.h"

SimpleStdevEvaluator::SimpleStdevEvaluator() : AbstractEvaluateOp()
{
    this->noTrees = 1;
    this->requiresPlanes = false;
}
SimpleStdevEvaluator::SimpleStdevEvaluator(StateP state, string datasetFileName, ParetoFrontier* paretoFrontier):
AbstractEvaluateOp(datasetFileName, paretoFrontier)
{
    this->noTrees = 1;
    this->_state = state;
    this->_initializedVariables = false;
    this->requiresPlanes = false;
}

bool SimpleStdevEvaluator::initialize(StateP state)
{
    this->_state = state;


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


AbstractEvaluateOp *SimpleStdevEvaluator::createNew() {
    return new SimpleStdevEvaluator();
}

FitnessP SimpleStdevEvaluator::evaluate(IndividualP individual)
{
    //"<Tree size=\"11\">- * 4 4 + * Y Y * X X</Tree>" -> circle
    //"<Tree size=\"15\">- * 5 5 + + * Z Z * Y Y * X X</Tree>" -> sphere
    //"<Tree size=\"13\">- - + X * * X X X * Y Y 1.5</Tree>" -> hyperbola
    //"<Tree size=\"25\">+ / * - X 1 - X 1 * 3 3 - / * - Y 2 - Y 2 * 4 4 1</Tree>" -> ellipse
    //"<Tree size=\"9\">+ - Z * 0.1 Y * 3 X</Tree>" -> harmonic oscillator
    //"<Tree size=\"10\">+ - Z * 0.1 Y * 9.8 sin X</Tree>" -> nonlinear harmonic oscillator
    Tree::Tree* tree = getTree(individual, "");
    FitnessP fitness (new FitnessMin);

    if (!_initializedVariables)
    {
        _variables = ::getAlgorithmVariables(tree, _state);
        _initializedVariables = true;
    }

    double domainSize = _points.size();
    double punishment = domainSize * domainSize * domainSize;

    if (!containsAllVariables(tree, _variables) || isLowStdevOnRandomValues(tree, _state, _variables))
    {
        fitness->setValue(punishment);
        return fitness;
    }

    vector<double> results;
    for (int i = 0; i < this->_points.size(); i++)
    {
        Point p = this->_points[i];
        double result;
        for (int j = 0; j < this->_variables.size(); j++)
        {
            tree->setTerminalValue(this->_variables[j], &p.coordinates[j]);
        }
        tree->execute(&result);

        results.push_back(result);
    }

    double stdev = getStdev(results);

    if (isnan(stdev) || fabs(stdev) > punishment)
    {
        fitness->setValue(punishment);
        return fitness;
    }

    fitness->setValue(stdev);

    _paretoFrontier->updateParetoFront(tree, stdev);

    return fitness;
}