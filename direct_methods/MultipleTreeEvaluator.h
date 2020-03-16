//
// Created by josipmrden on 27. 02. 2020..
//

#ifndef SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
#define SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H

#include <string>
#include <vector>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/Point.h>

using namespace std;


class MultipleTreeEvaluator : public EvaluateOp
{
private:
    string _datasetFileName;
    string _functionsOperator;
    vector<Point> _points;
    vector<string> _variables;
    bool _initializedVariables;
    StateP _state;
public:
    MultipleTreeEvaluator(StateP state, string datasetFileName, string functionsOperator);
    bool containsAllVariables(IndividualP individual);
    bool isLowStdevOnRandomValues(IndividualP individual);
    void initializeVariables(IndividualP individual);
    double getResult(IndividualP individual, Point p);
    FitnessP evaluate(IndividualP individual);
};


#endif //SYMBREGEXERCISE_SIMPLESTDEVEVALUATOR_H
