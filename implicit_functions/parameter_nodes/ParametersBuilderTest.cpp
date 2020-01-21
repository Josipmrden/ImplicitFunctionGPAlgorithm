#include <ecf/ECF.h>
#include "examples/SymbRegExercise/implicit_functions/parameter_nodes/ParametersBuilder.h"
#include <ecf/tree/Tree.h>
typedef std::string String;

//run with parameters (file from program arguments)
int main(int argc, char **argv)
{
    std::string parametersFileName = argv[1];
    ParametersBuilder *builder = new ParametersBuilder(parametersFileName);
    builder->fillDefault();
    builder->setAlgorithm("SteadyStateTournament");
    builder->setGenotype("Tree");
    builder->addGenotypeEntry("maxdepth", "4");
    builder->writeParameters();
}


