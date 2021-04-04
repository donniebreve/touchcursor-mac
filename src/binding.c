#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>

#include "macOSInternalKeyboard.h"
#include "cgEventVirtualKeyboard.h"
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
    int result = createCGEventSource();
    if (!result)
    {
        return 0;
    }
    return 1;
}
