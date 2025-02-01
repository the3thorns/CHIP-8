#ifndef CHIP8_H_
#define CHIP8_H_

#include "common.h"

#define MASK_FIRST_NIBBLE       0xF000
#define MASK_SECOND_NIBBLE      0x0F00
#define MASK_THIRD_NIBBLE       0x00F0
#define MASK_FOURTH_NIBBLE      0x000F

#define MASK_LEAST_SIG_BIT      0x0001

#define F0 (standard_font){0xF0, 0x90, 0x90, 0x90, 0xF0}
#define F1 (standard_font){0x20, 0x60, 0x20, 0x20, 0x70}
#define F2 (standard_font){0xF0, 0x10, 0xF0, 0x80, 0xF0}
#define F3 (standard_font){0xF0, 0x10, 0xF0, 0x10, 0xF0}
#define F4 (standard_font){0x90, 0x90, 0xF0, 0x10, 0x10}
#define F5 (standard_font){0xF0, 0x80, 0xF0, 0x10, 0xF0}
#define F6 (standard_font){0xF0, 0x80, 0xF0, 0x90, 0xF0}
#define F7 (standard_font){0xF0, 0x10, 0x20, 0x40, 0x40}
#define F8 (standard_font){0xF0, 0x90, 0xF0, 0x90, 0xF0}
#define F9 (standard_font){0xF0, 0x90, 0xF0, 0x10, 0xF0}
#define FA (standard_font){0xF0, 0x90, 0xF0, 0x90, 0x90}
#define FB (standard_font){0xE0, 0x90, 0xE0, 0x90, 0xE0}
#define FC (standard_font){0xF0, 0x80, 0x80, 0x80, 0xF0}
#define FD (standard_font){0xE0, 0x90, 0x90, 0x90, 0xE0}
#define FE (standard_font){0xF0, 0x80, 0xF0, 0x80, 0xF0}
#define FF (standard_font){0xF0, 0x80, 0xF0, 0x80, 0x80}


/**
 * Prepares resgisters and timers
 * This function must be called before every other CHIP-8 function
 */
void ch8_init();

/**
 * Loads memory from a rom file
 */

int ch8_load_memory(const char* path);


/**
 * Prints memory contents into stdout
 */

void ch8_dump_memory();

/**
 * Inserts a raw instruction into the CHIP-8 interpreter and executes it
 */
void ch8_execute_instruction(instruction ins);

/**
 * Prints the status of the registers and tiemrs
 */
void ch8_print_status();


/**
 * Frees all data allocated by the CHIP-8 interpreter
 */
void ch8_end();


typedef struct standard_font {
    byte r1;
    byte r2;
    byte r3;
    byte r4;
    byte r5;
}standard_font;


#endif // CHIP8_H_