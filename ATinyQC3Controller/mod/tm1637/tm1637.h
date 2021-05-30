/*
 * tm1637.h
 *
 * Modified: 5/27/2021 5:05:16 PM
 *  Author: IR
 *  Library: https://github.com/lpodkalicki/attiny-tm1637-library
 */

/**
* Copyright (c) 2017-2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
*
* This is ATtiny13/25/45/85 library for 4-Digit LED Display based on TM1637 chip.
*
* Features:
* - display raw segments
* - display digits
* - display colon
* - display on/off
* - brightness control
*
* References:
* - library: https://github.com/lpodkalicki/attiny-tm1637-library
* - documentation: https://github.com/lpodkalicki/attiny-tm1637-library/README.md
* - TM1637 data sheet: https://github.com/lpodkalicki/attiny-tm1637-library/blob/master/docs/TM1637_V2.4_EN.pdf
*/

#ifndef _ATTINY_TM1637_H_
#define _ATTINY_TM1637_H_

#include <stdint.h>

#define TM1637_STR_NONE 0
#define TM1637_STR_CNCL 1
#define TM1637_STR_VOLT 2
#define TM1637_STR_ACUR 3
#define TM1637_STR_DONE 4
#define TM1637_STR_BEGN 5
#define TM1637_STR_LED  6
#define TM1637_STR_8000 7
#define TM1637_STR_0800 8
#define TM1637_STR_0080 9
#define TM1637_STR_0008 10
#define TM1637_STR_NORM 11
#define TM1637_STR_DISC 12

#define TM1637_STRINGS \
    X(N, O, N, E)      \
    X(C, N, C, L)      \
    X(V, O, L, T)      \
    X(A, C, U, R)      \
    X(D, O, N, E)      \
    X(B, E, G, N)      \
    X(L, E, D, )       \
    X(8, , , )         \
    X(, 8, , )         \
    X(, , 8, )         \
    X(, , , 8)         \
    X(N, O, R, M)      \
    X(D, I, S, C)

// Main Settings
#define TM1637_DIO_PIN PB3
#define TM1637_CLK_PIN PB4
#define TM1637_DELAY_US (5)
#define TM1637_BRIGHTNESS (0) // 0 - 7
#define TM1637_FAUX_DECIMAL 1 // Cover the top part of the colon and ya have ya self a decimal point, 0 to disable
#define TM1637_POSITION_MAX (4)

// TM1637 commands
#define TM1637_CMD_SET_DATA 0x40
#define TM1637_CMD_SET_ADDR 0xC0
#define TM1637_CMD_SET_DISPLAY 0x80

// TM1637 data settings (use bitwise OR to construct complete command)
#define TM1637_SET_DATA_WRITE 0x00  // write data to the display register
#define TM1637_SET_DATA_READ 0x02   // read the key scan data
#define TM1637_SET_DATA_A_ADDR 0x00 // automatic address increment
#define TM1637_SET_DATA_F_ADDR 0x04 // fixed address
#define TM1637_SET_DATA_M_NORM 0x00 // normal mode
#define TM1637_SET_DATA_M_TEST 0x10 // test mode

// TM1637 display control command set (use bitwise OR to construct complete command)
#define TM1637_SET_DISPLAY_OFF 0x00 // off
#define TM1637_SET_DISPLAY_ON 0x08  // on

#define TM1637_display_string(stringName) TM1637_display_prog_string(TM1637_STR_##stringName)

/**
* Initialize pins and driver
*/
void TM1637_init();

/**
* Turn display on/off.
* value: 1 - on, 0 - off
*/
void TM1637_enable(const uint8_t value);

uint8_t TM1637_is_enabled(void);

/**
* Display digit ('0'..'9') at position (0x00..0x03)
*/
//void TM1637_display_digit(const uint8_t position, const uint8_t digit);

void TM1637_display_number(uint16_t value);

void TM1637_display_prog_string(const uint8_t progPosition);

void TM1637_refresh(void);

void TM1637_toggle(void);

void TM1637_toggle_digit(const uint8_t position);

void TM1637_flash(const uint8_t count, const uint8_t wait);

/**
* Clear all segments (including colon).
*/
void TM1637_clear(void);

#endif /* !_ATTINY_TM1637_H_ */
