#ifndef CHIP8_H_
#define CHIP8_H_

#include "common.h"

#define MASK_FIRST_NIBBLE       0xF000
#define MASK_SECOND_NIBBLE      0x0F00
#define MASK_THIRD_NIBBLE       0x00F0
#define MASK_FOURTH_NIBBLE      0x000F

#define MASK_LEAST_SIG_BIT      0x0001

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


#endif // CHIP8_H_