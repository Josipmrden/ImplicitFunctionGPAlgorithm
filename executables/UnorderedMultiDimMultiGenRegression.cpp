#include <ecf/ECF.h>
#include "../implicit_functions/ParetoFrontier.h"
#include <ecf/tree/Tree.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimMultiGenEval.h>

using namespace std;

int main(int argc, char **argv)
{
    string datasetFileName = argv[2];
    string paretoFrontFileName = argv[3];

    StateP state (new State); //init on stack
    TreeP tree (new Tree::Tree);
    state->addGenotype(tree);

    auto* frontier = new ParetoFrontier(paretoFrontFileName);
    auto* unorderedEvaluation = new UnorderedMultiDimMultiGenEval(datasetFileName, frontier, state);

    state->setEvalOp(unorderedEvaluation);
    state->initialize(argc, argv);
    state->run();

    frontier->writeParetoFront();

    return 0;
}


