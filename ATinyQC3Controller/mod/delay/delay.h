/*
 * delay.h
 *
 * Created: 5/28/2021 2:18:37 AM
 *  Author: IR
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include <util/delay.h>
#define delay(ms) _delay_ms(ms/(F_CPU/1000000));

#endif /* DELAY_H_ */