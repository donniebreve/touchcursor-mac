#ifndef binding_h
#define binding_h

#import <CoreFoundation/CFBag.h>

/**
 * Binds the input device.
 */
int bindInput(void);

/**
 * Binds the output device.
 */
int bindOutput(void);

/**
 * Starts the CFRunLoop.
 */
void startRunLoop(void);

/**
 * Get a list of keyboards
 */
void getKeyboards(CFStringRef** keyboards, int** count);

#endif
