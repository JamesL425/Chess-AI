//
// Created by jimbo on 10/08/2022.
//

#ifndef AI_FCLAYER_H
#define AI_FCLAYER_H

#include "utilities.h"

class FCLayer {
private:
    int input_size;
    int output_size;

    Eigen::MatrixXd weights;
    Eigen::VectorXd biases;
    activation_function a_func;

public:
    FCLayer() = default;
    FCLayer(int input_size_, int output_size_, activation_function a_func_);
    FCLayer(int input_size_, int output_size_, activation_function a_func_,
            std::vector<std::string>& weight_lines, std::string& bias_line);

    FCLayer(const FCLayer& FCLayer) = default;
    FCLayer(FCLayer&& FCLayer) = default;
    FCLayer& operator = (const FCLayer& other) = default;

    Eigen::VectorXd propagate_forward(Eigen::VectorXd& input_vector);
    Eigen::VectorXd backpropagate(Eigen::VectorXd& input_vector, Eigen::VectorXd& output_vector, Eigen::VectorXd& cost_vector);

    void randomise_values();
    void slightly_alter_values(int div_factor);
    std::string as_string();
};

#endif //AI_FCLAYER_H
