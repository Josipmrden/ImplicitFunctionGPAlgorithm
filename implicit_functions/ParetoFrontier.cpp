//
// Created by josipmrden on 16. 01. 2020..
//
#include "ParetoFrontier.h"
#include <fstream>
#include <string>
#include <utility>
#include <ecf/ECF.h>

ParetoFrontier::ParetoFrontier(std::string basicString)
{
    this->_paretoFrontFile = std::move(basicString);
    this->multiplier = pow(10, precision);
}

bool dominates(ParetoSolution one, ParetoSolution two)
{
    if (one.fitness < two.fitness && one.treeSize < two.treeSize)
    {
        return true;
    }

    if (one.fitness < two.fitness && one.treeSize == two.treeSize)
    {
        return true;
    }

    if (one.fitness == two.fitness && one.treeSize < two.treeSize)
    {
        return true;
    }

    if (one.representation == two.representation)
    {
        return true;
    }

    return false;
}

bool worseThan(ParetoSolution one, ParetoSolution two)
{
    if (one.fitness == two.fitness && one.treeSize == two.treeSize && one.representation != two.representation)
    {
        return false;
    }
    if (one.fitness < two.fitness || one.treeSize < two.treeSize)
    {
        return false;
    }

    return true;
}

void ParetoFrontier::updateParetoFront(Tree::Tree* tree, double newTreeFitness, const std::string& representation)
{
    int newTreeSize = tree->size();
    std::vector<ParetoSolution> newParetoFront;

    bool dominatedAny = false;
    bool worseThanAnyone = false;
    bool hasSame = false;

    ParetoSolution potential = createNewSolution(newTreeFitness, newTreeSize, representation);

    for (const auto& solution : this->_paretoFront)
    {
        if (potential.fitness != solution.fitness ||
            potential.treeSize != solution.treeSize ||
            potential.representation != solution.representation)
        {
            if (dominates(potential, solution))
            {
                dominatedAny = true;
                continue;
            }
            else if (worseThan(potential, solution))
            {
                worseThanAnyone = true;
            }
        }
        else
        {
            hasSame = true; //identical
        }
        newParetoFront.push_back(solution);
    }

    if ((dominatedAny || !worseThanAnyone) && !hasSame)
    {
        newParetoFront.push_back(potential);
    }

    this->_paretoFront = newParetoFront;
}

bool compareByLength(ParetoSolution &a, ParetoSolution &b)
{
    return a.treeSize < b.treeSize;
}

void ParetoFrontier::writeParetoFront()
{
    std::sort(this->_paretoFront.begin(), this->_paretoFront.end(), compareByLength);

    std::ofstream paretoFrontFile;
    paretoFrontFile.open (this->_paretoFrontFile);

    for (const auto& solution : this->_paretoFront)
    {
        paretoFrontFile << solution.fitness << " " << solution.treeSize << " " << solution.representation;
    }

    paretoFrontFile.close();
}

ParetoSolution ParetoFrontier::createNewSolution(double fitness, int size, std::string representation)
{
    ParetoSolution solution;
    solution.fitness = round(fitness * this->multiplier) / this->multiplier;
    solution.treeSize = size;
    solution.representation = std::move(representation);

    return solution;
}
