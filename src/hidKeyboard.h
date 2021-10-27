#ifndef hidKeyboard_h
#define hidKeyboard_h

#include <IOKit/hid/IOHIDManager.h>

/**
 * Binds the keyboard.
 */
int bindKeyboard(uint32_t productID, uint32_t vendorID, IOHIDManagerRef hidManager);

/**
 * Releases the keyboard.
 */
void releaseKeyboard(void);

#endif
