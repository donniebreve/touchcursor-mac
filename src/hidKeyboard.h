#ifndef hidKeyboard_h
#define hidKeyboard_h

#include <IOKit/hid/IOHIDManager.h>

/**
 * Binds the macOS internal keyboard.
 */
int bindKeyboard(uint32_t productID, uint32_t vendorID, IOHIDManagerRef hidManager);

#endif
