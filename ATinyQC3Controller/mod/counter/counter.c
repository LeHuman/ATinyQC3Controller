/*
 * counter.c
 *
 * Created: 5/28/2021 5:37:29 PM
 *  Author: IR
 */ 

#include "../mod.h"

uint8_t fastCounter() {
	static uint8_t c = 0;
	c++;
	return c % 4 == 0;
}

uint8_t slowCounter() {
	static uint8_t c = 0;
	c++;
	return c % 12 == 0;
}