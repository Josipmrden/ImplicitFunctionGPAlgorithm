#include "UnorderedMultiDimMultiGenEval.h"

using namespace std;

UnorderedMultiDimMultiGenEval::UnorderedMultiDimMultiGenEval() : AbstractEvaluateOp()
{
    this->noTrees = 2;
    this->requiresPlanes = true;
}
UnorderedMultiDimMultiGenEval::UnorderedMultiDimMultiGenEval(string datasetFileName, ParetoFrontier* paretoFrontier, StateP state)
    : AbstractEvaluateOp(datasetFileName, paretoFrontier)
{
    this->_state = state;
    this->noTrees = 2;
    this->requiresPlanes = true;
}

AbstractEvaluateOp *UnorderedMultiDimMultiGenEval::createNew()
{
    return new UnorderedMultiDimMultiGenEval();
}

// called only once, before the evolution and generates training data
bool UnorderedMultiDimMultiGenEval::initialize(StateP state)
{
    _state = state;
    _initializedVariables = false;
    _operations = {"+", "-", "*"};

    std::ifstream inputFileStream(_datasetFileName);
    int sampleSize;
    int varCount;

    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

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

FitnessP UnorderedMultiDimMultiGenEval::evaluate(IndividualP individual)
{
    //"<Tree size=\"3\">* 4 4</Tree>" -> circle
    //"<Tree size=\"7\"> + * Y Y * X X</Tree>" -> circle

    //"<Tree size=\"7\">+ * Y Y * X X</Tree>" -> sphere
    //"<Tree size=\"7\">- * 5 5 * Z Z</Tree>" -> sphere

    //"<Tree size=\"7\">+ * * X X X X</Tree>" -> hyperbola
    //"<Tree size=\"5\">+ * Y Y 1.5</Tree>" -> hyperbola

    //"<Tree size=\"11\">/ * - X 1 - X 1 * 3 3</Tree>" -> ellipse
    //"<Tree size=\"13\">- / * - Y 2 - Y 2 * 4 4 1</Tree>" -> ellipse

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> harmonic oscillator
    //"<Tree size=\"3\">* 3 X</Tree>" -> harmonic oscillator

    //"<Tree size=\"5\">- Z * 0.1 Y</Tree>" -> nonlinear harmonic oscillator
    //"<Tree size=\"4\">* 9.8 sin X</Tree>" -> nonlinear harmonic oscillator

    Tree::Tree* firstTree = getTreeAtIndex(individual, "", 0);
    Tree::Tree* secondTree = getTreeAtIndex(individual, "", 1);
    vector<Tree::Tree*> allTrees { firstTree, secondTree };

    if (!_initializedVariables)
    {
        _variables = getAllVariablesWithinTrees(allTrees, _state);
        _variablesPerTree = getVariablesPerTree(allTrees, _state);

        for (int i = 0; i < allTrees.size(); i ++)
        {
            map<string, int> mapper;
            _indexOfVariablesPerTree.push_back(mapper);
        }

        for (int i = 0; i < _variables.size(); i++)
        {
            _coordinateIndexes[_variables[i]] = i;
            for (int j = 0; j < _variablesPerTree.size(); j++)
            {
                vector<string> variablesForOneTree = _variablesPerTree[j];
                if (find(variablesForOneTree.begin(), variablesForOneTree.end(), _variables[i]) != variablesForOneTree.end())
                {
                    _indexOfVariablesPerTree[j][_variables[i]] = i;
                }
                else
                {
                    _indexOfVariablesPerTree[j][_variables[i]] = -1;
                }
            }
        }

        _initializedVariables = true;
    }

    FitnessP fitness (new FitnessMin);

    double sampleSize = _points.size();
    int dimensionSize = _points[0].coordinates.size();
    double punishment = sampleSize * sampleSize * sampleSize;

    if (!containsAllVariables(allTrees, _variables))
    {
        fitness->setValue(punishment);
        return fitness;
    }

    vector<Point> randomDataset = generateRandomDataset(_state, 10, dimensionSize);
    vector<double> randomPointsStdevs = getStdevsFromPoints(firstTree, secondTree, randomDataset, _variablesPerTree);
    vector<string> leftoverOperators;

    for (int i = 0; i < _operations.size(); i++)
    {
        if (randomPointsStdevs[i] > 10E-6)
        {
            leftoverOperators.push_back(_operations[i]);
        }
    }

    if (leftoverOperators.empty())
    {
        fitness->setValue(punishment);
        return fitness;
    }

    map<string, double> fitnesses;
    for (string operation : leftoverOperators)
    {
        fitnesses[operation] = 0.0;
    }

    for (int i = 0; i < sampleSize; i++) {
        Point point = _points[i];

        //calculate non moved value
        double notMovedEvaluation0 = this->executeTree(firstTree, _variablesPerTree[0], point);
        double notMovedEvaluation1 = this->executeTree(secondTree, _variablesPerTree[1], point);

        //derivation for each variable
        map<string, vector<double>> derivationByVariablesMap;
        for (string operation : leftoverOperators)
        {
            derivationByVariablesMap[operation] = {};
        }

        for (int j = 0; j < _variables.size(); j++) {
            double movedEvaluation0 = this->executeTreeForMovedPoint(firstTree, _indexOfVariablesPerTree[0], point, j);
            double movedEvaluation1 = this->executeTreeForMovedPoint(secondTree, _indexOfVariablesPerTree[1], point, j);

            for (string operation : leftoverOperators)
            {
                double notMovedEvaluation = calculateResult2Trees(notMovedEvaluation0, notMovedEvaluation1, operation);
                double movedEvaluation = calculateResult2Trees(movedEvaluation0, movedEvaluation1, operation);
                double derivativeByOneVariable = (movedEvaluation - notMovedEvaluation) / epsylon;

                derivationByVariablesMap[operation].push_back(derivativeByOneVariable);
            }
        }

        for (int j = 0; j < dimensionSize - 1; j++)
        {
            for (int k = j + 1; k < dimensionSize; k++)
            {
                double dataDerivative = _multiEllipses[i].getDerivationCached(j, k);

                for (string operation : leftoverOperators)
                {
                    double dv1 = derivationByVariablesMap[operation][j];
                    double dv2 = derivationByVariablesMap[operation][k];

                    double calculatedPointFitness = getFitnessFromDerivation(dataDerivative, dv1, dv2, punishment);
                    fitnesses[operation] += calculatedPointFitness;
                }
            }
        }
    }

    double minFitness = 0.0;
    for (std::map<string, double>::const_iterator it = fitnesses.begin(); it != fitnesses.end(); ++it)
    {
        if (it == fitnesses.begin())
        {
            minFitness = it->second;
        }
        else if (it->second < minFitness)
        {
            minFitness = it->second;
        }
    }

    minFitness /= _points.size();
    fitness->setValue(minFitness);

    _paretoFrontier->updateParetoFront(allTrees, minFitness);

    return fitness;
}

double UnorderedMultiDimMultiGenEval::executeTree(Tree::Tree* tree, vector<string> assignedVariables, Point point)
{
    for (uint j = 0, size = assignedVariables.size(); j < size; j++) {
        int pointIndex = _coordinateIndexes[assignedVariables[j]];
        double value = point.coordinates[pointIndex];

        tree->setTerminalValue(assignedVariables[j], &value);
    }

    double evaluation;
    tree->execute(&evaluation);

    return evaluation;
}

double UnorderedMultiDimMultiGenEval::executeTreeForMovedPoint(
        Tree::Tree* tree,
        map<string, int> indexOfVariables,
        Point point,
        int index)
{
    int previousIndex = index - 1;
    if (previousIndex >= 0)
    {
        string variableAtPreviousIndex = _variables[previousIndex];
        int indexOfPreviousVariableForTree = indexOfVariables[variableAtPreviousIndex];
        if (indexOfPreviousVariableForTree != -1)
        {
            double value = point.coordinates[indexOfPreviousVariableForTree];
            tree->setTerminalValue(variableAtPreviousIndex, &value);
        }
    }

    string variableToBeEpsyloned = _variables[index];
    int indexOfVariableForTree = indexOfVariables[variableToBeEpsyloned];
    if (indexOfVariableForTree != -1)
    {
        double value = point.coordinates[indexOfVariableForTree] + epsylon;
        tree->setTerminalValue(variableToBeEpsyloned, &value);
    }

    double evaluation;
    tree->execute(&evaluation);

    return evaluation;
}