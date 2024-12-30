#include "common.h"
#include "chip8.h"
#include <stdlib.h>

/**
 * This file contains all utilities of chip-8, serves as an API.
 * Later, the main.c file, selects which subroutines to use.
 */


/**
 * Memory
 */
byte memory[MEMORY_SIZE];

/**
 * CPU variables
 */

byte registers[16];
address pc;
address i;
byte delay_timer;
byte sound_timer;

/**
 * Functions
 */


static byte mask(instruction ins, uint16_t mask, byte size);

static void next_instruction();

static byte mask(instruction ins, uint16_t mask, byte size) {
    instruction ret = ins & mask;
    ret >>= size;

    return (byte)ret;
}

static void next_instruction() {
    pc += 4;
}

void ch8_init() {
    for (int j = 0; i < 16; i++) {
        registers[j] = 0;
    }

    pc = 0x200;
    delay_timer = 0;
    sound_timer = 0;
    i = 0;
}

void ch8_execute_instruction(instruction ins) {
    byte first = mask(ins, MASK_FIRST_NIBBLE, 12);
    byte rx;
    byte nn;
    byte ry;
    uint16_t result;
    byte shifted;
    byte vrx;
    byte vry;
    byte last;
    byte random;
    // Filter by the first nibble
    switch (first) {
        case 0:
            //! 0NNN: Not supported
            // TODO 00E0: Clear screen
            //* TODO: Define subroutines 00EE: Return from a subroutine
            break;
        case 1: // Only one option
            // * 1NNN: Jump to address 1NNN
            ins &= MASK_FIRST_NIBBLE;
            pc = ins;

            break;
        case 2:
            //* TODO: Define subroutines 2NNN: Execute subroutine starting at address NNN
            break;
        case 3:
            //* 3XNN: Skip the following instruction if the vlaue of register X equals NN
            rx = registers[ mask(ins, MASK_SECOND_NIBBLE, 8)];
            nn =  mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
            
            if (rx == nn) {
                next_instruction();
            }

            break;
        case 4:
            //* 4XNN: Skip the following instruction if the value of register VX is not equal to NN
            rx = registers[mask(ins, MASK_SECOND_NIBBLE, 8)];
            nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
            
            if (rx != nn) {
                next_instruction();
            }

            break;
        case 5:
            //* 5XY0: Skip the following instruction if the value of register VX is equal to the value of register VY
            rx = registers[mask(ins, MASK_SECOND_NIBBLE, 8)];
            ry = registers[mask(ins, MASK_THIRD_NIBBLE, 4)];

            if (rx == ry) {
                next_instruction();
            }

            break;
        case 6:
            //* 6XNN: Store number NN in register VX
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

            registers[rx] = nn;

            break;
        case 7:
            //* 7XNN: Add value NN to register VX
                rx = mask(ins, MASK_SECOND_NIBBLE, 8);
                nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

            registers[rx] += nn;

            break;
        case 8:
            last = mask(ins, MASK_FOURTH_NIBBLE, 0);
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            ry = mask(ins, MASK_THIRD_NIBBLE, 4);

            switch (last) {
                case 0:
                    registers[rx] = registers[ry];

                    break;
                case 1: //* OR
                    registers[rx] = registers[rx] | registers[ry];

                    break;
                case 2: //* AND
                    registers[rx] = registers[rx] & registers[ry];

                    break;
                case 3: //* XOR
                    registers[rx] = registers[rx] ^ registers[ry];

                    break;
                case 4: //* add
                    result = registers[rx] + registers[ry];

                    if (result > 255) {
                        // Overflow
                        registers[0xf] = 1;
                    } else {
                        registers[0xf] = 0;
                    }

                    registers[rx] = (byte) result;

                    break;
                case 5: //* sub
                    result = registers[rx] - registers[ry];

                    if (result > registers[ry]) {
                        registers[0xf] = 1;
                    } else {
                        // Borrow happened (underflow);
                        registers[0xf] = 0;
                    }

                    registers[rx] = (byte) result;

                    break;
                case 6: //* right shift
                    vry = registers[ry];
                    shifted = vry >> 1;
                    registers[0xf] = vry & MASK_LEAST_SIG_BIT;
                    registers[rx] = shifted;

                    break;
                case 7:
                    result = registers[ry] - registers[rx];

                    if (result < registers[ry]) {
                        registers[0xf] = 1;
                    } else {
                        // Borrow happened (underflow);
                        registers[0xf] = 0;
                    }

                    registers[rx] = (byte) result;

                    break;
                case 0xE: //* left shift
                    vry = registers[ry];
                    shifted = vry << 1;
                    registers[0xf] = vry & MASK_LEAST_SIG_BIT;
                    registers[rx] = shifted;

                    break;
            }

            break;
        case 9:
            //* 9XY0: Skip next instruction if VX is different from VY
            rx = registers[mask(ins, MASK_SECOND_NIBBLE, 8)];
            ry = registers[mask(ins, MASK_THIRD_NIBBLE, 4)];

            if (rx != ry) {
                next_instruction();
            }

            break;
        case 0xa:
            //* ANNN: Store NNN in I
            i = ins & 0x0FFF;

            break;
        case 0xb:
            //* BNNN: Jump to addresss NNN + V0
            pc = (ins & 0x0FFF) + (address) registers[0];

            break;
        case 0xc:
            //* CXNN: Set VX to a random number with a mask of NN
            random = rand() % 256;
            registers[mask(ins, MASK_SECOND_NIBBLE, 8)] = random & mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

            break;
        case 0xd:
            // TODO: Create graphics interface. DXYN: Draw a sprite (see docs)
            LOG("Todo")
            break;
        case 0xe:
            // TODO: Keypad integration. Two instructions to implement
            LOG("Todo")
            break;
        case 0xf:
            // TODO: Implement memory, timers, sprites and keypad.
            LOG("Todo");
            break;
        
        default:
            printf("Unsupported instruction\n");
        
    }
}

/*
byte registers[16] = {0};
address pc;
address i;
byte delay_timer;
byte sound_timer;
*/

void ch8_print_status() {
    printf("== Registers ==\n");
    for (int j = 0; j < 8; j++) {
        printf("Register %d: 0x%X \tRegister %d: 0x%X\n", j, registers[j], j+8, registers[j+8]);
    }
    printf("Register I: 0x%X\n", i);
    printf("Register PC: 0x%X\n\n", pc);
    
    printf("== TIMERS ==\n");
    printf("Delay timer: %d\n", (int) delay_timer);
    printf("Sound timer: %d\n", (int) sound_timer);
}