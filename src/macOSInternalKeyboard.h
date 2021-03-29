#ifndef macOSInternalKeyboard_h
#define macOSInternalKeyboard_h

#include <IOKit/hid/IOHIDManager.h>

#define appleVendorID 1452
#define appleProductID 630

/**
 * Binds the macOS internal keyboard.
 */
int bindMacOSInternalKeyboard(IOHIDManagerRef hidManager);

/**
 * The input value callback method.
 */
void macOSKeyboardInputValueCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDValueRef value);

#endif
