#ifndef GRAPHICS_H_
#define GRAPHICS_H_

/**
 * This header defines the graphical interface that CHIP-8 uses
 * The graphics.c file contains the implementation on raylib but it can be changed to any other graphics library
 */


/*
 * Creates the window
 */
void ch8g_init_window();


/*
 * Draws a pixel on the screen.
 * If the coordinates are not valid, will do nothing
 */

void ch8g_draw_pixel(int px, int py);


#endif // GRAPHICS_H_