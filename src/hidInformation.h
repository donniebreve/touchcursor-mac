#ifndef hidInformation_h
#define hidInformation_h

#include <IOKit/hid/IOHIDValue.h>

int getProductID(IOHIDDeviceRef device);
int getVendorID(IOHIDDeviceRef device);
int getProductName(IOHIDDeviceRef device, wchar_t* buffer, size_t length);

/*
 * Prints the return string.
 */
char* getIOReturnString(IOReturn ioReturn);

/*
 * Prints information about the device.
 */
void printDeviceInformation(IOHIDDeviceRef device, bool path, bool reportSize, bool reportDescriptor);

#endif
