#ifndef karabinerVirtualKeyboard_h
#define karabinerVirtualKeyboard_h

#include <stdio.h>
#include <IOKit/IOKitLib.h>

#define karabinerVirtualHIDRootName "org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDRoot_v061000"
#define karabinerVirtualHIDKeyboardName "org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDKeyboard_v061000"

#define pqrsVendorID 5824
#define pqrsProductID 10203

/**
 * Primary input report definition for the Karabiner keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[32];
} KarabinerVirtualKeyboardReport;

// The io connect reference for the virtual keyboard
extern io_connect_t outputConnect;
// The karabiner keyboard report state
extern KarabinerVirtualKeyboardReport karabinerVirtualKeyboardReport;

/**
 * Consumer input report definition for the Karabiner keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t keys[32];
} KarabinerVirtualKeyboardConsumerReport;

/**
 * Primary input report definition for the Karabiner keyboard
 */
typedef struct {
    uint8_t id;
    uint8_t modifiers;
} KarabinerVirtualKeyboardLedReport;

/**
 * Binds the Karabiner virtual keyboard.
 */
IOHIDDeviceRef bindKarabinerVirtualKeyboard(IOHIDManagerRef hidManager);

/**
 * Sends a HID report to the Karabiner Device.
 */
void sendKarabinerVirtualKeyboardReport(void);

/**
 * Prints the input report information.
 */
void printKarabinerVirtualKeyboardReport(KarabinerVirtualKeyboardReport* report);

/**
 * Adds a modifier to the state.
 */
void setKarabinerVirtualKeyboardModifierDown(uint8_t code);

/**
 * Removes a modifier from the state.
 */
void setKarabinerVirtualKeyboardModifierUp(uint8_t code);

/**
 * Adds a key to the state.
 */
void setKarabinerVirtualKeyboardKeyDown(uint8_t code);

/**
 * Removes a key from the state.
 */
void setKarabinerVirtualKeyboardKeyUp(uint8_t code);

#endif
