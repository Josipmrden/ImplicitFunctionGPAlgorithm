//
// Created by josipmrden on 20. 02. 2020..
//

#ifndef SYMBREGEXERCISE_UNORDEREDDATASETPREPARATION_H
#define SYMBREGEXERCISE_UNORDEREDDATASETPREPARATION_H

#include <string>
#include <vector>
#include <math.h>
#include <examples/SymbRegExercise/utils/Point.h>

using namespace std;

struct np {
    Point mainPoint;
    vector<Point> localField;
};

typedef struct np LocalNeighboringPoints;

class UnorderedDatasetPreparation {
private:
    int _noNeighbors;
    string _datasetFileName;
public:
    vector<Point> _points;
    UnorderedDatasetPreparation(int _noNeighbors, string datasetFileName);
    void initialize();
    double calculateDistance(Point x, Point y);
    LocalNeighboringPoints getNeighbors(Point x);
};


#endif //SYMBREGEXERCISE_UNORDEREDDATASETPREPARATION_H
