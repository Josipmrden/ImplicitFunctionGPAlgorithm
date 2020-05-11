#ifndef SYMBREGEXERCISE_UNORDEREDMULTIDIMIMPLICITEVAL_H
#define SYMBREGEXERCISE_UNORDEREDMULTIDIMIMPLICITEVAL_H

#include <string>
#include <vector>
#include <cmath>
#include "../ParetoFrontier.h"
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>

using namespace std;

class UnorderedMultiDimImplicitEvaluation : public AbstractEvaluateOp
{
private:
    StateP _state;
    vector<MultiDimEllipse> _multiEllipses;
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    double epsylon = 10E-5;
public:
    AbstractEvaluateOp* createNew() override;
    bool initialize(StateP state) override;
    FitnessP evaluate(IndividualP individual) override;
    UnorderedMultiDimImplicitEvaluation();
    UnorderedMultiDimImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);

    string getName() override
    {
        return "IEUND";
    }
};

#endif
