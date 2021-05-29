/*
 * millis.h
 *
 * Created: 5/28/2021 6:28:25 PM
 *  Author: IR
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

uint16_t millis(void);

void millis_Init(void);

#define millis_Reset() millis_Enable()

void millis_Enable();

void millis_Disable();

void millis_TakeSnap(uint16_t future);

extern uint16_t millis_snap;

#endif /* MILLIS_H_ */