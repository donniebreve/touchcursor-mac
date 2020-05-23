#ifndef binding_h
#define binding_h

/**
 * Creates the HID manager.
 */
void createHIDManager(void);

/**
 * Binds the input device.
 */
int bindInput(void);

/**
 * Binds the output device.
 */
int bindOutput(void);

/**
 * Emits a key event.
 */
void emit(int code, int value);

#endif
