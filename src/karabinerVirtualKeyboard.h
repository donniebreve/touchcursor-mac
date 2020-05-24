#ifndef karabinerVirtualKeyboard_h
#define karabinerVirtualKeyboard_h

#include <stdio.h>
#include <IOKit/IOKitLib.h>

#define karabinerVirtualHIDRootName "org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDRoot_v061000"
#define karabinerVirtualHIDKeyboardName "org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDKeyboard_v061000"

// The io connect reference for the virtual keyboard
extern io_connect_t outputConnect;

/*
 * Primary input report definition for the macOS keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[32];
} KarabinerVirtualKeyboardReport;

// The karabiner keyboard report state
extern KarabinerVirtualKeyboardReport karabinerVirtualKeyboardReport;

/*
 * Binds the Karabiner virtual keyboard.
 */
IOHIDDeviceRef bindKarabinerVirtualKeyboard(void);

/*
 * Prints the input report information.
 */
void printKarabinerVirtualKeyboardInputReport(KarabinerVirtualKeyboardReport* report);

/**
 * Adds a modifier to the state.
 */
void modifierDown(uint8_t code);

/**
 * Removes a modifier from the state.
 */
void modifierUp(uint8_t code);

/**
 * Adds a key to the state.
 */
void keyDown(uint8_t code);

/**
 * Removes a key from the state.
 */
void keyUp(uint8_t code);

#endif
