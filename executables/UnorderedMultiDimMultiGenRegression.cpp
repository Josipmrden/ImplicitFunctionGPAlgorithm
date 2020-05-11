#include <ecf/ECF.h>
#include "../implicit_functions/ParetoFrontier.h"
#include <ecf/tree/Tree.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimMultiGenEval.h>
#include <examples/SymbRegExercise/utils/primitives/Exp.h>
#include <examples/SymbRegExercise/utils/primitives/Sqrt.h>
#include <examples/SymbRegExercise/utils/primitives/Tanh.h>
#include <examples/SymbRegExercise/utils/primitives/Square.h>
#include <examples/SymbRegExercise/utils/primitives/Negated.h>


using namespace std;

int main(int argc, char **argv)
{
    string datasetFileName = argv[2];
    string paretoFrontFileName = argv[3];

    StateP state (new State); //init on stack
    TreeP tree (new Tree::Tree);

    Tree::PrimitiveP exp(new Exp);
    tree->addFunction(exp);
    Tree::PrimitiveP sqrt(new Sqrt);
    tree->addFunction(sqrt);
    Tree::PrimitiveP square(new Square);
    tree->addFunction(square);
    Tree::PrimitiveP tanh(new Tanh);
    tree->addFunction(tanh);
    Tree::PrimitiveP negated(new Negated);
    tree->addFunction(negated);

    state->addGenotype(tree);

    auto* frontier = new ParetoFrontier(paretoFrontFileName);
    auto* unorderedEvaluation = new UnorderedMultiDimMultiGenEval(datasetFileName, frontier, state);

    state->setEvalOp(unorderedEvaluation);
    state->initialize(argc, argv);
    state->run();

    frontier->writeParetoFront();

    return 0;
}


