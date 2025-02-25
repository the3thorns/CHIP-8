#include "common.h"
#include "graphics.h"
#include <stdlib.h>
#include "SDL.h"

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
byte texture_bitmap[64][32]; // SDL_PIXELFORMAT_RGB332: Used in embedded systems, but will do the job

/*
 * Function implementations
 */

static void dump_sprite(int x, int y, int N, byte* memory, address i, byte* vf);

void ch8g_init_graphics() {
    // Init SDL2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 32, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        perror("Failed to build window");
        SDL_Quit();
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    if (texture == NULL) {
        perror("Failed to create texture");
        exit(-1);
    }    

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
    if (texture != NULL) SDL_DestroyTexture(texture);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}

void ch8g_draw_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    // TODO: Test
    dump_sprite(x, y, N, memory, i, vf);

    *vf = 0;
    byte mask = 128; // Mask for the memory byte
    int yy = y % HEIGHT;
    int xx = x % WIDTH;
    int ii = i;

    byte *pixels;
    int pitch; // 64

    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

    for (int h = 0; h < N; h++) {
        for (int w = 0; w < 8; w++) {
            byte masked = memory[ii] & mask;
            mask >>= 1;
            byte trep = texture_bitmap[xx][yy] != 0 ? 1: 0;
            byte mrep = masked == 0 ? 0 : 1;

            byte compare = texture_bitmap[xx][yy];
            texture_bitmap[xx][yy] = (trep ^ mrep) * 255;

            if (compare == 255 && texture_bitmap[xx][yy] == 0) {
                *vf = 1;
            }

            pixels[yy * pitch + xx] = texture_bitmap[xx][yy];

            xx = (xx + 1) % WIDTH;
        }
        yy = (yy + 1) % HEIGHT;
        mask = 128;
        ii++;
    }

    SDL_UnlockTexture(texture);
}

static void dump_sprite(int x, int y, int N, byte* memory, address i, byte* vf) {
    for (int n = 0; n < N; n++) {
        byte mem = memory[i];
        for (int w = 0; w < 8; w++) {
            byte p = mem & (128 >> w) != 0;
            printf("%d", (int)p);
        }
        printf("\n");
    }
    printf("\n");
}

void ch8g_draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ch8g_clear_screen() {
    SDL_RenderClear(renderer);
}