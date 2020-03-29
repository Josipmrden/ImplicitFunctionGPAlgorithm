#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include <examples/SymbRegExercise/implicit_functions/SymbolicRegressionUtil.h>
#include "Unordered2DImplicitEvaluation.h"
using namespace std;

Unordered2DImplicitEvaluation::Unordered2DImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state) {
    _state = state;
    _datasetFileName = datasetFileName;
    _paretoFrontier = paretoFrontier;
    _initializedVariables = false;
}

// called only once, before the evolution � generates training data
bool Unordered2DImplicitEvaluation::initialize(StateP state)
{
    std::ifstream inputFileStream(_datasetFileName);
    int sampleSize;
    int varCount;

    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    vector<Cone> cones;
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

        string stringA, stringB, stringC, stringD, stringE;
        getline(coneCoeffsStream, stringA, ' ');
        getline(coneCoeffsStream, stringB, ' ');
        getline(coneCoeffsStream, stringC, ' ');
        getline(coneCoeffsStream, stringD, ' ');
        getline(coneCoeffsStream, stringE, ' ');

        double coeffA = stod(stringA);
        double coeffB = stod(stringB);
        double coeffC = stod(stringC);
        double coeffD = stod(stringD);
        double coeffE = stod(stringE);

        Point p;
        p.coordinates = sampleVariables;
        Cone c(p, coeffA, coeffB, coeffC, coeffD, coeffE);
        cones.push_back(c);
        points.push_back(p);
    }

    _points = points;
    _cones = cones;
}

FitnessP Unordered2DImplicitEvaluation::evaluate(IndividualP individual)
{
    FitnessP fitness (new FitnessMin);
    Tree::Tree* tree = getTree(individual, "");
    double sampleSize = _points.size();
    double punishment = sampleSize * sampleSize * sampleSize;

    if (!_initializedVariables)
    {
        _variables = getAlgorithmVariables(tree, _state);
        _initializedVariables = true;
    }

    double totalFitness = 0.0;
    for(uint i = 0; i < sampleSize; i++) {
        Point point = _points[i];

        //calculate non moved value
        double notMovedResult = executeTree(tree, _variables, point);

        vector<double> derivationByVariables;
        //point after current
        for (uint j = 0; j < _variables.size(); j++) {
            double movedResult = executeTreeForMovedPointFast(tree, _variables, point, j, epsylon);
            double derivative = (movedResult - notMovedResult) / epsylon;
            derivationByVariables.push_back(derivative);
        }

        //calculate derivatives
        double dataDerivative = this->_cones[i].getDerivation(_points[i]);
        double dy = derivationByVariables[1];
        double dx = derivationByVariables[0];
        double pointFitness = getFitnessFromDerivation(dataDerivative, dy, dx, punishment);

        totalFitness += pointFitness;
    }

    totalFitness /= _points.size();
    fitness->setValue(totalFitness);

    _paretoFrontier->updateParetoFront(tree, totalFitness, tree->toString());

    return fitness;
}