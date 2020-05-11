//
// Created by josipmrden on 23. 04. 2020..
//

#include "TweakableParameters.h"

TweakableParameters::TweakableParameters()
{
    fixedSelType = false;
    fixedPopSize = false;
    fixedMutProb = false;
    currentSelIndex = 0;
    currentMutIndex = 0;
    currentPopIndex = 0;
    populationSizes = {50, 100, 200, 500, 1000};
    mutationProbablities = {0.1, 0.2, 0.3, 0.5, 0.7, 0.9};
    selectionTypes = {Tournament3, RouletteWheel};
}
TweakableParameters::TweakableParameters(vector<int> popSizes, vector<double> mutProbs, vector<SelectionType> selectTypes)
{
    fixedSelType = false;
    fixedPopSize = false;
    fixedMutProb = false;
    currentSelIndex = 0;
    currentMutIndex = 0;
    currentPopIndex = 0;
    this->populationSizes = popSizes;
    this->mutationProbablities = mutProbs;
    this->selectionTypes = selectTypes;
}

bool TweakableParameters::hasNext(TweakableType type)
{

    switch(type)
    {
        case Population:
        if (fixedPopSize)
        {
            return false;
        }
        if (currentPopIndex > (int)(populationSizes.size() - 1))
        {
            return false;
        }
        return true;
        case Mutation:
            if (fixedMutProb)
            {
                return false;
            }
            if (currentMutIndex > (int)(mutationProbablities.size() - 1))
            {
                return false;
            }
            return true;
        case Selection:
            if (fixedSelType)
            {
                return false;
            }
            if (currentSelIndex > (int)(selectionTypes.size() - 1))
            {
                return false;
            }
            return true;
    }
}
ParameterSet TweakableParameters::getNext(TweakableType type)
{
    int popParam = populationSizes[currentPopIndex];
    double mutProb = mutationProbablities[currentMutIndex];
    SelectionType st = selectionTypes[currentSelIndex];

    switch(type)
    {
        case Population:
            if (!fixedPopSize && currentPopIndex < (int)(populationSizes.size() - 1))
            {
                currentPopIndex++;
            }
            break;
        case Mutation:
            if (!fixedMutProb && currentMutIndex < (int)(mutationProbablities.size() - 1))
            {
                currentMutIndex++;
            }
            break;
        case Selection:
            if (!fixedSelType && currentSelIndex < (int)(selectionTypes.size() - 1))
            {
                currentSelIndex++;
            }
            break;
    }

    ParameterSet ps(popParam, mutProb, st);
    return ps;
}


void TweakableParameters::fixPopSize(int popSize)
{
    int index = -1;
    fixedPopSize = true;

    for (int i = 0; i < populationSizes.size(); i++)
    {
        if (popSize == populationSizes[i])
        {
            index = i;
            break;
        }
    }

    currentPopIndex = index;
}
void TweakableParameters::fixMutProb(int mutProb)
{
    int index = -1;
    fixedMutProb = true;

    for (int i = 0; i < mutationProbablities.size(); i++)
    {
        if (mutProb == mutationProbablities[i])
        {
            index = i;
            break;
        }
    }

    currentMutIndex = index;
}
void TweakableParameters::fixSelType(SelectionType selType)
{
    int index = -1;
    fixedSelType = true;

    for (int i = 0; i < selectionTypes.size(); i++)
    {
        if (selType == selectionTypes[i])
        {
            index = i;
            break;
        }
    }

    currentSelIndex = index;
}
