#include "cpu.h"

namespace cp8 {
    Cpu::Cpu() {
        memory = Memory();
        // Register inicialization

        this->v0 = 0;
        this->v1 = 0;
        this->v2 = 0;
        this->v3 = 0;
        this->v4 = 0;
        this->v5 = 0;
        this->v6 = 0;
        this->v7 = 0;
        this->v8 = 0;
        this->v9 = 0;
        this->va = 0;
        this->vb = 0;
        this->vc = 0;
        this->vd = 0;
        this->ve = 0;
        this->vf = 0;

        this->pc = 0x200;
        this->i = 0;
    }

    byte Cpu::mask(instruction ins, uint16_t mask, byte size) {
        byte ret = ins && mask;
        ret >>= sizeof(instruction) - size;

        return ret;
    }

    void Cpu::execute_instruction(instruction ins) {
        byte first = this->mask(ins, MASK_FIRST, 4);

        // Filter by the first nibble
        switch (first) {
            case 0:
                // 0NNN: Not supported
                // TODO 00E0: Clear screen
                //* TODO: Define subroutines 00EE: Return from a subroutine
                break;
            case 1: // Only one option
                // * 1NNN: Jump to address 1NNN
                ins &= MASK_FIRST;
                this->pc = ins;
                break;
            case 2:
                //* TODO: Define subroutines 2NNN: Execute subroutine starting at address NNN
                break;
            case 3:
                //* 3XNN: Skip the following instruction if the vlaue of register X equals NN
                byte value = this->mask(ins, MASK_LAST_TWO, 8);
                
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
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
                LOG("Unsuported instruction");
            
        }
    }

    // Starts Cpu execution
    void Cpu::run() {
        // Fetch instruction
        while (true) {
            instruction ins = memory.fetch(this->pc);
            this->pc+=sizeof(instruction);
            this->execute_instruction(ins);
        }
        // Execute instruction
    }
}