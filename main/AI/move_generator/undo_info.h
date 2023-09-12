//
// Created by jimbo on 31/07/2022.
//

#ifndef MOVE_GENERATOR_UNDO_INFO_H
#define MOVE_GENERATOR_UNDO_INFO_H

#include "types.h"
#include "move.h"

struct UndoInfo {
    Piece taken_piece;
    Move previous_move;
    bool wck;
    bool wcq;
    bool bck;
    bool bcq;
    int half_move;

public:
    UndoInfo();
    UndoInfo(Piece p, Move p_move, bool wck_, bool wcq_,
             bool bck_, bool bcq_, int half_move_);
};

#endif //MOVE_GENERATOR_UNDO_INFO_H
