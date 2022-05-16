#ifndef binding_h
#define binding_h

/**
 * Binds the input device.
 */
int bindInput(int filterCallback(int, int));

/**
 * Binds the output device.
 */
int bindOutput(void);

/**
 * Starts the CFRunLoop.
 */
void startRunLoop(void);

#endif
