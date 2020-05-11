#ifndef SYMBREGEXERCISE_ABSTRACTEVALUATEOP_H
#define SYMBREGEXERCISE_ABSTRACTEVALUATEOP_H

#include <string>
#include <vector>
#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>
#include "ParetoFrontier.h"

using namespace std;

class AbstractEvaluateOp : public EvaluateOp
{
public:
    string _datasetFileName;
    ParetoFrontier* _paretoFrontier;
    int noTrees;
    bool requiresPlanes;
    AbstractEvaluateOp();
    AbstractEvaluateOp(string datasetFileName, ParetoFrontier* paretoFrontier);
    virtual AbstractEvaluateOp* createNew() = 0;
    virtual bool initialize(StateP state) = 0;
    virtual FitnessP evaluate(IndividualP individual) = 0;
    virtual string getName() = 0;
};

#endif