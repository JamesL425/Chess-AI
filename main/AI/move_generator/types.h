//
// Created by jimbo on 13/07/2022.
//

#ifndef MOVE_GENERATOR_TYPES_H
#define MOVE_GENERATOR_TYPES_H

enum Colour : int {
    black, white
};

constexpr Colour operator ~ (Colour colour) {
    return Colour (colour ^ white);
}

enum Piece : int {
    p, n, b, r, q, k,
    P, N, B, R, Q, K,
    x
};

constexpr bool is_black(Piece piece) {
    return piece <= k;
}

constexpr bool is_white(Piece piece) {
    return (piece > k && piece < x);
}

enum Direction : int {
    north = 8, north_east = 7,
    east = -1, south_east = -9,
    south = -8, south_west = -7,
    north_west = 9, west = 1
};

enum Square : int {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NO_SQUARE
};

enum MoveFlag : int {
    QUIET, CAPTURE, OO, OOO, DOUBLE_PUSH, EN_PASSANT,
    PROMOTE_N, PROMOTE_B, PROMOTE_R,PROMOTE_Q,
    PROMOTE_CAP_N, PROMOTE_CAP_B, PROMOTE_CAP_R, PROMOTE_CAP_Q,
    NONE
};

enum Outcome : int {
    BLACK_WIN, DRAW, WHITE_WIN, NO_OUTCOME
};

namespace chess_constants {
    const int BOARD_SIZE = 64;
}

#endif //MOVE_GENERATOR_TYPES_H
