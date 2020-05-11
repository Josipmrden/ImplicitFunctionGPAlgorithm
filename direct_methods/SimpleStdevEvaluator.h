//
// Created by josipmrden on 27. 02. 2020..
//

#ifndef SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
#define SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H

#include <string>
#include <vector>
#include <examples/SymbRegExercise/implicit_functions/ParetoFrontier.h>
#include <examples/SymbRegExercise/implicit_functions/AbstractEvaluateOp.h>

using namespace std;


class SimpleStdevEvaluator : public AbstractEvaluateOp
{
private:
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
public:
    SimpleStdevEvaluator();
    AbstractEvaluateOp* createNew() override;
    SimpleStdevEvaluator(StateP state, string datasetFileName, ParetoFrontier* paretoFrontier);
    bool initialize(StateP state) override;
    FitnessP evaluate(IndividualP individual) override;

    string getName() override
    {
        return "SSE";
    }
};


#endif //SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
