#include "common.h"
#include "chip8.h"
#include "graphics.h"
#include <stdlib.h>
#include "SDL.h"

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
address sp; // Stack pointer
address i;
byte delay_timer;
byte sound_timer;

/**
 * Keypad variables
 */

int key_detected;

/**
 * Function declarations
 */

static byte mask(instruction ins, uint16_t mask, byte size);

static void next_instruction();

static void load_standard_fonts();

static void init_memory(int memory_size);

static void check_f_instruction(instruction ins);


/**
 *  Functions
 */

static byte mask(instruction ins, uint16_t mask, byte size) {
    instruction ret = ins & mask;
    ret >>= size;

    return (byte)ret;
}


static void next_instruction() {
    pc += 2;
}

static void load_standard_fonts() {
    standard_font arr[] = {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, FA, FB, FC, FD, FE, FF};

    for (int j = 0; j < 16; j++) {
        memory[j * 8] = arr[j].r1;
        memory[j * 8 + 1] = arr[j].r2;
        memory[j * 8 + 2] = arr[j].r3;
        memory[j * 8 + 3] = arr[j].r4;
        memory[j * 8 + 4] = arr[j].r5;
    }
}

void ch8_init() {
    for (int j = 0; i < 16; i++) {
        registers[j] = 0;
    }

    pc = 0x200;
    delay_timer = 0;
    sound_timer = 0;
    i = 0;
    sp = INITIAL_STACK_ADDRESS;
    init_memory(MEMORY_SIZE);
    load_standard_fonts();
}

static void init_memory(int memory_size) {
    memory = (byte*) malloc(sizeof(byte) * memory_size);
}

int ch8_load_memory(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        // Raise error
        perror("File not found or does not exist");
        exit(-1);
    }

    // Free addresses: [0x200, 0xE8F]
    // Final 352 bytes are reserved.

    // Insert data from file to memory
    byte ins[2];
    for (int j = 0x200; j <= 0xE8F; j+=2) {
        // fread reads binary data from file stream
        if (fread(&ins, sizeof(byte), 2, file) > 0) {
            // Adjusted for little endian
            memory[j] = ins[0];
            memory[j+1] = ins[1];
        }
    }

    fclose(file);

    return 0;
}

void ch8_dump_memory() {
    if (memory != NULL) {
        for (int j = 0X0; j <= 0xE8F; j+=2) {
            instruction* mem_ins = (instruction*) &memory[j];
            printf("Add 0x%X: 0x%X\n", j, (int) *mem_ins);
        }
    }
}

instruction ch8_fech_instruction() {
    // For little endian devices
    instruction ins;
    byte *a = (byte*)&ins;
    a[0] = memory[pc + 1];
    a[1] = memory[pc];

    pc += 2;

    return ins;
}

void ch8_execute_instruction(instruction ins) {
    byte first = mask(ins, MASK_FIRST_NIBBLE, 12);
    byte rx;
    byte nn;
    byte ry;
    uint16_t result;
    int16_t sresult;
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
                    ch8g_clear_screen();
                    break;
                case 0xE:
                    // TODO: Define subroutines 00EE: Return from a subroutine
                    pc = *(address*)(&memory[sp]);
                    sp -= 2;
                    LOG("Todo (00EE): Define subroutines");
                    break;
            }

            break;
        case 1: // Only one option
            // * 1NNN: Jump to address NNN
            ins &= 0x0FFF;
            pc = ins;
            LOG("Here {1NNN}");
            break;
        case 2:
            //* TODO: Define subroutines 2NNN: Execute subroutine starting at address NNN
            sp += 2;
            *(address *)(&memory[sp]) = pc;
            pc = ins & 0xFFF;
            LOG("Here {2NNN}");
            break;
        case 3:
            //* 3XNN: Skip the following instruction if the vlaue of register X equals NN
            rx = registers[ mask(ins, MASK_SECOND_NIBBLE, 8)];
            nn =  mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
            
            if (rx == nn) {
                next_instruction();
            }
            LOG("Here {3XNN}");

            break;
        case 4:
            //* 4XNN: Skip the following instruction if the value of register VX is not equal to NN
            rx = registers[mask(ins, MASK_SECOND_NIBBLE, 8)];
            nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
            
            if (rx != nn) {
                next_instruction();
            }

            LOG("Here {4XNN}");

            break;
        case 5:
            //* 5XY0: Skip the following instruction if the value of register VX is equal to the value of register VY
            rx = registers[mask(ins, MASK_SECOND_NIBBLE, 8)];
            ry = registers[mask(ins, MASK_THIRD_NIBBLE, 4)];

            if (rx == ry) {
                next_instruction();
            }

            LOG("Here {5XY0}");
            break;
        case 6:
            //* 6XNN: Store number NN in register VX
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

            registers[rx] = nn;
            
            LOG("Here {6XNN}");
            break;
        case 7:
            //* 7XNN: Add value NN to register VX
                rx = mask(ins, MASK_SECOND_NIBBLE, 8);
                nn = mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

            registers[rx] += nn;

            LOG("Here {7XNN}");
            break;
        case 8:
            last = mask(ins, MASK_FOURTH_NIBBLE, 0);
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            ry = mask(ins, MASK_THIRD_NIBBLE, 4);

            switch (last) {
                case 0:
                    registers[rx] = registers[ry];

                    break;
                case 1: //* OR - Resets register vx (COSMAC QUIRK)
                    registers[rx] = registers[rx] | registers[ry];
                    registers[15] = 0;
                    break;
                case 2: //* AND - Resets register vx (COSMAC QUIRK)
                    registers[rx] = registers[rx] & registers[ry];
                    registers[15] = 0;
                    break;
                case 3: //* XOR - Resets register vx (COSMAC QUIRK)
                    registers[rx] = registers[rx] ^ registers[ry];
                    registers[15] = 0;
                    break;
                case 4: //* add
                    result = (uint16_t)registers[rx] + (uint16_t)registers[ry];

                    registers[rx] = (byte) (result & 0xFF);

                    registers[0xf] = result > 255 ? 1 : 0;

                    break;
                case 5: //* sub
                    sresult = (int16_t) (registers[rx] - (int16_t) registers[ry]);

                    registers[rx] = (byte) (sresult & 0xFF);

                    if (registers[rx] > registers[ry]) {
                        registers[0xf] = 1;
                    } else {
                        // Borrow happened (underflow);
                        registers[0xf] = 0;
                    }

                    break;
                case 6: //* COSMACVIP version of the instruction
                    nn = registers[ry];
                    registers[rx] = registers[ry] >> 1;
                    registers[15] = (nn & 1);

                    break;
                case 7:
                    result = registers[ry] - registers[rx];

                    registers[rx] = (byte) result;

                    if (result < registers[ry]) {
                        registers[0xf] = 1;
                    } else {
                        // Borrow happened (underflow);
                        registers[0xf] = 0;
                    }

                    break;
                case 0xE: //* COSMACVIP version of the instruction
                    nn = registers[ry];
                    registers[rx] = registers[ry] << 1;
                    registers[15] = nn >> 7;

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
            //* DXYN: Draw sprite with pos RX, RY (being RX and RY both registers) and height N
            LOG("TEST Draw sprite");
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            ry = mask(ins, MASK_THIRD_NIBBLE, 4);
            nn = mask(ins, MASK_FOURTH_NIBBLE, 0);

            ch8g_draw_sprite(registers[rx], registers[ry], nn, memory, i, &registers[0xf]);
            break;
        case 0xe:
            //* EX9E:Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
            //* EXA1: Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
            // TODO: TEST
            LOG("TEST (EX{9E|A1}): Keypad integration");
            rx = mask(ins, MASK_SECOND_NIBBLE, 8);
            switch(mask(ins, MASK_THIRD_NIBBLE, 4)) {
                case (9): {
                    if (key_detected == registers[rx]) {
                        next_instruction();
                    }

                    break;
                }
                case (10): {
                    if (key_detected != registers[rx]) {
                        next_instruction();
                    }

                    break;
                }
            }
            break;
        case 0xf:
            check_f_instruction(ins);
            break;
        default:
            fprintf(stderr, "Unsupported instruction\n");
        
    }
}

static void check_f_instruction(instruction ins) {
    byte second = mask(ins, MASK_SECOND_NIBBLE, 8);
    byte third = mask(ins, MASK_THIRD_NIBBLE, 4);
    byte fourth = mask(ins, MASK_FOURTH_NIBBLE, 0);
    byte x = mask(ins, MASK_SECOND_NIBBLE, 8);

    byte prev;

    switch (third) {
        case 0x0:
            switch (fourth) {
                case 0x7:
                    registers[x] = delay_timer;
                    break;
                case 0xA:
                    //* FX0A: Wait for a keypress and store the result in register VX
                    // TODO: TEST
                    while (key_detected == -1) {
                        ;
                    }
                    registers[second] = key_detected;
                    LOG("TEST (FX0{7|A}): Delay timer and keypress");
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
            //* FX29: Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
            i = registers[second] * 8;
            LOG("TEST (FX29)");
            break;
        case 0x3:
            //* FX33: Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2

            prev = memory[i + 2] = registers[second] % 10;
            memory[i + 1] = (registers[second] % 100 - prev) / 10;
            prev = registers[second] % 100;
            memory[i] = (registers[second] % 1000 - prev) / 100;

            LOG("TODO (FX33): BCD");
            break;
        case 0x5:
            //* FX55: Store the values of registers V0 to VX inclusive in memory starting at address I. I is set to I + X + 1 after operation
            for (int c = 0; c <= second; c++) {
                memory[i + c] = registers[c];
            }
            i = i + second + 1;
            LOG("TEST (FX55): Store values V0 to VX");
            break;
        case 0x6:
            //* FX65: Fill registers V0 to VX inclusive with the values stored in memory starting at address I. I is set to I + X + 1 after operation
            for (int c = 0; c <= second; c++) {
                registers[c] = memory[i + c];
            }
            i = i + second + 1;
            LOG("TEST (FX65): Fill register from V0 to VX");
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