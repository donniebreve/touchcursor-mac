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

/*
 * Binds the Karabiner virtual keyboard.
 */
IOHIDDeviceRef bindKarabinerVirtualKeyboard(void);

/*
 * Prints the input report information.
 */
void printKarabinerVirtualKeyboardInputReport(KarabinerVirtualKeyboardReport* report);

#endif
