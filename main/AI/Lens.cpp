//
// Created by jimbo on 11/08/2022.
//

#include "Lens.h"

Lens::Lens(LensType lens_type_, size lens_size_) {
    lens_type = lens_type_;
    lens_size = lens_size_;
    total_length = lens_size.first * lens_size.second;
    weights = get_random_matrix(total_length, 1, 1);
}

Lens::Lens(LensType lens_type_, std::vector<std::string>& lines) {
    const std::array<int, 8> squares = {
            1, 4, 9, 16, 25, 36, 49, 64
    };
    lens_type = lens_type_;
    int square_size;

    switch (lens_type) {
        case SQUARE_LENS:
            square_size = static_cast<int>(
                    std::find(squares.begin(),
                              squares.end(),
                              static_cast<int>(lines.size())) - squares.begin()) + 1;
            lens_size = size(square_size, square_size);
            break;

        case ROW_LENS:
            lens_size = size(1, static_cast<int>(lines.size()));
            break;

        case COL_LENS:
            lens_size = size(static_cast<int>(lines.size()), 1);
            break;
    }

    total_length = lens_size.first * lens_size.second;
    weights.resize(total_length, 1);

    for (int i = 0; i < total_length; ++i) {
        weights(i, 0) = std::stod(lines[i]);
    }
}

double Lens::evaluate(const Eigen::VectorXd& input_block) {
    return weights.dot(input_block);
}

void Lens::randomise_values() {
    weights = get_random_matrix(total_length, 1, 1);
}

void Lens::slightly_alter_values(int div_factor) {
    Eigen::MatrixXd weight_changes = get_random_matrix(total_length, 1, div_factor);
    weights += weight_changes;

    weights.cwiseMax(1);
    weights.cwiseMin(-1);
}

std::string Lens::as_string() {
    std::string out_str {};

    out_str += "Lens Type: " + std::to_string(lens_type) + "\n";
    out_str += "Weights:\n";

    for (int row = 0; row < weights.rows(); ++row) {
        for (int col = 0; col < weights.cols(); ++col) {
            out_str += std::to_string(weights(row, col));
            out_str += " ";
        }

        out_str.pop_back();
        out_str += "\n";
    }

    return out_str;
}

LensType Lens::get_lens_type() const {
    return lens_type;
}

size Lens::get_lens_size() const {
    return lens_size;
}

int Lens::get_total_length() const {
    return total_length;
}
