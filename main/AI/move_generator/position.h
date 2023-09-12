//
// Created by jimbo on 31/07/2022.
//

#ifndef MOVE_GENERATOR_POSITION_H
#define MOVE_GENERATOR_POSITION_H

#include "types.h"
#include "undo_info.h"
#include <vector>
#include <array>
#include <algorithm>
#include <cctype>
#include <unordered_map>

class Position {
private:
    std::array<Piece, 64> board{};

    std::vector<UndoInfo> undo_information;

    std::unordered_map<std::string, int> comp_strings{};

    Colour to_play;

    int cur_move{};

    bool wck;
    bool wcq;
    bool bck;
    bool bcq;

    int half_move{};

public:
    Position();
    Position(const Position& position) = default;
    Position(Position&& position) = default;
    Position& operator = (const Position& other) = default;

    explicit Position(const std::string& fen_string);
    [[nodiscard]] std::string get_fen();
    [[nodiscard]] static std::string comp_from_fen(const std::string& fen_string);

    int play_move(Move move, bool to_generate);
    int undo_move(bool to_generate);

    Position create_new_position(Move move);

    std::vector<Move> generate_pseudolegals();
    std::vector<Move> generate_moves();
    std::vector<Move> generate_bishop(int sq);
    std::vector<Move> generate_rook(int sq);

    bool is_legal(Move move);
    bool is_check(int sq, Colour c);
    bool is_check_pos();
    bool is_checkmate();
    bool is_stalemate();
    int is_game_over();
    Outcome get_game_outcome();
    const char* game_over_txt();

    std::string as_string();
    [[nodiscard]] std::string get_properties() const;

    std::array<Piece, 64>& get_board();

    bool white_to_play();
    bool previous_move_was_capture();
};

template<typename T>
void extend_vector(std::vector<T> &vec, const std::vector<T> &ext);

#endif //MOVE_GENERATOR_POSITION_H
