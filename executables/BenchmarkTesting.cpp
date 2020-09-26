#include <ecf/ECF.h>
#include "../implicit_functions/ParetoFrontier.h"
#include <ecf/tree/Tree.h>
#include <examples/SymbRegExercise/implicit_functions/cross_validation/InstanceRunner.h>
#include <examples/SymbRegExercise/direct_methods/SimpleStdevEvaluator.h>
#include <examples/SymbRegExercise/direct_methods/MultipleTreeEvaluator.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimImplicitEvaluation.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimMultiGenEval.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/GepEvaluation.h>
#include <examples/SymbRegExercise/ap/APEvalOp.h>
#include "../gep/GEPChromosome.h"
#include "../gep/AlgGEP.h"

using namespace std;

TweakableParameters createTweakableParams()
{
    return TweakableParameters();
}

int main(int argc, char **argv) {
    string logDirectory = argv[2];
    string evaluatorString = argv[3];
    string population = argv[4];
    string dataset = argv[5];

    StateP state(new State);
    state->addAlgorithm((GEPP)new GEP());
    GEPChromosome::GEPChromosome *chromosome =  new GEPChromosome::GEPChromosome();
    state->addGenotype((GEPChromosomeP) chromosome);

    auto *mte = new MultipleTreeEvaluator();
    auto *sse = new SimpleStdevEvaluator();
    auto *umdie = new UnorderedMultiDimImplicitEvaluation();
    auto *umdmge = new UnorderedMultiDimMultiGenEval();
    auto* gep = new GepEvaluation();
    auto* ap = new APEvalOp();

    vector<AbstractEvaluateOp *> evaluators = {sse, mte, umdie, umdmge, gep};
    AbstractEvaluateOp *evaluator;

    int noRuns = 10;
    string functionset = "+ - * /";
    string gaussFunctionSet = "+ - * / sin exp sqrt ngt";

    if (evaluatorString == "sse") {
        evaluator = sse;
    } else if (evaluatorString == "mte") {
        evaluator = mte;
    } else if (evaluatorString == "umdie") {
        evaluator = umdie;
    } else if (evaluatorString == "umdmge") {
        evaluator = umdmge;
    } else if (evaluatorString == "gep") {
        evaluator = gep;
    } else if (evaluatorString == "ap")
    {
        evaluator = ap;
    }

    vector<DatasetInfo> datasets;
    string pointsDatasetDir = "./chosen_points_datasets/";
    string planeDatasetDir = "./chosen_plane_datasets/";
    string datasetType = pointsDatasetDir;
    //argv[1] = "parameters.txt";

    if (evaluator->requiresPlanes) {
        datasetType = planeDatasetDir;
    }

    if (evaluator->noTrees > 1) {
        argv[1] = "multipleGenotypeParameters.txt";
    }

    if (dataset == "0") {
        datasets.push_back(
                DatasetInfo("CIRCLE_4_0_0_100_150", datasetType + "circle_4_0_0_100_150", "X Y 4", functionset));
    } else if (dataset == "1") {
        datasets.push_back(
                DatasetInfo("SPHERE_5_0_0_250_300", datasetType + "sphere_5_0_0_250_300", "X Y Z 5", functionset));
    } else if (dataset == "2") {
        datasets.push_back(
                DatasetInfo("ELLIPSE_1_2_3_4_150_200", datasetType + "ellipse_1_2_3_4_150_200", "X Y 1 2 3 4",
                            functionset));
    } else if (dataset == "3") {
        datasets.push_back(
                DatasetInfo("HYPERBOLA_1.5_100_150", datasetType + "hyperbola_1.5_100_150", "X Y 1.5", functionset));
    } else if (dataset == "4") {
        datasets.push_back(DatasetInfo("HARMOSC_50_100", datasetType + "harmosc_50_100", "X Y Z 0.1 3", functionset));
    } else if (dataset == "5") {
        datasets.push_back(
                DatasetInfo("NONLINHARMOSC_33_38", datasetType + "nlhmo_33_38", "X Y Z 0.1 9.8", functionset));
    }else if (dataset == "6") {
        datasets.push_back(
                DatasetInfo("TESTCIRCLE_100_150", datasetType + "moved_circle_6_1_2_100_150", "X Y 6 1 2", functionset));
    }else if (dataset == "7") {
        datasets.push_back(
                DatasetInfo("TESTELLIPSE_100_150", datasetType + "moved_ellipse_2.5_5_3_4.5_100_150", "X Y 2.5 5 3 4.5 1", functionset));
    }
    else if (dataset == "8")
    {
        datasets.push_back(
                DatasetInfo("TESTGAUSS_200_220", datasetType + "gauss_200_220", "X Y 2 3.14159", gaussFunctionSet));
    } else if (dataset == "9")
    {
        datasets.push_back(DatasetInfo("TEST_CHALLENGE", datasetType + "I.18.4", "M1 M2 R1 R2 R", functionset));
    }

    TweakableParameters tp = createTweakableParams();

    if (population == "100")
    {
        tp.populationSizes = {100};
    }
    else if (population == "500")
    {
        tp.populationSizes = {500};
    }
    else if (population == "200")
    {
        tp.populationSizes = {200};
    }

    if (evaluatorString == "mte")
    {
        tp.mutationProbablities = {0.2};
    }

    auto *instanceRunner = new InstanceRunner(noRuns, datasets, evaluator, tp, argc, argv, state,
                                              logDirectory);

    while (tp.hasNext(TweakableType::Population)) {
        ParameterSet ps = tp.getNext(TweakableType::Population);
        instanceRunner->runInstance(ps);
        break;
    }

    return 0;
}


