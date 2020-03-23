#include <cmath>
#include <ecf/ECF.h>
#include "Unordered2DImplicitEvaluation.h"

using namespace std;

double unordered_epsylon = 10e-5;

Unordered2DImplicitEvaluation::Unordered2DImplicitEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state) {
    this->_state = state;
    this->_datasetFileName = datasetFileName;
    this->_paretoFrontier = paretoFrontier;
    this->_initializedVariables = false;
}

void Unordered2DImplicitEvaluation::initializeVariables(IndividualP individual)
{
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
    voidP val = tree->getParameterValue(this->_state, "terminalset");
    std::string terminals = *((std::string*) val.get());

    istringstream lineStream(terminals);

    while (true)
    {
        string variableString;
        getline(lineStream, variableString, ' ');

        if (variableString.empty())
        {
            break;
        }

        if (variableString.size() == 1 && isalpha(variableString[0]))
        {
            this->_variables.push_back(variableString);
        }
    }
}

// called only once, before the evolution � generates training data
bool Unordered2DImplicitEvaluation::initialize(StateP state)
{
    std::ifstream inputFileStream(this->_datasetFileName);
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

    this->_points = points;
    this->_cones = cones;
}

FitnessP Unordered2DImplicitEvaluation::evaluate(IndividualP individual)
{
    //feenableexcept(FE_INVALID | FE_OVERFLOW);
    if (!this->_initializedVariables)
    {
        this->initializeVariables(individual);
        this->_initializedVariables = true;
    }

    FitnessP fitness (new FitnessMin);

    // get the genotype we defined in the configuration file
    Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();

    double equalsZeroFitness = 0;

    int sampleSize = _points.size();

    double fitnessValue = 0.0;
    double fitnessDerivativeValue = 0.0;
    for(uint i = 0; i < sampleSize; i++) {
        Point point = _points[i];
        //calculate non moved value
        for (uint j = 0; j < _variables.size(); j++) {
            double value = point.coordinates[j];
            tree->setTerminalValue(_variables[j], &value);
        }
        double resultStill;
        tree->execute(&resultStill);
        equalsZeroFitness += fabs(resultStill);

        vector<double> derivationByVariables;
        //point after current
        for (uint j = 0; j < _variables.size(); j++) {
            double eps = unordered_epsylon;
            double derivative;
            while (true) {
                for (uint k = 0; k < _variables.size(); k++) {
                    double value = point.coordinates[k];
                    if (j == k) {
                        value += eps;
                    }
                    tree->setTerminalValue(_variables[k], &value);
                }
                double resultAfter;
                tree->execute(&resultAfter);
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
        double dataDerivative = this->_cones[i].getDerivation(_points[i]);
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
    fitnessValue /= _points.size();

    fitness->setValue(fitnessValue);

    _paretoFrontier->updateParetoFront(tree, fitnessValue, tree->toString());

    return fitness;
}