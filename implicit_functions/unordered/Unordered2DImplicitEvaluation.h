#ifndef SymbRegEvalOp_h
#define SymbRegEvalOp_h


#include <string>
#include <vector>
#include <examples/SymbRegExercise/utils/Cone.h>
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include "../ParetoFrontier.h"

extern vector<string> availableLetters;
using namespace std;

class Unordered2DImplicitEvaluation : public EvaluateOp
{
private:
    string _datasetFileName;
    vector<Cone> _cones;
    vector<Point> _points;
    ParetoFrontier* _paretoFrontier;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
    double epsylon = 10E-5;
public:
    Unordered2DImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);
    bool initialize(StateP state);
    FitnessP evaluate(IndividualP individual);
};

#endif
