/*
 * QC3Control.c
 *
 * Modified: 5/28/2021 10:12:50 PM
 *  Author: IR
 *  Library: https://github.com/vdeconinck/QC3Control
 */ 

#include "../mod.h"

uint8_t _handshakeDone;  //!< Is the handshake done?
uint8_t _continuousMode; //!< Are we in continuous adjustment (QC3) mode?
uint8_t _classB;         //!< Do we have a class B QC source (up to 20V) ?
uint16_t _milliVoltNow = 5000; //!< Voltage currently set (in mV). Using the word "now" instead of "current" to prevent confusion between "current" and "voltage" :-)
uint8_t _DmGndPin = 0; //!< Pin pin connected to the bottom of the D- 1K5-10K bridge in legacy "3-wire" circuit. Must be 0 if using a recommended "2-wire" circuit // Do not port legacy circuit for now

void switchToContinuousMode();

// Low level functions to obtain desired voltages
void dmHiZ();
void dm0V();
void dm600mV();
void dm3300mV();
void dp600mV();
void dp3300mV();

void QC3_begin(uint8_t classB) {

    _classB = classB;
	
	millis_Enable();

    // The spec requires that D+ remains at 0.6V during _WaitTime.
    dp600mV(); // Setting D+ to 0.6V is done by default (Arduino pins are input on boot)
    if (_DmGndPin != 0) {
        // We're in "QC2 schema" + DmGndPin mode
        dmHiZ(); // Which is done by default

        //because the Arduino starts the right way we just wait till millis() passes
        //Has the advantage that if you call this last in setup, all other setup
        //stuff will be counted as "waiting" :)
        // After QC_T_GLITCH_BC_DONE_MS, the QC source removes the short between D+ and D- and pulls D- down.
        // We need to stay in this state for at least 2 more milliseconds before we can start requesting voltages.
        while (millis() < QC_T_GLITCH_BC_DONE_MS + 2);
    } else {
        // We're in "QC3 schema". There's no way to let D- "float", but setting it to 0.6V will prevent the D+/D- short from pulling D+/D- down
        dm600mV(); // Which is done by default

        //because the Arduino starts the right way we just wait till millis() passes
        //Has the advantage that if you call this last in setup, all other setup
        //stuff will be counted as "waiting" :) (No longer the case)
        while (millis() < QC_T_GLITCH_BC_DONE_MS);

        // After QC_T_GLITCH_BC_DONE_MS, the QC source removes the short between D+ and D- and pulls D- down.
        // We need to stay in this state for at least 2 more milliseconds before we can start requesting voltages.
        dm0V(); // "Acknowledge" by simulating that we "follow" the internal pull down
        _delay_ms(2);
    }
	
	millis_Disable();

    _handshakeDone = 1;
    _continuousMode = 0;
}

void QC3_set5V() {
    if (!_handshakeDone) {
        QC3_begin(0);
    }

    dp600mV();
    dm0V();

    _delay_ms(QC_T_GLICH_V_CHANGE_MS);

    _milliVoltNow = 5000;
    _continuousMode = 0;
}

void QC3_set9V() {
    if (!_handshakeDone) {
        QC3_begin(0);
    }

    if (_continuousMode) {
        // Transition from continuous to discrete values requires first going to 5V
        QC3_set5V();
    }

    dp3300mV();
    dm600mV();

    _delay_ms(QC_T_GLICH_V_CHANGE_MS);

    _milliVoltNow = 9000;
    _continuousMode = 0;
}

void QC3_set12V() {
    if (!_handshakeDone) {
        QC3_begin(0);
    }

    if (_continuousMode) {
        // Transition from continuous to discrete values requires first going to 5V
        QC3_set5V();
    }

    dp600mV();
    dm600mV();

    _delay_ms(QC_T_GLICH_V_CHANGE_MS);

    _milliVoltNow = 12000;
    _continuousMode = 0;
}

void QC3_set20V() {
    if (_classB) {
        if (!_handshakeDone) {
            QC3_begin(0);
        }

        if (_continuousMode) {
            // Transition from continuous to discrete values requires first going to 5V
            QC3_set5V();
        }

        dp3300mV();
        dm3300mV();

        _delay_ms(QC_T_GLICH_V_CHANGE_MS);

        _milliVoltNow = 20000;
        _continuousMode = 0;
    }
}

void QC3_incrementVoltage() {
    if (!_handshakeDone) {
        QC3_begin(0);
    }
    if (_milliVoltNow < (_classB ? QC3_CLASS_B_MAX_VOLTAGE_MV : QC3_CLASS_A_MAX_VOLTAGE_MV)) {
        if (!_continuousMode) {
            switchToContinuousMode();
        }
        // From http://www.onsemi.com/pub/Collateral/NCP4371-D.PDF :
        // "For the single request, an HVDCP recognizes a rising edge on D+ for an increment ..."
        dp3300mV();
        _delay_ms(QC_T_ACTIVE_MS);
        dp600mV();
        _delay_ms(QC_T_INACTIVE_MS);
    }

    _milliVoltNow += 200;
}

void QC3_decrementVoltage() {
    if (!_handshakeDone) {
        QC3_begin(0);
    }
    if (_milliVoltNow > QC3_MIN_VOLTAGE_MV) {
        if (!_continuousMode) {
            switchToContinuousMode();
        }

        // From http://www.onsemi.com/pub/Collateral/NCP4371-D.PDF :
        // "... and falling edge on D? for a decrement"
        dm600mV();
        _delay_ms(QC_T_ACTIVE_MS);
        dm3300mV();
        _delay_ms(QC_T_INACTIVE_MS);
    }
    _milliVoltNow -= 200;
}

uint8_t QC3_isContinuous() {
	return _continuousMode;
}

/* Returns the closest multiple of 200
 * e.g. passing 4901 or 4950 or 4999 or 5000 or 5001 or 5050 or 5100 returns 5000
 */
uint16_t getClosestValidMilliVolt(uint16_t mV) {
    return 200 * ((mV + 99) / 200);
}

void QC3_setMilliVoltage(uint16_t milliVolt) {
    if (!_handshakeDone) {
        QC3_begin(0);
    }

    if (milliVolt <= QC3_MIN_VOLTAGE_MV) {
        // below lower boundary: limit
        milliVolt = QC3_MIN_VOLTAGE_MV;
    } else if (milliVolt >= (_classB ? QC3_CLASS_B_MAX_VOLTAGE_MV : QC3_CLASS_A_MAX_VOLTAGE_MV)) {
        // above upper boundary: limit
        milliVolt = (_classB ? QC3_CLASS_B_MAX_VOLTAGE_MV : QC3_CLASS_A_MAX_VOLTAGE_MV);
    } else {
        // within boundaries: round
        milliVolt = getClosestValidMilliVolt(milliVolt); // useful if an invalid value was passed
    }

    if (milliVolt == _milliVoltNow)
        return;
		
	switch (milliVolt) {
		case 5000:
			QC3_set5V();
			return;
		case 9000:
			QC3_set9V();
			return;
		case 12000:
			QC3_set12V();
			return;
		case 20000:
			QC3_set20V();
			return;
	}

    if (milliVolt > _milliVoltNow) {
        while (_milliVoltNow < milliVolt) {
            QC3_incrementVoltage();
        }
    } else {
        while (_milliVoltNow > milliVolt) {
            QC3_decrementVoltage();
        }
    }
}

uint16_t QC3_getMilliVoltage() {
    return _milliVoltNow;
}

void switchToContinuousMode() {

    dp600mV();
    dm3300mV();

    _delay_ms(QC_T_GLICH_V_CHANGE_MS);

    _continuousMode = 1;
}

// Low level functions to obtain desired voltages

void dmHiZ() {
    if (_DmGndPin != 0) {
        // "disconnect" DM resistors : D- will be pulled down by QC source
        //pinMode(_DmPin, INPUT);
        //pinMode(_DmGndPin, INPUT);
    } else {
        // Hi-Z is impossible with this hardware !
    }
}

void dm0V() {
    if (_DmGndPin != 0) {
        //pinMode(_DmPin, INPUT); // "disconnect" DM top resistor
        //digitalWrite(_DmGndPin, LOW);
        //pinMode(_DmGndPin, OUTPUT); // and pull D- down
    } else {
		pinLow(QC3_DM_PIN);
		pinOutput(QC3_DM_PIN); // pull D- down
    }
}

void dm600mV() {
    if (_DmGndPin != 0) {
        //digitalWrite(_DmPin, HIGH); // Activate DM divider so it sets D- to about 0.6V
        //pinMode(_DmPin, OUTPUT);
        //digitalWrite(_DmGndPin, LOW);
        //pinMode(_DmGndPin, OUTPUT);
    } else {
		pinInput(QC3_DM_PIN); // Let the DM divider set D- to about 0.6V
    }
}

void dm3300mV() {
    if (_DmGndPin != 0) {
        //digitalWrite(_DmPin, HIGH); // Pull D- up (3.3V minimum)
        //pinMode(_DmPin, OUTPUT);
        //pinMode(_DmGndPin, INPUT); // And leave DM bottom resistor "unconnected"
    } else {
		pinHigh(QC3_DM_PIN); // Pull D- up (3.3V minimum)
		pinOutput(QC3_DM_PIN); // pull D- down
    }
}

void dp600mV() {
	pinInput(QC3_DP_PIN); // Let the DP divider set D+ to about 0.6V
}

void dp3300mV() {
	pinHigh(QC3_DP_PIN); // Pull D+ up (3.3V minimum)
	pinOutput(QC3_DP_PIN);
}
