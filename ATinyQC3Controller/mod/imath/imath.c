/*
* imath.c
*
* Created: 5/27/2021 10:40:36 PM
*  Author: IR
*/

int ipow(int base, int exp) {
    int result = 1;
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
