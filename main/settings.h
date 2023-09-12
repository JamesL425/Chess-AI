//
// Created by jimbo on 12/08/2022.
//

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include "raylib.h"
#include <array>

namespace settings {
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 1024;
    const int SQUARE_SIZE = 128;

    const float FONT_SIZE = 72;
    const float FONT_SPACING = 0;
    const float TEXT_BUFFER = 20;
    const float LINE_THICKNESS = 4;
    const float LINE_ROUNDNESS = 10;
    const int LINE_SEGMENTS = 10;

    const int PIECE_SIZE_MULTIPLIER = 5;

    const int DEPTH =  4;
    const bool FLIP = true;

    const int FPS = 30;

    // icy sea theme
//    const Color WHITE_TILE_COLOUR = {207, 220, 227, 255};
//    const Color BLACK_TILE_COLOUR = {126, 160, 180, 255};

    // regular theme
    const Color WHITE_TILE_COLOUR = {238, 238, 210, 255};
    const Color BLACK_TILE_COLOUR = {118, 150, 86, 255};

    // other miscellaneous colours
    const Color OCEAN_BLUE = {21, 126, 237, 40};
    const Color LIGHT_GREEN = {88, 240, 156, 255};
    const Color TEAL = {2, 72, 89, 255};
    const Color YELLOW_ORANGE = {242, 193, 46, 255};

    const Color TINT = WHITE;
    const Color HIGHLIGHT_TINT = YELLOW_ORANGE;

    const char PATH_TO_NN1[] = "../assets/nn/nn.txt";
    const char PATH_TO_NN2[] = "../assets/nn/nn.txt";
    const char PATH_TO_ICON[] = "../assets/images/icon3.png";

    const std::array<const char*, 12> PATHS_TO_PIECES = {
            "../assets/images/chess_pieces/black_pawn.png",
            "../assets/images/chess_pieces/black_knight.png",
            "../assets/images/chess_pieces/black_bishop.png",
            "../assets/images/chess_pieces/black_rook.png",
            "../assets/images/chess_pieces/black_queen.png",
            "../assets/images/chess_pieces/black_king.png",
            "../assets/images/chess_pieces/white_pawn.png",
            "../assets/images/chess_pieces/white_knight.png",
            "../assets/images/chess_pieces/white_bishop.png",
            "../assets/images/chess_pieces/white_rook.png",
            "../assets/images/chess_pieces/white_queen.png",
            "../assets/images/chess_pieces/white_king.png"
    };

    const char PATH_TO_CHECK_SOUND[] = "../assets/sounds/check_sound.wav";
    const char PATH_TO_PLAY_SOUND[] = "../assets/sounds/play_sound.wav";
    const char PATH_TO_CAPTURE_SOUND[] = "../assets/sounds/capture_sound.wav";

    const char PATH_TO_FONT[] = "../assets/fonts/Silkscreen-Regular.ttf";
}

#endif //UI_SETTINGS_H
