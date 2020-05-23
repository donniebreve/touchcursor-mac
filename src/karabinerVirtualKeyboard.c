#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>
#include <IOKit/hidsystem/IOHIDTypes.h>

#include "hidInformation.h"
#include "karabinerVirtualKeyboard.h"

io_connect_t outputConnect;

// Karabiner virtual keyboard HID report descriptor
const uint8_t karabinerVirtualKeyboardReportDescriptor[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    // Modifier keys
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0xE0,        //   Usage Minimum (0xE0)
    0x29, 0xE7,        //   Usage Maximum (0xE7)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x08,        //   Report Count (8)
    // Reserved
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x08,        //   Report Size (8)
    // Keys (32)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (2)
    0x05, 0x0C,        //   Usage Page (Consumer)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x19, 0x00,        //   Usage Minimum (Unassigned)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,        // Usage (0x01)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x03,        //   Report ID (3)
    0x05, 0xFF,        //   Usage Page (Reserved 0xFF)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x06,        // Usage (0x06)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x04,        //   Report ID (4)
    0x06, 0x01, 0xFF,  //   Usage Page (Vendor Defined 0xFF01)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
};

/*
 * Prints the input report information.
 */
void printKarabinerVirtualKeyboardInputReport(KarabinerVirtualKeyboardReport* report)
{
    printf("  id:        %i\n",   report->id);
    printf("  modifiers: %02x\n", report->modifiers);
    printf("  reserved:  %02x\n", report->reserved);
    for (int i = 0; i < 32; i++)
    {
        printf("  keys[%i]:   %i\n", i, report->keys[i]);
    }
}

/**
 * Binds the Karabiner virtual keyboard.
 */
IOHIDDeviceRef bindKarabinerVirtualKeyboard(void)
{
    // Find the karabiner root service
    CFMutableDictionaryRef rootServiceMatch = IOServiceNameMatching(karabinerVirtualHIDRootName);
    io_service_t rootService = IOServiceGetMatchingService(kIOMasterPortDefault, rootServiceMatch);
    // Open the service
    IOReturn serviceOpenResult = IOServiceOpen(rootService, mach_task_self(), kIOHIDServerConnectType, &outputConnect);
    if (serviceOpenResult != kIOReturnSuccess)
    {
        printf("%s\n", getIOReturnString(serviceOpenResult));
    }
    // Create the virtual keyboard
    // This is the pqrs::karabiner_virtual_hid_device::properties::keyboard_initialization class, one uint8_t
    uint8_t properties = 0; // Country Code (33?)
    // This is the method reference to karabiner_virtual_hid_device::user_client_method::initialize_virtual_hid_keyboard
    // See user_client_method enum in karabiner_virtual_hid_device.hpp
    uint32_t initialize_virtual_hid_keyboard_method = 0;
    IOReturn virtualKeyboardResult = IOConnectCallStructMethod(outputConnect, initialize_virtual_hid_keyboard_method, &properties, sizeof(properties), NULL, 0);
    if (virtualKeyboardResult != kIOReturnSuccess)
    {
        printf("%s\n", getIOReturnString(virtualKeyboardResult));
    }
    
    // Keyboard reports it is ready even when it is not, sleep a little to give it some time to start
    usleep(500);
    /* This seems unecessary if we just wait a little bit
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
    
    // Find the karabiner keyboard service
    printf("Opening the Karabiner virtual keyboard... ");
    CFMutableDictionaryRef keyboardServiceMatch = IOServiceNameMatching(karabinerVirtualHIDKeyboardName);
    io_service_t keyboardService = IOServiceGetMatchingService(kIOMasterPortDefault, keyboardServiceMatch);
    IOHIDDeviceRef outputDevice = IOHIDDeviceCreate(kCFAllocatorDefault, keyboardService);
    if (outputDevice <= 0)
    {
        printf("Failed to create the device\n");
    }
    // Open the karabiner keyboard
    virtualKeyboardResult = IOHIDDeviceOpen(outputDevice, kIOHIDOptionsTypeNone);
    if (virtualKeyboardResult != kIOReturnSuccess)
    {
        printf("%s\n", getIOReturnString(virtualKeyboardResult));
        return NULL;
    }
    else
    {
        printf("Success\n");
        return outputDevice;
    }
}
