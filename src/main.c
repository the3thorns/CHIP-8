#include "common.h"
#include "chip8.h"
#include "graphics.h"
#include "events.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
// #include <raylib.h>

/**
 * It is part of the GNU C Standard Library
 * ! WARNING: If you are in windows, use a package that includes getopt.h or other implementaiton. Windows does not support this function in Visual C/C++
 */
//#include <getopt.h>

/**
 * Function declarations
 */

void interpreter_loop();


int main(int argc, char** argv) {

    ch8_init();
    
    if (argc == 2) {
        ch8_load_memory(argv[1]);
    }

    interpreter_loop();
    
    ch8_end();

    return 0;
}

void interpreter_loop() {
    ch8g_init_graphics();
    
    // Loop
    while (!ch8g_window_closing()) {
        ch8events_poll_events();
        instruction ins = ch8_fech_instruction();
        ch8_execute_instruction(ins);
        ch8g_draw();
    }

    ch8g_close_graphics();
}
