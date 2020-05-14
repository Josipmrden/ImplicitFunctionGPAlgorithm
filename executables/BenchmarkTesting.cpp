#include <ecf/ECF.h>
#include "../implicit_functions/ParetoFrontier.h"
#include <ecf/tree/Tree.h>
#include <examples/SymbRegExercise/implicit_functions/cross_validation/InstanceRunner.h>
#include <examples/SymbRegExercise/direct_methods/SimpleStdevEvaluator.h>
#include <examples/SymbRegExercise/direct_methods/MultipleTreeEvaluator.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimImplicitEvaluation.h>
#include <examples/SymbRegExercise/implicit_functions/unordered/UnorderedMultiDimMultiGenEval.h>

using namespace std;

TweakableParameters createTweakableParams()
{
    return TweakableParameters();
}

int main(int argc, char **argv)
{
    string logDirectory = argv[2];
    string evaluatorString = argv[3];

    StateP state (new State);

    auto* mte = new MultipleTreeEvaluator();
    auto* sse = new SimpleStdevEvaluator();
    auto* umdie = new UnorderedMultiDimImplicitEvaluation();
    auto* umdmge = new UnorderedMultiDimMultiGenEval();

    vector<AbstractEvaluateOp*> evaluators = {sse, mte, umdie, umdmge};
    AbstractEvaluateOp* evaluator;

    int noRuns = 20;
    string functionset = "+ - * / sin";

    if (evaluatorString == "sse")
    {
        evaluator = sse;
    }
    else if (evaluatorString == "mte")
    {
        evaluator = mte;
    }
    else if (evaluatorString == "umdie")
    {
        evaluator = umdie;
    }
    else if (evaluatorString == "umdmge")
    {
        evaluator = umdmge;
    }

    vector<DatasetInfo> datasets;
    string pointsDatasetDir = "./chosen_points_datasets/";
    string planeDatasetDir = "./chosen_plane_datasets/";
    string datasetType = pointsDatasetDir;
    argv[1] = "parameters.txt";

    if (evaluator->requiresPlanes)
    {
        datasetType = planeDatasetDir;
    }

    if (evaluator->noTrees > 1)
    {
        argv[1] = "multipleGenotypeParameters.txt";
    }

    datasets.push_back(DatasetInfo("CIRCLE_4_0_0_100_150", datasetType + "circle_4_0_0_100_150", "X Y 4", functionset));
    datasets.push_back(DatasetInfo("SPHERE_5_0_0_250_300", datasetType + "sphere_5_0_0_250_300", "X Y Z 5", functionset));
    datasets.push_back(DatasetInfo("ELLIPSE_1_2_3_4_150_200", datasetType + "ellipse_1_2_3_4_150_200", "X Y 1 2 3 4", functionset));
    datasets.push_back(DatasetInfo("HYPERBOLA_1.5_100_150", datasetType + "hyperbola_1.5_100_150", "X Y 1.5", functionset));
    datasets.push_back(DatasetInfo("HARMOSC_50_100", datasetType + "harmosc_50_100", "X Y Z 0.1 3", functionset));
    datasets.push_back(DatasetInfo("NONLINHARMOSC_33_38", datasetType + "nlhmo_33_38", "X Y Z 0.1 9.8", functionset));

    TweakableParameters tp = createTweakableParams();

    auto *instanceRunner = new InstanceRunner(noRuns, datasets, evaluator, tp, argc, argv, state,
                                              logDirectory);

    while (tp.hasNext(TweakableType::Population)) {
        ParameterSet ps = tp.getNext(TweakableType::Population);
        instanceRunner->runInstance(ps);
    }

    return 0;
}


