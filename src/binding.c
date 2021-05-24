#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>
#import <CoreFoundation/CFBase.h>

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
    createHIDManager();
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
    result = createCGEventTap();
    if (!result)
    {
        return 0;
    }
    
    return 1;
}

/**
 * Starts the CFRunLoop.
 */
void startRunLoop()
{
    CFRunLoopRun();
}

void getKeyboards(CFStringRef** keyboards, int** count)
{
    // Get set of devices
    CFSetRef deviceSet = IOHIDManagerCopyDevices(hidManager);
    CFIndex deviceCount = CFSetGetCount(deviceSet);
    IOHIDDeviceRef* devices = calloc(deviceCount, sizeof(IOHIDDeviceRef));
    CFSetGetValues(deviceSet, (const void **)devices);

    // Construct keyboard list
    *count = realloc(*count, sizeof(int));
    **count = 0;
    CFStringRef strRef;
    for(CFIndex i = 0; i < deviceCount; i++)
    {
        strRef = (CFStringRef) IOHIDDeviceGetProperty(
            devices[i],
            CFSTR(kIOHIDProductKey));
        if (strRef != NULL) {
            *keyboards = realloc(*keyboards, ((**count)+1) * sizeof(CFStringRef));
            (*keyboards)[**count] = (CFStringRef) IOHIDDeviceGetProperty(
                devices[i],
                CFSTR(kIOHIDProductKey));
            ++(**count);
        }
    }

    free(devices);
}
