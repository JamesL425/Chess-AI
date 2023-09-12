//
// Created by jimbo on 31/07/2022.
//

#ifndef MOVE_GENERATOR_MOVE_H
#define MOVE_GENERATOR_MOVE_H

#include "types.h"
#include <string>

struct Move {
    Piece piece_type;

    Square starting_square;
    Square ending_square;

    MoveFlag flag;

    Move();
    Move(const Move& m) = default;
    Move(Move&& m) = default;
    Move& operator = (const Move& other) = default;
    bool operator == (const Move& other) const = default;
    Move(Piece p, Square s, Square e, MoveFlag f);

public:
    [[nodiscard]] std::string as_string() const;
    [[nodiscard]] bool is_capture() const;
    [[nodiscard]] bool is_promotion() const;
};

#endif //MOVE_GENERATOR_MOVE_H
