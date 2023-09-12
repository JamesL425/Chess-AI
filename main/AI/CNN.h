//
// Created by jimbo on 09/08/2022.
//

#ifndef AI_CNN_H
#define AI_CNN_H

#include "FCNN.h"
#include "ConvLayer.h"
#include "PoolingLayer.h"

class CNN {
private:
    ConvLayer conv_layer;
    PoolingLayer pooling_layer{};
    FCNN fcnn;

public:
    CNN() = default;
    CNN(std::vector<size> &lens_sizes, std::vector<int> &layer_structure, activation_function a_func);
    explicit CNN(const std::string& file_name);
    CNN(const CNN& neural_network) = default;
    CNN(CNN&& neural_network) = default;
    CNN& operator = (const CNN& other) = default;

    double evaluate(Eigen::MatrixXd input_mat);
    double alpha_beta(Position& position, int depth, double alpha, double beta, bool is_maximising);
    Move get_best_move(Position position, int depth);

    void randomise_values();
    void slightly_alter_values(int div_factor);
    void slightly_alter_other(const CNN& other, int div_factor);

    void write_to_file(const std::string& file_name);
    std::string get_info_string();
};


#endif //AI_CNN_H
