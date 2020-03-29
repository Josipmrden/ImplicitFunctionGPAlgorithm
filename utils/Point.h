//
// Created by josipmrden on 27. 02. 2020..
//

#ifndef SYMBREGEXERCISE_POINT_H
#define SYMBREGEXERCISE_POINT_H

#endif //SYMBREGEXERCISE_POINT_H
#include <vector>
using namespace std;

class Point
{
public:
    vector<double> coordinates;
    Point getEpsylonedPoint(int index, double epsylon);
};