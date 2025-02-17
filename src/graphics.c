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
    // ! Not working, temporal feature

}

void ch8g_draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ch8g_clear_screen() {
    SDL_RenderClear(renderer);
}