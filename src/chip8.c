#include "common.h"
#include "chip8.h"
#include "graphics.h"
#include <stdlib.h>

/**
 * This file contains all utilities of chip-8, serves as an API.
 * Later, the main.c file, selects which subroutines to use.
 */


/**
 * Memory
 */
byte* memory;

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

static void check_f_instruction(instruction ins);

static void next_instruction() {
    pc += 2;
}

void ch8_init() {
    for (int j = 0; i < 16; i++) {
        registers[j] = 0;
    }

    pc = 0x200;
    delay_timer = 0;
    sound_timer = 0;
    i = 0;
    memory = NULL;
}

static void init_memory(int memory_size) {
    memory = (byte*) malloc(sizeof(byte) * memory_size);
}

int ch8_load_memory(const char* path) {
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        // Raise error
        perror("File not found or does not exist");
        exit(-1);
    }

    init_memory(MEMORY_SIZE);

    // Free addresses: [0x200, 0xE8F]
    // Final 352 bytes are reserved.

    // Insert data from file to memory
    instruction ins;
    for (int j = 0x200; j <= 0xE8F; j+=2) {
        // fread reads binary data from file stream
        fread(&ins, sizeof(instruction), 1, file);
        instruction* mem_ins = (instruction*) &memory[j];
        *mem_ins = ins;
    }

    fclose(file);

    return 0;
}

void ch8_dump_memory(FILE* output_stream) {
    if (memory != NULL) {
        for (int j = 0x200; j <= 0xE8F; j+=2) {
            instruction* mem_ins = (instruction*) &memory[j];
            printf("Add 0x%X: 0x%X\n", j, (int) *mem_ins);
        }
    }
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
            last = mask(ins, MASK_FOURTH_NIBBLE, 0);

            switch (last) {
                case 0x0:
                    //* 00E0: Clear screen
                    // ch8g_clear_screen(STANDARD_WIDTH, STANDARD_HEIGHT);
                    break;
                case 0xE:
                    // TODO: Define subroutines 00EE: Return from a subroutine

                    break;
            }

            break;
        case 1: // Only one option
            // * 1NNN: Jump to address NNN
            ins &= 0x0111;
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
                    shifted = registers[rx] >> registers[ry];
                    registers[0xf] = registers[rx] & MASK_LEAST_SIG_BIT;
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
                    shifted = registers[rx] << registers[ry];
                    registers[0xf] = registers[rx] & 0b10000000;
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
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            ry = mask(ins, MASK_THIRD_NIBBLE, 4);
            nn = mask(ins, MASK_FOURTH_NIBBLE, 0);

            // ch8g_draw_sprite(rx, ry, nn, memory, i, &registers[0xf]);
            break;
        case 0xe:
            // TODO: Keypad integration. Two instructions to implement
            LOG("Todo")
            break;
        case 0xf:
            check_f_instruction(ins);
            break;
        default:
            printf("Unsupported instruction\n");
        
    }
}

static void check_f_instruction(instruction ins) {
    byte third = mask(ins, MASK_THIRD_NIBBLE, 4);
    byte fourth = mask(ins, MASK_FOURTH_NIBBLE, 0);
    byte x = mask(ins, MASK_SECOND_NIBBLE, 8);

    switch (third) {
        case 0x0:
            switch (fourth) {
                case 0x7:
                    registers[x] = delay_timer;
                    break;
                case 0xA:
                    // TODO: Keypad implementation
                    LOG("Keypad not implemented");
                    break;
            }

            break;
        case 0x1:
            switch (fourth) {
                case 0x5:
                    delay_timer = registers[x];
                    break;
                case 0x8:
                    sound_timer = registers[x];
                    break;
                case 0xE:
                    i += registers[x];
                    break;
            }

            break;
        case 0x2:
            // TODO: One instruction
            //* Font
            break;
        case 0x3:
            // TODO: One instruction
            
            break;
        case 0x5:
            // TODO: One instruction. MEMORY REQUIRED
            break;
        case 0x6:
            // TODO: One instruction. MEMORY REQUIRED
            break;
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
        printf("Register %X: 0x%X \tRegister %X: 0x%X\n", j, registers[j], j+8, registers[j+8]);
    }
    printf("Register I: 0x%X\n", i);
    printf("Register PC: 0x%X\n\n", pc);
    
    printf("== TIMERS ==\n");
    printf("Delay timer: %d\n", (int) delay_timer);
    printf("Sound timer: %d\n", (int) sound_timer);
}

void ch8_end() {
    if (memory != NULL){
        free(memory);
    }
}