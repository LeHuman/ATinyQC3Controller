/*
 * display.c
 *
 * Created: 5/27/2021 8:23:41 PM
 *  Author: Bitwise
 */ 

#include "../mod.h"

const char *currStr = "None";
double currNum = 0.0;
bool showStr = false;
bool displayed = true;

bool isEnabled() {
	return displayed;
}

void enable(bool enabled = true) {
	if (enabled == displayed)
	return;
	displayed = enabled;
	TM1637_enable(enabled);
	refreshLast();
}

void toggle() {
	enable(!displayed);
}

void refreshLast() {
	refresh(showStr);
}

void refresh(bool lastStr) {
	if (lastStr) {
		show(currStr);
		} else {
		show(currNum);
	}
}

void blink(uint32_t hold = 150, int blinks = 2) {
	bool state = displayed;
	for (int i = 0; i < blinks; i++) {
		enable(!state);
		delay(hold);
		enable(state);
		delay(hold);
	}
}

void hold(const char *s, uint32_t hold = 25) {
	show(s);
	delay(hold);
}

void hold(const double num, uint32_t hold = 25) {
	show(num);
	delay(hold);
}

void show(const char *s) {
	showStr = true;
	currStr = s;
	showString(s);
}

void show(const double num) {
	showStr = false;
	currNum = num;
	showNumber(num, 2, 4);
}

void clear() {
	TM1637_clear();
}

void init(){
	TM1637_init();
}