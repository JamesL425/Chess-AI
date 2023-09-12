//
// Created by jimbo on 12/08/2022.
//

#ifndef UI_MENU_H
#define UI_MENU_H

#include "game.h"

std::vector<Button> make_buttons(const std::vector<const char*>& button_txts, Font& font);
void draw_menu(std::vector<Button> buttons, Font& font);
void menu();

#endif //UI_MENU_H
