//
// Created by josipmrden on 28. 03. 2020..
//

#ifndef SYMBREGEXERCISE_MULTIDIMELLIPSE_H
#define SYMBREGEXERCISE_MULTIDIMELLIPSE_H

#include <map>
#include "Cone.h"

using namespace std;

class MultiDimEllipse {
private:
    Point _center;
    vector<double> _coeffs;
    const double epsylon = 10E-5;
    map<pair<int, int>, double> _pointDerivations;
public:
    MultiDimEllipse(Point center, vector<double> coeffs);
    int getDimension();
    double getDerivationCached(int index1, int index2);
    static int getNumberOfCoefficients(int dimension);
    double valueAt(Point p);
    void initializeDerivations();
    double getDerivation(Point p, int index1, int index2);
};


#endif //SYMBREGEXERCISE_MULTIDIMELLIPSE_H
