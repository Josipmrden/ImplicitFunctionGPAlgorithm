#ifndef SYMBREGEXERCISE_UNORDEREDMULTIDIMMULTIGENIMPLICITEVAL_H
#define SYMBREGEXERCISE_UNORDEREDMULTIDIMMULTIGENIMPLICITEVAL_H

#include <string>
#include <vector>
#include <cmath>
#include <ecf/ECF.h>
#include "../ParetoFrontier.h"
#include <examples/SymbRegExercise/utils/MultiDimEllipse.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>

using namespace std;

class UnorderedMultiDimMultiGenEval : public AbstractEvaluateOp
{
private:
    StateP _state;
    vector<MultiDimEllipse> _multiEllipses;
    vector<Point> _points;
    vector<string> _variables;
    vector<vector<string>> _variablesPerTree;
    vector<map<string, int>> _indexOfVariablesPerTree;
    map<string, int> _coordinateIndexes;
    vector<string> _operations;
    bool _initializedVariables;
    double epsylon = 10E-5;
public:
    AbstractEvaluateOp* createNew() override;
    bool initialize(StateP state);
    FitnessP evaluate(IndividualP individual) override;
    UnorderedMultiDimMultiGenEval();
    UnorderedMultiDimMultiGenEval(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);
    double executeTree(Tree::Tree* tree, vector<string> assignedVariables, Point point);
    double executeTreeForMovedPoint(
            Tree::Tree *tree,
            map<string, int> indexOfVariables,
            Point point,
            int index);

    string getName() override
    {
        return "IEUNDMG";
    }
};
typedef boost::shared_ptr<UnorderedMultiDimMultiGenEval> AntEvalOpP;

#endif
