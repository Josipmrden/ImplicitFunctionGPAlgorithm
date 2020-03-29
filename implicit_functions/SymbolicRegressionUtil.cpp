//
// Created by josipmrden on 29. 03. 2020..
//

#include "SymbolicRegressionUtil.h"

Tree::Tree* getTree(IndividualP individual, string customIndividual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    if (!customIndividual.empty())
    {
        //std::string izraz = "<Tree size=\"13\">- 25 + * X X + * Y Y * Z Z</Tree>";
        XMLNode xTree = XMLNode::parseString(customIndividual.c_str());
        tree->read(xTree);
    }

    return tree;
}

double executeTree(Tree::Tree* tree, vector<string> variables, Point point)
{
    for (uint j = 0; j < variables.size(); j++) {
        double value = point.coordinates[j];
        tree->setTerminalValue(variables[j], &value);
    }

    double evaluation;
    tree->execute(&evaluation);

    return evaluation;
}

double executeTreeForMovedPoint(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon)
{
    for (uint j = 0; j < variables.size(); j++) {
        double value = point.coordinates[j];

        if (index == j)
        {
            value += epsylon;
        }

        tree->setTerminalValue(variables[j], &value);
    }

    double evaluation;
    tree->execute(&evaluation);

    return evaluation;
}

double executeTreeForMovedPointFast(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon)
{
    if (index != 0)
    {
        double value = point.coordinates[index - 1];
        tree->setTerminalValue(variables[index - 1], &value);
    }

    double epsylonedValue = point.coordinates[index] + epsylon;
    tree->setTerminalValue(variables[index], &epsylonedValue);

    double evaluation;
    tree->execute(&evaluation);

    return evaluation;
}

vector<string> getAlgorithmVariables(Tree::Tree* tree, StateP state)
{
    vector<string> variables;
    voidP val = tree->getParameterValue(state, "terminalset");
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
            variables.push_back(variableString);
        }
    }

    return variables;
}

bool containsAllVariables(Tree::Tree* tree, vector<string> variables)
{
    string expression = tree->toString();

    for (int i = 0, size = variables.size(); i < size; i++)
    {
        string substringToSearch = " " + variables[i] + " ";
        if (expression.find(substringToSearch) == string::npos)
        {
            return false;
        }
    }
    return true;
}

bool isLowStdevOnRandomValues(Tree::Tree* tree, StateP state, vector<string> variables)
{
    int variablesSize = variables.size();
    vector<double> results;

    for (int i = 0; i < 10; i++)
    {
        double result;
        for (int j = 0; j < variablesSize; j++)
        {
            double randomValue = state->getRandomizer()->getRandomDouble();
            tree->setTerminalValue(variables[j], &randomValue);
        }

        tree->execute(&result);
        results.push_back(result);
    }

    double stdev = getStdev(results);

    return stdev < 10E-6;
}

double getFitnessFromDerivation(double experimentalDerivation, double dv1, double dv2, double punishment)
{
    if (isnan(dv1) || isnan(dv2) || (dv1 == 0.0 && dv2 == 0.0))
    {
        return punishment;
    }

    double solutionDerivation = - dv2/dv1;
    double fitness = log(1 + fabs(experimentalDerivation - solutionDerivation));

    if (fitness > punishment)
    {
        fitness = punishment;
    }

    return fitness;
}