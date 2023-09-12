//
// Created by jimbo on 12/08/2022.
//

#include "game.h"

std::array<Texture2D, 12> get_piece_textures() {;
    std::array<Texture2D, 12> piece_textures {};

    for (int i = 0; i < (int) settings::PATHS_TO_PIECES.size(); ++i) {
        Image image = LoadImage(settings::PATHS_TO_PIECES[i]);
        ImageResize(&image, image.width * settings::PIECE_SIZE_MULTIPLIER, image.height * settings::PIECE_SIZE_MULTIPLIER);
        piece_textures[i] = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    return piece_textures;
}

void unload_piece_textures(std::array<Texture2D, 12>& piece_textures) {
    for (auto& piece_texture : piece_textures) {
        UnloadTexture(piece_texture);
    }
}

void play_move_sound(Position& position, Sound& play_sound, Sound& check_sound, Sound& capture_sound) {
    auto board = position.get_board();

    if (position.is_check(static_cast<int>(std::find(board.begin(),board.end(), (position.white_to_play()) ? K : k)
        - board.begin()), (Colour) position.white_to_play())) {
        PlaySound(check_sound);
    }

    else if (position.previous_move_was_capture()) {
        PlaySound(capture_sound);
    }

    else {
        PlaySound(play_sound);
    }
}

Square get_clicked_square() {
    auto pos = GetMousePosition();

    int row = static_cast<int>(pos.y) / settings::SQUARE_SIZE;
    int col = static_cast<int>(pos.x) / settings::SQUARE_SIZE;

    if (settings::FLIP) {
        return Square((7 - row) * 8 + col);
    }

    return Square(row * 8 + col);
}

std::vector<Move> get_possible_moves_from_square(std::vector<Move>& possible_moves, Square sq) {
    std::vector<Move> out_vec{};

    for (auto possible_move : possible_moves) {
        if (possible_move.starting_square == sq) {
            out_vec.emplace_back(possible_move);
        }
    }

    return out_vec;
}

std::vector<Square> get_end_squares(std::vector<Move>& moves) {
    std::vector<Square> out_vec {};

    std::for_each(moves.begin(), moves.end(), [&out_vec] (Move move) {out_vec.emplace_back(move.ending_square);});
    return out_vec;
}

void draw_promotion_menu(std::vector<Button>& buttons, Font& font) {
    BeginDrawing();

    ClearBackground(settings::TEAL);

    for (Button button : buttons) {
        DrawTextEx(font, button.txt, Vector2{button.x, button.y}, settings::FONT_SIZE, settings::FONT_SPACING, settings::LIGHT_GREEN);

        DrawRectangleRounded(
                Rectangle{
                        button.x - settings::TEXT_BUFFER, button.y - settings::TEXT_BUFFER, button.width + 2 * (settings::TEXT_BUFFER), button.height + 2 * (settings::TEXT_BUFFER)
                }, settings::LINE_ROUNDNESS, settings::LINE_SEGMENTS, settings::OCEAN_BLUE
        );

        DrawRectangleRoundedLines(
                Rectangle{
                        button.x - settings::TEXT_BUFFER, button.y - settings::TEXT_BUFFER, button.width + 2 * (settings::TEXT_BUFFER), button.height + 2 * (settings::TEXT_BUFFER)
                }, settings::LINE_ROUNDNESS, settings::LINE_SEGMENTS, settings::LINE_THICKNESS, settings::YELLOW_ORANGE
        );
    }

//    DrawFPS(settings::SCREEN_WIDTH - settings::SQUARE_SIZE, settings::SQUARE_SIZE);

    EndDrawing();
}

void get_promotion_move(Move& move, Font& font) {
    bool got_move = false;

    const std::vector<const char*> button_txts = {
            "Knight",
            "Bishop",
            "Rook",
            "Queen"
    };

    std::vector<Button> buttons = make_buttons(button_txts, font);

    bool capture = move.is_capture();

    while (!got_move) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for (Button button: buttons) {
                if (CheckCollisionPointRec(GetMousePosition(), button.rect)) {
                    if (button.txt == button_txts[0]) {
                        move.flag = (capture) ? PROMOTE_CAP_N : PROMOTE_N;
                    }

                    else if (button.txt == button_txts[1]) {
                        move.flag = (capture) ? PROMOTE_CAP_B : PROMOTE_B;
                    }

                    else if (button.txt == button_txts[2]) {
                        move.flag = (capture) ? PROMOTE_CAP_R : PROMOTE_R;
                    }

                    else if (button.txt == button_txts[3]) {
                        move.flag = (capture) ? PROMOTE_CAP_Q : PROMOTE_Q;
                    }

                    got_move = true;
                }
            }
        }

        draw_promotion_menu(buttons, font);
    }
}

void draw_game_over_screen(const char* game_over_txt, Font& font) {
    auto text_size = MeasureTextEx(font, game_over_txt, settings::FONT_SIZE, settings::FONT_SPACING);
    Vector2 pos = {((float) settings::SCREEN_WIDTH / 2) - (text_size.x / 2), ((float) settings::SCREEN_HEIGHT / 2) - (text_size.y / 2)};

    float total_time = 0;
    while (!WindowShouldClose() && total_time < 4) {
        BeginDrawing();
        ClearBackground(settings::TEAL);

        DrawTextEx(font, game_over_txt, pos, settings::FONT_SIZE, settings::FONT_SPACING, settings::LIGHT_GREEN);

        EndDrawing();

        total_time += GetFrameTime();
    }
}

void draw_game(Position& position, std::array<Texture2D, 12>& piece_textures, Square clicked_square) {
    auto board = position.get_board();

    BeginDrawing();

    ClearBackground(BLANK);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int start_x = col * settings::SQUARE_SIZE;
            int start_y = row * settings::SQUARE_SIZE;

            DrawRectangle(start_x, start_y,settings::SQUARE_SIZE, settings::SQUARE_SIZE,
                          ((row + col) % 2 == 0) ? settings::WHITE_TILE_COLOUR : settings::BLACK_TILE_COLOUR);

            int cur_sq = (settings::FLIP ? 7 - row : row) * 8 + col;

            Piece piece = board[cur_sq];

            if (piece != x) {
                DrawTexture(piece_textures[piece], start_x + (settings::SQUARE_SIZE / 2) - (piece_textures[piece].width / 2),
                            start_y + (settings::SQUARE_SIZE / 2) - (piece_textures[piece].height / 2),
                            (cur_sq == clicked_square) ? settings::HIGHLIGHT_TINT : settings::TINT);
            }
        }
    }

//    DrawFPS(settings::SCREEN_WIDTH - settings::SQUARE_SIZE, settings::SQUARE_SIZE);

    EndDrawing();
}

void play_move(Position& position, Move move, Sound& play_sound, Sound& check_sound, Sound& capture_sound) {
    position.play_move(move, false);
    play_move_sound(position, play_sound, check_sound, capture_sound);
}

void game(bool white_is_human, bool black_is_human, Position& position, Font& font) {
    auto piece_textures = get_piece_textures();
    Sound play_sound = LoadSound(settings::PATH_TO_PLAY_SOUND);
    Sound check_sound = LoadSound(settings::PATH_TO_CHECK_SOUND);
    Sound capture_sound = LoadSound(settings::PATH_TO_CAPTURE_SOUND);

    SetWindowTitle("Chess - Game");
    FCNN FCNN_1, FCNN_2;

    if (!white_is_human) {
        std::stringstream buffer;
        std::ifstream file(settings::PATH_TO_NN1);
        buffer << file.rdbuf();
        file.close();

        FCNN_1 = FCNN{buffer.str()};
    }

    if (!black_is_human) {
        std::stringstream buffer;
        std::ifstream file(settings::PATH_TO_NN2);
        buffer << file.rdbuf();
        file.close();

        FCNN_1 = FCNN{buffer.str()};
    }

    Square clicked_square = NO_SQUARE;
    std::vector<Move> possible_moves_from_square = {};
    std::vector<Square> possible_end_squares{};
    bool played = false;

    while (!WindowShouldClose()) {
        if (position.is_game_over()) {
            draw_game(position, piece_textures, NO_SQUARE);
            WaitTime(1);
            draw_game_over_screen(position.game_over_txt(), font);
            break;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
        ((white_is_human && position.white_to_play()) || (black_is_human && !position.white_to_play()))) {
            if (clicked_square != NO_SQUARE) {
                auto square_itr = std::find(possible_end_squares.begin(), possible_end_squares.end(), get_clicked_square());

                if (square_itr != possible_end_squares.end()) {
                    int idx = static_cast<int>(square_itr - possible_end_squares.begin());
                    Move move = possible_moves_from_square[idx];

                    if (move.is_promotion()) {
                        get_promotion_move(move, font);
                    }

                    play_move(position, move, play_sound, check_sound, capture_sound);
                    played = true;
                }

                clicked_square = NO_SQUARE;
            }

            else {
                Square temp_clicked_square = get_clicked_square();

                std::vector<Move> possible_moves = position.generate_moves();

                possible_moves_from_square = get_possible_moves_from_square(possible_moves, temp_clicked_square);

                possible_end_squares = get_end_squares(possible_moves_from_square);

                if (!possible_moves_from_square.empty()) {
                    possible_end_squares = get_end_squares(possible_moves_from_square);
                    clicked_square = temp_clicked_square;
                }
            }
        }

        if (((!white_is_human && position.white_to_play()) || (!black_is_human && !position.white_to_play())) && !played) {
            draw_game(position, piece_textures, clicked_square);
            play_move(position, position.white_to_play() ?
            get_best_mc_move(FCNN_1, position) : get_best_mc_move(FCNN_2, position),
            play_sound, check_sound, capture_sound);
        }

        played = false;
        draw_game(position, piece_textures, clicked_square);
    }

    unload_piece_textures(piece_textures);
    UnloadSound(play_sound);
    UnloadSound(check_sound);
    UnloadSound(capture_sound);
}