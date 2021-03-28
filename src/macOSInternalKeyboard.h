#ifndef macOSInternalKeyboard_h
#define macOSInternalKeyboard_h

#include <stdio.h>
#include <IOKit/hid/IOHIDManager.h>

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
    uint8_t keys[6];
    uint8_t extendedModifiers;
} MacOSInternalKeyboardReport;

/**
 * Consumer input report definition for the MacOS keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t media;
    uint8_t unknown[2];
} MacOSKeyboardConsumerReport;

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

/**
 * Prints the input report information.
 */
void printMacOSInternalKeyboardInputReport(MacOSInternalKeyboardReport* report);

#endif
