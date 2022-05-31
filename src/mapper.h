#ifndef mapper_h
#define mapper_h

#include <stdio.h>

// The state machine states
enum states
{
    idle,
    hyper,
    delay,
    map
};

extern enum states state;

/**
 * Processes a key input event. Converts and emits events as necessary.
 */
void processKey(int type, int code, int value);

/**
 * Drops all the currently held keys.
 */
void dropKeys(void);

#endif
