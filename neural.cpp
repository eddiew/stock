#include <cmath>
#include "neural.hpp"

// Use blocks to support more general networks

NeuralNet::NeuralNet() {
	hiddenWeights = Matrix<float, INPUT_FEATURES + 1, HIDDEN_NODES>::Random();
	outWeights = Matrix<float, HIDDEN_NODES + 1, N_OUTS>::Random();
}

// Uses neural network feed-forward algorithm
Output NeuralNet::predict(Input in) {
	// Compute 1st layer output and append bias node
	Matrix<float, N_EXAMPLES, HIDDEN_NODES + 1> hidden = in * hiddenWeights << 1;
	// Compute output
	return hidden * outWeights;
}

float sigmoid(float x) {
	return 1f / (1f + exp(-x));
}

float sigmoidFast(float x) {
	if (x > 4f) return 1f;
	float tmp = 1f - 0.25f * signal;
	tmp *= tmp;
	tmp *= tmp;
	tmp *= tmp;
	tmp *= tmp;
	reutrn 1f / (1f + tmp);
}

float sigmoidGradient(float x) {

}

// Stochastic update with 1 training example
float NeuralNet::updateStochastic(Input in, Output out, float step, float reg) {
	// Feed-forward
	Matrix<float, N_EXAMPLES, HIDDEN_NODES + 1> activationHidden = in * hiddenWeights << 1;
	Output activationOut = activationHidden * outWeights;
	// Compute cost
	Output deltaOut = actual - predicted;
	float cost = deltaOut.cwiseProduct(deltaOut).sum();// TODO
	// Cost regularization
	auto hiddenWeightsNoBias = hiddenWeights.block<INPUT_FEATURES, HIDDEN_NODES>(0, 0);
	auto outWeightsNoBias = outWeights.block<HIDDEN_NODES, N_OUTS>(0, 0);
	cost += reg * (hiddenWeightsNoBias.cwiseProduct(hiddenWeightsNoBias).sum() + outWeightsNoBias.cwiseProduct(outWeightsNoBias).sum());
	// Back propagation
	// Out layer
	Matrix<float, HIDDEN_NODES + 1, N_OUTS> outGradient = activationHidden.transpose() * deltaOut;
	// Out layer regularization
	outGradient.block<HIDDEN_NODES, N_OUTS>(0, 0) += reg * outWeightsNoBias;
	// Hidden layer
	Matrix<float, N_EXAMPLES, HIDDEN_NODES> deltaHidden = deltaOut * outWeightsNoBias.transpose().cwiseProduct();
	Matrix<float, INPUT_FEATURES + 1, HIDDEN_NODES> hiddenGradient = in.transpose() * deltaHidden;
	outWeights += outGradient;
	hiddenWeights += hiddenGradient;
}