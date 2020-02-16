#ifndef SYMBREGEXERCISE_PARETOFRONTIER_H
#define SYMBREGEXERCISE_PARETOFRONTIER_H
#include <string>
#include <vector>
#include <ecf/ECF.h>

struct ps {
    double fitness;
    int treeSize;
    std::string representation;
};
typedef struct ps ParetoSolution;

class ParetoFrontier {
public:
    ParetoFrontier(std::string basicString);
    void writeParetoFront();
    void updateParetoFront(Tree::Tree* tree, double newTreeFitness, const std::string& representation);
    ParetoSolution createNewSolution(double fitness, int size, std::string representation);

private:
    std::vector<ParetoSolution> _paretoFront;
    std::string _paretoFrontFile;
    int precision = 6;
    double multiplier;
};


#endif
