//
// Created by josipmrden on 27. 02. 2020..
//
#include <vector>
#include "Point.h"

using namespace std;


Point Point::getEpsylonedPoint(int index, double epsylon)
{
    vector<double> newCoordinates;
    for (int i = 0; i < coordinates.size(); i++)
    {
        double value = coordinates[i];
        if (i == index)
        {
            value += epsylon;
        }

        newCoordinates.push_back(value);
    }

    Point newPoint;
    newPoint.coordinates = newCoordinates;

    return newPoint;
}