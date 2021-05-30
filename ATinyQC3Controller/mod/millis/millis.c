/*
 * millis.c
 *
 * Created: 5/28/2021 6:28:33 PM
 *  Author: IR
 */

#include "../mod.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <inttypes.h>

uint16_t millis_snap = 0;
uint16_t ms = 0;
uint16_t us = 0;

//ISR(TIM0_OVF_vect) {
    //us += 2000000000 / F_CPU; // Not sure exactly why, but this seems to be working @ 1Mhz
    //while (us > 1000) {
        //ms += us / 1000;
        //us %= 1000;
    //}
//}

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

void millis_Init(void) { // TODO: Increase prescaler as F_CPU increases
	OCR0A = ((F_CPU / 1000) / 8); // 125 @ 1 Mhz
	
	TCCR0A  |= (1 << WGM01); // Clear when matching (CTC)
	TCCR0B |= (1 << CS01); // 1/8th clock rate
	
	TIMSK |= (1 << OCIE0A); // Enable Counter0 Output Compare Match A interrupt
	
    // TCCR0B |= (1 << CS01); // 1/8th clock rate
    // TIMSK |= (1 << TOIE0); // Enable timer overflow interrupt
}