//
// Created by josipmrden on 29. 03. 2020..
//

#include <examples/SymbRegExercise/gep/GEPChromosome.h>
#include "SymbolicRegressionUtil.h"

//"<Tree size=\"13\">- 25 + * X X + * Y Y * Z Z</Tree>"; - how to write a solution

Tree::Tree* getTree(IndividualP individual, string customIndividual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    if (!customIndividual.empty())
    {
        XMLNode xTree = XMLNode::parseString(customIndividual.c_str());
        tree->read(xTree);
    }

    return tree;
}

Tree::Tree* getTreeAtIndex(IndividualP individual, string customIndividual, int index)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype(index).get();
    if (!customIndividual.empty())
    {
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

double calculateResult2Trees(double result1, double result2, string operation)
{
    if (operation == "+")
    {
        return result1 + result2;
    }
    else if (operation == "-")
    {
        return result1 - result2;
    }
    else if (operation == "*")
    {
        return result1 + result2;
    }
    else
    {
        return result1 / result2;
    }
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

        if (isalpha(variableString[0]))
        {
            variables.push_back(variableString);
        }
    }

    return variables;
}

vector<string> getAllVariablesWithinTrees(vector<Tree::Tree*> trees, StateP state)
{
    set<string> finalVariables;

    for (int i = 0; i < trees.size(); i++)
    {
        vector<string> partialVariables = getAlgorithmVariables(trees[i], state);
        for (int j = 0; j < partialVariables.size(); j++)
        {
            finalVariables.insert(partialVariables[j]);
        }
    }

    vector<string> resultingVariables(finalVariables.begin(), finalVariables.end());

    return resultingVariables;
}

vector<vector<string>> getVariablesPerTree(vector<Tree::Tree*> trees, StateP state)
{
    vector<vector<string>> variablesPerTree;
    for (int i = 0; i < trees.size(); i++)
    {
        vector<string> partialVariables = getAlgorithmVariables(trees[i], state);
        variablesPerTree.push_back(partialVariables);
    }

    return variablesPerTree;
}

bool isSubstringInTree(Tree::Tree* tree, string variableSubstring)
{
    string expression = tree->toString();
    if (expression.find(variableSubstring) == string::npos)
    {
        return false;
    }

    return true;
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

bool containsAllVariables(vector<Tree::Tree*> trees, vector<string> variables)
{
    for (int i = 0, size = variables.size(); i < size; i++)
    {
        string variableSubstring = " " + variables[i] + " ";
        bool found = false;

        for (int j = 0; j < trees.size(); j++)
        {
            if (isSubstringInTree(trees[j], variableSubstring))
            {
                found = true;
                break;
            }
        }

        if (!found)
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

vector<Point> generateRandomDataset(StateP state, int sampleSize, int varSize)
{
    vector<Point> points;
    for (int i = 0; i < sampleSize; i++)
    {
        Point p;
        for (int j = 0; j < varSize; j++)
        {
            double randomValue = state->getRandomizer()->getRandomDouble();
            p.coordinates.push_back(randomValue);
        }
        points.push_back(p);
    }

    return points;
}

vector<double> getStdevsFromPoints(Tree::Tree* firstTree, Tree::Tree* secondTree, vector<Point> points, vector<vector<string>> variables)
{
    vector<double> additionResults;
    vector<double> subtractionResults;
    vector<double> multiplicationResults;
    vector<double> divisionResults;

    for (int i = 0; i < points.size(); i++)
    {
        Point p = points[i];

        double firstExecution = executeTree(firstTree, variables[0], p);
        double secondExecution = executeTree(secondTree, variables[1], p);

        double result1 = calculateResult2Trees(firstExecution, secondExecution, "+");
        double result2 = calculateResult2Trees(firstExecution, secondExecution, "-");
        double result3 = calculateResult2Trees(firstExecution, secondExecution, "*");
        double result4 = calculateResult2Trees(firstExecution, secondExecution, "/");

        additionResults.push_back(result1);
        subtractionResults.push_back(result2);
        multiplicationResults.push_back(result3);
        divisionResults.push_back(result4);
    }

    double stdev1 = getStdev(additionResults);
    double stdev2 = getStdev(subtractionResults);
    double stdev3 = getStdev(multiplicationResults);
    double stdev4 = getStdev(divisionResults);

    return { stdev1, stdev2, stdev3, stdev4 };
}

double getFitnessFromDerivation(double experimentalDerivation, double dv1, double dv2, double punishment)
{
    if (isnan(dv2) || isnan(dv1) || dv1 == 0.0)
    {
        return punishment;
    }

    double solutionDerivation = - dv2/dv1;

    if (isnan(solutionDerivation))
    {
        return punishment;
    }

    double fitness = log(1 + fabs(experimentalDerivation - solutionDerivation));

    if (fitness > punishment)
    {
        return punishment;
    }

    return fitness;
}