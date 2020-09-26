#include "GepEvaluation.h"

using namespace std;

GepEvaluation::GepEvaluation() : AbstractEvaluateOp()
{
    this->noTrees = 1;
    this->requiresPlanes = true;
};
GepEvaluation::GepEvaluation(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state)
        : AbstractEvaluateOp(datasetFileName, paretoFrontier)
{
    this->noTrees = 1;
    _state = state;
    _initializedVariables = false;
    this->requiresPlanes = true;
}

AbstractEvaluateOp *GepEvaluation::createNew() {
    return new GepEvaluation();
}

// called only once, before the evolution and generates training data
bool GepEvaluation::initialize(StateP state)
{
    _state = state;
    _initializedVariables = false;

    std::ifstream inputFileStream(_datasetFileName);
    int sampleSize;
    int varCount;

    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    vector<MultiDimEllipse> ellipses;
    vector<Point> points;
    for(int i = 0; i < sampleSize; i += 1) {
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

FitnessP GepEvaluation::evaluate(IndividualP individual)
{
    //"<Tree size=\"11\">- * 4 4 + * Y Y * X X</Tree>" -> circle
    //"<Tree size=\"15\">- * 5 5 + + * Z Z * Y Y * X X</Tree>" -> sphere
    //"<Tree size=\"13\">- - + X * * X X X * Y Y 1.5</Tree>" -> hyperbola
    //"<Tree size=\"25\">+ / * - X 1 - X 1 * 3 3 - / * - Y 2 - Y 2 * 4 4 1</Tree>" -> ellipse
    //"<Tree size=\"9\">+ - Z * 0.1 Y * 3 X</Tree>" -> harmonic oscillator
    //"<Tree size="25">- + / * - X 3 - X 3 * 2.5 2.5 / * - Y 4.5 - Y 4.5 * 5 5 1</Tree>" -> nonlinear harmonic oscillator

    GEPChromosome::GEPChromosome* genotype = (boost::static_pointer_cast<GEPChromosome::GEPChromosome> (individual->getGenotype())).get();
    genotype->assemble();
    Tree::Tree* tree = getTree(individual, "");

    if (!_initializedVariables)
    {
        _variables = getAlgorithmVariables(tree, _state);
        _initializedVariables = true;
    }

    FitnessP fitness (new FitnessMin);

    double sampleSize = _points.size();
    int dimensionSize = _points[0].coordinates.size();
    double punishment = sampleSize * sampleSize * sampleSize;

    if (!containsAllVariables(tree, _variables) || isLowStdevOnRandomValuesGep(genotype, _state, _variables))
    {
        fitness->setValue(punishment);
        return fitness;
    }

    double totalFitness = 0.0;

    for(int i = 0; i < sampleSize; i++) {
        Point point = _points[i];

        //calculate non moved value
        double notMovedEvaluation = executeTreeGep(genotype, _variables, point);

        //derivation for each variable
        vector<double> derivationByVariables;
        for (int j = 0; j < _variables.size(); j++) {
            double movedEvaluation = executeTreeForMovedPointFastGep(genotype, _variables, point, j, epsylon);
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

                if (totalFitness > punishment)
                {
                    fitness->setValue(totalFitness);
                    return fitness;
                }
            }
        }
    }

    totalFitness /= _points.size();
    fitness->setValue(totalFitness);

    _paretoFrontier->updateParetoFront(tree, totalFitness);

    return fitness;
}

bool GepEvaluation::isLowStdevOnRandomValuesGep(GEPChromosome::GEPChromosome* individual, StateP state, vector<string> variables)
{
    int variablesSize = variables.size();
    vector<double> results;

    for (int i = 0; i < 10; i++)
    {
        double result;
        for (int j = 0; j < variablesSize; j++)
        {
            double randomValue = state->getRandomizer()->getRandomDouble();
            individual->setTerminalValue(variables[j], &randomValue);
        }

        individual->execute(&result);
        results.push_back(result);
    }

    double stdev = getStdev(results);

    return stdev < 10E-6;
}

double GepEvaluation::executeTreeGep(GEPChromosome::GEPChromosome* individual, vector<string> variables, Point point)
{
    for (uint j = 0; j < variables.size(); j++) {
        double value = point.coordinates[j];
        individual->setTerminalValue(variables[j], &value);
    }

    double evaluation;
    individual->execute(&evaluation);

    return evaluation;
}

double GepEvaluation::executeTreeForMovedPointFastGep(GEPChromosome::GEPChromosome* individual, vector<string> variables, Point point, int index, double epsylon)
{
    if (index != 0)
    {
        double value = point.coordinates[index - 1];
        individual->setTerminalValue(variables[index - 1], &value);
    }

    double epsylonedValue = point.coordinates[index] + epsylon;
    individual->setTerminalValue(variables[index], &epsylonedValue);

    double evaluation;
    individual->execute(&evaluation);

    return evaluation;
}