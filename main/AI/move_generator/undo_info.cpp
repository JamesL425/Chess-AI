//
// Created by jimbo on 31/07/2022.
//

#include "undo_info.h"

UndoInfo::UndoInfo() {
    taken_piece = x;
    previous_move = {};
    wck = false;
    wcq = false;
    bck = false;
    bcq = false;
    half_move = 0;
};

UndoInfo::UndoInfo(Piece p, Move p_move, bool wck_, bool wcq_,
                   bool bck_, bool bcq_, int half_move_) {
    taken_piece = p;
    previous_move = p_move;
    wck = wck_;
    wcq = wcq_;
    bck = bck_;
    bcq = bcq_;
    half_move = half_move_;
};