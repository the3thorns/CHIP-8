#include "sniffer.h"

namespace cp8 {
    SnifferCH8::SnifferCH8(Cpu *cpu) {
        this->cpu = cpu;
        this->status = this->cpu->share_status();
    }

    void SnifferCH8::execute_instruction(instruction ins) {
        cpu->embedd_instruction(ins);

        // Show status of the CPU
        this->show_cpu_status(ins);
    }

    void SnifferCH8::show_cpu_status(instruction ins) {
        std::cout << std::endl;

        std::cout << "Instruction: ";
        std::cout << SHOW_HEX << ins << std::endl;
        std::cout << CLEAR_HEX;
        std::cout << std::endl;

        for (int i = 0; i < 8; i++) {
            std::cout << "v" << i << ": " << SHOW_HEX << (int) REGISTER(i) << CLEAR_HEX << std::setw(8) 
                << "v" << i + 8 << ": " << SHOW_HEX << (int) REGISTER(i + 8) << CLEAR_HEX << std::endl;
        }
        std::cout << "pc: " << SHOW_HEX << PC << CLEAR_HEX << std::endl;
        std::cout << "i:  " << SHOW_HEX << I << CLEAR_HEX << std::endl;
        
        std::cout << std::endl;
    }
}