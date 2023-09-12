//
// Created by jimbo on 10/08/2022.
//

#ifndef AI_CONVLAYER_H
#define AI_CONVLAYER_H

#include "Lens.h"


class ConvLayer {
private:
    size input_size;
    int n_out_matrices;

    std::vector<Lens> lenses {};

public:
    ConvLayer() = default;
    ConvLayer(size input_size_, int n_out_matrices_, std::vector<size>& lens_sizes);
    explicit ConvLayer(std::string& input_string);
    ConvLayer(const ConvLayer& conv_layer) = default;
    ConvLayer(ConvLayer&& conv_layer) = default;
    ConvLayer& operator = (const ConvLayer& other) = default;

    std::vector<Eigen::MatrixXd> propagate_forward(Eigen::MatrixXd& input_mat);
    void randomise_values();
    void slightly_alter_values(int div_factor);
    std::string as_string();

    int get_n_matrices() const;
};


#endif //AI_CONVLAYER_H
