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
#include "touchcursor.h"
#include "macOSInternalKeyboard.h"
#include "cgEventVirtualKeyboard.h"
//#include "karabinerVirtualKeyboard.h"
#include "binding.h"

// The HID manager object
static IOHIDManagerRef hidManager;

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
    int result = bindMacOSInternalKeyboard(hidManager);
    if (!result)
    {
        return 0;
    }
    return 1;
}

/**
 * Binds the output device.
 */
int bindOutput()
{
    int result = bindCGEventVirtualKeyboard();
    if (!result)
    {
        return 0;
    }
    return 1;
}

/**
 * Binds the output device.
 * Deprecated
 */
int bindKarabinerVirtualKeyboardOutput()
{
//    outputDevice = bindKarabinerVirtualKeyboard(hidManager);
//    if (!outputDevice)
//    {
//        return 0;
//    }
//    return 1;
    return 0;
}
