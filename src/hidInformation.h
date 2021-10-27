#ifndef hidInformation_h
#define hidInformation_h

#include <IOKit/hid/IOHIDValue.h>

struct KeyboardInformation {
   CFStringRef  Name;
   int          ProductID;
   int          VendorID;
};

// The HID manager object
extern IOHIDManagerRef hidManager;

/**
 * Creates the HID manager.
 */
void createHIDManager(void);

/**
 * Gets a list of available keyboard devices.
 */
int getKeyboardList(struct KeyboardInformation**);

int getProductName(IOHIDDeviceRef device, UInt8* buffer, size_t length);
int getProductID(IOHIDDeviceRef device);
int getVendorID(IOHIDDeviceRef device);
char* getIOReturnString(IOReturn ioReturn);

void printDeviceName(IOHIDDeviceRef device);

/**
 * Prints information about the device.
 */
void printDeviceInformation(IOHIDDeviceRef device, bool path, bool reportSize, bool reportDescriptor, bool elements);

#endif
