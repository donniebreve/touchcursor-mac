#ifndef binding_h
#define binding_h

// The Run Loop
extern CFRunLoopRef runLoop;

/**
 * Binds the input device.
 */
int bindInput(int productID, int vendorID);

/**
 * Releases the input device.
 */
void releaseInput(void);

/**
 * Binds the output device.
 */
int bindOutput(void);

/**
 * Releases the output device.
 */
void releaseOutput(void);

/**
 * Starts the CFRunLoop.
 */
void startRunLoop(void);

/**
 * Stops the CFRunLoop.
 */
void stopRunLoop(void);

#endif
