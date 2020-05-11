//
// Created by josipmrden on 23. 04. 2020..
//

#ifndef SYMBREGEXERCISE_TWEAKABLEPARAMETERS_H
#define SYMBREGEXERCISE_TWEAKABLEPARAMETERS_H

#include "ParameterSet.h"
#include <vector>
using namespace std;

class TweakableParameters {
private:
    bool fixedPopSize;
    bool fixedMutProb;
    bool fixedSelType;

    int currentPopIndex;
    int currentMutIndex;
    int currentSelIndex;
public:
    vector<int> populationSizes;
    vector<double> mutationProbablities;
    vector<SelectionType> selectionTypes;

    TweakableParameters();
    TweakableParameters(vector<int> popSizes, vector<double> mutProbs, vector<SelectionType> selectTypes);
    void fixPopSize(int popSize);
    void fixMutProb(int mutProb);
    void fixSelType(SelectionType selType);
    bool hasNext(TweakableType type);
    ParameterSet getNext(TweakableType type);
};


#endif //SYMBREGEXERCISE_TWEAKABLEPARAMETERS_H
