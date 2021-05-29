/*
 * input.h
 *
 * Created: 5/27/2021 12:04:50 PM
 *  Author: IR
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

#define FUNC_PIN ADC0D

#define ANALOG_VAL ADCH
#define pinHigh(port) PORTB |= (1<<port)
#define pinLow(port) PORTB &= ~(1<<port)
#define pinOutput(port) DDRB |= (1<<port)
#define pinInput(port) DDRB &= ~(1<<port)
#define enableButtonInput() ADMUX &= ~(1 << MUX0)
#define enablePotInput() ADMUX |= (1 << MUX0)
#define updateAnalog() ADCSRA |= (1 << ADSC); delay(8)
#define loadBtnVal() enableButtonInput(); updateAnalog()
#define loadPotVal() enablePotInput(); updateAnalog()
#define btnPressed() (ANALOG_VAL <= 230)
#define potVal() ANALOG_VAL

uint8_t buttonDown();

void btnWait();

void input_init();

#endif /* BUTTON_H_ */