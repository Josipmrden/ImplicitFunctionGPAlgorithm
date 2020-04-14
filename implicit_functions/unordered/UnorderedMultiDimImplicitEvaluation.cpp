#include "UnorderedMultiDimImplicitEvaluation.h"

using namespace std;

UnorderedMultiDimImplicitEvaluation::UnorderedMultiDimImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state) {
    _state = state;
    _datasetFileName = datasetFileName;
    _paretoFrontier = paretoFrontier;
    _initializedVariables = false;
}

// called only once, before the evolution and generates training data
bool UnorderedMultiDimImplicitEvaluation::initialize(StateP state)
{
    std::ifstream inputFileStream(_datasetFileName);
    int sampleSize;
    int varCount;

    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    vector<MultiDimEllipse> ellipses;
    vector<Point> points;
    for(int i = 0; i < sampleSize; i += 2) {
        string pointLine;
        getline(inputFileStream, pointLine);
        istringstream lineStream(pointLine);

        vector<double> sampleVariables;
        for (int j = 0; j < varCount; j++)
        {
            string variableString;
            getline(lineStream, variableString, ' ');
            double variableValue = stod(variableString);

            sampleVariables.push_back(variableValue);
        }

        string coneCoeffsLine;
        getline(inputFileStream, coneCoeffsLine);
        istringstream coneCoeffsStream(coneCoeffsLine);

        vector<double> coeffs;
        int coeffsSize = MultiDimEllipse::getNumberOfCoefficients(varCount);
        string coeffStr;

        for (int j = 0; j < coeffsSize; j++)
        {
            getline(coneCoeffsStream, coeffStr, ' ');
            double value = stod(coeffStr);
            coeffs.push_back(value);
        }

        Point p;
        p.coordinates = sampleVariables;
        MultiDimEllipse c(p, coeffs);
        c.initializeDerivations();

        _multiEllipses.push_back(c);
        _points.push_back(p);
    }
}

FitnessP UnorderedMultiDimImplicitEvaluation::evaluate(IndividualP individual)
{
    Tree::Tree* tree = getTree(individual, "<Tree size=\"13\">- 25 + * X X + * Y Y * Z Z</Tree>");

    if (!_initializedVariables)
    {
        _variables = getAlgorithmVariables(tree, _state);
        _initializedVariables = true;
    }

    FitnessP fitness (new FitnessMin);

    double sampleSize = _points.size();
    int dimensionSize = _points[0].coordinates.size();
    double punishment = sampleSize * sampleSize * sampleSize;

    if (!containsAllVariables(tree, _variables) || isLowStdevOnRandomValues(tree, _state, _variables))
    {
        fitness->setValue(punishment);
        return fitness;
    }

    double totalFitness = 0.0;

    for(int i = 0; i < sampleSize; i++) {
        Point point = _points[i];

        //calculate non moved value
        double notMovedEvaluation = executeTree(tree, _variables, point);

        //derivation for each variable
        vector<double> derivationByVariables;
        for (int j = 0; j < _variables.size(); j++) {
            double movedEvaluation = executeTreeForMovedPointFast(tree, _variables, point, j, epsylon);
            double derivativeByOneVariable = (movedEvaluation - notMovedEvaluation) / epsylon;
            derivationByVariables.push_back(derivativeByOneVariable);
        }

        for (int j = 0; j < dimensionSize - 1; j++)
        {
            for (int k = j+1; k < dimensionSize; k++)
            {
                double dataDerivative = _multiEllipses[i].getDerivationCached(j, k);
                double dv1 = derivationByVariables[j];
                double dv2 = derivationByVariables[k];

                double calculatedPointFitness = getFitnessFromDerivation(dataDerivative, dv1, dv2, punishment);
                totalFitness += calculatedPointFitness;
            }
        }
    }

    totalFitness /= _points.size();
    fitness->setValue(totalFitness);

    _paretoFrontier->updateParetoFront(tree, totalFitness);

    return fitness;
}