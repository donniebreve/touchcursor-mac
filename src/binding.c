#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOReturn.h>
#include <IOKit/hid/IOHIDBase.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <pthread.h>

#include "hidInformation.h"
#include "keys.h"
#include "macOSInternalKeyboard.h"
#include "karabinerVirtualKeyboard.h"
#include "touchcursor.h"
#include "binding.h"

// The HID manager object
static IOHIDManagerRef hidManager;

// The input and output devices
static IOHIDDeviceRef inputDevice;
static IOHIDDeviceRef outputDevice;

//std::optional<int64_t>
//get_initial_key_repeat(iokit_registry_entry_id::value_t registry_entry_id) {
//    return get_integer_property<int64_t>(
//        registry_entry_id,
//        CFSTR(kIOHIDServiceInitialKeyRepeatDelayKey));
//}
//
//std::optional<int64_t>
//get_key_repeat(iokit_registry_entry_id::value_t registry_entry_id) {
//    return get_integer_property<int64_t>(
//        registry_entry_id,
//        CFSTR(kIOHIDServiceKeyRepeatDelayKey));
//}

/**
 * Creates the HID manager.
 */
void createHIDManager()
{
    // Create the HID manager
    hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    // Match all devices
    IOHIDManagerSetDeviceMatching(hidManager, NULL);
    // Set the run loop
    IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
}

/**
 * Binds the input device.
 */
int bindInput()
{
    inputDevice = bindMacOSInternalKeyboard(hidManager);
    if (!inputDevice)
    {
        return 0;
    }
    // Register the input value callback
    IOHIDDeviceRegisterInputValueCallback(
        inputDevice,
        macOSKeyboardInputValueCallback,
        NULL);
    return 1;
}

/**
 * Binds the output device.
 */
int bindOutput()
{
    outputDevice = bindKarabinerVirtualKeyboard(hidManager);
    if (!outputDevice)
    {
        return 0;
    }
    return 1;
}
