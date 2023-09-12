//
// Created by jimbo on 11/08/2022.
//

#ifndef AI_LENS_H
#define AI_LENS_H

#include "utilities.h"

enum LensType : int {
    SQUARE_LENS, ROW_LENS, COL_LENS
};

class Lens {
private:
    Eigen::VectorXd weights;
    LensType lens_type;
    size lens_size;
    int total_length;
    
public:
    Lens(LensType lens_type_, size lens_size_);
    Lens(LensType lens_type_, std::vector<std::string>& lines);
    Lens(const Lens& lens_) = default;
    Lens(Lens&& lens_) = default;
    Lens& operator = (const Lens& other) = default;

    double evaluate(const Eigen::VectorXd& input_block);
    
    void randomise_values();
    void slightly_alter_values(int div_factor);
    std::string as_string();
    
    [[nodiscard]] LensType get_lens_type() const;
    [[nodiscard]] size get_lens_size() const;
    [[nodiscard]] int get_total_length() const;
};

#endif //AI_LENS_H
