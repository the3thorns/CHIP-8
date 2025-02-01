#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdint.h>

/**
 * Macros
 */


#define DEBUG

#ifdef DEBUG
    #define LOG(x) printf("%s\n", x);
#else
    #define LOG(x)
#endif

#define MEMORY_SIZE 4096

#define TARGET_FPS 30

#ifndef TARGET_FPS
    #define TARGET_FPS 30
#endif

/**
 * Typedefs
*/

typedef uint8_t byte;
typedef uint16_t address;
typedef uint16_t instruction;



#endif // COMMON_H_