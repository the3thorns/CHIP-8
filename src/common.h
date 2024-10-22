#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <cstdint>

/**
 * Macros
 */


#define DEBUG

#ifdef DEBUG
    #define LOG(x) std::cout << (x) << std::endl;
#else
    #define LOG(x)
#endif

/**
 * Typedefs
*/

typedef uint8_t byte;
typedef uint16_t address;
typedef uint16_t instruction;



#endif // COMMON_H_