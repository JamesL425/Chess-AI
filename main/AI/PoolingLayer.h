//
// Created by jimbo on 10/08/2022.
//

#ifndef AI_POOLINGLAYER_H
#define AI_POOLINGLAYER_H

#include "utilities.h"

double average_pool(const Eigen::MatrixXd& input_mat);

double max_pool(const Eigen::MatrixXd& input_mat);

class PoolingLayer {
private:
    int n_in_matrices;
    activation_function a_func;

public:
    PoolingLayer() = default;
    PoolingLayer(int n_in_matrices_, activation_function a_func_);
    PoolingLayer(const PoolingLayer& pooling_layer) = default;
    PoolingLayer(PoolingLayer&& pooling_layer) = default;
    PoolingLayer& operator = (const PoolingLayer& other) = default;

    [[nodiscard]] Eigen::VectorXd propagate_forward(const std::vector<Eigen::MatrixXd>& input_vector) const;
    static std::string as_string();
};


#endif //AI_POOLINGLAYER_H
