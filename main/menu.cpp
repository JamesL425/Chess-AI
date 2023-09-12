//
// Created by jimbo on 12/08/2022.
//

#include "menu.h"

void draw_menu(std::vector<Button> buttons, Font& font) {
    BeginDrawing();

    ClearBackground(settings::TEAL);

    for (Button& button : buttons) {
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

void menu() {
    Font font = LoadFont(settings::PATH_TO_FONT);
    SetWindowTitle("Chess - Menu");

    const std::vector<const char*> button_txts = {
            "Player vs Player",
            "Player vs AI",
            "AI vs AI"
    };

    std::vector<Button> buttons = make_buttons(button_txts, font);

    while (!WindowShouldClose()) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for (Button button : buttons) {
                if (CheckCollisionPointRec(GetMousePosition(), button.rect)) {
                    if (button.txt == button_txts[0]) {
                        Position position {};
                        game(true, true, position, font);
                    }

                    else if (button.txt == button_txts[1]) {
                        Position position {};
                        game(false, true, position, font);
                    }

                    else if (button.txt == button_txts[2]) {
                        Position position {};
                        game(false, false, position, font);
                    }

                    SetWindowTitle("Chess - Menu");
                }
            }
        }

        draw_menu(buttons, font);
    }

    SetWindowTitle("Chess");
    UnloadFont(font);
}