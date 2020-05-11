//
// Created by josipmrden on 23. 04. 2020..
//

#include "InstanceRunner.h"

InstanceRunner::InstanceRunner(int noRuns,
        vector<DatasetInfo> datasets,
        AbstractEvaluateOp *evaluateOp,
        TweakableParameters tp,
        int argc,
        char** argv,
        StateP state,
        string logDirectory)
{
    this->_noRuns = noRuns;
    this->_datasets = datasets;
    this->_evaluateOp = evaluateOp;
    this->_tweakableParameters = tp;
    this->argc = argc;
    this->argv = argv;
    this->_state = state;
    this->_logDirectory = logDirectory;
}

void createDirectory(string filePath)
{
    int status;
    string command = "mkdir -p " + filePath;
    status = system(command.c_str()); // Creating a directory
    if (status == -1)
        cerr << "Error : " << strerror(errno) << endl;
    else
        cout << "Directories are created: " << filePath << endl;
}

void InstanceRunner::runInstance(ParameterSet parameterSet)
{
    string logDir = this->_logDirectory;
    createDirectory(logDir);
    string evaluateOpName = _evaluateOp->getName();
    logDir += "/" + evaluateOpName;
    createDirectory(logDir);

    string parametersName = parameterSet.getName();
    for (int i = 0; i < _datasets.size(); i++)
    {
        DatasetInfo dataset = _datasets[i];
        string datasetLogDir = logDir + "/D_" + dataset.name;

        createDirectory(datasetLogDir);

        string datasetParameterLogDir = datasetLogDir + "/" + parametersName;
        createDirectory(datasetParameterLogDir);

        string paretoFrontFilename = datasetParameterLogDir + "/paretoFront.txt";
        auto* paretoFrontier = new ParetoFrontier(paretoFrontFilename);

        for (int j = 0; j < _noRuns; j++)
        {
            _evaluateOp = _evaluateOp->createNew();
            _evaluateOp->_datasetFileName = dataset.fileName;
            _evaluateOp->_paretoFrontier = paretoFrontier;

            string runLogDir = datasetParameterLogDir + "/" + to_string(j+1);
            string batchStatsString = runLogDir + "/stats.txt";
            string logFilenameString = runLogDir + "/log.txt";
            string milestoneFilenameString = runLogDir + "/m.txt";

            createDirectory(runLogDir);

            StateP state (new State);
            TreeP tree (new Tree::Tree);
            Tree::PrimitiveP exp(new Exp);
            tree->addFunction(exp);
            Tree::PrimitiveP sqrt(new Sqrt);
            tree->addFunction(sqrt);
            Tree::PrimitiveP square(new Square);
            tree->addFunction(square);
            Tree::PrimitiveP tanh(new Tanh);
            tree->addFunction(tanh);
            state->addGenotype(tree);
            _state = state;
            _state->setEvalOp(_evaluateOp);

            _state->initialize(argc, argv);

            for (int treeIndex = 0; treeIndex < _evaluateOp->noTrees; treeIndex++)
            {
                _state->getGenotypes()[treeIndex]->setParameterValue(
                        _state,
                        "terminalset",
                        (voidP) new string(dataset.terminalset));
                _state->getGenotypes()[treeIndex]->setParameterValue(
                        _state,
                        "functionset",
                        (voidP) new string(dataset.functionset));
            }

            _state->getRegistry()->modifyEntry("population.size", (voidP) &parameterSet.populationSize);
            _state->getRegistry()->modifyEntry("mutation.indprob", (voidP) &parameterSet.mutationProbablity);

            _state->getRegistry()->modifyEntry("batch.statsfile", (voidP) new string(batchStatsString));
            _state->getRegistry()->modifyEntry("log.filename", (voidP) new string (logFilenameString));
            _state->getRegistry()->modifyEntry("milestone.filename", (voidP) new string (milestoneFilenameString));
            _state->getPopulation()->initialize(_state);
            _state->run();
        }

        paretoFrontier->writeParetoFront();
    }
}