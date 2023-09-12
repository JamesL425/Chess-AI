#include "menu.h"

int main() {
    InitAudioDevice();
    InitWindow(settings::SCREEN_HEIGHT, settings::SCREEN_WIDTH, "Chess");

    SetTargetFPS(settings::FPS);

    Image icon_image = LoadImage(settings::PATH_TO_ICON);
    SetWindowIcon(icon_image);
    UnloadImage(icon_image);
    SetWindowPosition((1920 - settings::SCREEN_WIDTH) / 2, (1080 - settings::SCREEN_HEIGHT) / 2);

    menu();

    CloseWindow();
    CloseAudioDevice();
    return 0;
}
