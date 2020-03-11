//
// Created by josipmrden on 27. 02. 2020..
//
#include "SimpleStdevEvaluator.h"
#include "../utils/MathFunctions.h"

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

bool SimpleStdevEvaluator::containsAllVariables(IndividualP individual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    string expression = tree->toString();

    for (int i = 0; i < this->_variables.size(); i++)
    {
        string substringToSearch = " " + this->_variables[i] + " ";
        if (expression.find(substringToSearch) == string::npos)
        {
            return false;
        }
    }

    return true;
}

void SimpleStdevEvaluator::initializeVariables(IndividualP individual)
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

bool SimpleStdevEvaluator::isLowStdevOnRandomValues(IndividualP individual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();

    vector<double> results;
    for (int i = 0; i < 10; i++)
    {
        double result;
        for (int j = 0; j < this->_variables.size(); j++)
        {
            double randomValue = this->_state->getRandomizer()->getRandomDouble();
            tree->setTerminalValue(this->_variables[j], &randomValue);
        }

        tree->execute(&result);
        results.push_back(result);
    }

    double stdev = getStdev(results);

    return stdev < 10E-6;

}

FitnessP SimpleStdevEvaluator::evaluate(IndividualP individual)
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

    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();

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