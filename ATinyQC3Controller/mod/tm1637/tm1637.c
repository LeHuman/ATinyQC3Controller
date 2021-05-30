/*
 * tm1637.c
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

#include "../mod.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define TM1637_DIO_HIGH() (PORTB |= _BV(TM1637_DIO_PIN))
#define TM1637_DIO_LOW() (PORTB &= ~_BV(TM1637_DIO_PIN))
#define TM1637_DIO_OUTPUT() (DDRB |= _BV(TM1637_DIO_PIN))
#define TM1637_DIO_INPUT() (DDRB &= ~_BV(TM1637_DIO_PIN))
#define TM1637_DIO_READ() (((PINB & _BV(TM1637_DIO_PIN)) > 0) ? 1 : 0)
#define TM1637_CLK_HIGH() (PORTB |= _BV(TM1637_CLK_PIN))
#define TM1637_CLK_LOW() (PORTB &= ~_BV(TM1637_CLK_PIN))

#define CHAR_ 0b00000000

#define CHAR_0 0b00111111
#define CHAR_1 0b00000110
#define CHAR_2 0b01011011
#define CHAR_3 0b01001111
#define CHAR_4 0b01100110
#define CHAR_5 0b01101101
#define CHAR_6 0b01111101
#define CHAR_7 0b00000111
#define CHAR_8 0b01111111
#define CHAR_9 0b01101111

#define CHAR_A 0b01110111
#define CHAR_B 0b01111100
#define CHAR_C 0b00111001
#define CHAR_D 0b01011110
#define CHAR_E 0b01111001
#define CHAR_F 0b01110001
#define CHAR_G 0b00111101
#define CHAR_H 0b01110110
#define CHAR_I 0b00000110
#define CHAR_J 0b00011110
#define CHAR_K 0b01110110
#define CHAR_L 0b00111000
#define CHAR_M 0b00010101
#define CHAR_N 0b00110111
#define CHAR_O 0b00111111
#define CHAR_P 0b01110011
#define CHAR_Q 0b01100111
#define CHAR_R 0b00110001
#define CHAR_S 0b01101101
#define CHAR_T 0b01111000
#define CHAR_U 0b00111110
#define CHAR_V 0b00011100
#define CHAR_W 0b00101010
#define CHAR_X 0b01110110
#define CHAR_Y 0b01101110
#define CHAR_Z 0b01011011

#define bitSet(num, bit) num |= (1 << bit)
#define bitClr(num, bit) num &= ~(1 << bit)
#define bitGet(num, bit) 1 & (num >> bit)

static void TM1637_send_command(const uint8_t value);
static void TM1637_start(void);
static void TM1637_stop(void);
static uint8_t TM1637_write_byte(uint8_t value);

uint8_t byteBuf[TM1637_POSITION_MAX] = {0};
uint8_t enabled = 0;
uint8_t lastWasStr = 0;
uint16_t lastValue = -1;
uint8_t enabledDigits = 0b1111;

PROGMEM const uint8_t _digit2segments[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

PROGMEM const uint8_t _definedStrings[] = {
#define X(char3, char2, char1, char0) CHAR_##char3, CHAR_##char2, CHAR_##char1, CHAR_##char0,
    TM1637_STRINGS
#undef X
};

void TM1637_start(void) {
    TM1637_DIO_HIGH();
    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);
    TM1637_DIO_LOW();
}

void TM1637_stop(void) {
    TM1637_CLK_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_HIGH();
}

uint8_t TM1637_write_byte(uint8_t value) {
    uint8_t i, ack;

    for (i = 0; i < 8; ++i, value >>= 1) {
        TM1637_CLK_LOW();
        _delay_us(TM1637_DELAY_US);

        if (value & 0x01) {
            TM1637_DIO_HIGH();
        } else {
            TM1637_DIO_LOW();
        }

        TM1637_CLK_HIGH();
        _delay_us(TM1637_DELAY_US);
    }

    TM1637_CLK_LOW();
    TM1637_DIO_INPUT();
    TM1637_DIO_HIGH();
    _delay_us(TM1637_DELAY_US);

    ack = TM1637_DIO_READ();
    if (ack) {
        TM1637_DIO_OUTPUT();
        TM1637_DIO_LOW();
    }
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_OUTPUT();

    return ack;
}

void TM1637_send_command(const uint8_t value) {
    TM1637_start();
    TM1637_write_byte(value);
    TM1637_stop();
}

void TM1637_enable(const uint8_t enable) {
    enabled = enable;
    enabledDigits = 0b1111 * enable;
    TM1637_send_command(TM1637_CMD_SET_DISPLAY | TM1637_BRIGHTNESS | (enable ? TM1637_SET_DISPLAY_ON : TM1637_SET_DISPLAY_OFF));
}

uint8_t TM1637_is_enabled(void) {
    return enabled;
}

void TM1637_push_buff() {
    TM1637_send_command(TM1637_CMD_SET_DATA | TM1637_SET_DATA_A_ADDR);
    TM1637_start();
    TM1637_write_byte(TM1637_CMD_SET_ADDR);
    for (uint8_t i = 0; i < TM1637_POSITION_MAX; ++i) {
        TM1637_write_byte(byteBuf[i]);
    }
    TM1637_stop();
}

void TM1637_display_prog_string(const uint8_t progPosition) {
    lastWasStr = 1;
    lastValue = progPosition;
    byteBuf[0] = pgm_read_byte_near((uint8_t *)&_definedStrings + 0 + progPosition * 4);
    byteBuf[1] = pgm_read_byte_near((uint8_t *)&_definedStrings + 1 + progPosition * 4);
    byteBuf[2] = pgm_read_byte_near((uint8_t *)&_definedStrings + 2 + progPosition * 4);
    byteBuf[3] = pgm_read_byte_near((uint8_t *)&_definedStrings + 3 + progPosition * 4);
    TM1637_push_buff();
}

void _TM1637_display_number(uint16_t value) {
    lastWasStr = 0;
    lastValue = value;
    for (int8_t i = TM1637_POSITION_MAX - 1; i >= 0; --i) {
        byteBuf[i] = pgm_read_byte_near((uint8_t *)&_digit2segments + value % 10);
        value /= 10;
    }
#if TM1637_FAUX_DECIMAL == 1
    byteBuf[1] |= 0x80;
#endif
}

void TM1637_display_number(uint16_t value) {
    //enabledDigits = 0b1111;
    _TM1637_display_number(value);
    TM1637_push_buff();
}

void TM1637_refresh(void) {
    TM1637_enable(1);
    if (lastWasStr) {
        TM1637_display_prog_string(lastValue);
    } else {
        TM1637_display_number(lastValue);
    }
}

void TM1637_toggle(void) {
    TM1637_enable(!enabled);
}

void TM1637_toggle_digit(const uint8_t position) {
    if (!lastWasStr) {
        _TM1637_display_number(lastValue);
        if (bitGet(enabledDigits, position)) {
            byteBuf[position] = 0;
            bitClr(enabledDigits, position);
        } else {
            bitSet(enabledDigits, position);
        }
#if TM1637_FAUX_DECIMAL == 1
        byteBuf[1] |= 0x80;
#endif
        TM1637_push_buff();
    }
}

void TM1637_flash(const uint8_t count, const uint8_t wait) {
    for (uint8_t i = 0; i < count; ++i) {
        TM1637_toggle();
        for (uint8_t j = 0; j < wait; ++j) {
            _delay_ms(50);
        }
    }
}

void TM1637_clear(void) {
    for (uint8_t i = 0; i < TM1637_POSITION_MAX; ++i) {
        byteBuf[i] = 0;
    }
    TM1637_push_buff();
}

void TM1637_init() {
    DDRB |= (1 << TM1637_DIO_PIN | 1 << TM1637_CLK_PIN);
    PORTB &= ~(1 << TM1637_DIO_PIN | 1 << TM1637_CLK_PIN);
    TM1637_enable(1);
    TM1637_clear();
}