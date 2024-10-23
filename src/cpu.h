#ifndef CPU_H_
#define CPU_H_

#include "common.h"
#include "memory.h"


/**
 * Masking macros
 */

#define MASK_FIRST_NIBBLE       0xF000
#define MASK_SECOND_NIBBLE      0x0F00
#define MASK_THIRD_NIBBLE       0x00F0
#define MASK_FOURTH_NIBBLE      0x000F

#define MASK_LEAST_SIG_BIT      0x0001
/**
 * Here CPU logic and instruction execution will be implemented.
 */

namespace cp8 {

    class Cpu {
        private:
            /**
             * Memory class
             */
            Memory memory;
            
            /**
             * Registers
             */
            byte registers[16] = {0};
            // Program counter
            address pc;
            // Register used to store memory addresses
            address i;

            /**
             * Private functions
             */
            byte mask(instruction ins, uint16_t mask, byte size);
            void execute_instruction(instruction ins);
            void next_instruction();

        public:
            Cpu();
            void run(); // Starts the cpu execution
    };
}

#endif