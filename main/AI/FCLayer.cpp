//
// Created by jimbo on 10/08/2022.
//

#include "FCLayer.h"

FCLayer::FCLayer(int input_size_, int output_size_, activation_function a_func_) {
    input_size = input_size_;
    output_size = output_size_;

    a_func = a_func_;

    weights = get_random_matrix(output_size, input_size, 1);
    biases = get_random_matrix(output_size, 1, 1);
}

FCLayer::FCLayer(int input_size_, int output_size_, activation_function a_func_,
                 std::vector<std::string>& weight_lines, std::string& bias_line) {
    input_size = input_size_;
    output_size = output_size_;

    a_func = a_func_;

    weights.resize(output_size, input_size);
    biases.resize(output_size, 1);

    for (int row = 0; row < output_size; ++row) {
        std::vector<std::string> weight_vals = split_string(weight_lines[row], ' ');
        for (int col = 0; col < input_size; ++col) {
            weights(row, col) = std::stod(weight_vals[col]);
        }
    }

    std::vector<std::string> bias_vals = split_string(bias_line, ' ');
    for (int row = 0; row < output_size; ++row) {
        biases(row, 0) = std::stod(bias_vals[row]);
    }
}

Eigen::VectorXd FCLayer::propagate_forward(Eigen::VectorXd& input_mat) {
    return (weights * input_mat + biases).unaryExpr(a_func);
}

Eigen::VectorXd FCLayer::backpropagate(Eigen::VectorXd& input_vector, Eigen::VectorXd& output_vector, Eigen::VectorXd& cost_vector) {
    const double learning_rate = 0.01;

    for (int i = 0; i < cost_vector.size(); ++i) {
        cost_vector[i] *= tanh_prime(output_vector[i]);
    }

    Eigen::MatrixXd delta_weights = cost_vector * input_vector.transpose();
    Eigen::MatrixXd delta_biases = cost_vector;

    Eigen::MatrixXd out = weights.transpose() * cost_vector;

    weights -= delta_weights * learning_rate;
    biases -= delta_biases * learning_rate;

    weights.cwiseMax(1);
    weights.cwiseMin(-1);
    biases.cwiseMax(1);
    biases.cwiseMin(-1);

    return out;
}

void FCLayer::randomise_values() {
    weights = get_random_matrix(output_size, input_size, 1);
    biases = get_random_matrix(output_size, 1, 1);
}

void FCLayer::slightly_alter_values(int div_factor) {
    Eigen::MatrixXd weight_changes = get_random_matrix(output_size, input_size, div_factor);
    Eigen::MatrixXd bias_changes = get_random_matrix(output_size, 1, div_factor);

    weights += weight_changes;
    biases += bias_changes;

    weights.cwiseMax(1);
    weights.cwiseMin(-1);

    biases.cwiseMax(1);
    biases.cwiseMin(-1);
}

std::string FCLayer::as_string() {
    std::string out_str{};

    out_str += "FCLayer\n";
    out_str += std::to_string(input_size) + "\n";
    out_str += std::to_string(output_size) + "\n";

    out_str += "Weights:\n";

    for (int row = 0; row < weights.rows(); ++row) {
        for (int col = 0; col < weights.cols(); ++col) {
            out_str += std::to_string(weights(row, col));
            out_str += " ";
        }

        out_str.pop_back();
        out_str += "\n";
    }

    out_str += "Biases:\n";

    for (auto bias : biases) {
        out_str += std::to_string(bias) + " ";
    }

    out_str += "\n";

    return out_str;
}
