#include "sniffer.h"

namespace cp8 {
    SnifferCH8::SnifferCH8(Cpu *cpu) {
        this->cpu = cpu;
        this->status = this->cpu->share_status();
    }

    void SnifferCH8::execute_instruction(instruction ins) {
        cpu->embedd_instruction(ins);

        // Show status of the CPU
        this->show_cpu_status();
    }

    void SnifferCH8::show_cpu_status() {
        // TODO
    }
}