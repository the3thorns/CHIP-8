#ifndef GRAPHICS_H_
#define GRAPHICS_H_

/**
 * This header defines the graphical interface that CHIP-8 uses
 * The graphics.c file contains the implementation on raylib but it can be changed to any other graphics library
 */

#include "common.h"

#define STANDARD_WIDTH  64
#define STANDARD_HEIGHT 32


/*
 * Creates the window
 */
void ch8g_init_window(int width, int height);


/**
 * Draws a sprite on the coordinates x, y
 * Width: 8 pixels
 * Height: N pixels
 */

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf);

/*
 * Clears the screen
 */
void ch8g_clear_screen();


#endif // GRAPHICS_H_