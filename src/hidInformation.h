#ifndef hidInformation_h
#define hidInformation_h

#include <IOKit/hid/IOHIDValue.h>

// The HID manager object
extern IOHIDManagerRef hidManager;

/**
 * Creates the HID manager.
 */
void createHIDManager(void);

/**
 * Gets a list of available keyboard devices.
 */
CFArrayRef getKeyboardList(void);

int getProductID(IOHIDDeviceRef device);
int getVendorID(IOHIDDeviceRef device);
int getProductName(IOHIDDeviceRef device, wchar_t* buffer, size_t length);
char* getIOReturnString(IOReturn ioReturn);

void printDeviceName(IOHIDDeviceRef device);

/**
 * Prints information about the device.
 */
void printDeviceInformation(IOHIDDeviceRef device, bool path, bool reportSize, bool reportDescriptor, bool elements);

#endif
