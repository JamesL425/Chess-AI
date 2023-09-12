//
// Created by jimbo on 10/08/2022.
//

#include "PoolingLayer.h"

double average_pool(const Eigen::MatrixXd& input_mat) {
    return input_mat.mean();
}

double max_pool(const Eigen::MatrixXd& input_mat) {
    return input_mat.maxCoeff();
}

PoolingLayer::PoolingLayer(int n_in_matrices_, activation_function a_func_) {
    n_in_matrices = n_in_matrices_;
    a_func = a_func_;
}

Eigen::VectorXd PoolingLayer::propagate_forward(const std::vector<Eigen::MatrixXd>& input_vector) const {
    std::vector<double> out_vector;

    for (const auto& mat : input_vector) {
        out_vector.emplace_back(a_func(average_pool(mat)));
    }

    return Eigen::VectorXd::Map(&out_vector[0], n_in_matrices);
}

std::string PoolingLayer::as_string() {
    return "PoolingLayer\n";
}

