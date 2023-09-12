//
// Created by jimbo on 10/08/2022.
//

#ifndef AI_UTILITIES_H
#define AI_UTILITIES_H

#include <random>
#include <cmath>
#include <Eigen/Dense>
#include "move_generator/position.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

typedef std::pair<int, int> size;

int generate_random_int(int lo, int hi);

std::vector<double> pieces_to_doubles(std::array<Piece, 64>& board);

Eigen::VectorXd position_to_matrix(Position& position);

Eigen::MatrixXd get_random_matrix(int rows, int cols, int div_factor);

void pad_matrix_to_square(Eigen::MatrixXd& old_mat, size old_size, int new_size);

Eigen::VectorXd block_to_vector(Eigen::MatrixXd block);

typedef double (*activation_function) (double);
double ReLU(double inp);
double sigmoid(double inp);
double tanh_prime(double inp);

//std::array<double, 13> get_random_piece_array(int div_factor);
//void add_other_array(std::array<double, 13>& old_array, std::array<double, 13> add_array);

std::vector<std::string> split_string(std::string& str, char delim);

#endif //AI_UTILITIES_H
