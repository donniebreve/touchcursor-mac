# Karabiner virtual keyboard information

## Device name
The device name changes with new releases, documentation mentions KEXTs not being refreshed.

Example: org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDRoot_v061000

## HID Report Descriptor

``` c
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
    0xC0,              // End Collection (35 bytes)
    
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control, kHIDUsage_Csmr_ConsumerControl)
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
    0xC0,              // End Collection (33 bytes)
    
    0x06, 0x00, 0xFF,  // Usage Page (kHIDPage_AppleVendor)
    0x09, 0x01,        // Usage (kHIDUsage_AppleVendor_TopCase)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x03,        //   Report ID (3)
    0x05, 0xFF,        //   Usage Page (kHIDPage_AppleVendorTopCase)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection (33 bytes)
    
    0x06, 0x00, 0xFF,  // Usage Page (kHIDPage_AppleVendor)
    0x09, 0x06,        // Usage (kHIDUsage_AppleVendor_Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x04,        //   Report ID (4)
    0x06, 0x01, 0xFF,  //   Usage Page (kHIDPage_AppleVendorKeyboard)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection (33 bytes)
    
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x06,        // Usage (Keyboard)
    0xa1, 0x01,        // Collection (Application)
    0x85, 0x05,        //   Report Id (5)
    0x05, 0x08,        //   Usage Page (LED)
    0x95, 0x02,        //   Report Count (2)
    0x75, 0x01,        //   Report Size (1)
    0x19, 0x01,        //   Usage Minimum (1)
    0x29, 0x02,        //   Usage Maximum (2)
    0x91, 0x02,        //   Output (Data, Variable, Absolute)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x06,        //   Report Size (6)
    0x91, 0x01,        //   Output (Constant)
    0xc0,              // End Collection (2 bytes)

    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x06,        // Usage (Keyboard)
    0xa1, 0x01,        // Collection (Application)
    0x85, 0x06,        //   Report Id (6)
    0x05, 0x08,        //   Usage Page (LED)
    0x95, 0x02,        //   Report Count (2)
    0x75, 0x01,        //   Report Size (1)
    0x19, 0x01,        //   Usage Minimum (1)
    0x29, 0x02,        //   Usage Maximum (2)
    0x81, 0x02,        //   Input (Data, Variable, Absolute)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x06,        //   Report Size (6)
    0x81, 0x01,        //   Input (Constant)
    0xc0,              // End Collection (2 bytes)
};
```

## HID Value elements
