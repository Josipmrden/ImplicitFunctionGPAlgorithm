#ifndef SymbRegEvalOp_h
#define SymbRegEvalOp_h

#include <string>
#include <vector>
#include "ParetoFrontier.h"
#include "AbstractEvaluateOp.h"

extern vector<string> availableLetters;
extern double epsylon;
extern double MIN;

class ImplicitEvaluationWithPartialDerivatives : public AbstractEvaluateOp
{
private:
    std::vector<std::vector<double>> _domain;
    std::vector<double> _partialDerivatives;
public:
    ImplicitEvaluationWithPartialDerivatives();
    ImplicitEvaluationWithPartialDerivatives(std::string datasetFileName, ParetoFrontier* paretoFrontier);
    AbstractEvaluateOp* createNew() override;
    bool initialize(StateP state) override;
    FitnessP evaluate(IndividualP individual) override;

    string getName() override
    {
        return "IEO";
    }
};

#endif
