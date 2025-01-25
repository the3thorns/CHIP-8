#include "common.h"
#include "graphics.h"
#include <raylib.h>

void ch8g_init_window(int width, int height) {
    InitWindow(width, height, "CHIP-8");

    SetTargetFPS(30);

    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
}

bool ch8g_window_closing() {
    return WindowShouldClose();
}

void ch8g_close_window() {
    CloseWindow();
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    LOG("TODO");
    return;
}

void ch8g_clear_screen() {
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
}