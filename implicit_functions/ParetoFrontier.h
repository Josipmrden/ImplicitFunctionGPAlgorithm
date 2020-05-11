#ifndef SYMBREGEXERCISE_PARETOFRONTIER_H
#define SYMBREGEXERCISE_PARETOFRONTIER_H

#include <string>
#include <vector>
#include <ecf/ECF.h>

using namespace std;

struct ps {
    double fitness;
    int treeSize;
    int noTrees;
    string representation;
};
typedef struct ps ParetoSolution;

class ParetoFrontier {
public:
    ParetoFrontier();
    ParetoFrontier(string basicString);
    void writeParetoFront();
    void updateParetoFront(Tree::Tree* tree, double newTreeFitness);
    void updateParetoFront(vector<Tree::Tree*> trees, double newTreeFitness);
    ParetoSolution createNewSolution(double fitness, int size, int noTrees, string representation);
    void reset();
private:
    vector<ParetoSolution> _paretoFront;
    string _paretoFrontFile;
    int precision = 6;
    double multiplier;
    void tryAddingNewSolutionToFront(ParetoSolution potential);
};


#endif
