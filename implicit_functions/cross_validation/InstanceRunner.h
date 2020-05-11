//
// Created by josipmrden on 23. 04. 2020..
//

#ifndef SYMBREGEXERCISE_INSTANCERUNNER_H
#define SYMBREGEXERCISE_INSTANCERUNNER_H

#include <ecf/ECF.h>
#include "TweakableParameters.h"
#include "DatasetInfo.h"
#include <vector>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>
#include <examples/SymbRegExercise/utils/primitives/Exp.h>
#include <examples/SymbRegExercise/utils/primitives/Sqrt.h>
#include <examples/SymbRegExercise/utils/primitives/Tanh.h>
#include <examples/SymbRegExercise/utils/primitives/Square.h>

#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

class InstanceRunner {
private:
    int _noRuns;
    vector<DatasetInfo> _datasets;
    string _logDirectory;
    AbstractEvaluateOp* _evaluateOp;
    TweakableParameters _tweakableParameters;
    StateP _state;
    int argc;
    char **argv;
public:
    InstanceRunner(int noRuns,
            vector<DatasetInfo> datasets,
            AbstractEvaluateOp *evaluateOp,
            TweakableParameters tp,
            int argc,
            char** argv,
            StateP state,
            string logDirectory);
    void runInstance(ParameterSet parameterSet);
};


#endif //SYMBREGEXERCISE_INSTANCERUNNER_H
