//
// Created by josipmrden on 20. 02. 2020..
//

#include <fstream>
#include <sstream>
#include "UnorderedDatasetPreparation.h"
UnorderedDatasetPreparation::UnorderedDatasetPreparation(int noNeighbors, string datasetFileName)
{
    this->_noNeighbors = noNeighbors;
    this->_datasetFileName = datasetFileName;
}

void UnorderedDatasetPreparation::initialize()
{
    ifstream inputFileStream(this->_datasetFileName);
    int sampleSize;
    int varCount;
    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    for(int i = 0; i < sampleSize; i++) {
        string line;
        getline(inputFileStream, line);
        istringstream lineStream(line);

        std::vector<double> sampleVariables;
        for (int j = 0; j < varCount; j++)
        {
            string variableString;
            std::getline(lineStream, variableString, ' ');
            double variableValue = std::stod(variableString);

            sampleVariables.push_back(variableValue);
        }

        Point pt;
        pt.coordinates = sampleVariables;

        _points.push_back(pt);
    }
}

double UnorderedDatasetPreparation::calculateDistance(Point x, Point y)
{
    double distance = 0;

    for (int i = 0; i < x.coordinates.size(); i++)
    {
        double first = x.coordinates[i];
        double second = y.coordinates[i];

        distance += (first - second) * (first - second);
    }

    return sqrt(distance);
}

bool tryInsert(Point x, double distance, vector<Point> &localField, vector<double> &distances)
{
    int maxIndex = -1;
    double maxDistance = -1;
    for (int i = 0; i < distances.size(); i++)
    {
        if (maxIndex == -1)
        {
            maxIndex = i;
            maxDistance = distances[i];
            continue;
        }
        if (maxDistance < distances[i])
        {
            maxIndex = i;
            maxDistance = distances[i];
        }
    }

    if (distance >= maxDistance)
    {
        return false;
    }

    localField[maxIndex] = x;
    distances[maxIndex] = distance;
}

double getMaxDistance(vector<double> distances)
{
    int maxIndex = -1;
    double maxDistance = -1;
    for (int i = 0; i < distances.size(); i++)
    {
        if (maxIndex == -1)
        {
            maxIndex = i;
            maxDistance = distances[i];
            continue;
        }
        if (maxDistance < distances[i])
        {
            maxIndex = i;
            maxDistance = distances[i];
        }
    }

    return maxDistance;
}

LocalNeighboringPoints UnorderedDatasetPreparation::getNeighbors(Point x)
{
    vector<Point> localField;
    vector<double> distances;

    double maxDistance = -1;
    for (int i = 0; i < _points.size(); i++) {
        Point potentialNeighor = _points[i];
        double distance = calculateDistance(x, potentialNeighor);

        if (i == 0)
        {
            maxDistance = distance;
        }

        if (localField.size() >= _noNeighbors)
        {
            if (distance > maxDistance)
            {
                continue;
            }
            bool inserted = tryInsert(potentialNeighor, distance, localField, distances);
            if (inserted)
            {
                maxDistance = getMaxDistance(distances);
            }
        }
        else
        {
            localField.push_back(potentialNeighor);
            distances.push_back(distance);
            maxDistance = maxDistance < distance ? distance : maxDistance;
        }
    }

    LocalNeighboringPoints localPoints;

    localPoints.mainPoint = x;
    localPoints.localField = localField;

    return localPoints;
}