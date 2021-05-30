/*
 * millis.c
 *
 * Created: 5/28/2021 6:28:33 PM
 *  Author: IR
 */

#include "../mod.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/atomic.h>

uint16_t millis_snap = 0;
uint16_t ms = 0;
uint16_t us = 0;

ISR(TIMER0_COMPA_vect) {
    ms++;
}

uint16_t millis(void) {
    uint16_t _ms;
    cli();
    _ms = ms;
    sei();
    return _ms;
}

void millis_Enable() {
    cli();
    ms = 0;
    us = 0;
    sei();
}

void millis_Disable() {
    cli();
}

void millis_TakeSnap(uint16_t future) {
    millis_snap = future + millis();
}

void millis_Init(void) {          // TODO: Increase prescaler as F_CPU increases
    OCR0A = ((F_CPU / 1000) / 8); // 125 @ 1 Mhz

    TCCR0A |= (1 << WGM01); // Clear when matching (CTC)
    TCCR0B |= (1 << CS01);  // 1/8th clock rate

    TIMSK |= (1 << OCIE0A); // Enable Counter0 Output Compare Match A interrupt
}