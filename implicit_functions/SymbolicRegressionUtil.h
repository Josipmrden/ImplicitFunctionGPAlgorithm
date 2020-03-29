//
// Created by josipmrden on 29. 03. 2020..
//

#ifndef SYMBREGEXERCISE_IMPLICITUTIL_H
#define SYMBREGEXERCISE_IMPLICITUTIL_H

#endif //SYMBREGEXERCISE_IMPLICITUTIL_H

#include <cmath>
#include <ecf/ECF.h>
#include <examples/SymbRegExercise/utils/MathFunctions.h>
#include "../utils/Cone.h"

Tree::Tree* getTree(IndividualP individual, string customIndividual);

vector<string> getAlgorithmVariables(Tree::Tree* tree, StateP state);
bool containsAllVariables(Tree::Tree* tree, vector<string> variables);
bool isLowStdevOnRandomValues(Tree::Tree* tree, StateP state, vector<string> variables);

double executeTree(Tree::Tree* tree, vector<string> variables, Point point);
double executeTreeForMovedPoint(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon);
double executeTreeForMovedPointFast(Tree::Tree* tree, vector<string> variables, Point point, int index, double epsylon);

double getFitnessFromDerivation(double experimentalDerivation, double dv1, double dv2, double punishment);
