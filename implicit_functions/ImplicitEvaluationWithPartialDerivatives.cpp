#include <cmath>
#include <ecf/ECF.h>
#include "ImplicitEvaluationWithPartialDerivatives.h"
#include "ParetoFrontier.h"

using String = std::string;

std::vector<String> availableLetters = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
double epsylon = 10e-5;
double MIN = 10e-10;
int evalNo = 0;

ImplicitEvaluationWithPartialDerivatives::ImplicitEvaluationWithPartialDerivatives(std::string datasetFileName, ParetoFrontier* paretoFrontier) {
    this->_datasetFileName = datasetFileName;
    this->_paretoFrontier = paretoFrontier;
}

// called only once, before the evolution � generates training data
bool ImplicitEvaluationWithPartialDerivatives::initialize(StateP state)
{
    std::ifstream inputFileStream(this->_datasetFileName);
    int sampleSize;
    int varCount;
    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    for(int i = 0; i < sampleSize; i++) {
        String line;
        std::getline(inputFileStream, line);
        std::istringstream lineStream(line);

        std::vector<double> sampleVariables;
        for (int j = 0; j < varCount; j++)
        {
            String variableString;
            std::getline(lineStream, variableString, ' ');
            double variableValue = std::stod(variableString);

            sampleVariables.push_back(variableValue);
        }
        _domain.push_back(sampleVariables);
    }

    for (int i = 0; i < sampleSize; i++) {
        std::vector<double> varValues = _domain[i];
        std::vector<double> previousVarValues;
        std::vector<double> nextVarValues;
        double dx, dy;
        if (i == 0) {
            nextVarValues = _domain[i + 1];
            dx = nextVarValues[0] - varValues[0];
            dy = nextVarValues[1] - varValues[1];
        }
        else if (i == _domain.size() - 1) {
            previousVarValues = _domain[i - 1];
            dx = varValues[0] - previousVarValues[0];
            dy = varValues[1] - previousVarValues[1];
        }
        else {
            nextVarValues = _domain[i + 1];
            previousVarValues = _domain[i - 1];
            dx = nextVarValues[0] - previousVarValues[0];
            dy = nextVarValues[1] - previousVarValues[1];
            if (fabs(dy) < epsylon) {
                dy = nextVarValues[1] - varValues[1];
            }
        }
        double derivation = dx / dy;
        _partialDerivatives.push_back(derivation);
    }
}

double calculate(std::vector<double> values) {
    double x = values[0];
    double y = values[1];
    return x*x + y*y - 25;
}

FitnessP ImplicitEvaluationWithPartialDerivatives::evaluate(IndividualP individual)
{

    evalNo++;
    //feenableexcept(FE_INVALID | FE_OVERFLOW);
    // we try to minimize the function value, so we use FitnessMin fitness (for minimization problems)

    FitnessP fitness (new FitnessMin);

    // get the genotype we defined in the configuration file
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();

    double equalsZeroFitness = 0;

    int sampleSize = _domain.size();
    int variablesSize = _domain[0].size();

    double fitnessValue = 0.0;
    double fitnessDerivativeValue = 0.0;
    for(uint i = 0; i < sampleSize; i++) {
        //calculate non moved value
        for (uint j = 0; j < variablesSize; j++) {
            String variableString = availableLetters[j];
            double value = _domain[i][j];
            tree->setTerminalValue(variableString, &value);
        }
        double resultStill;
        tree->execute(&resultStill);
        equalsZeroFitness += fabs(resultStill);

        std::vector<double> derivationByVariables;
        //point after current
        for (uint j = 0; j < variablesSize; j++) {
            double eps = epsylon;
            double derivative;
            while (true) {
                //std::vector<double> movingValues;
                for (uint k = 0; k < variablesSize; k++) {
                    String variableString = availableLetters[k];
                    double value = _domain[i][k];
                    if (j == k) {
                        value += eps;
                    }
                    tree->setTerminalValue(variableString, &value);
                    //movingValues.push_back(value);
                }
                double resultAfter;
                tree->execute(&resultAfter);
                //resultAfter = calculate(movingValues);
                derivative = (resultAfter - resultStill) / eps;
                if (derivative == 0.0) {
                    eps *= 10;
                    if (eps == 1) {
                        break;
                    }
                    continue;
                }
                break;
            }
            derivationByVariables.push_back(derivative);
        }

        //calculate derivatives
        double dataDerivative = _partialDerivatives[i];
        double a = derivationByVariables[1];
        double b = derivationByVariables[0];
        double difference;

        if (isnan(a) || isnan(b))
        {
            difference = log(1 + fabs(dataDerivative));
        } else if (a == 0.0 && b == 0.0) {
            difference = log(1 + fabs(dataDerivative));
        } else {
            double derivationAssumed = - a / b;
            difference = log(1 + fabs(derivationAssumed - dataDerivative));
        }

        fitnessDerivativeValue += difference;
    }

    fitnessValue += fitnessDerivativeValue;
    fitnessValue += log(1 + equalsZeroFitness);
    fitnessValue /= _partialDerivatives.size();

    fitness->setValue(fitnessValue);

    _paretoFrontier->updateParetoFront(tree, fitnessValue);

    return fitness;
}