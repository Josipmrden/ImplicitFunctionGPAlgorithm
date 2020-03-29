#ifndef SymbRegEvalOp_h
#define SymbRegEvalOp_h

#include <string>
#include <vector>
#include "ParetoFrontier.h"

extern vector<string> availableLetters;
extern double epsylon;
extern double MIN;

class ImplicitEvaluationWithPartialDerivatives : public EvaluateOp
{
private:
    std::string _datasetFileName;
    std::vector<std::vector<double>> _domain;
    std::vector<double> _partialDerivatives;
    ParetoFrontier* _paretoFrontier;
public:
    ImplicitEvaluationWithPartialDerivatives(std::string datasetFileName, ParetoFrontier* paretoFrontier);
    bool initialize(StateP state);
    FitnessP evaluate(IndividualP individual);
};

#endif
