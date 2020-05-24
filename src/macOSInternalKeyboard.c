#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"

// Mac specific internal keyboard HID report descriptor
const uint8_t macOSInternalKeyboardReportDescriptor[] = {
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
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    // Reserved
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    // LEDs
    0x95, 0x05,        //   Report Count (5)
    0x75, 0x01,        //   Report Size (1)
    0x05, 0x08,        //   Usage Page (LEDs)
    0x19, 0x01,        //   Usage Minimum (Num Lock)
    0x29, 0x05,        //   Usage Maximum (Kana)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x03,        //   Report Size (3)
    0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    // Keys (6)
    0x95, 0x06,        //   Report Count (6)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    // Media
    0x05, 0x0C,        //   Usage Page (Consumer)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x01,        //   Report Count (1)
    0x09, 0xB8,        //   Usage (Eject)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0xFF,        //   Usage Page (Reserved 0xFF)
    0x09, 0x03,        //   Usage (0x03)
    0x75, 0x07,        //   Report Size (7)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
                       // 11 bytes
    
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x52,        //   Report ID (82)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x01,        //   Report Count (1)
    0x09, 0xCD,        //   Usage (Play/Pause)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0xB3,        //   Usage (Fast Forward)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0xB4,        //   Usage (Rewind)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0xB5,        //   Usage (Scan Next Track)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0xB6,        //   Usage (Scan Previous Track)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x85, 0x09,        //   Report ID (9)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x01,        //   Report Count (1)
    0x06, 0x01, 0xFF,  //   Usage Page (Vendor Defined 0xFF01)
    0x09, 0x0B,        //   Usage (0x0B)
    0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x02,        //   Report Count (2)
    0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection
                       // 6 bytes?
    
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x06,        // Usage (0x06)
    0xA1, 0x01,        // Collection (Application)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x06,        //   Usage (0x06)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x40,        //   Report Count (64)
    0x85, 0x3F,        //   Report ID (63)
    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0xC0,              // End Collection
                       // 65 bytes
    
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x0F,        // Usage (0x0F)
    0xA1, 0x01,        // Collection (Application)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x0F,        //   Usage (0x0F)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x0F,        //   Report Count (15)
    0x85, 0xBF,        //   Report ID (-65)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
                       // 16 bytes
};

/*
 * The input report callback method. Unsure if this is working.
 * Not used, kept for reference for now.
 */
static void macOSKeyboardInputReportCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDReportType type,
    uint32_t reportID,
    uint8_t* report,
    CFIndex reportLength)
{
    if (reportID == 1)
    {
        MacOSInternalKeyboardReport* keyboardReport = (MacOSInternalKeyboardReport*)report;
        printMacOSInternalKeyboardInputReport(keyboardReport);
    }
}

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
                printDeviceInformation(inputDevice, false, false, false);
                
                // To do: Filter the input values we want to see
                //IOHIDDeviceSetInputValueMatching(, )
                
                // Open the device and capture all input
                printf("Capturing the MacOS Internal Keyboard... ");
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

/*
 * Prints the input report information.
 */
void printMacOSInternalKeyboardInputReport(MacOSInternalKeyboardReport* report)
{
    printf("  id:        %i\n",   report->id);
    printf("  modifiers: %02x\n", report->modifiers);
    printf("  reserved:  %02x\n", report->reserved);
    for (int i = 0; i < 1; i++)
    {
        printf("  keys[%i]:   %i\n", i, report->keys[i]);
    }
    printf("  media:     %02x\n", report->media);
}
