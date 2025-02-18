#include "common.h"
#include "graphics.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

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
 * Variables
 */

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *texture;

/*
 * Function implementations
 */

void ch8g_init_graphics(int width, int height) {

    // Check if width and height are multiple of 8

    if ((width >= 64 && width % 8 != 0) || (height >= 32 && height % 8 != 0)) {
        perror("Width and height of screen are not multiple of 8");
        exit(-1);
    }

    // Init SDL2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow("CHIP-8", 100, 100, 64, 32, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        perror("Failed to build window");
        SDL_Quit();
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = NULL;
}

bool ch8g_window_closing() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            return true;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            Sint32 h = event.window.data1 / ASPECT_RATIO;
            SDL_RenderSetLogicalSize(renderer, event.window.data1, h); // renderer, width, height
            SDL_SetWindowSize(window, event.window.data1, h);
        }
    }

    return false;
}

void ch8g_close_graphics() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
    SDL_Quit();
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    // TODO: Test
    *vf = 0;

    int n_pixel = y * WIDTH + x; // Number of pixel on screen
    int n_byte = n_pixel / 8; // Index of the byte that contains the pixel

    byte byte_position = 0;
    byte initial_byte_position = 0;
    byte mask = 0b10000000;
    byte pixels_byte; // Used to store the byte of the pixels array
    byte *pixels;
    int pitch;

    // Initial for loop
    for (int j = n_byte; j < n_pixel; j++) {
        mask >>= 1;
        byte_position++;
    }

    initial_byte_position = byte_position;

    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

    pixels_byte = pixels[n_byte];

    for (int h = 0; h < N; h++) {
        for (int w = 0; w < 8; w++) {
            if (byte_position >= 8) {
                n_pixel = y * WIDTH + x;
                n_byte = n_pixel / 8;
                byte_position = 0;
                pixels_byte = pixels[n_byte];
                mask = 128;
            }
            // Check for collision

            pixels_byte ^= pixels_byte & mask;
            
            if (pixels_byte != pixels[n_byte]) {
                *vf = 1;
            }

            pixels[n_byte] = pixels_byte;

            byte_position++;
            mask >>= 1;
            x = (x + 1) % WIDTH;
        }
        // Recalculate bit and byte position
        y = (y + 1) % HEIGHT;
        n_pixel = y * WIDTH + x;
        n_byte = n_pixel / 8;
        mask = 128;
        mask >>= initial_byte_position;
        byte_position = initial_byte_position;
        pixels_byte = pixels[n_byte];
    }

    SDL_UnlockTexture(texture);
}

void ch8g_draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ch8g_clear_screen() {
    SDL_RenderClear(renderer);
}