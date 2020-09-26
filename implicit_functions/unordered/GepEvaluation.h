#ifndef SYMBREGEXERCISE_GEPEVAL_H
#define SYMBREGEXERCISE_GEPEVAL_H

#include <string>
#include <vector>
#include <cmath>
#include "../ParetoFrontier.h"
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>
#include <examples/SymbRegExercise/gep/GEPChromosome.h>

using namespace std;

class GepEvaluation : public AbstractEvaluateOp
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
    GepEvaluation();
    GepEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);

    bool isLowStdevOnRandomValuesGep(GEPChromosome::GEPChromosome* individual, StateP state, vector<string> variables);
    double executeTreeGep(GEPChromosome::GEPChromosome* individual, vector<string> variables, Point point);
    double executeTreeForMovedPointFastGep(GEPChromosome::GEPChromosome* individual, vector<string> variables, Point point, int index, double epsylon);

    string getName() override
    {
        return "GEP";
    }
};

#endif
