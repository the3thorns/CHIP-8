#include "cpu.h"

namespace cp8 {
    Cpu::Cpu() {
        memory = Memory();
        // Register inicialization



        this->pc = 0x200;
        this->i = 0;
    }

    byte Cpu::mask(instruction ins, uint16_t mask, byte size) {
        instruction ret = ins & mask;
        ret >>= size;

        return (byte)ret;
    }

    /**
     * TODO: Add error checking
     */
    void Cpu::execute_instruction(instruction ins) {
        byte first = this->mask(ins, MASK_FIRST_NIBBLE, 12);

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
                this->pc = ins;
                break;
            case 2:
                //* TODO: Define subroutines 2NNN: Execute subroutine starting at address NNN
                break;
            case 3:
                //* 3XNN: Skip the following instruction if the vlaue of register X equals NN
                byte rx = this->registers[this->mask(ins, MASK_SECOND_NIBBLE, 8)];
                byte nn = this->mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
                
                if (rx == nn) {
                    next_instruction();
                }
                break;
            case 4:
                //* 4XNN: Skip the following instruction if the value of register VX is not equal to NN
                byte rx = this->registers[this->mask(ins, MASK_SECOND_NIBBLE, 8)];
                byte nn = this->mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);
                
                if (rx != nn) {
                    next_instruction();
                }
                break;
            case 5:
                //* 5XY0: Skip the following instruction if the value of register VX is equal to the value of register VY
                byte rx = this->registers[this->mask(ins, MASK_SECOND_NIBBLE, 8)];
                byte ry = this->registers[this->mask(ins, MASK_THIRD_NIBBLE, 4)];

                if (rx == ry) {
                    next_instruction();
                }
                break;
            case 6:
                //* 6XNN: Store number NN in register VX
                byte rx = this->mask(ins, MASK_SECOND_NIBBLE, 8);
                byte nn = this->mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

                this->registers[rx] = nn;
                break;
            case 7:
                //* 7XNN: Add value NN to register VX
                byte rx = this->mask(ins, MASK_SECOND_NIBBLE, 8);
                byte nn = this->mask(ins, (MASK_THIRD_NIBBLE | MASK_FOURTH_NIBBLE), 0);

                this->registers[rx] += nn;
                break;
            case 8:
                byte last = this->mask(ins, MASK_FOURTH_NIBBLE, 0);
                byte rx = this->mask(ins, MASK_SECOND_NIBBLE, 8);
                byte ry = this->mask(ins, MASK_THIRD_NIBBLE, 4);

                switch (last) {
                    case 0:
                        this->registers[rx] = this->registers[ry];
                        break;
                    case 1: //* OR
                        this->registers[rx] = this->registers[rx] | this->registers[ry];
                        break;
                    case 2: //* AND
                        this->registers[rx] = this->registers[rx] & this->registers[ry];
                        break;
                    case 3: //* XOR
                        this->registers[rx] = this->registers[rx] ^ this->registers[ry];
                        break;
                    case 4: //* add
                        uint16_t result = this->registers[rx] + this->registers[ry];

                        if (result > 255) {
                            // Overflow
                            this->registers[0xf] = 1;
                        } else {
                            this->registers[0xf] = 0;
                        }

                        this->registers[rx] = (byte) result;

                        break;
                    case 5: //* sub
                        uint16_t result = this->registers[rx] - this->registers[ry];

                        if (result > this->registers[ry]) {
                            this->registers[0xf] = 1;
                        } else {
                            // Borrow happened (underflow);
                            this->registers[0xf] = 0;
                        }

                        this->registers[rx] = (byte) result;

                        break;
                    case 6: //* right shift
                        byte vry = this->registers[ry];
                        byte shifted = vry >> 1;
                        this->registers[0xf] = vry & MASK_LEAST_SIG_BIT;
                        this->registers[rx] = shifted;

                        break;
                    case 7:
                        uint16_t result = this->registers[ry] - this->registers[rx];

                        if (result < this->registers[ry]) {
                            this->registers[0xf] = 1;
                        } else {
                            // Borrow happened (underflow);
                            this->registers[0xf] = 0;
                        }

                        this->registers[rx] = (byte) result;
                        break;
                    case 0xE: //* left shift
                        byte vry = this->registers[ry];
                        byte shifted = vry << 1;
                        this->registers[0xf] = vry & MASK_LEAST_SIG_BIT;
                        this->registers[rx] = shifted;

                        break;
                }
                break;
            case 9:
                break;
            case 0xa:
                break;
            case 0xb:
                break;
            case 0xc:
                break;
            case 0xd:
                break;
            case 0xe:
                break;
            case 0xf:
                break;
            
            default:
                std::cout << "Unsupported instruction" << std::endl;
            
        }
    }

    void Cpu::next_instruction() {
        this->pc+=sizeof(instruction);
    }
    // Starts Cpu execution
    void Cpu::run() {
        // Fetch instruction
        this->pc = 0x200; // First aviable address for instructions
        while (true) {
            instruction ins = memory.fetch(this->pc);
            this->next_instruction();
            this->execute_instruction(ins);
        }
        // Execute instruction
    }
}