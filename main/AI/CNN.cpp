//
// Created by jimbo on 09/08/2022.
//

#include "CNN.h"

CNN::CNN(std::vector<size> &lens_sizes, std::vector<int> &layer_structure, activation_function a_func) {
    int n_matrices = static_cast<int>(lens_sizes.size());
    conv_layer = ConvLayer{size{8, 8}, n_matrices, lens_sizes};
    pooling_layer = PoolingLayer{n_matrices, a_func};
    fcnn = FCNN{layer_structure, a_func};
}

CNN::CNN(const std::string &file_name) {
    std::stringstream buffer;
    std::ifstream file(file_name);
    buffer << file.rdbuf();
    file.close();

    std::string cur_str;
    std::getline(buffer, cur_str, '@');

    conv_layer = ConvLayer{cur_str};

    pooling_layer = PoolingLayer{conv_layer.get_n_matrices(), &tanh};

    std::getline(buffer, cur_str, '@');
    std::getline(buffer, cur_str, '@');

    fcnn = FCNN{cur_str};

}

double CNN::evaluate(Eigen::MatrixXd input_mat) {
    std::vector<Eigen::MatrixXd> in_vector = conv_layer.propagate_forward(input_mat);
    Eigen::VectorXd out_vector = pooling_layer.propagate_forward(in_vector);

    return fcnn.evaluate(out_vector);
}

double CNN::alpha_beta(Position& position, int depth, double alpha, double beta, bool is_maximising) {
    int game_over_val = position.is_game_over();

    if (depth == 0 || game_over_val) {
        switch (game_over_val) {
            case 0:
                return evaluate(position_to_matrix(position));
            case 1:
                return 0.0;
            case 2:
                return (position.white_to_play()) ? -1.0 : 1.0;
        }
    }

    if (is_maximising) {
        double value = -1000;
        std::vector<Move> possible_moves = position.generate_moves();

        for (Move move : possible_moves) {
            position.play_move(move, false);

            value = std::max(value, alpha_beta(position, depth - 1, alpha, beta, false));

            position.undo_move(false);

            if (value >= beta) {
                break;
            }

            alpha = std::max(alpha, value);
        }

        return value;
    }

    else {
        double value = 1000;
        std::vector<Move> possible_moves = position.generate_moves();

        for (Move move : possible_moves) {
            position.play_move(move, false);

            value = std::min(value, alpha_beta(position, depth - 1, alpha, beta, true));

            position.undo_move(false);

            if (value <= alpha) {
                break;
            }

            beta = std::min(beta, value);
        }

        return value;
    }
}

Move select_node(Position& position) {
    std::vector<Move> possible_moves = position.generate_moves();
    std::vector<double> evals{};

    for (Move move : possible_moves) {
        position.play_move(move, false);

        evals.emplace_back(); //U

        position.undo_move(false);
    }

    long long best_move_loc = ((position.white_to_play()) ? std::max_element(evals.begin(), evals.end()) : std::min_element(evals.begin(), evals.end())) - evals.begin();

    return possible_moves[best_move_loc];
}

Move CNN::get_best_move(Position position, int depth) {
    std::vector<Move> possible_moves = position.generate_moves();
    std::vector<double> evals{};

    for (Move move : possible_moves) {
        position.play_move(move, false);

        evals.emplace_back(alpha_beta(position, depth, 1000, 1000, (position.white_to_play())));

        position.undo_move(false);
    }

    long long best_move_loc = ((position.white_to_play()) ? std::max_element(evals.begin(), evals.end()) : std::min_element(evals.begin(), evals.end())) - evals.begin();

    return possible_moves[best_move_loc];
}

void CNN::randomise_values() {
    conv_layer.randomise_values();

    fcnn.randomise_values();

//    piece_weights = get_random_piece_array(1);
}

void CNN::slightly_alter_values(int div_factor) {
    conv_layer.slightly_alter_values(div_factor);

    fcnn.slightly_alter_values(div_factor);

//    std::array<double, 13> add_array = get_random_piece_array(1);
//    add_other_array(piece_weights, add_array);
}

void CNN::slightly_alter_other(const CNN &other, int div_factor) {
    conv_layer = other.conv_layer;
    pooling_layer = other.pooling_layer;
    fcnn = other.fcnn;
//    piece_weights = other.piece_weights;

    conv_layer.slightly_alter_values(div_factor);

    fcnn.slightly_alter_values(div_factor);

//    std::array<double, 13> add_array = get_random_piece_array(10);
//    add_other_array(piece_weights, add_array);
}

void CNN::write_to_file(const std::string &file_name) {
    std::ofstream file(file_name);

//    file << "Piece Weights:\n";
//    for (auto piece_weight : piece_weights) {
//        file << piece_weight << " ";
//    }
//
//    file << "\n";

    file << conv_layer.as_string() << "@\n";
    file << PoolingLayer::as_string() << "@\n";

    file << fcnn.as_string() << "@\n";

    file.close();
}

std::string CNN::get_info_string() {
    std::string out_string {};

    out_string += conv_layer.as_string() + "@\n";
    out_string += PoolingLayer::as_string() + "@\n";
    out_string += fcnn.as_string() + "@\n";

    return out_string;
}
