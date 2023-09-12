//
// Created by jimbo on 31/07/2022.
//

#include "position.h"

Position::Position() {
    board = {Piece::R, Piece::N, Piece::B, Piece::Q, Piece::K, Piece::B, Piece::N, Piece::R,
             Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P,
             Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
             Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
             Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
             Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
             Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p,
             Piece::r, Piece::n, Piece::b, Piece::q, Piece::k, Piece::b, Piece::n, Piece::r
    };

    to_play = Colour::white;
    undo_information = {};

    cur_move = 1;

    wck = true;
    wcq = true;
    bck = true;
    bcq = true;

    comp_strings[comp_from_fen(get_fen())] = 1;
}

Position::Position(const std::string& fen_string) {
    std::array<const char, 12> PIECE_NAMES = {
            'p', 'n', 'b', 'r', 'q', 'k',
            'P', 'N', 'B', 'R', 'Q', 'K'
    };

    board = {};
    std::fill(board.begin(), board.end(), x);

    auto itr = fen_string.begin();

    for (int row = 7; row >= 0; --row) {
        int col = 0;
        char c = *itr;

        while (c != '/' && c != ' ' && col < 9) {
            if (std::isdigit(c)) {
                col += static_cast<int>(c) - 48;
            }
            else {
                auto piece = std::find(PIECE_NAMES.begin(), PIECE_NAMES.end(), c) - PIECE_NAMES.begin();
                board[row * 8 + col] = static_cast<Piece>(piece);
                ++col;
            }

            ++itr;
            c = *itr;

        }
        ++itr;
    }

    to_play = (*itr == 'w') ? white : black;

    wck = false;
    wcq = false;
    bck = false;
    bcq = false;

    itr += 2;
    char c = *itr;

    while (c == 'K' || c == 'k' || c == 'Q' || c == 'q' || c == '-') {
        switch (c) {
            case 'k':
                bck = true;
                break;

            case 'K':
                wck = true;
                break;

            case 'q':
                bcq = true;
                break;

            case 'Q':
                wcq = true;
                break;

            default:
                break;
        }

        ++itr;
        c = *itr;
    }

    if (c == ' ') {
        ++itr;
        c = *itr;
    }

    if (c != '-') {
        int col = c - 97;

        ++itr;
        c = *itr;

        int row = c;

        undo_information.emplace_back(UndoInfo(x, Move((to_play == white) ? p : P, Square((row - 1) * 8 + col + 2 * ((to_play == white) ? north : south)),
                                               Square((row - 1) * 8 + col), DOUBLE_PUSH), wck, wcq, bck, bcq, 0));

        undo_move(true);

        comp_strings[comp_from_fen(get_fen())] = 1;

        play_move(Move((to_play == white) ? p : P, Square((row - 1) * 8 + col + 2 * ((to_play == white) ? north : south)),
                       Square((row - 1) * 8 + col), DOUBLE_PUSH), true);
    }

    comp_strings[comp_from_fen(get_fen())] = 1;

    itr += 2;
    c = *itr;
    std::string h_num{};

    while (c != ' ') {
        h_num += c;
        ++itr;
        c = *itr;
    }

    half_move = std::stoi(h_num);

    ++itr;
    std::string c_num{};

    while (itr != fen_string.end()) {
        c_num += *itr;
        ++itr;
    }

    cur_move = std::stoi(c_num);
};

std::string Position::get_fen() {
    const char PIECE_NAMES[12] = {
            'p', 'n', 'b', 'r', 'q', 'k',
            'P', 'N', 'B', 'R', 'Q', 'K'
    };

    const char* SQUARE_NAMES[64] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
    };

    std::string out_string{};

    for (int row = 7; row >= 0; --row) {
        int col = 0;
        int empty_squares = 0;

        while (col < 8) {
            int sq = row * 8 + col;

            if (board[sq] == x) {
                empty_squares += 1;
            }
            else {
                if (empty_squares > 0) {
                    out_string += std::to_string(empty_squares);
                    empty_squares = 0;
                }

                out_string += PIECE_NAMES[board[sq]];
            }

            ++col;
        }

        if (empty_squares) {
            out_string += std::to_string(empty_squares);
        }

        if (row != 0) {
            out_string += '/';
        }
    }

    out_string += ' ';
    out_string += (to_play == white) ? 'w' : 'b';
    out_string += ' ';

    if (wck) {
        out_string += 'K';
    }
    if (wcq) {
        out_string += 'Q';
    }
    if (bck) {
        out_string += 'k';
    }
    if (bcq) {
        out_string += 'q';
    }

    out_string += ' ';

    if (!(undo_information.empty())) {
        if (undo_information[undo_information.size() - 1].previous_move.flag == DOUBLE_PUSH) {
            out_string += SQUARE_NAMES[undo_information[undo_information.size() - 1].previous_move.ending_square];
        }
        else {
            out_string += '-';
        }
    }
    else {
        out_string += '-';
    }

    out_string += ' ';
    out_string += std::to_string(half_move);

    out_string += ' ';
    out_string += std::to_string(cur_move);

    return out_string;
};

std::string Position::comp_from_fen(const std::string& fen_string) {
    std::string out_string = fen_string;
    int space_counter = 0;

    while (space_counter < 5) {
        if (out_string.back() == ' ') {
            ++space_counter;
        }

        out_string.pop_back();
    }
    return out_string;
}

int Position::play_move(Move move, bool to_generate) {
    Piece undo_piece = x;

    switch(move.flag) {
        case QUIET:
        case DOUBLE_PUSH:
            board[move.starting_square] = x;
            board[move.ending_square] = move.piece_type;
            break;

        case CAPTURE:
            undo_piece = board[move.ending_square];
            board[move.starting_square] = x;
            board[move.ending_square] = move.piece_type;
            break;

        case OO:
            board[move.starting_square] = x;
            board[move.ending_square + (int) west] = x;
            board[move.ending_square + (int) east] = (to_play == white) ? R : r;
            board[move.ending_square] = (to_play == white) ? K : k;
            break;

        case OOO:
            board[move.starting_square] = x;
            board[move.ending_square + 2 * east] = x;
            board[move.ending_square + (int) west] = (to_play == white) ? R : r;
            board[move.ending_square] = (to_play == white) ? K : k;
            break;

        case EN_PASSANT:
            undo_piece = board[move.ending_square + (int) ((to_play == white) ? south : north)];
            board[move.starting_square] = x;
            board[move.ending_square + (int) ((to_play == white) ? south : north)] = x;
            board[move.ending_square] = move.piece_type;
            break;

        case PROMOTE_N:
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? N : n;
            break;

        case PROMOTE_B:
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? B : b;
            break;

        case PROMOTE_R:
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? R : r;
            break;

        case PROMOTE_Q:
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? Q : q;
            break;

        case PROMOTE_CAP_N:
            undo_piece = board[move.ending_square];
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? N : n;
            break;

        case PROMOTE_CAP_B:
            undo_piece = board[move.ending_square];
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? B : b;
            break;

        case PROMOTE_CAP_R:
            undo_piece = board[move.ending_square];
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? R : r;
            break;

        case PROMOTE_CAP_Q:
            undo_piece = board[move.ending_square];
            board[move.starting_square] = x;
            board[move.ending_square] = (to_play == white) ? Q : q;
            break;

        case NONE:
            return 1;

    }

    undo_information.emplace_back(UndoInfo(
            undo_piece, move, wck, wcq, bck, bcq, half_move
    ));

    ++half_move;
    if (move.piece_type == p || move.piece_type == P || move.flag == CAPTURE) {
        half_move = 0;
    }

    if (move.piece_type == ((to_play == white) ? K : k)) {
        if (to_play == white) {
            wck = false;
            wcq = false;
        } else {
            bck = false;
            bcq = false;
        }
    }

    else if (move.piece_type == ((to_play == white) ? R : r)) {
        if (to_play == white) {
            if (move.starting_square == a1) {
                wcq = false;
            }
            else if (move.starting_square == h1) {
                wck = false;
            }

        } else {
            if (move.starting_square == a8) {
                bcq = false;
            }
            else if (move.starting_square == h8) {
                bck = false;
            }
        }
    }

    if (to_play == black) {
        ++cur_move;
    }

    to_play = ~to_play;

    if (!(to_generate)) {
        std::string comp_str = comp_from_fen(get_fen());

        if (comp_strings.find(comp_str) != comp_strings.end()) {
            ++comp_strings[comp_str];
        } else {
            comp_strings[comp_str] = 1;
        }
    }


    return 0;
}

int Position::undo_move(bool to_generate) {
    if (undo_information.empty()) {
        return 1;
    }

    UndoInfo cur_undo_info = undo_information[undo_information.size() - 1];

    Move previous_move = cur_undo_info.previous_move;

    if (previous_move.flag == NONE) {
        return 1;
    }

    if (!(to_generate)) {
        std::string comp_str = comp_from_fen(get_fen());
        if (comp_strings[comp_str] > 1) {
            --comp_strings[comp_str];
        } else {
            comp_strings.erase(comp_from_fen(get_fen()));
        }
    }

    if (to_play == white) {
        --cur_move;
    }

    to_play = ~to_play;

    switch(previous_move.flag) {
        case QUIET:
        case DOUBLE_PUSH:
        case PROMOTE_N:
        case PROMOTE_B:
        case PROMOTE_R:
        case PROMOTE_Q:
            board[previous_move.starting_square] = previous_move.piece_type;
            board[previous_move.ending_square] = x;
            break;

        case CAPTURE:
        case PROMOTE_CAP_N:
        case PROMOTE_CAP_B:
        case PROMOTE_CAP_R:
        case PROMOTE_CAP_Q:
            board[previous_move.starting_square] = previous_move.piece_type;
            board[previous_move.ending_square] = cur_undo_info.taken_piece;
            break;

        case OO:
            board[previous_move.starting_square] = (to_play == white) ? K : k;
            board[previous_move.ending_square + (int) west] = (to_play == white) ? R : r;
            board[previous_move.ending_square + (int) east] = x;
            board[previous_move.ending_square] = x;
            break;

        case OOO:
            board[previous_move.starting_square] = (to_play == white) ? K : k;
            board[previous_move.ending_square + 2 * (int) east] = (to_play == white) ? R : r;
            board[previous_move.ending_square + (int) west] = x;
            board[previous_move.ending_square] = x;
            break;

        case EN_PASSANT:
            board[previous_move.starting_square] = (to_play == white) ? P : p;
            board[previous_move.ending_square + (int) ((to_play == white) ? south : north)] = (to_play == white) ? p : P;
            board[previous_move.ending_square] = x;
            break;

        case NONE:
            break;
    }

    half_move = cur_undo_info.half_move;

    wck = cur_undo_info.wck;
    wcq = cur_undo_info.wcq;
    bck = cur_undo_info.bck;
    bcq = cur_undo_info.bcq;

    undo_information.pop_back();

    return 0;
}

Position Position::create_new_position(Move move) {
    Position new_pos = *this;
    new_pos.play_move(move, false);
    return new_pos;
}

std::vector<Move> Position::generate_pseudolegals() {
    std::vector<Move> moves{};

    if (!(undo_information.empty())) {
        Move previous_move = undo_information[undo_information.size() - 1].previous_move;

        // en passant
        if (previous_move.flag == DOUBLE_PUSH) {
            if (previous_move.ending_square % 8 != 0 &&
                board[previous_move.ending_square + (int) east] == ((to_play == white) ? P : p)) {
                moves.emplace_back(Move((to_play == white) ? P : p, Square(previous_move.ending_square + (int) east),
                                        Square(previous_move.ending_square + (int) ((to_play == white) ? north : south)),
                                        EN_PASSANT));
            }

            if (previous_move.ending_square % 8 != 7 &&
                board[previous_move.ending_square + (int) west] == ((to_play == white) ? P : p)) {
                moves.emplace_back(Move((to_play == white) ? P : p, Square(previous_move.ending_square + (int) west),
                                        Square(previous_move.ending_square + (int) ((to_play == white) ? north : south)),
                                        EN_PASSANT));
            }
        }
    }

    const int knight_moves[8] {
            north + north_west, north + north_east,
            south + south_west, south + south_east,
            west + north_west, west + south_west,
            east + north_east, east + south_east
    };

    const int king_moves[8] {
            north, south, east, west,
            north_west, north_east,
            south_west, south_east
    };

    for (int pos = 0; pos < chess_constants::BOARD_SIZE; pos++) {
        Piece piece = board[pos];

        if ((to_play == white && is_black(piece))
            || (to_play == black && is_white(piece))) {
            continue;
        }

        switch (piece) {
            case P:
            case p:
                // double push
                if (pos / 8 == 1 || pos / 8 == 6) {
                    if ((to_play == white && board[pos + north] == x && board[pos + 2 * north] == x)
                        || (to_play == black && board[pos + south] == x && board[pos + 2 * south] == x)) {
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + 2 * ((to_play == white) ? north : south)),
                                     DOUBLE_PUSH));
                    }
                }

                // non-promotion rank
                if ((pos / 8 < 6  && to_play == white) ||
                    (pos / 8 > 1 && to_play == black)) {
                    // normal move
                    if ((piece == P && board[pos + north] == x)
                        || (piece == p && board[pos + south] == x)) {
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + ((to_play == white) ? north : south)), QUIET));
                    }

                    // capture-left
                    if (pos % 8 != 7) {
                        if ((piece == P && is_black(board[pos + north_west]))
                            || (piece == p && is_white(board[pos + south_west]))) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_west : south_west)), CAPTURE));
                        }
                    }

                    // capture-right
                    if (pos % 8 != 0) {
                        if ((piece == P && is_black(board[pos + north_east]))
                            || (piece == p && is_white(board[pos + south_east]))) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_east : south_east)), CAPTURE));
                        }
                    }

                }
                    // promotion rank
                else {
                    // promote
                    if ((piece == P && board[pos + north] == x)
                        || (piece == p && board[pos + south] == x)) {
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + ((to_play == white) ? north : south)), PROMOTE_N));
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + ((to_play == white) ? north : south)), PROMOTE_B));
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + ((to_play == white) ? north : south)), PROMOTE_R));
                        moves.emplace_back(
                                Move(piece, Square(pos), Square(pos + ((to_play == white) ? north : south)), PROMOTE_Q));
                    }

                    // promote-capture-left
                    if (pos % 8 != 7) {
                        if ((piece == P && is_black(board[pos + north_west]))
                            || (piece == p && is_white(board[pos + south_west]))) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_west : south_west)), PROMOTE_CAP_N));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_west : south_west)), PROMOTE_CAP_B));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_west : south_west)), PROMOTE_CAP_R));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_west : south_west)), PROMOTE_CAP_Q));
                        }
                    }

                    // promote-capture-right
                    if (pos % 8 != 0) {
                        if ((piece == P && is_black(board[pos + north_east]))
                            || (piece == p && is_white(board[pos + south_east]))) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_east : south_east)), PROMOTE_CAP_N));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_east : south_east)), PROMOTE_CAP_B));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_east : south_east)), PROMOTE_CAP_R));
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(pos + ((to_play == white) ? north_east : south_east)), PROMOTE_CAP_Q));
                        }
                    }
                }
                break;

            case N:
            case n:
                for (int knight_move : knight_moves) {
                    int sq = pos + knight_move;
                    if (sq >= 0 && sq < chess_constants::BOARD_SIZE && abs((pos % 8) - (sq % 8)) < 3) {
                        if ((to_play == white && is_white(board[sq]))
                            || (to_play == black && is_black(board[sq]))) {
                            continue;
                        }
                        if (board[sq] == x) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(sq), QUIET)
                            );
                        } else {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(sq), CAPTURE)
                            );
                        }
                    }
                }
                break;

            case B:
            case b:
                extend_vector(moves, generate_bishop(pos));
                break;

            case R:
            case r:
                extend_vector(moves, generate_rook(pos));
                break;

            case Q:
            case q:
                extend_vector(moves, generate_bishop(pos));
                extend_vector(moves, generate_rook(pos));
                break;

            case K:
            case k:
                for (int king_move : king_moves) {
                    int sq = pos + king_move;
                    if (sq >= 0 && sq < chess_constants::BOARD_SIZE && abs((pos % 8) - (sq % 8)) < 2) {
                        if ((to_play == white && is_white(board[sq]))
                            || (to_play == black && is_black(board[sq]))) {
                            continue;
                        }
                        if (board[sq] == x) {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(sq), QUIET)
                            );
                        } else {
                            moves.emplace_back(
                                    Move(piece, Square(pos), Square(sq), CAPTURE)
                            );
                        }
                    }
                }
                break;

            case x:
                continue;
        }
    }

    // castle
    if (to_play == white) {
        if (wck) {
            if (!(is_check((int) e1, to_play) || is_check((int) f1, to_play) || is_check((int) g1, to_play))
                && board[f1] == x && board[g1] == x && board[e1] == K && board[h1] == R) {
                moves.emplace_back(Move(
                        K, e1, g1, OO
                ));
            }
        }

        if (wcq) {
            if (!(is_check((int) c1, to_play) || is_check((int) d1, to_play) || is_check((int) e1, to_play))
                && board[b1] == x && board[c1] == x && board[d1] == x && board[e1] == K && board[a1] == R) {
                moves.emplace_back(Move(
                        K, e1, c1, OOO
                ));
            }
        }
    }

    else {
        if(bck) {
            if (!(is_check((int) e8, to_play) || is_check((int) f8, to_play) || is_check((int) g8, to_play))
                && board[f8] == x && board[g8] == x && board[e8] == k && board[h8] == r) {
                moves.emplace_back(Move(
                        k, e8, g8, OO
                ));
            }
        }

        if (bcq) {
            if (!(is_check((int) c8, to_play) || is_check((int) d8, to_play) || is_check((int) e8, to_play))
                && board[b8] == x && board[c8] == x && board[d8] == x && board[e8] == k && board[a8] == r) {
                moves.emplace_back(Move(
                        k, e8, c8, OOO
                ));
            }
        }
    }

    return moves;
}

std::vector<Move> Position::generate_moves() {
    std::vector<Move> moves = generate_pseudolegals();

    std::vector<Move> legal_moves{};

    for (Move move : moves) {
        if (is_legal(move)) {
            legal_moves.emplace_back(move);
        }
    }

    return legal_moves;
}

std::vector<Move> Position::generate_bishop(int sq) {
    std::vector<Move> moves{};
    const int directions[4] = {
            north_west, north_east, south_west, south_east
    };

    for (int direction : directions) {
        int prev_sq = sq;
        int cur_sq = sq + direction;

        while (cur_sq >= 0 &&
               cur_sq < chess_constants::BOARD_SIZE &&
               abs((cur_sq % 8) - (prev_sq % 8)) < 2) {
            if ((to_play == white && is_white(board[cur_sq]))
                || (to_play == black && is_black(board[cur_sq]))) {
                break;
            }

            if ((to_play == white && is_black(board[cur_sq]))
                || (to_play == black && is_white(board[cur_sq]))) {
                moves.emplace_back(Move(
                        board[sq], Square(sq), Square(cur_sq), CAPTURE
                ));
                break;
            }

            moves.emplace_back(Move(
                    board[sq], Square(sq), Square(cur_sq), QUIET
            ));

            prev_sq += direction;
            cur_sq += direction;
        }
    }

    return moves;
}

std::vector<Move> Position::generate_rook(int sq) {
    std::vector<Move> moves{};
    const int directions[4] = {
            north, south, east, west
    };

    for (int direction : directions) {
        int prev_sq = sq;
        int cur_sq = sq + direction;

        while (cur_sq >= 0 &&
               cur_sq < chess_constants::BOARD_SIZE &&
               abs((cur_sq % 8) - (prev_sq % 8)) < 2) {
            if ((to_play == white && is_white(board[cur_sq]))
                || (to_play == black && is_black(board[cur_sq]))) {
                break;
            }

            if ((to_play == white && is_black(board[cur_sq]))
                || (to_play == black && is_white(board[cur_sq]))) {
                moves.emplace_back(Move(
                        board[sq], Square(sq), Square(cur_sq), CAPTURE
                ));
                break;
            }

            moves.emplace_back(Move(
                    board[sq], Square(sq), Square(cur_sq), QUIET
            ));

            prev_sq += direction;
            cur_sq += direction;
        }
    }

    return moves;
}

bool Position::is_legal(Move move) {
    play_move(move, true);

    if (!(is_check(static_cast<int>(
                           std::find(board.begin(), board.end(), (to_play == white) ? k : K) - board.begin()),~to_play))) {
        undo_move(true);
        return true;
    }

    undo_move(true);
    return false;
}

bool Position::is_check(int sq, Colour c) {
    // pawn-left
    if ((sq % 8 != 7
        && (sq / 8 != 0 && c == black))
        || (sq / 8 != 7 && c == white)) {
        if (board[sq + ((c == black) ? south_west : north_west)] == ((c == black) ? P : p)) {
            return true;
        }
    }

    // pawn-right
    if ((sq % 8 != 0
        && (sq / 8 != 0 && c == black))
        || (sq / 8 != 7 && c == white)) {
        if (board[sq + ((c == black) ? south_east : north_east)] == ((c == black) ? P : p)) {
            return true;
        }
    }

    const int knight_moves[8] {
            north + north_west, north + north_east,
            south + south_west, south + south_east,
            west + north_west, west + south_west,
            east + north_east, east + south_east
    };

    const int king_moves[8] {
            north, south, east, west,
            north_west, north_east,
            south_west, south_east
    };

    const int bishop_directions[4] = {
            north_west, north_east, south_west, south_east
    };

    const int rook_directions[4] = {
            north, south, east, west
    };

    // knight
    for (int n_direction : knight_moves) {
        int cur_sq = sq + n_direction;
        if (cur_sq >= 0 && cur_sq < chess_constants::BOARD_SIZE && abs((sq % 8) - (cur_sq % 8)) < 3) {
            if (board[cur_sq] == ((c == black) ? N : n)) {
                return true;
            }
        }
    }

    // bishop
    for (int b_direction : bishop_directions) {
        int prev_sq = sq;
        int cur_sq = sq + b_direction;

        while (cur_sq >= 0 &&
               cur_sq < chess_constants::BOARD_SIZE &&
               abs((cur_sq % 8) - (prev_sq % 8)) < 2) {
            if (board[cur_sq] == ((c == black) ? B : b)
                || board[cur_sq] == ((c == black) ? Q : q)) {
                return true;
            }

            if (board[cur_sq] != x) {
                break;
            }

            prev_sq += b_direction;
            cur_sq += b_direction;
        }
    }

    // rook
    for (int r_direction : rook_directions) {
        int prev_sq = sq;
        int cur_sq = sq + r_direction;

        while (cur_sq >= 0 &&
               cur_sq < chess_constants::BOARD_SIZE &&
               abs((cur_sq % 8) - (prev_sq % 8)) < 2) {
            if (board[cur_sq] == ((c == black) ? R : r)
                || board[cur_sq] == ((c == black) ? Q : q)) {
                return true;
            }

            if (board[cur_sq] != x) {
                break;
            }

            prev_sq += r_direction;
            cur_sq += r_direction;
        }
    }

    // king
    for (int k_direction : king_moves) {
        int cur_sq = sq + k_direction;
        if (cur_sq >= 0 && cur_sq < chess_constants::BOARD_SIZE && abs((sq % 8) - (cur_sq % 8)) < 2) {
            if (board[cur_sq] == ((c == black) ? K : k)) {
                return true;
            }
        }
    }

    return false;
}

bool Position::is_check_pos() {
    const Piece king_piece = (to_play == white) ? K : k;
    auto king_loc = std::find(board.begin(), board.end(), king_piece);

    if (king_loc == board.end()) {
        return true;
    }

    int king_sq = static_cast<int>(king_loc - board.begin());

    if (is_check(king_sq, to_play)) {
        return true;
    }

    return false;
}

bool Position::is_checkmate() {
    const Piece king_piece = (to_play == white) ? K : k;
    auto king_loc = std::find(board.begin(), board.end(), king_piece);

    if (king_loc == board.end()) {
        return true;
    }

    int king_sq = static_cast<int>(king_loc - board.begin());

    if (is_check(king_sq, to_play) && generate_moves().empty()) {
        return true;
    }

    return false;
};

bool Position::is_stalemate() {
    // no moves (could be checkmate)
    std::vector<Move> possible_moves = generate_moves();
    if (possible_moves.empty()) {
        return true;
    }

    // 50-move rule
    if (half_move >= 100) {
        return true;
    }

    // 3-move repetition
    if (std::any_of(comp_strings.begin(), comp_strings.end(),
                    [] (auto pair) {return pair.second >= 3;})) {
        return true;
    }

    return false;
};

int Position::is_game_over() {
    if (is_checkmate()) {
        return 2;
    }
    if (is_stalemate()) {
        return 1;
    }
    return 0;
}

Outcome Position::get_game_outcome() {
    if (is_checkmate()) {
        return (to_play == white) ? BLACK_WIN : WHITE_WIN;
    }
    if (is_stalemate()) {
        return DRAW;
    }
    return NO_OUTCOME;
}

const char* Position::game_over_txt() {
    int game_over_val = is_game_over();

    switch (game_over_val) {
        case 0:
            return "GAME_NOT_OVER";
        case 1:
            return "DRAW";
        case 2:
            return (to_play == white) ? "BLACK WON" : "WHITE WON";
        default:
            return "ERROR: (game over val not correct)";
    }
}

std::string Position::as_string() {
    const char PIECE_NAMES[15] = {
            'p', 'n', 'b', 'r', 'q', 'k',
            'P', 'N', 'B', 'R', 'Q', 'K',
            ' '
    };

    const char* COLOUR_NAMES[2] = {
            "BLACK", "WHITE"
    };

    std::string out_string{};

    out_string += "******* MOVE " +
                  std::to_string(cur_move) + " *******\n";

    out_string += COLOUR_NAMES[to_play];
    out_string += " TO PLAY\n";

    for (int i = 0; i < chess_constants::BOARD_SIZE; i++) {
        out_string += "|";
        out_string += PIECE_NAMES[board[i]];
        out_string += "|";

        if (i % 8 == 7) {
            out_string += "\n";
        }
    }

    out_string += "\n";
    return out_string;
}

std::string Position::get_properties() const {
    std::string out_string{};

    out_string += "cur_move: " + std::to_string(cur_move) + "\n";
    out_string += "half_move: " + std::to_string(half_move) + "\n";
    out_string += "wck: " + std::to_string(wck) + "\n";
    out_string += "wcq: " + std::to_string(wcq) + "\n";
    out_string += "bck: " + std::to_string(bck) + "\n";
    out_string += "bcq: " + std::to_string(bcq) + "\n";

    return out_string;
}

std::array<Piece, 64>& Position::get_board() {
    return board;
}

bool Position::white_to_play() {
    return (to_play == white);
}

bool Position::previous_move_was_capture() {
    return (undo_information[undo_information.size() - 1].previous_move.is_capture());
}

template<typename T>
void extend_vector(std::vector<T> &vec, const std::vector<T> &ext) {
    vec.insert(std::end(vec), std::begin(ext), std::end(ext));
}