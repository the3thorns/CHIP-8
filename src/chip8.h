#ifndef CHIP8_H_
#define CHIP8_H_

#include "common.h"

#define MASK_FIRST_NIBBLE       0xF000
#define MASK_SECOND_NIBBLE      0x0F00
#define MASK_THIRD_NIBBLE       0x00F0
#define MASK_FOURTH_NIBBLE      0x000F

#define MASK_LEAST_SIG_BIT      0x0001

void execute_instruction(instruction ins);

void print_status();


#endif // CHIP8_H_