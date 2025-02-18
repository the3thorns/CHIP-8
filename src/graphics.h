#ifndef GRAPHICS_H_
#define GRAPHICS_H_

/**
 * This header defines the graphical interface that CHIP-8 uses
 * The graphics.c file contains the implementation on raylib but it can be changed to any other graphics library
 */

#include "common.h"
#include <stdbool.h>

#define HEIGHT       32
#define WIDTH        64
#define ASPECT_RATIO 2


/*
 * Creates the window
 */
void ch8g_init_graphics(int width, int height);

/*
 * Checks if the X-button of the window is pressed
 */

bool ch8g_window_closing();

/*
 * Closes the window
 */

void ch8g_close_graphics();

/*
 * Draws a sprite on the coordinates x, y in the buffer
 * Width: 8 pixels
 * Height: N pixels
 */

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf);

/*
 * This function seems weird but it has sense
 * This function outputs the contets of the byte buffer which stores the pixels that should be drawn on the screen
 */

void ch8g_draw();

/*
 * Clears the screen and the buffer back to black
 */
void ch8g_clear_screen();


#endif // GRAPHICS_H_