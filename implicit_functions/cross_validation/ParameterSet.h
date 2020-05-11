//
// Created by josipmrden on 23. 04. 2020..
//

#ifndef SYMBREGEXERCISE_PARAMETERSET_H
#define SYMBREGEXERCISE_PARAMETERSET_H


#include <string>
#include "CrossValUtil.h"
using namespace std;

class ParameterSet {
public:
    int populationSize;
    double mutationProbablity;
    SelectionType selectionType;
    string getName();
    ParameterSet(int popSize, double mutProb, SelectionType selType);
};


#endif //SYMBREGEXERCISE_PARAMETERSET_H
