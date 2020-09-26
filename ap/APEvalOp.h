#ifndef ApEvalOp_h
#define ApEvalOp_h

#include <string>
#include <vector>
#include <cmath>
#include <examples/SymbRegExercise/implicit_functions/ParetoFrontier.h>
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>

using namespace std;

class APEvalOp : public AbstractEvaluateOp
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
    APEvalOp();
    APEvalOp(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);

    bool isLowStdevOnRandomValuesAP(IndividualP individual, StateP state, vector<string> variables);
    double executeTreeForMovedPointFastAP(IndividualP individual, vector<string> variables, Point point, int index, double epsylon);
    double executeTreeAP(IndividualP individual, vector<string> variables, Point point);

    string getName() override
    {
        return "AP";
    }
};

#endif
