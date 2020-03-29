//
// Created by josipmrden on 27. 02. 2020..
//
#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>
#include "SimpleStdevEvaluator.h"

SimpleStdevEvaluator::SimpleStdevEvaluator(StateP state, string datasetFileName)
{
    this->_state = state;
    this->_datasetFileName = datasetFileName;
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

FitnessP SimpleStdevEvaluator::evaluate(IndividualP individual)
{
    Tree::Tree* tree = getTree(individual, "");
    FitnessP fitness (new FitnessMin);

    if (!_initializedVariables)
    {
        _variables = ::getAlgorithmVariables(tree, _state);
        _initializedVariables = true;
    }

    if (!containsAllVariables(tree, _variables) || isLowStdevOnRandomValues(tree, _state, _variables))
    {
        int domainSize = _points.size();
        fitness->setValue(domainSize * domainSize * domainSize);
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

    fitness->setValue(stdev);
    return fitness;
}