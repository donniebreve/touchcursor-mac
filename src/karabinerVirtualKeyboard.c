#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDBase.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>
#include <IOKit/hidsystem/IOHIDShared.h>
#include <IOKit/hidsystem/IOHIDTypes.h>

#include "hidInformation.h"
#include "keys.h"
#include "karabinerVirtualKeyboard.h"

// The io connect reference for the virtual keyboard service
io_connect_t serviceConnection;
// The karabiner keyboard report state
KarabinerVirtualKeyboardReport karabinerVirtualKeyboardReport;

/**
 * Binds the Karabiner virtual keyboard.
 */
IOHIDDeviceRef bindKarabinerVirtualKeyboard(IOHIDManagerRef hidManager)
{
    // Default values
    karabinerVirtualKeyboardReport.id = 1;
    karabinerVirtualKeyboardReport.reserved = 0;
    for (int i = 0; i < 32; i++) karabinerVirtualKeyboardReport.keys[i] = 0;
    // Find the karabiner root service
    CFMutableDictionaryRef rootServiceMatch = IOServiceNameMatching(karabinerVirtualHIDRootName);
    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, rootServiceMatch);
    if (service == 0)
    {
        printf("error: pqrs root service not found.\n");
        return 0;
    }
    // Open the service
    IOReturn result = IOServiceOpen(service, mach_task_self(), kIOHIDServerConnectType, &serviceConnection);
    if (result != kIOReturnSuccess)
    {
        printf("error: serviceOpenResult=%s\n", getIOReturnString(result));
        return 0;
    }
    // Create the virtual keyboard
    // This is the pqrs::karabiner_virtual_hid_device::properties::keyboard_initialization class, one uint8_t
    uint8_t properties = 0; // Country Code (33?)
    // This is the method reference to karabiner_virtual_hid_device::user_client_method::initialize_virtual_hid_keyboard
    // See user_client_method enum in karabiner_virtual_hid_device.hpp
    uint32_t initialize_virtual_hid_keyboard_method = 0;
    result = IOConnectCallStructMethod(serviceConnection, initialize_virtual_hid_keyboard_method, &properties, sizeof(properties), NULL, 0);
    if (result != kIOReturnSuccess)
    {
        printf("error: deviceConnectResult=%s\n", getIOReturnString(result));
    }
    
    // Keyboard reports it is ready even when it is not, sleep a little to give it some time to start
    usleep(500);

    /* This seems unecessary if we wait instead
    // Check if the keyboard is ready
    bool isReady = false;
    size_t size = sizeof(isReady);
    // This is the method reference to karabiner_virtual_hid_device::user_client_method::is_virtual_hid_keyboard_ready
    // See user_client_method enum in karabiner_virtual_hid_device.hpp
    uint32_t is_virtual_hid_keyboard_ready_method = 2;
    while (!isReady)
    {
        virtualKeyboardResult = IOConnectCallStructMethod(connect, is_virtual_hid_keyboard_ready_method, NULL, 0, &isReady, &size);
        if (virtualKeyboardResult != kIOReturnSuccess)
        {
            printf("%s\n", getIOReturnString(virtualKeyboardResult));
        }
    }
    */
    
//    // Find the karabiner keyboard service
//    printf("Opening the Karabiner virtual keyboard... ");
//    CFMutableDictionaryRef keyboardServiceMatch = IOServiceNameMatching(karabinerVirtualHIDKeyboardName);
//    io_service_t keyboardService = IOServiceGetMatchingService(kIOMasterPortDefault, keyboardServiceMatch);
//    IOHIDDeviceRef outputDevice = IOHIDDeviceCreate(kCFAllocatorDefault, keyboardService);
//    if (outputDevice <= 0)
//    {
//        printf("Failed to create the device\n");
//    }
//    // Open the karabiner keyboard
//    virtualKeyboardResult = IOHIDDeviceOpen(outputDevice, kIOHIDOptionsTypeNone);
//    if (virtualKeyboardResult != kIOReturnSuccess)
//    {
//        printf("%s\n", getIOReturnString(virtualKeyboardResult));
//        return NULL;
//    }
//    else
//    {
//        printf("Success\n");
//        return outputDevice;
//    }

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
            if (productID == pqrsProductID && vendorID == pqrsVendorID)
            {
                IOHIDDeviceRef inputDevice = devices[i];
                printDeviceInformation(inputDevice, false, false, false, false);
                // Open the device and capture all input
                printf("Capturing the Karabiner Virtual Keyboard... ");
                // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
                // Use kIOHIDOptionsTypeSeizeDevice to capture the device and all inputs
                IOReturn result = IOHIDDeviceOpen(devices[i], kIOHIDOptionsTypeNone);
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
    return NULL;
}

/**
 * Sends a HID report to the Karabiner Device.
 */
void sendKarabinerVirtualKeyboardReport()
{
    printKarabinerVirtualKeyboardReport(&karabinerVirtualKeyboardReport);
    uint32_t post_keyboard_input_report_method = 3;
    IOReturn virtualKeyboardResult = IOConnectCallStructMethod(serviceConnection, post_keyboard_input_report_method, &karabinerVirtualKeyboardReport, sizeof(karabinerVirtualKeyboardReport), NULL, 0);
    if (virtualKeyboardResult != kIOReturnSuccess)
    {
        printf("sendReport: result=%s\n", getIOReturnString(virtualKeyboardResult));
    }
}

/**
 * Prints the report information.
 */
static int outputNum = 0;
void printKarabinerVirtualKeyboardReport(KarabinerVirtualKeyboardReport* report)
{
    printf("%i karabiner\n", outputNum++);
    printf("  id:        %i\n", report->id);
    printf("  modifiers: %02x\n", report->modifiers);
    printf("  reserved:  %02x\n", report->reserved);
    for (int i = 0; i < 6; i++)
    {
        printf("  keys[%i]:   %i\n", i, report->keys[i]);
    }
    printf("\n");
}

/**
 * Adds a modifier to the state.
 */
void setKarabinerVirtualKeyboardModifierDown(uint8_t code)
{
    uint8_t modifier = 0;
    switch (code)
    {
        case KEY_LEFTCTRL: { modifier = 1; break; }
        case KEY_LEFTSHIFT: { modifier = 2; break; }
        case KEY_LEFTALT: { modifier = 4; break; }
        case KEY_LEFTMETA: { modifier = 8; break; }
        case KEY_RIGHTCTRL: { modifier = 16; break; }
        case KEY_RIGHTSHIFT: { modifier = 32; break; }
        case KEY_RIGHTALT: { modifier = 64; break; }
        case KEY_RIGHTMETA: { modifier = 128; break; }
    }
    karabinerVirtualKeyboardReport.modifiers |= modifier;
}

/**
 * Removes a modifier from the state.
 */
void setKarabinerVirtualKeyboardModifierUp(uint8_t code)
{
    uint8_t modifier = 0;
    switch (code)
    {
        case KEY_LEFTCTRL: { modifier = 1; break; }
        case KEY_LEFTSHIFT: { modifier = 2; break; }
        case KEY_LEFTALT: { modifier = 4; break; }
        case KEY_LEFTMETA: { modifier = 8; break; }
        case KEY_RIGHTCTRL: { modifier = 16; break; }
        case KEY_RIGHTSHIFT: { modifier = 32; break; }
        case KEY_RIGHTALT: { modifier = 64; break; }
        case KEY_RIGHTMETA: { modifier = 128; break; }
    }
    karabinerVirtualKeyboardReport.modifiers &= ~(modifier);
}

/**
 * Adds a key to the state.
 */
void setKarabinerVirtualKeyboardKeyDown(uint8_t code)
{
    bool indexSet = false;
    uint8_t keyIndex = 0;
    for (int i = 0; i < 32; i++)
    {
        if (!indexSet && karabinerVirtualKeyboardReport.keys[i] == 0)
        {
            keyIndex = i;
            indexSet = true;
        }
        if (karabinerVirtualKeyboardReport.keys[i] == code)
        {
            return;
        }
    }
    karabinerVirtualKeyboardReport.keys[keyIndex] = code;
}

/**
 * Removes a key from the state.
 */
void setKarabinerVirtualKeyboardKeyUp(uint8_t code)
{
    for (int i = 0; i < 32; i++)
    {
        if (karabinerVirtualKeyboardReport.keys[i] == code)
        {
            karabinerVirtualKeyboardReport.keys[i] = 0;
        }
    }
}
