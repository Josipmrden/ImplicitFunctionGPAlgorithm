//
// Created by josipmrden on 28. 03. 2020..
//

#include "MultiDimEllipse.h"

MultiDimEllipse::MultiDimEllipse(Point center, vector<double> coeffs)
{
    this->_center = center;
    this->_coeffs = coeffs;
}

int MultiDimEllipse::getDimension()
{
    return _center.coordinates.size();
}


double MultiDimEllipse::valueAt(Point p)
{
    double result = 0.0;
    int count = 0;

    for (int i = 0; i < getDimension(); i++)
    {
        result += _coeffs[count] * p.coordinates[i] * p.coordinates[i];
        count++;
    }

    for (int i = 0; i < getDimension() - 1; i++)
    {
        for (int j = i + 1; j < getDimension(); j++)
        {
            result += _coeffs[count] * p.coordinates[i] * p.coordinates[j];
            count++;
        }
    }

    for (int i = 0; i < getDimension(); i++)
    {
        result += _coeffs[count] * p.coordinates[i];
        count++;
    }

    result -= 1.0;

    return result;
}

double MultiDimEllipse::getDerivation(Point p, int index1, int index2)
{
    Point v1_eps = p.getEpsylonedPoint(index1, this->epsylon);
    Point v2_eps = p.getEpsylonedPoint(index2, this->epsylon);

    double still = valueAt(p);
    double v1_moved = valueAt(v1_eps);
    double v2_moved = valueAt(v2_eps);

    double dv1 = v1_moved - still;
    double dv2 = v2_moved - still;

    double result = - dv2 / dv1;

    return result;
}

int MultiDimEllipse::getNumberOfCoefficients(int dimension)
{
    return dimension + dimension + dimension * (dimension - 1) / 2;
}

void MultiDimEllipse::initializeDerivations()
{
    for (int i = 0; i < getDimension() - 1; i++)
    {
        for (int j = i + 1; j < getDimension(); j++)
        {
            double derivation = getDerivation(_center, i, j);
            pair<int, int> key = make_pair(i, j);
            _pointDerivations[key] = derivation;
        }
    }
}

double MultiDimEllipse::getDerivationCached(int index1, int index2)
{
    return _pointDerivations[make_pair(index1, index2)];
}



