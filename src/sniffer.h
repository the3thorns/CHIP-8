#ifndef SNIFFER_H_
#define SNIFFER_H_

#include <iomanip>

#include "common.h"
#include "cpu.h"

#define REGISTER(x) this->status->registers[(x)]
#define SHOW_HEX    std::setbase(16) << std::showbase
#define CLEAR_HEX   std::resetiosflags(std::ios_base::basefield)
#define PC          this->status->pc
#define I           this->status->i

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
            void show_cpu_status(instruction ins);
    };
}

#endif // SNIFFER_H_