/*
 * state.c
 *
 * Created: 5/28/2021 4:49:48 PM
 *  Author: IR
 */ 

#include "../mod.h"

state_t *currentState;

void stateRun(state_t *initState) {
	currentState = initState;
	while (1) {
		btnWait();
		currentState = currentState->run();
	}
}

void nilFunc(void){}