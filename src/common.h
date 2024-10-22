#ifndef COMMON_H_
#define COMMON_H_

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

typedef char byte;



#endif // COMMON_H_