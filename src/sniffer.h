#ifndef SNIFFER_H_
#define SNIFFER_H_

#include "common.h"
#include "cpu.h"

#define register(x) this->status->registers[(x)]

/**
 * This class is atached to the CPU and prompts the state of the interpreter
 */

namespace cp8 {
    class SnifferCH8 {
        private:
            Cpu *cpu;
            CpuStatus* status;
        
        public:
            SnifferCH8(Cpu *cpu);
            void execute_instruction(instruction ins);
            void show_cpu_status();
    };
}

#endif // SNIFFER_H_