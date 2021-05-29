/*
 * millis.c
 *
 * Created: 5/28/2021 6:28:33 PM
 *  Author: IR
 */ 

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "../mod.h"

uint16_t millis_snap = 0;
uint16_t ms = 0;
uint16_t us = 0;

ISR(TIM0_OVF_vect) {
	us += 256 * (F_CPU/1000000);
	while (us > 1000) {
		ms += us / 1000;
		us %= 1000;
	}
}

uint16_t millis(void) {
	uint16_t _ms;
	cli();
	_ms = ms;
	sei();
	return _ms;
}

void millis_Enable(){
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

void millis_Init(void) {
	TCCR0B |= ( 1<<CS01 );	// 1/8th clock rate
	TIMSK  |= ( 1<<TOIE0 );	// Enable timer overflow interrupt
}