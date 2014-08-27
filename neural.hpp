#ifndef _NEURAL_
#define _NEURAL_

#include <Eigen/Core>
using namespace Eigen;
using namespace std;

#define DAYS_TO_REMEMBER 14
#define FEATURES_PER_DAY 5

#define INPUT_FEATURES DAYS_TO_REMEMBER * FEATURES_PER_DAY
#define HIDDEN_NODES INPUT_FEATURES / 2
#define N_EXAMPLES 1
#define N_OUTS 3 // buy, hold, sell

// Must have bias node pre-set
typedef Matrix<float, N_EXAMPLES, INPUT_FEATURES + 1> Input;
typedef Matrix<float, N_EXAMPLES, N_OUTS> Output;

class NeuralNet {
public:
	NeuralNet();
	// Returns prediction for Input in
	Output predict(Input in);
	// Returns new cost after updating
	float updateStochastic(Input in, Output out, float step, float reg);
private:
	Matrix<float, INPUT_FEATURES, HIDDEN_NODES> hiddenWeights;
	Matrix<float, HIDDEN_NODES, N_OUTS> outWeights;
};

#endif