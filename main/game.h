//
// Created by jimbo on 12/08/2022.
//

#ifndef UI_GAME_H
#define UI_GAME_H

#include "AI/CNN.h"
#include "settings.h"
#include "Button.h"
#include "AI/monte_carlo_tree_search.h"

std::array<Texture2D, 12> get_piece_textures();
void unload_piece_textures(std::array<Texture2D, 12>& piece_textures);

void play_move_sound(Position& position, Sound& play_sound, Sound& check_sound, Sound& capture_sound);

Square get_clicked_square();
std::vector<Move> get_possible_moves_from_square(std::vector<Move>& possible_moves, Square sq);
std::vector<Square> get_end_squares(std::vector<Move>& moves);

void draw_promotion_menu(std::vector<Button>& buttons, Font& font);
void get_promotion_move(Move& move, Font& font);

void draw_game_over_screen(const char* game_over_txt, Font& font);

void draw_game(Position& position, std::array<Texture2D, 12>& piece_textures, Square clicked_square);

void play_move(Position& position, Move move, Sound& play_sound, Sound& check_sound, Sound& capture_sound);

void game(bool white_is_human, bool black_is_human, Position& position, Font& font);

#endif //UI_GAME_H
