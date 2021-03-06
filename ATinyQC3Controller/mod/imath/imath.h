/*
 * imath.h
 *
 * Created: 5/27/2021 10:40:32 PM
 *  Author: IR
 */

#include <avr/io.h>

#ifndef IMATH_H_
#define IMATH_H_

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

#define clamp(val, minV, maxV) max(min(val, maxV), minV)

// Exponentiation by squaring
uint16_t ipow(uint16_t base, uint16_t exp);

#endif /* IMATH_H_ */