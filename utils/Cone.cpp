//
// Created by josipmrden on 23. 03. 2020..
//

#include "Cone.h"
double cone_epsylon = 10E-5;
Cone::Cone(Point center, double A, double B, double C, double D, double E)
{
    this->_center = center;
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
    this->E = E;
}
double Cone::valueAt(Point p)
{
    double x = p.coordinates[0];
    double y = p.coordinates[1];
    double result = this->A * x * x + this->B * x * y + this->C * y * y + this->D * x + this->E * y - 1;

    return result;
}
double Cone::getDerivation(Point p)
{
    Point movedX, movedY;
    movedX.coordinates.push_back(p.coordinates[0] + cone_epsylon);
    movedX.coordinates.push_back(p.coordinates[1]);
    movedY.coordinates.push_back(p.coordinates[0]);
    movedY.coordinates.push_back(p.coordinates[1] + cone_epsylon);

    double resultStill = this->valueAt(p);
    double resultX = this->valueAt(movedX);
    double resultY = this->valueAt(movedY);

    double functionChangeX = resultX - resultStill;
    double functionChangeY = resultY - resultStill;

    double derivation = - functionChangeY / functionChangeX;

    return derivation;
}