//
// Created by josipmrden on 27. 02. 2020..
//

#ifndef SYMBREGEXERCISE_MULTIPLETREEEVALUATOR_H
#define SYMBREGEXERCISE_MULTIPLETREEEVALUATOR_H

#include <string>
#include <vector>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>
#include <examples/SymbRegExercise/implicit_functions/ParetoFrontier.h>

using namespace std;


class MultipleTreeEvaluator : public AbstractEvaluateOp
{
private:
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
public:
    bool initialize(StateP state) override;
    FitnessP evaluate(IndividualP individual) override;
    AbstractEvaluateOp* createNew() override;

    MultipleTreeEvaluator();
    MultipleTreeEvaluator(StateP state, string datasetFileName, ParetoFrontier* paretoFrontier);

    bool containsAllVariables(IndividualP individual);
    bool isLowStdevOnRandomValues(IndividualP individual);
    void initializeVariables(IndividualP individual);
    double getResult(IndividualP individual, Point p, string op);
    double getFitnessFromPoints(IndividualP individual, vector<Point> points);

    string getName() override
    {
        return "MTE";
    }
};


#endif //SYMBREGEXERCISE_MULTIPLETREEEVALUATOR_H
