#include "common.h"
#include "chip8.h"
#include "graphics.h"
#include "events.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// Global variablesSs
const double SECONDS_PER_TIC = 1 / TICK_FREQUENCY;

// Function declarations
void interpreter_loop();

extern byte delay_timer;
extern byte sound_timer;

void interpreter_loop() {

    const double timer_update = SECONDS_PER_TIC / 60;
    double delay_timer_counter = 0;
    double sound_timer_counter = 0;
    struct timespec start_time, end_time;
    struct timespec req, rem;

    ch8g_init_graphics();
    while (!ch8g_window_closing()) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        ch8events_poll_events();
        instruction ins = ch8_fetch_instruction();
        ch8_execute_instruction(ins);
        ch8g_draw();

        // Measure time
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
        delay_timer_counter += elapsed_time;
        sound_timer_counter += elapsed_time;

        // Update timers
        if (delay_timer_counter >= timer_update) {
            delay_timer--;
            delay_timer_counter = 0;
        } else if (sound_timer_counter >= timer_update) {
            sound_timer--;
            delay_timer_counter = 0;
        }

        double pause = SECONDS_PER_TIC - elapsed_time;
        if (pause > 0) {
            req.tv_sec = (time_t) pause;
            req.tv_nsec = (long)(pause - req.tv_sec) * 1000000000.0;
            int res = nanosleep(&req, &rem);

            if (res != 0) {
                printf("Interruption detected\n");
            }
        }
    }

    ch8g_close_graphics();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Número incorrecto de argumentos");
        return -1;
    }

    ch8_init();
    ch8_load_memory(argv[1]);

    interpreter_loop();

    ch8_end();
}
