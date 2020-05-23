#ifndef touchcursor_h
#define touchcursor_h

#include <stdio.h>

/**
 * Processes a key input event. Converts and emits events as necessary.
 */
void processKey(int code, int value);

/**
 * Drops all the currently held keys.
 */
void dropKeys(void);

#endif
