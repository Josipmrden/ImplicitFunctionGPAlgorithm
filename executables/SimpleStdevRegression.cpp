//
// Created by josipmrden on 01. 03. 2020..
//

#include <ecf/ECF.h>
#include <ecf/tree/Tree.h>
#include <examples/SymbRegExercise/direct_methods/SimpleStdevEvaluator.h>
using namespace std;

//run with parameters (file from program arguments)
int main(int argc, char **argv)
{
    string datasetFileName = argv[2];
    string paretoFrontFileName = argv[3];

    StateP state (new State); //init on stack
    TreeP tree (new Tree::Tree);
    state->addGenotype(tree);

    auto* paretoFrontier = new ParetoFrontier(paretoFrontFileName);
    auto* simpleStdevEvalOp = new SimpleStdevEvaluator(state, datasetFileName, paretoFrontier);

    state->setEvalOp(simpleStdevEvalOp);
    state->initialize(argc, argv);
    state->run();

    vector<IndividualP> hof = state->getHoF()->getBest();
    IndividualP ind = hof[0];
    cout << ind->toString();

    state->getEvalOp()->evaluate(ind);


    return 0;
}
