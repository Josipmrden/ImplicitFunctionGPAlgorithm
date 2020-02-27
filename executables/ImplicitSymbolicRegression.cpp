#include <ecf/ECF.h>
#include "ImplicitEvaluationWithPartialDerivatives.h"
#include "ParetoFrontier.h"
#include <ecf/tree/Tree.h>
typedef std::string String;

//run with parameters (file from program arguments)
int main(int argc, char **argv)
{
    String datasetFileName = argv[2];
    String paretoFrontFileName = argv[3];

    StateP state (new State); //init on stack
    TreeP tree (new Tree::Tree);
    state->addGenotype(tree);

    auto* frontier = new ParetoFrontier(paretoFrontFileName);
    auto* implicitEvalOp = new ImplicitEvaluationWithPartialDerivatives(datasetFileName, frontier);

    state->setEvalOp(implicitEvalOp);
    state->initialize(argc, argv);
    state->run();

    std::vector<IndividualP> hof = state->getHoF()->getBest();
    IndividualP ind = hof[0];
    std::cout << ind->toString();

    state->getEvalOp()->evaluate(ind);

    frontier->writeParetoFront();

    return 0;
}


