#ifndef macOSInternalKeyboard_h
#define macOSInternalKeyboard_h

#include <stdio.h>

#define appleVendorID 1452
#define appleProductID 630
#define macOSKeyboardMaxInputReportSize 65

/*
 * The report buffer.
 */
static uint8_t macOSKeyboardReportBuffer[65];

/*
 * Primary input report definition for the macOS internal keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t leds;
    uint8_t keys[6];
    uint8_t media;
} MacOSInternalKeyboardReport;

/*
 * Binds the macOS internal keyboard.
 */
IOHIDDeviceRef bindMacOSInternalKeyboard(IOHIDManagerRef hidManager);

/*
 * Prints the input report information.
 */
void printMacOSInternalKeyboardInputReport(MacOSInternalKeyboardReport* report);

#endif
