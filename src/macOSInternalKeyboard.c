#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>
#include <pthread.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"
#include "touchcursor.h"
#include "emit.h"

/**
 * Binds the macOS internal keyboard.
 */
IOHIDDeviceRef bindMacOSInternalKeyboard(IOHIDManagerRef hidManager)
{
    // Get set of devices
    CFSetRef deviceSet = IOHIDManagerCopyDevices(hidManager);
    CFIndex deviceCount = CFSetGetCount(deviceSet);
    IOHIDDeviceRef* devices = calloc(deviceCount, sizeof(IOHIDDeviceRef));
    CFSetGetValues(deviceSet, (const void **)devices);
    // Iterate devices
    for (CFIndex i = 0; i < deviceCount; i++)
    {
        // Check if the device is a keyboard, this also opens the device
        if (IOHIDDeviceConformsTo(devices[i], kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard))
        {
            // Check if this is the apple keyboard, expand this later to use a GUI selection
            uint32_t productID = getProductID(devices[i]);
            uint32_t vendorID = getVendorID(devices[i]);
            if (productID == appleProductID && vendorID == appleVendorID)
            {
                IOHIDDeviceRef inputDevice = devices[i];
                printDeviceInformation(inputDevice, false, false, false, false);
                // Open the device and capture all input
                printf("Capturing the MacOS Internal Keyboard... ");
                // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
                // Use kIOHIDOptionsTypeSeizeDevice to capture the device and all inputs
                IOReturn result = IOHIDDeviceOpen(devices[i], kIOHIDOptionsTypeSeizeDevice);
                if (result != kIOReturnSuccess)
                {
                    printf("%s\n", getIOReturnString(result));
                    return NULL;
                }
                printf("Success\n");
                return inputDevice;
            }
        }
    }
    printf("Failed to capture the MacOS Internal Keyboard.\n");
    return NULL;
}

/**
 * The input value callback method.
 */
void macOSKeyboardInputValueCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDValueRef value)
{
    IOHIDElementRef element = IOHIDValueGetElement(value);
    uint32_t code = IOHIDElementGetUsage(element);
    uint32_t down = (int)IOHIDValueGetIntegerValue(value);
    //printf("received code: %d value: %d\n", code, down);
    if (code == 3) // I've forgotten the relevance of code 3
    {
        emit(0, code, down);
    }
    if (3 < code && code < 232)
    {
        processKey(0, code, down);
        // Since we do not get repeated key down events from MacOS
        // Send the key event again to move to the hyper state after a short delay
        if (down && state == delay)
        {
            pthread_t thread_id;
            int* arg = malloc(sizeof(int));
            *arg = code;
            pthread_create(&thread_id, NULL, sendDelayed, arg);
            pthread_detach(thread_id);
        }
    }
}

/**
 * "Sends" a key event after a short delay.
 */
void* sendDelayed(void* arg)
{
    int code = *(int*)arg;
    usleep(100 * 1000); // milliseconds * microseconds
    if (state == delay)
    {
        processKey(0, code, 1);
    }
    free(arg);
    return 0;
}

/**
 * Prints the input report information.
 */
static int outputNum = 0;
void printMacOSInternalKeyboardInputReport(MacOSInternalKeyboardReport* report)
{
    printf("%i: mac\n", outputNum++);
    printf("  id:        %i\n", report->id);
    printf("  modifiers: %02x\n", report->modifiers);
    printf("  reserved:  %02x\n", report->reserved);
    for (int i = 0; i < 6; i++)
    {
        printf("  keys[%i]:   %i\n", i, report->keys[i]);
    }
    printf("  extended:  %02x\n", report->extendedModifiers);
}
