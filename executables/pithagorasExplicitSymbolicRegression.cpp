#include <ecf/ECF.h>
#include "examples/SymbRegExercise/simple_regression/PithagoraSymbRegEvalOp.h"

//run with parameters (file from program arguments)
int main(int argc, char **argv)
{
	StateP state (new State); //init on stack

	std::string datasetFileName = argv[2];

	PithagoraSymbRegEvalOp* pithagoraEvalOp = new PithagoraSymbRegEvalOp(datasetFileName);
	// set the evaluation operator
	state->setEvalOp(pithagoraEvalOp);

	//initialize state for running
	state->initialize(argc, argv);
	state->run();

	vector<IndividualP> p;
	p = state->getHoF()->getBest();

	return 0;
}
