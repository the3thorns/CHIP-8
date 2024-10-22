#include "common.h"
#include "memory.h"


/**
 * Masking macros
 */

#define MASK_FIRST          0b1111000000000000
#define MASK_LAST_TWO       0b0000000011111111

/**
 * Here CPU logic and instruction execution will be implemented.
 */

namespace cp8 {

    class Cpu {
        private:
            Memory memory;
            // Registers
            byte v0;
            byte v1;
            byte v2;
            byte v3;
            byte v4;
            byte v5;
            byte v6;
            byte v7;
            byte v8;
            byte v9;
            byte va;
            byte vb;
            byte vc;
            byte vd;
            byte ve;
            byte vf;

            address pc; // Represents the current instruction about to execute
            address i;

            // Private instructions

            byte mask(instruction ins, uint16_t mask, byte size);
            void execute_instruction(instruction ins);

        public:
            Cpu();
            void run(); // Starts the cpu execution
    };
}