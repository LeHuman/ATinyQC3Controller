/*
 * state.h
 *
 * Created: 5/28/2021 4:49:39 PM
 *  Author: IR
 */

#ifndef STATE_H_
#define STATE_H_

typedef struct state_t state_t;
typedef struct page_t page_t;

typedef void (*pageFunc)(void);
typedef state_t *(*stateFunc)(void);

void nilFunc(void);

struct page_t {
    uint8_t ID;
    pageFunc run;
};

struct state_t {
    uint8_t count;
    stateFunc run;
    page_t pageHold;
    page_t pages[];
};

void stateRun(state_t *initState);

extern state_t *currentState;

#endif /* STATE_H_ */