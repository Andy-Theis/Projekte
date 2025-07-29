// neural_net.hpp
#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class NeuralNet {
public:
    NeuralNet(int input_size, int hidden_size, int output_size);
    std::vector<double> feedforward(const std::vector<double>& input);
    void train(const std::vector<std::vector<double>>& inputs,
               const std::vector<std::vector<double>>& targets,
               int epochs, double learning_rate);

private:
    int input_size, hidden_size, output_size;
    std::vector<std::vector<double>> weights_input_hidden;
    std::vector<std::vector<double>> weights_hidden_output;

    std::vector<double> sigmoid(const std::vector<double>& x);
    std::vector<double> sigmoid_derivative(const std::vector<double>& x);
    double rand_weight();
};

#endif

// neural_net.cpp
#include "neural_net.hpp"

NeuralNet::NeuralNet(int input_size_, int hidden_size_, int output_size_)
    : input_size(input_size_), hidden_size(hidden_size_), output_size(output_size_) {
    weights_input_hidden.resize(input_size, std::vector<double>(hidden_size));
    weights_hidden_output.resize(hidden_size, std::vector<double>(output_size));

    for (auto& row : weights_input_hidden)
        for (auto& w : row) w = rand_weight();
    for (auto& row : weights_hidden_output)
        for (auto& w : row) w = rand_weight();
}

double NeuralNet::rand_weight() {
    return ((double) rand() / RAND_MAX) * 2 - 1; // [-1, 1]
}

std::vector<double> NeuralNet::sigmoid(const std::vector<double>& x) {
    std::vector<double> result;
    for (double val : x)
        result.push_back(1.0 / (1.0 + exp(-val)));
    return result;
}

std::vector<double> NeuralNet::sigmoid_derivative(const std::vector<double>& x) {
    std::vector<double> result;
    for (double val : x)
        result.push_back(val * (1 - val));
    return result;
}

std::vector<double> NeuralNet::feedforward(const std::vector<double>& input) {
    std::vector<double> hidden(hidden_size);
    for (int j = 0; j < hidden_size; ++j)
        for (int i = 0; i < input_size; ++i)
            hidden[j] += input[i] * weights_input_hidden[i][j];
    hidden = sigmoid(hidden);

    std::vector<double> output(output_size);
    for (int k = 0; k < output_size; ++k)
        for (int j = 0; j < hidden_size; ++j)
            output[k] += hidden[j] * weights_hidden_output[j][k];
    output = sigmoid(output);
    return output;
}

void NeuralNet::train(const std::vector<std::vector<double>>& inputs,
                      const std::vector<std::vector<double>>& targets,
                      int epochs, double lr) {
    for (int e = 0; e < epochs; ++e) {
        for (size_t n = 0; n < inputs.size(); ++n) {
            std::vector<double> hidden(hidden_size);
            for (int j = 0; j < hidden_size; ++j)
                for (int i = 0; i < input_size; ++i)
                    hidden[j] += inputs[n][i] * weights_input_hidden[i][j];
            hidden = sigmoid(hidden);

            std::vector<double> output(output_size);
            for (int k = 0; k < output_size; ++k)
                for (int j = 0; j < hidden_size; ++j)
                    output[k] += hidden[j] * weights_hidden_output[j][k];
            output = sigmoid(output);

            std::vector<double> output_error(output_size);
            for (int k = 0; k < output_size; ++k)
                output_error[k] = targets[n][k] - output[k];
            std::vector<double> output_delta = sigmoid_derivative(output);
            for (int k = 0; k < output_size; ++k)
                output_delta[k] *= output_error[k];

            std::vector<double> hidden_error(hidden_size, 0.0);
            for (int j = 0; j < hidden_size; ++j)
                for (int k = 0; k < output_size; ++k)
                    hidden_error[j] += output_delta[k] * weights_hidden_output[j][k];
            std::vector<double> hidden_delta = sigmoid_derivative(hidden);
            for (int j = 0; j < hidden_size; ++j)
                hidden_delta[j] *= hidden_error[j];

            for (int j = 0; j < hidden_size; ++j)
                for (int k = 0; k < output_size; ++k)
                    weights_hidden_output[j][k] += lr * output_delta[k] * hidden[j];
            for (int i = 0; i < input_size; ++i)
                for (int j = 0; j < hidden_size; ++j)
                    weights_input_hidden[i][j] += lr * hidden_delta[j] * inputs[n][i];
        }
    }
}
