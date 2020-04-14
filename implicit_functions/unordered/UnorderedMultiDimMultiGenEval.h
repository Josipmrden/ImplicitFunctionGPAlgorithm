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

using namespace std;

class UnorderedMultiDimMultiGenEval : public EvaluateOp
{
private:
    StateP _state;
    string _datasetFileName;
    vector<MultiDimEllipse> _multiEllipses;
    vector<Point> _points;
    ParetoFrontier* _paretoFrontier;
    vector<string> _variables;
    vector<vector<string>> _variablesPerTree;
    vector<map<string, int>> _indexOfVariablesPerTree;
    map<string, int> _coordinateIndexes;
    vector<string> _operations;
    bool _initializedVariables;
    double epsylon = 10E-5;
public:
    UnorderedMultiDimMultiGenEval(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state);
    bool initialize(StateP state);
    FitnessP evaluate(IndividualP individual);
    double executeTree(Tree::Tree* tree, vector<string> assignedVariables, Point point);
    double executeTreeForMovedPoint(
            Tree::Tree *tree,
            map<string, int> indexOfVariables,
            Point point,
            int index);
};
typedef boost::shared_ptr<UnorderedMultiDimMultiGenEval> AntEvalOpP;

#endif
