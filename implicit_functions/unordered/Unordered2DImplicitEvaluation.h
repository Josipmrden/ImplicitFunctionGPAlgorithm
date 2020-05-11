#ifndef SYMBREGEXERCISE_UNORDERED2DIMPLICITEVAL_H
#define SYMBREGEXERCISE_UNORDERED2DIMPLICITEVAL_H

#include <string>
#include <vector>
#include <examples/SymbRegExercise/utils/Cone.h>
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>
#include "../ParetoFrontier.h"

extern vector<string> availableLetters;
using namespace std;

class Unordered2DImplicitEvaluation : public AbstractEvaluateOp
{
private:
    vector<Cone> _cones;
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
    double epsylon = 10E-5;
public:
    AbstractEvaluateOp* createNew() override;
    bool initialize(StateP state) override;
    FitnessP evaluate(IndividualP individual) override;
    Unordered2DImplicitEvaluation();
    Unordered2DImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);

    string getName() override
    {
        return "IEU2D";
    }
};

#endif
