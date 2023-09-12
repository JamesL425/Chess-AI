//
// Created by jimbo on 12/08/2022.
//

#include "Button.h"

Button::Button(float x_, float y_, const char* txt_, Font& font, bool centre) {
    x = x_;
    y = y_;
    txt = txt_;

    auto text_size = MeasureTextEx(font, txt, settings::FONT_SIZE, settings::FONT_SPACING);

    width = text_size.x;
    height = text_size.y;

    if (centre) {
        x -= width / 2;
        y -= height / 2;
    }

    rect = Rectangle{x, y, width, height};
}

std::vector<Button> make_buttons(const std::vector<const char*>& button_txts, Font& font) {
    float dist = settings::SCREEN_HEIGHT / (float) (button_txts.size() + 1);

    std::vector<Button> buttons{};

    for (int i = 0; i < (int) button_txts.size(); ++i) {
        buttons.emplace_back(Button{
                settings::SCREEN_WIDTH / 2, (i + 1) * dist, button_txts[i], font, true
        });
    }

    return buttons;
}
