#include "common.h"
#include "graphics.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

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

/*
 * Function implementations
 */

void ch8g_init_window(int width, int height) {

    // Check if width and height are multiple of 8

    if ((width >= 64 && width % 8 != 0) || (height >= 32 && height % 8 != 0)) {
        perror("Width and height of screen are not multiple of 8");
        exit(-1);
    }

}

bool ch8g_window_closing() {
}

void ch8g_close_window() {
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
}

void ch8g_draw_buffer_contents() {
}

void ch8g_clear_screen() {
}