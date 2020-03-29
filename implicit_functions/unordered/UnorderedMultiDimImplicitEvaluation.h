#ifndef SymbRegEvalOp_h
#define SymbRegEvalOp_h

#include <string>
#include <vector>
#include <cmath>
#include <ecf/ECF.h>
#include "../ParetoFrontier.h"
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>

extern vector<string> availableLetters;
using namespace std;

class UnorderedMultiDimImplicitEvaluation : public EvaluateOp
{
private:
    StateP _state;
    string _datasetFileName;
    vector<MultiDimEllipse> _multiEllipses;
    vector<Point> _points;
    ParetoFrontier* _paretoFrontier;
    vector<string> _variables;
    bool _initializedVariables;
    double epsylon = 10E-5;
public:
    UnorderedMultiDimImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);
    bool initialize(StateP state);
    FitnessP evaluate(IndividualP individual);
};

#endif
