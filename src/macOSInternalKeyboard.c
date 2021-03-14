#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"

/*
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
                IOReturn result = IOHIDDeviceOpen(devices[i], kIOHIDOptionsTypeNone); //kIOHIDOptionsTypeSeizeDevice);
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

/*
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
