//
// Created by jimbo on 12/08/2022.
//

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "settings.h"
#include <utility>
#include <vector>

struct Button {
    float x;
    float y;
    float width;
    float height;
    Rectangle rect{};
    const char* txt;

    Button(float x_, float y_, const char* txt_, Font& font, bool centre);
};

std::vector<Button> make_buttons(const std::vector<const char*>& button_txts, Font& font);

#endif //UI_BUTTON_H
