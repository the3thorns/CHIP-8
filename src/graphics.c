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
byte texture_bitmap[32];

/*
 * Function implementations
 */

void ch8g_init_graphics() {
    // Init SDL2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow("CHIP-8", 100, 100, 64, 32, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        perror("Failed to build window");
        SDL_Quit();
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_INDEX1LSB, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_RenderSetLogicalSize(renderer, 480, 240);
    SDL_SetWindowSize(window, 480, 240);
}

bool ch8g_window_closing() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            return true;
        } 
        //else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        //    Sint32 h = event.window.data1 / ASPECT_RATIO;
        //    SDL_RenderSetLogicalSize(renderer, event.window.data1, h); // renderer, width, height
        //    SDL_SetWindowSize(window, event.window.data1, h);
        //}
    }

    return false;
}

void ch8g_close_graphics() {
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    if (texture != NULL) SDL_DestroyTexture(texture);
    SDL_Quit();
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    // TODO: Test
    *vf = 0;
    int n_x = x;
    int n_pixel = y * WIDTH + x; // Number of pixel on screen
    int n_byte = n_pixel / 8; // Index of the byte that contains the pixel
    byte byte_position = 0;
    byte initial_byte_position = 0;
    byte mask_texture = 128;
    byte mask_memory = 128;
    // Locate bit to modify
    for (int j = n_byte; j < n_pixel; j++) {
        mask_texture >>= 1;
        byte_position++;
    }
    initial_byte_position = byte_position;

    int pixel_byte = texture_bitmap[n_byte];
    int memory_byte = memory[i];
    // Iterate at a bit level through the bidimensional array
    for (int h = 0; h < N; h++) {
        for (int w = 0; w < 8; w++) {
            
            // Check
            if (byte_position == 8) {
                n_byte++;
                pixel_byte = texture_bitmap[n_byte];
                mask_texture = 128;
                byte_position = 0;
            }

            // Mask
            

            // Advance
            n_x = (n_x + 1) % WIDTH;
            byte_position++;
            mask_texture >>= 1;
            mask_memory >>= 1;
        }

        // Next y
        y = (y + 1) % HEIGHT;
        byte_position = initial_byte_position;
        mask_texture = 128;
        mask_memory = 128,
        mask_texture >>= initial_byte_position;
        n_x = x;
    }
}

void ch8g_draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ch8g_clear_screen() {
    SDL_RenderClear(renderer);
}