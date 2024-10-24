#ifndef MEMORY_H_
#define MEMORY_H_

#include "common.h"

#define MEMORY_SIZE 4096



namespace cp8 {

    class Memory {
        private:
            byte _mem[MEMORY_SIZE];

        public:
            // TODO
            instruction fetch(address a);

            // TODO
            void load_data();
            void free_data();
    };
}

#endif // MEMORY_H_