/*
* ATinyQC3Controller.c
*
* Created: 5/27/2021 11:40:03 AM
* Author : Bitwise
*/

#include "mod/mod.h"

// Voltages in mV
#define MIN_VOLT 475U
#define MAX_VOLT 2000U

#define DEFAULT_VOLT 500U

uint16_t voltage = DEFAULT_VOLT;
uint16_t queuedVoltage = 0;

state_t *mainMenu(void);
state_t mainState = {
    .run = mainMenu,
    .pages = {
        {TM1637_STR_LED, TM1637_toggle},
        {TM1637_STR_VOLT, nilFunc},
        {TM1637_STR_ACUR, nilFunc},
    },
    .count = 3,
    .pageHold = {0},
};

state_t *voltSelect(void);
state_t voltState = {
    .run = voltSelect,
    .pages = {
        {TM1637_STR_CNCL, nilFunc},
    },
    .count = 1,
    .pageHold = {TM1637_STR_DONE, nilFunc},
};

state_t *acurSelect(void);
state_t acurState = {
    .run = acurSelect,
    .pages = {
        {TM1637_STR_8000, nilFunc},
        {TM1637_STR_0800, nilFunc},
        {TM1637_STR_0080, nilFunc},
        {TM1637_STR_0008, nilFunc},
        {TM1637_STR_CNCL, nilFunc},
    },
    .count = 5,
    .pageHold = {TM1637_STR_DONE, nilFunc},
};

state_t *acurSelect(void) {
    uint16_t setVolt = voltage;
	uint8_t pos = 0;
    TM1637_enable(1);
    millis_Enable();
    millis_TakeSnap(0);
    while (1) {
	    loadPotVal();
		setVolt = (setVolt / 1000 % 10) * 1000 * (pos != 0) + (setVolt / 100 % 10) * 100 * (pos != 1) + (setVolt / 10 % 10) * 10 * (pos != 2) + (setVolt % 10) * (pos != 3);
		setVolt += clamp(((potVal() - 40) * 5) / 97, 0, 9) * ipow(10, 3-pos); // ~40+ bytes for negative comparison
		
	    TM1637_display_number(setVolt);

	    if (millis() > (millis_snap + 300)) {
		    millis_TakeSnap(0);
		    TM1637_toggle_digit(pos);
	    }

	    loadBtnVal();
	    if (btnPressed()) {
		    TM1637_enable(1);
		    switch (nav_getSelection()) {
				case TM1637_STR_8000:
					pos	= 0;
					break;
				case TM1637_STR_0800:
					pos	= 1;
					break;
				case TM1637_STR_0080:
					pos	= 2;
					break;
				case TM1637_STR_0008:
					pos	= 3;
					break;
			    case TM1637_STR_DONE:
					queuedVoltage = setVolt;
			    case TM1637_STR_CNCL:
					TM1637_enable(1);
					return &mainState;
		    }
		    millis_Reset();
		    millis_TakeSnap(0);
	    }
    }
    millis_Disable();
    TM1637_enable(1);
    return &mainState;
}

state_t *voltSelect(void) {
    uint16_t setVolt = voltage;
    TM1637_enable(1);
    millis_Enable();
    millis_TakeSnap(0);
    while (1) {
        loadPotVal();
		setVolt = clamp((potVal() + 20U) * 8U, MIN_VOLT, MAX_VOLT);
        TM1637_display_number(setVolt);

        if (millis() > (millis_snap + 250U) || (millis() > (millis_snap + 100U) && (!TM1637_is_enabled() || setVolt == MIN_VOLT || setVolt == MAX_VOLT))) {
            millis_TakeSnap(0);
            TM1637_toggle();
        }

        loadBtnVal();

        if (btnPressed()) {
            TM1637_enable(1);
            switch (nav_getSelection()) {
            case TM1637_STR_DONE:
                queuedVoltage = setVolt;
            case TM1637_STR_CNCL:
                TM1637_enable(1);
                return &mainState;
            }
            millis_Enable();
            millis_TakeSnap(0);
        }
    }
    millis_Disable();
    TM1637_enable(1);
    return &mainState;
}

void normalizeVoltage() {
    if (!queuedVoltage)
        return;

    TM1637_enable(1);
    TM1637_display_string(NORM);
    _delay_ms(500);

    int8_t dv = 1 * (queuedVoltage < (MIN_VOLT + MAX_VOLT) / 2) - 1 * (queuedVoltage >= (MIN_VOLT + MAX_VOLT) / 2);
    uint8_t run = 1;

    while (run) {
        run = 0;

        if ((queuedVoltage % 10 != 0) || (queuedVoltage > MAX_VOLT && (queuedVoltage % 10 != MAX_VOLT % 10)) || (queuedVoltage < MIN_VOLT && (queuedVoltage % 10 != MIN_VOLT % 10))) {
            queuedVoltage += dv;
            run = 1;
        } else if (((queuedVoltage / 10) % 2 != 0) || (queuedVoltage > MAX_VOLT && ((queuedVoltage / 10) % 10 != (MAX_VOLT / 10) % 10)) || (queuedVoltage < MIN_VOLT && ((queuedVoltage / 10) % 10 != (MIN_VOLT / 10) % 10))) {
            queuedVoltage += dv * 10;
            run = 1;
        } else if (((queuedVoltage > MAX_VOLT) && ((queuedVoltage / 100) % 10 != (MAX_VOLT / 100) % 10)) || (queuedVoltage < MIN_VOLT && ((queuedVoltage / 100) % 10 != (MIN_VOLT / 100) % 10))) {
            queuedVoltage += dv * 100;
            run = 1;
        } else if ((queuedVoltage > MAX_VOLT && ((queuedVoltage / 1000) % 10 != (MAX_VOLT / 1000) % 10)) || (queuedVoltage < MIN_VOLT && ((queuedVoltage / 1000) % 10 != (MIN_VOLT / 1000) % 10))) {
            queuedVoltage += dv * 1000;
            run = 1;
        }

        TM1637_display_number(queuedVoltage);
        _delay_ms(100);
    }

    TM1637_display_number(queuedVoltage);
    TM1637_flash(4, 2);
    voltage = queuedVoltage;
    queuedVoltage = 0;

    _delay_ms(500);
    TM1637_display_string(DONE);
    _delay_ms(500);
}

void confirmVoltage() {
    if (QC3_getMilliVoltage() / 10 == voltage) {
        return;
    }

    QC3_setMilliVoltage(voltage * 10);
    uint16_t curr = voltage;
    voltage = QC3_getMilliVoltage() / 10;
    if (voltage != curr) {
        TM1637_display_number(voltage);
        TM1637_flash(4, 2);
    }
}

state_t *mainMenu(void) {
    normalizeVoltage();
    confirmVoltage();

    enableButtonInput();
    updateAnalog();
    _delay_ms(20);

    while (1) {
        TM1637_display_number(voltage);
        updateAnalog();
        if (btnPressed()) {
            switch (nav_getSelection()) {
            case TM1637_STR_VOLT:
                return &voltState;
            case TM1637_STR_ACUR:
                return &acurState;
            }
        }
    }
}

int main(void) {
    millis_Init();
    input_init();
    TM1637_init();

    TM1637_display_string(BEGN);
    TM1637_flash(4, 4);
    QC3_begin(1);
	QC3_set5V();

    stateRun(&mainState);
}
