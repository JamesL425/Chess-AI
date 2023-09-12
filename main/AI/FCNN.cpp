//
// Created by jimbo on 09/08/2022.
//

#include "FCNN.h"

FCNN::FCNN(std::vector<int>& layer_structure, activation_function a_func) {
    for (int i = 1; i < static_cast<int>(layer_structure.size()); ++i) {
        layers.emplace_back(FCLayer{layer_structure[i - 1], layer_structure[i], a_func});
    }
}

FCNN::FCNN(std::string &input_string) {
    std::vector<std::string> lines = split_string(input_string, '\n');
    int n_lines = static_cast<int>(lines.size());

    for (int i = 0; i < n_lines; ++i) {
        std::string cur_line = lines[i];

        if (cur_line == "\n" || cur_line == "@" || cur_line.empty()) {
            continue;
        }

        if (cur_line == "FCLayer") {
            ++i;
            int input_size = std::stoi(lines[i]);

            ++i;
            int output_size = std::stoi(lines[i]);

            i += 2;

            std::vector<std::string> weight_lines {};

            while (lines[i][0] == '0' || lines[i][0] == '-') {
                weight_lines.emplace_back(lines[i]);
                ++i;
            }

            ++i;
            std::string bias_line = lines[i];

            layers.emplace_back(FCLayer{input_size, output_size, &tanh, weight_lines, bias_line});

        }
    }
}

FCNN::FCNN(const std::string& file_name) {
    std::stringstream buffer;
    std::ifstream file(file_name);
    buffer << file.rdbuf();
    file.close();

    std::string cur_str;
    std::getline(buffer, cur_str, '@');

    std::vector<std::string> lines = split_string(cur_str, '\n');
    int n_lines = static_cast<int>(lines.size());

    for (int i = 0; i < n_lines; ++i) {
        std::string cur_line = lines[i];

        if (cur_line == "\n" || cur_line == "@" || cur_line.empty()) {
            continue;
        }

        if (cur_line == "FCLayer") {
            ++i;
            int input_size = std::stoi(lines[i]);

            ++i;
            int output_size = std::stoi(lines[i]);

            i += 2;

            std::vector<std::string> weight_lines {};

            while (lines[i][0] == '0' || lines[i][0] == '-') {
                weight_lines.emplace_back(lines[i]);
                ++i;
            }

            ++i;
            std::string bias_line = lines[i];

            layers.emplace_back(FCLayer{input_size, output_size, &tanh, weight_lines, bias_line});

        }
    }
}

double FCNN::evaluate(Eigen::VectorXd input_vector) {
    Eigen::VectorXd out_vector = std::move(input_vector);

    for (FCLayer layer : layers) {
        out_vector = layer.propagate_forward(out_vector);
    }

    return out_vector(0);
}

Move FCNN::get_best_move(Position position) {
    std::vector<Move> possible_moves = position.generate_moves();
    std::vector<double> evals{};

    for (Move move : possible_moves) {
        position.play_move(move, false);

        evals.emplace_back(evaluate(position_to_matrix(position)));

        position.undo_move(false);
    }

    long long best_move_loc = ((position.white_to_play()) ? std::max_element(evals.begin(), evals.end()) : std::min_element(evals.begin(), evals.end())) - evals.begin();

    return possible_moves[best_move_loc];
}

std::vector<Eigen::VectorXd> FCNN::generate_run(Eigen::VectorXd input_vector) {
    std::vector<Eigen::VectorXd> run = {input_vector};

    for (FCLayer layer : layers) {
        input_vector = layer.propagate_forward(input_vector);
        run.emplace_back(input_vector);
    }

    return run;
}

void FCNN::backpropagate(std::vector<Eigen::VectorXd> run, Eigen::VectorXd cost_vector) {
    for (int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
        cost_vector = layers[i].backpropagate(run[i], run[i + 1], cost_vector);
    }
}

void FCNN::randomise_values() {
    for (FCLayer layer : layers) {
        layer.randomise_values();
    }
}

void FCNN::slightly_alter_values(int div_factor) {
    for (FCLayer layer : layers) {
        layer.slightly_alter_values(div_factor);
    }
}

void FCNN::slightly_alter_other(const FCNN& other, int div_factor) {
    layers = other.layers;

    for (FCLayer layer : layers) {
        layer.slightly_alter_values(div_factor);
    }
}

std::string FCNN::as_string() {
    std::string out_string{};

    for (auto layer : layers) {
        out_string += layer.as_string();
    }

    return out_string;
}

void FCNN::write_to_file(const std::string &file_name) {
    std::ofstream file(file_name);

    file << as_string() << "@\n";

    file.close();
}
