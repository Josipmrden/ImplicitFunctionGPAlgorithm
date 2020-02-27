//
// Created by josipmrden on 20. 02. 2020..
//
#include <string>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedDatasetPreparation.h>

using namespace std;

int main(int argc, char **argv)
{
    int noNeighbors = 10;
    string datasetFileName = argv[1];
    UnorderedDatasetPreparation* prep = new UnorderedDatasetPreparation(noNeighbors, datasetFileName);

    prep->initialize();
    prep->getNeighbors(prep->_points[0]);

    return 0;
}
