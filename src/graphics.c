#include "common.h"
#include "graphics.h"
#include <stdlib.h>
#include <raylib.h>

/*
TODO: Allow multiple window size
32 x 64 pixels
*/

/*
 * Variables
 */

/*
Screen

========================================
| (0 ,0)                    (width, 0) |
|                                      |
|                                      |
|                                      |
| (0, height)          (width, height) |
========================================
*/

int screen_width;
int screen_height;

byte *screen_bitmap;

/*
 * Function implementations
 */

void ch8g_init_window(int width, int height) {

    // Check if width and height are multiple of 8

    if ((width >= 64 && width % 8 != 0) || (height >= 32 && height % 8 != 0)) {
        perror("Width and height of screen are not multiple of 8");
        exit(-1);
    }

    screen_width = width;
    screen_height = height;

    // Creates OpenGL context and window
    InitWindow(width, height, "CHIP-8");

    //SetTargetFPS(TARGET_FPS);

    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();

    screen_bitmap = (byte*) calloc(screen_width * screen_height, sizeof(byte));
}

bool ch8g_window_closing() {
    return WindowShouldClose();
}

void ch8g_close_window() {
    free(screen_bitmap);
    CloseWindow();
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    *vf = 0;
}

void ch8g_draw_buffer_contents() {

}

void ch8g_clear_screen() {
    for (int j = 0; j < screen_width * screen_height; j++) {
        screen_bitmap[j] = 0;
    }
}