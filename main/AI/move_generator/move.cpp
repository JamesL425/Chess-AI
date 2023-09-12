//
// Created by jimbo on 31/07/2022.
//

#include "move.h"

Move::Move() {
    piece_type = x;
    starting_square = NO_SQUARE;
    ending_square = NO_SQUARE;
    flag = NONE;
}

Move::Move(Piece p, Square s, Square e, MoveFlag f)  {
    piece_type = p;
    starting_square = s;
    ending_square = e;
    flag = f;
}

std::string Move::as_string() const {
    const char PIECE_NAMES[6] = {
            'P', 'N', 'B', 'R', 'Q', 'K'
    };

    const char* SQUARE_NAMES[65] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
            "NO_SQUARE"
    };

    if (flag == OO or flag == OOO) {
        return ((flag == OO) ? "O-O" : "O-O-O");

    }

    std::string out{};

    out += PIECE_NAMES[piece_type % 6];
    out += SQUARE_NAMES[starting_square];

    if (flag == CAPTURE || flag == PROMOTE_CAP_N ||
        flag == PROMOTE_CAP_B || flag == PROMOTE_CAP_R ||
        flag == PROMOTE_CAP_Q) {
        out += "x";
    }

    out += SQUARE_NAMES[ending_square];

    if (flag == PROMOTE_CAP_N) {
        out += "=N";
    }
    else if (flag == PROMOTE_CAP_B) {
        out += "=B";
    }
    else if (flag == PROMOTE_CAP_R) {
        out += "=R";
    }
    else if (flag == PROMOTE_CAP_Q) {
        out += "=Q";
    }

    return out;
}

bool Move::is_capture() const {
    return (flag == CAPTURE ||
            flag == PROMOTE_CAP_B ||
            flag == PROMOTE_CAP_N ||
            flag == PROMOTE_CAP_Q ||
            flag == PROMOTE_CAP_R
            );
}

bool Move::is_promotion() const {
    return (flag == PROMOTE_B ||
            flag == PROMOTE_N ||
            flag == PROMOTE_Q ||
            flag == PROMOTE_R ||
            flag == PROMOTE_CAP_B ||
            flag == PROMOTE_CAP_N ||
            flag == PROMOTE_CAP_Q ||
            flag == PROMOTE_CAP_R);
}

