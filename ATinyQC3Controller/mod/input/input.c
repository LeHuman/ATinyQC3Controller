/*
 * input.c
 *
 * Created: 5/27/2021 12:04:57 PM
 *  Author: IR
 */

#include "../mod.h"
#include <avr/io.h>

uint8_t buttonDown() {
    static uint8_t down = 0;
    updateAnalog();
    if (down) {
        down = 2 * btnPressed();
    } else {
        down = btnPressed();
    }
    return down;
}

void btnWait() {
    enableButtonInput();
    while (btnPressed()) {
        updateAnalog();
    }
}

void input_init() {
    ADMUX |= (1 << ADLAR); // Left Adjust the ADCH and ADCL registers to get 8-bit res
    ADCSRA = 0b10000101;   // Enable ADC & set division factor-32 for 250kHz ADC clock
}