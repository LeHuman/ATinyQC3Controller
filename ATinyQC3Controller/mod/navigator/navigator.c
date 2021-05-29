/*
 * navigator.c
 *
 * Created: 5/28/2021 6:11:25 PM
 *  Author: IR
 */ 

#include "../mod.h"

uint8_t c = -1;

void reset() {
	c = -1;
};

void select() {
	if (c >= currentState->count) {
		TM1637_flash(8,1);
	} else {
		TM1637_flash(4,2);
		currentState->pages[c].run();
		btnWait();
	}
}

page_t *next() {
	c++;

	if (c >= currentState->count) {
		TM1637_display_string(NONE);
		return 0;
	}

	TM1637_display_prog_string(currentState->pages[c].ID);
	return currentState->pages + c;
};

uint8_t nav_getSelection(void) {
	uint8_t state = TM1637_is_enabled();
	TM1637_enable(1);
	
	enableButtonInput();
	delay(50);
	updateAnalog();
	
	millis_Enable();
	
	if (currentState->pageHold.ID != 0) {
		while (btnPressed()){
			updateAnalog();
			if (millis() > CONFIRM_MILLIS * 2) {
				TM1637_display_prog_string(currentState->pageHold.ID);
				TM1637_flash(4,2);
				currentState->pageHold.run(); // undefined if currentState->pageHold is not valid and ID != 0
				btnWait();
				TM1637_enable(state);
				return currentState->pageHold.ID;
			}
		}
	}
	
	reset();
	page_t *sel = next();
	
	millis_Reset();
	millis_TakeSnap(CONFIRM_MILLIS);
	buttonDown();
	while (sel != 0 && millis() < millis_snap) {
		if (buttonDown() == 1) {
			millis_TakeSnap(CONFIRM_MILLIS);
			sel = next();
		}
	}
	millis_Disable();
	TM1637_enable(state);
	select();
	reset();
	return sel->ID;
}