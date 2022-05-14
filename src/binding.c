#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>

#include "macOSInternalKeyboard.h"
#include "cgEventVirtualKeyboard.h"
#include "binding.h"

// The HID manager object
static IOHIDManagerRef hidManager;

CFMutableDictionaryRef myCreateDeviceMatchingDictionary(UInt32 usagePage,
        UInt32 usage) {
    CFMutableDictionaryRef ret = CFDictionaryCreateMutable(kCFAllocatorDefault,
            0, &kCFTypeDictionaryKeyCallBacks,
            &kCFTypeDictionaryValueCallBacks);
    if (!ret)
        return NULL;

    CFNumberRef pageNumberRef = CFNumberCreate(kCFAllocatorDefault,
            kCFNumberIntType, &usagePage );
    if (!pageNumberRef) {
        CFRelease(ret);
        return NULL;
    }

    CFDictionarySetValue(ret, CFSTR(kIOHIDDeviceUsagePageKey), pageNumberRef);
    CFRelease(pageNumberRef);

    CFNumberRef usageNumberRef = CFNumberCreate(kCFAllocatorDefault,
            kCFNumberIntType, &usage);
    if (!usageNumberRef) {
        CFRelease(ret);
        return NULL;
    }

    CFDictionarySetValue(ret, CFSTR(kIOHIDDeviceUsageKey), usageNumberRef);
    CFRelease(usageNumberRef);

    return ret;
}


/**
 * Creates the HID manager.
 */
void createHIDManager()
{
    // Create the HID manager
    hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    CFMutableDictionaryRef ret = CFDictionaryCreateMutable(kCFAllocatorDefault,
                0, &kCFTypeDictionaryKeyCallBacks,
                &kCFTypeDictionaryValueCallBacks);

    CFMutableDictionaryRef keyboard =
        myCreateDeviceMatchingDictionary(0x01, 6);
    CFMutableDictionaryRef keypad =
        myCreateDeviceMatchingDictionary(0x01, 7);

    CFMutableDictionaryRef matchesList[] = {
        keyboard,
        keypad,
    };
    CFArrayRef matches = CFArrayCreate(kCFAllocatorDefault,
            (const void **)matchesList, 2, NULL);
    IOHIDManagerSetDeviceMatchingMultiple(hidManager, matches);

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
