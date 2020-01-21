#include <cmath>
#include <ecf/ECF.h>
#include "PithagoraSymbRegEvalOp.h"
using String = std::string;

std::vector<String> availableLetters = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};

PithagoraSymbRegEvalOp::PithagoraSymbRegEvalOp(std::string datasetFileName) {
    this->datasetFileName = datasetFileName;
}

// called only once, before the evolution � generates training data
bool PithagoraSymbRegEvalOp::initialize(StateP state)
{
    std::ifstream inputFileStream(this->datasetFileName);
    int sampleSize;
    int varCount;
    inputFileStream>>sampleSize;
    inputFileStream>>varCount;
    inputFileStream.ignore(1, '\n');

    for(int i = 0; i < sampleSize; i++) {
        String line;
        std::getline(inputFileStream, line);
        std::istringstream lineStream(line);

        std::vector<double> sampleVariables;
        for (int j = 0; j <= varCount; j++)
        {
            String variableString;
            std::getline(lineStream, variableString, ' ');
            double variableValue = std::stod(variableString);

            if (j != varCount)
            {
                sampleVariables.push_back(variableValue);
            }
            else
            {
                codomain.push_back(variableValue);
            }
        }
        domain.push_back(sampleVariables);
    }
}


FitnessP PithagoraSymbRegEvalOp::evaluate(IndividualP individual)
{
	// we try to minimize the function value, so we use FitnessMin fitness (for minimization problems)
	FitnessP fitness (new FitnessMin);

	// get the genotype we defined in the configuration file
	Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
	// (you can also use boost smart pointers:)
	//TreeP tree = boost::static_pointer_cast<Tree::Tree> (individual->getGenotype());

	int sampleSize = domain.size();
	int variablesSize = domain[0].size();

	double fitnessValue = 0.0;

	for(uint i = 0; i < sampleSize; i++) {
	    for (uint j = 0; j < variablesSize; j++) {
	        String variableString = availableLetters[j];
            tree->setTerminalValue(variableString, &(domain[i])[j]);
        }
	    double result;
	    tree->execute(&result);
	    double offset = codomain[i] - result;
        fitnessValue += offset * offset;
    }

	fitness->setValue(fitnessValue);

	return fitness;
}
