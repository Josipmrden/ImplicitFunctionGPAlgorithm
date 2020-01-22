//
// Created by josipmrden on 21. 01. 2020..
//
#include <ecf/ECF.h>
#include <ecf/tree/Tree.h>
#include <regex>
#include <vector>
#include <examples/SymbRegExercise/implicit_functions/parameter_nodes/ParametersBuilder.h>
#include <examples/SymbRegExercise/implicit_functions/ParetoFrontier.h>
#include <examples/SymbRegExercise/implicit_functions/ImplicitEvaluationWithPartialDerivatives.h>

using namespace std;

map<string, string> oneValueAlgorithmParameters;
map<string, vector<string>> multipleValueAlgorithmParameters;
map<string, string> oneValueGenotypeParameters;
map<string, vector<string>> multipleValueGenotypeParameters;
map<string, string> oneValueRegistryParameters;
map<string, vector<string>> multipleValueRegistryParameters;
vector<string> parametersByPriority;

void addParameter(string type, string name, vector<string> values) {
    int size = values.size();
    if (type == "algorithm") {
        if (size != 1)
        {
            multipleValueAlgorithmParameters[name] = values;
        }
        else
        {
            oneValueAlgorithmParameters[name] = values[0];
        }
    }
    else if (type == "genotype")
    {
        if (size != 1) {
            multipleValueGenotypeParameters[name] = values;
        }
        else
        {
            oneValueGenotypeParameters[name] = values[0];
        }
    }
    else
    {
        if (size != 1)
        {
            multipleValueRegistryParameters[name] = values;
        }
        else
        {
            oneValueRegistryParameters[name] = values[0];
        }
    }
}

void parseCVFile(string cvFileName) {
    ifstream infile(cvFileName);

    regex commandLineRegex("(\"[^\"]+\"|[^\\s\"]+)");
    for (string line; getline(infile, line);)
    {
        auto words_begin = sregex_iterator (line.begin(), line.end(), commandLineRegex);
        auto words_end = sregex_iterator ();

        string parameterType;
        string parameterName;
        vector<string> parameterValues;

        int j = 0;
        for (sregex_iterator i = words_begin; i != words_end; i++)
        {
            smatch match = *i;
            string match_str = match.str();

            if (match_str[0] == '\"' && match_str[match_str.size() - 1] == '\"')
            {
                match_str = match_str.substr(1, match_str.size() - 2);
            }
            if (j == 0)
            {
                parameterType = match_str;
            }
            else if (j == 1)
            {
                parameterName = match_str;
            }
            else
            {
                parameterValues.push_back(match_str);
            }
            j++;
        }

        if (parameterValues.size() != 1)
        {
            parametersByPriority.push_back(parameterName);
        }
        addParameter(parameterType, parameterName, parameterValues);
    }
}

void setInitialParameterValues(ParametersBuilder* builder)
{
    builder->fillDefault();

    map<string, string>::iterator it;
    map<string, vector<string>>::iterator multipleIt;
    for (it = oneValueAlgorithmParameters.begin(); it != oneValueAlgorithmParameters.end(); it++)
    {
        builder->addAlgorithmEntry(it->first, it->second);
    }
    for (it = oneValueGenotypeParameters.begin(); it != oneValueGenotypeParameters.end(); it++)
    {
        builder->addGenotypeEntry(it->first, it->second);
    }
    for (it = oneValueRegistryParameters.begin(); it != oneValueRegistryParameters.end(); it++)
    {
        builder->addRegistryEntry(it->first, it->second);
    }
    for (multipleIt = multipleValueAlgorithmParameters.begin(); multipleIt != multipleValueAlgorithmParameters.end(); multipleIt++)
    {
        builder->addAlgorithmEntry(multipleIt->first, (multipleIt->second)[0]);
    }
    for (multipleIt = multipleValueGenotypeParameters.begin(); multipleIt != multipleValueGenotypeParameters.end(); multipleIt++)
    {
        builder->addGenotypeEntry(multipleIt->first, (multipleIt->second)[0]);
    }
    for (multipleIt = multipleValueRegistryParameters.begin(); multipleIt != multipleValueRegistryParameters.end(); multipleIt++)
    {
        builder->addRegistryEntry(multipleIt->first, (multipleIt->second)[0]);
    }
}

vector<string> findParameterValues(string parameter)
{
    if (multipleValueAlgorithmParameters.find(parameter) != multipleValueAlgorithmParameters.end()) {
        return multipleValueAlgorithmParameters[parameter];
    }
    if (multipleValueGenotypeParameters.find(parameter) != multipleValueGenotypeParameters.end()) {
        return multipleValueGenotypeParameters[parameter];
    }
    if (multipleValueRegistryParameters.find(parameter) != multipleValueRegistryParameters.end()) {
        return multipleValueRegistryParameters[parameter];
    }

    return vector<string>();
}

void addParameterValue(ParametersBuilder* builder, string parameter, string value) {
    if (multipleValueAlgorithmParameters.find(parameter) != multipleValueAlgorithmParameters.end())
    {
        builder->addAlgorithmEntry(parameter, value);
    }
    else if (multipleValueGenotypeParameters.find(parameter) != multipleValueGenotypeParameters.end())
    {
        builder->addGenotypeEntry(parameter, value);
    } else if (multipleValueRegistryParameters.find(parameter) != multipleValueRegistryParameters.end())
    {
        builder->addRegistryEntry(parameter, value);
    }
}

//run with parameters (file from program arguments)
int main(int argc, char **argv)
{
    string paramsFileName = argv[1];
    string cvFileName = argv[2];
    string datasetFileName = argv[3];
    string paretoFrontFileName = argv[4];

    parseCVFile(cvFileName);

    auto* builder = new ParametersBuilder(paramsFileName);
    setInitialParameterValues(builder);

    StateP state (new State); //init on stack
    TreeP tree (new Tree::Tree);
    state->addGenotype(tree);
    auto* frontier = new ParetoFrontier(paretoFrontFileName);
    auto* implicitEvalOp = new ImplicitEvaluationWithPartialDerivatives(datasetFileName, frontier);
    state->setEvalOp(implicitEvalOp);

    for (const string& parameter : parametersByPriority)
    {
        vector<string> parameterValues = findParameterValues(parameter);
        int minFitnessIndex = -1;
        double minFitness = -1;
        for (int i = 0; i < parameterValues.size(); i++)
        {
            string value = parameterValues[i];
            addParameterValue(builder, parameter, value);
            builder->writeParameters();
            state->initialize(argc, argv);
            state->run();

            std::vector<IndividualP> hof = state->getHoF()->getBest();
            IndividualP ind = hof[0];
            double fitness = state->getEvalOp()->evaluate(ind)->getValue();
            if (i == 0 || fitness < minFitness) {
                minFitnessIndex = i;
                minFitness = fitness;
            }
        }
        addParameterValue(builder, parameter, parameterValues[minFitnessIndex]);
    }

    frontier->writeParetoFront();

    return 0;
}


