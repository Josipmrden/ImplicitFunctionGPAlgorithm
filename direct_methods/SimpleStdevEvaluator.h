//
// Created by josipmrden on 27. 02. 2020..
//

#ifndef SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
#define SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H

#include <string>
#include <vector>

using namespace std;


class SimpleStdevEvaluator : public EvaluateOp
{
private:
    string _datasetFileName;
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
public:
    SimpleStdevEvaluator(StateP state, string datasetFileName);
    FitnessP evaluate(IndividualP individual);
};


#endif //SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
