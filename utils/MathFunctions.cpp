//
// Created by josipmrden on 27. 02. 2020..
//
#include "MathFunctions.h"

using namespace std;

double getAverage(vector<double> values)
{
    double sum = 0.0;
    for (int i = 0, size = values.size(); i < size; i++)
    {
        sum += values[i];
    }

    double average = sum / values.size();

    return average;
}

double getStdev(vector<double> values)
{
    double average = getAverage(values);
    double sum = 0.0;

    for (int i = 0, size = values.size(); i < size; i++)
    {
        double diff = values[i] - average;
        sum += diff * diff;
    }

    sum /= values.size();

    double stdev = sqrt(sum);

    return stdev;
}

double getRandomized(int lower, int upper)
{
    uniform_real_distribution<double> unif(lower, upper);
    default_random_engine re;
    double random_number = unif(re);

    return random_number;
}