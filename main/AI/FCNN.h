//
// Created by jimbo on 09/08/2022.
//

#ifndef AI_FCNN_H
#define AI_FCNN_H


#include "FCLayer.h"
#include <utility>
#include <algorithm>
#include <fstream>


class FCNN {
private:
    std::vector<FCLayer> layers{};

public:
    FCNN() = default;
    explicit FCNN(std::string& input_string);
    explicit FCNN(const std::string& file_name);
    FCNN(std::vector<int>& layer_structure, activation_function a_func);
    FCNN(const FCNN& neural_network) = default;
    FCNN(FCNN&& neural_network) = default;
    FCNN& operator = (const FCNN& other) = default;

    double evaluate(Eigen::VectorXd input_vector);
    Move get_best_move(Position position);

    std::vector<Eigen::VectorXd> generate_run(Eigen::VectorXd input_vector);

    void backpropagate(std::vector<Eigen::VectorXd> run, Eigen::VectorXd cost_vector);

    void randomise_values();
    void slightly_alter_values(int div_factor);
    void slightly_alter_other(const FCNN& other, int div_factor);
    std::string as_string();

    void write_to_file(const std::string &file_name);
};

#endif //AI_FCNN_H
