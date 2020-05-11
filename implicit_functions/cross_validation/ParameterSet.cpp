//
// Created by josipmrden on 23. 04. 2020..
//

#include "ParameterSet.h"

ParameterSet::ParameterSet(int popSize, double mutProb, SelectionType selType)
{
    this->populationSize = popSize;
    this->mutationProbablity = mutProb;
    this->selectionType = selType;
}

string ParameterSet::getName()
{
    string selType;
    switch(selectionType)
    {
        case SelectionType::Tournament3:
            selType = "TOUR";
            break;
        case SelectionType::RouletteWheel:
            selType = "RW";
            break;
    }

    return to_string(populationSize) + "_" + to_string(mutationProbablity) + "_" + selType;
}
