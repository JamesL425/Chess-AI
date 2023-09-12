//
// Created by jimbo on 10/08/2022.
//

#include "ConvLayer.h"

ConvLayer::ConvLayer(size input_size_, int n_out_matrices_, std::vector<size>& lens_sizes) {
    input_size = input_size_;
    n_out_matrices = n_out_matrices_;

    for (size lens_size : lens_sizes) {
        if (lens_size.first == lens_size.second) {
            lenses.emplace_back(Lens{SQUARE_LENS, lens_size});
        }
        else if (lens_size.first > lens_size.second) {
            lenses.emplace_back(Lens{COL_LENS, lens_size});
        }
        else {
            lenses.emplace_back(Lens{ROW_LENS, lens_size});
        }
    }
}

ConvLayer::ConvLayer(std::string& input_string) {
    std::vector<std::string> lines = split_string(input_string, '\n');
    int n_lines = static_cast<int>(lines.size());

    for (int i = 0; i < n_lines; ++i) {
        std::string cur_line = lines[i];

        if (cur_line == "\n" || cur_line == "@" || cur_line == "ConvLayer\n" || cur_line.empty()) {
            continue;
        }

        if (cur_line[0] == 'L') {
            std::vector<std::string> weight_lines {};
            i += 2;

            while (lines[i][0] == '-' || lines[i][0] == '0') {
                weight_lines.emplace_back(lines[i]);
                if (i == n_lines - 1) {
                    break;
                }

                ++i;
            }

            lenses.emplace_back(Lens{(LensType) (cur_line[11] - '0'), weight_lines});
            --i;
        }
    }

    input_size = size(8, 8);
    n_out_matrices = static_cast<int>(lenses.size());
}

std::vector<Eigen::MatrixXd> ConvLayer::propagate_forward(Eigen::MatrixXd &input_mat) {
    std::vector<Eigen::MatrixXd> out_vec {};

    for (Lens lens : lenses) {
        Eigen::MatrixXd out_mat{};
        int lens_size;

        switch (lens.get_lens_type()) {
            case SQUARE_LENS:
                out_mat.resize(8, 8);

                lens_size = lens.get_lens_size().second;

                for (int row = 0; row < 8; ++row) {
                    for (int col = 0; col < 8; ++col) {
                        int final_row = row + lens_size;
                        int final_col = col + lens_size;

                        bool needs_padding = false;

                        int row_gap = 0;
                        int col_gap = 0;

                        if (final_row > 8) {
                            row_gap = final_row - 8;

                            final_row = 8;
                            needs_padding = true;
                        }

                        if (final_col > 8) {
                            col_gap = final_col - 8;

                            final_col = 8;
                            needs_padding = true;
                        }

                        Eigen::MatrixXd shadowed_block = input_mat.block(row, col, lens_size - row_gap, lens_size - col_gap);

                        if (needs_padding) {
                            pad_matrix_to_square(shadowed_block, size{shadowed_block.rows(), shadowed_block.cols()}, lens_size);
                        }

                        Eigen::VectorXd in_vec = block_to_vector(shadowed_block);

                        out_mat(row, col) = lens.evaluate(in_vec);
                    }
                }

                break;

            case ROW_LENS:
                out_mat.resize(1, 8);
                for (int row = 0; row < 8; ++row) {
                    out_mat(0, row) = lens.evaluate(input_mat.row(row));
                }

                break;

            case COL_LENS:
                out_mat.resize(1, 8);
                for (int col = 0; col < 8; ++col) {
                    out_mat(0, col) = lens.evaluate(input_mat.col(col));
                }

                break;
        }
        out_vec.emplace_back(out_mat);
    }

    return out_vec;
}

void ConvLayer::randomise_values() {
    for (auto lens : lenses) {
        lens.randomise_values();
    }
}

void ConvLayer::slightly_alter_values(int div_factor) {
    for (auto lens : lenses) {
        lens.slightly_alter_values(div_factor);
    }
}

std::string ConvLayer::as_string() {
    std::string out_string {};
    out_string += "ConvLayer\n";

    for (auto lens : lenses) {
        out_string += lens.as_string();
    }

    return out_string;
}

int ConvLayer::get_n_matrices() const {
    return n_out_matrices;
}
