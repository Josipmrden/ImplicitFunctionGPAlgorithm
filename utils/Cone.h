//
// Created by josipmrden on 23. 03. 2020..
//

#ifndef SYMBREGEXERCISE_CONE_H
#define SYMBREGEXERCISE_CONE_H


#include "Point.h"

extern double cone_epsylon;

class Cone {

private:
    Point _center;
    double A;
    double B;
    double C;
    double D;
    double E;

public:
    Cone(Point center, double A, double B, double C, double D, double E);
    double valueAt(Point p);
    double getDerivation(Point p);
};


#endif //SYMBREGEXERCISE_CONE_H
