/*
* imath.c
*
* Created: 5/27/2021 10:40:36 PM
*  Author: IR
*/

#include <avr/io.h>

uint16_t ipow(uint16_t base, uint16_t exp) {
    uint16_t result = 1;
    for (;;) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}
