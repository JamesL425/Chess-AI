//
// Created by jimbo on 10/08/2022.
//

#include "utilities.h"

int generate_random_int(int lo, int hi) {
    std::random_device rand_device;
    std::default_random_engine engine(rand_device());
    std::uniform_int_distribution<int> distribution(lo, hi);
    return distribution(engine);
}

std::vector<double> pieces_to_doubles(std::array<Piece, 64>& board) {
    std::vector<double> out_vec {};

    const double az_piece_weights[13] = {
            -1, -3.05, -3.33, -5.63, -9.5, -4,
            1, 3.05, 3.33, 5.63, 9.5, 4, 0
    };

    for (int sq = 0; sq < chess_constants::BOARD_SIZE; ++sq) {
        out_vec.emplace_back(az_piece_weights[board[sq]]);
    }

    return out_vec;
}

Eigen::VectorXd position_to_matrix(Position& position) {
    return Eigen::Map<Eigen::VectorXd>(&pieces_to_doubles(position.get_board())[0], 64);
}

Eigen::MatrixXd get_random_matrix(int rows, int cols, int div_factor) {
    std::random_device rand_device;
    std::default_random_engine engine(rand_device());
    std::uniform_real_distribution<double> distribution(-1, 1);

    return Eigen::MatrixXd::Zero(rows, cols)
            .unaryExpr(([&] (double) {return distribution(engine) / div_factor;}));
}

void pad_matrix_to_square(Eigen::MatrixXd& old_mat, size old_size, int new_size) {
    old_mat.conservativeResize(new_size, new_size);

    for (int i = 0; i < new_size; ++i) {
        old_mat.row(i).tail(new_size - old_size.second).setZero();
        old_mat.col(i).tail(new_size - old_size.first).setZero();
    }

    old_mat.bottomRightCorner(new_size - old_size.first, new_size - old_size.second).setZero();
}

Eigen::VectorXd block_to_vector(Eigen::MatrixXd block) {
    std::vector<double> temp_vec {};
    temp_vec.insert(temp_vec.end(), std::make_move_iterator(block.data()), std::make_move_iterator(block.data() + block.size()));
    return Eigen::VectorXd::Map(&temp_vec[0], temp_vec.size());
}

double ReLU(double inp) {
    return (inp > 0.0) ? inp : 0.0;
}

double sigmoid(double inp) {
    return pow(M_E, inp) / (1.0 + pow(M_E, inp));
}

double tanh_prime(double inp) {
    return 1.0 - (tanh(inp) * tanh(inp));
}

//std::array<double, 13> get_random_piece_array(int div_factor) {
//    std::array<double, 13> out_array {};
//    std::random_device rand_device;
//    std::default_random_engine engine(rand_device());
//    std::uniform_real_distribution<double> distribution(-1, 1);
//
//    for (auto& val : out_array) {
//        val = distribution(engine) / div_factor;
//    }
//
//    out_array[12] = 0;
//    return out_array;
//}
//
//void add_other_array(std::array<double, 13>& old_array, std::array<double, 13> add_array) {
//    for (int i = 0; i < old_array.size(); ++i) {
//        old_array[i] += add_array[i];
//    }
//}

std::vector<std::string> split_string(std::string& str, char delim) {
    std::vector<std::string> out_vec {};
    std::stringstream string_stream {str};

    for (std::string line; std::getline(string_stream, line, delim);) {
        out_vec.emplace_back(line);
    }

    return out_vec;
}

