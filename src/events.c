#include "events.h"
#include "SDL.h"
#include <stdbool.h>

extern int key_detected;
extern bool window_needs_to_close;

void ch8events_poll_events() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            window_needs_to_close = true;
        } else if (event.type == SDL_KEYDOWN) {
            // Check key type
            switch (event.key.keysym.sym) {
                case(SDLK_1):
                    key_detected = 1;
                    break;
                case(SDLK_2):
                    key_detected = 2;
                    break;
                case(SDLK_3):
                    key_detected = 3;
                    break;
                case(SDLK_4):
                    key_detected = 0xc;
                    break;
                case(SDLK_q):
                    key_detected = 4;
                    break;
                case(SDLK_w):
                    key_detected = 5;
                    break;
                case(SDLK_e):
                    key_detected = 6;
                    break;
                case(SDLK_r):
                    key_detected = 0xd;
                    break;
                case(SDLK_a):
                    key_detected = 7;
                    break;
                case(SDLK_s):
                    key_detected = 8;
                    break;
                case(SDLK_d):
                    key_detected = 9;
                    break;
                case(SDLK_f):
                    key_detected = 0xe;
                    break;
                case(SDLK_z):
                    key_detected = 0xa;
                    break;
                case(SDLK_x):
                    key_detected = 0;
                    break;
                case(SDLK_c):
                    key_detected = 0xb;
                    break;
                case(SDLK_v):
                    key_detected = 0xf;
                    break;
                default:
                    key_detected = -1;
            }
        }
    }
}