//
// Created by josipmrden on 16. 01. 2020..
//
#include "ParetoFrontier.h"
#include <fstream>
#include <string>
#include <utility>
#include <ecf/ECF.h>

ParetoFrontier::ParetoFrontier(std::string basicString) {
    this->_paretoFrontFile = std::move(basicString);
}

void ParetoFrontier::updateParetoFront(Tree::Tree* tree, double newTreeFitness, const std::string& representation) {
    int newTreeSize = tree->size();
    std::vector<ParetoSolution> newParetoFront;

    bool dominatedAny = false;
    bool worseThanAnyone = false;
    bool hasSame = false;

    for (const auto& solution : this->_paretoFront) {
        if (newTreeFitness != solution.fitness || newTreeSize != solution.treeSize) {
            if (newTreeFitness <= solution.fitness && newTreeSize <= solution.treeSize) {
                dominatedAny = true;
                continue;
            } else if (newTreeFitness >= solution.fitness && newTreeSize >= solution.treeSize) {
                worseThanAnyone = true;
            }
        } else {
            hasSame = true;
        }
        newParetoFront.push_back(solution);
    }

    if ((dominatedAny || !worseThanAnyone) && !hasSame) {
        ParetoSolution solution = createNewSolution(newTreeFitness, newTreeSize, representation);
        newParetoFront.push_back(solution);
    }

    this->_paretoFront = newParetoFront;
}

void ParetoFrontier::writeParetoFront() {
    std::ofstream paretoFrontFile;
    paretoFrontFile.open (this->_paretoFrontFile);

    for (const auto& solution : this->_paretoFront) {
        paretoFrontFile << solution.fitness << " " << solution.treeSize << std::endl;
        paretoFrontFile << solution.representation;
    }

    paretoFrontFile.close();
}

ParetoSolution ParetoFrontier::createNewSolution(double fitness, int size, std::string representation) {
    ParetoSolution solution;
    solution.fitness = fitness;
    solution.treeSize = size;
    solution.representation = std::move(representation);
    return solution;
}
