//
// Created by josipmrden on 29. 03. 2020..
//

#ifndef SYMBREGEXERCISE_IMPLICITUTIL_H
#define SYMBREGEXERCISE_IMPLICITUTIL_H

#endif //SYMBREGEXERCISE_IMPLICITUTIL_H

#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include "examples/SymbRegExercise/utils/Cone.h"

Tree::Tree* getTree(IndividualP individual, string customIndividual);
Tree::Tree* getTreeAtIndex(IndividualP individual, string customIndividual, int index);

vector<string> getAlgorithmVariables(Tree::Tree* tree, StateP state);
vector<string> getAllVariablesWithinTrees(vector<Tree::Tree*> trees, StateP state);
vector<vector<string>> getVariablesPerTree(vector<Tree::Tree*> trees, StateP state);

bool containsAllVariables(Tree::Tree* tree, vector<string> variables);
bool containsAllVariables(vector<Tree::Tree*> trees, vector<string> variables);

vector<Point> generateRandomDataset(StateP state, int sampleSize, int varSize);

bool isLowStdevOnRandomValues(Tree::Tree* tree, StateP state, vector<string> variables);

vector<double> getStdevsFromPoints(Tree::Tree* firstTree, Tree::Tree* secondTree, vector<Point> points, vector<vector<string>> variables);

double executeTree(Tree::Tree* tree, vector<string> variables, Point point);
double executeTreeForMovedPoint(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon);
double executeTreeForMovedPointFast(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon);
double calculateResult2Trees(double result1, double result2, string operation);

double getFitnessFromDerivation(double experimentalDerivation, double dv1, double dv2, double punishment);
