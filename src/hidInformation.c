#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>

#include "hidInformation.h"

// The HID manager object
IOHIDManagerRef hidManager;

// Information

// Super useful "wiki"
// https://stackoverflow.com/questions/30308192/how-to-hook-remap-an-arbitrary-keyboard-event-on-osx

// HID report descriptor explanation
// https://eleccelerator.com/usbdescreqparser/
// https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/

// HID report values
// https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2

// Reading from a keyboard
// https://github.com/lcgamboa/USBIP-Virtual-USB-Device/blob/master/c/hid-keyboard.c
// https://github.com/libusb/hidapi/blob/master/mac/hid.c (uses cross platform methods)
// https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-870.1.10/tools/hidScript/hidScript.cpp.auto.html
// https://cpp.hotexamples.com/examples/-/-/IOHIDDeviceGetProperty/cpp-iohiddevicegetproperty-function-examples.html

// Code that creates a HID keyboard from userspace (none of this works)
// https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-503.1.13/tools/IOHIDUserDeviceTest.c
// https://github.com/lcgamboa/USBIP-Virtual-USB-Device/blob/master/c/hid-keyboard.c
// https://github.com/unbit/foohid
// https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-870.1.10/tools/hidScript/hidScript.cpp.auto.html

// Reading input samples
// https://stackoverflow.com/questions/40954979/mouse-iohiddeviceref-not-delivering-any-input-reports-in-macos-10-12

// |        | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
// | byte 0 | 231   | 230   | 229   | 228   | 227   | 226   | 225   | 224   |
// | byte 1 |       |       |       |       |       |       |       |       |
// | byte 2 | key 0: 0-255                                                  |
// ...
// | byte 7 | key 5: 0-255                                                 |

// Generic Keyboard HID report descriptor
const uint8_t genericKeyboardReportDescriptor[] = {
    // Device
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x06,       // Usage (Keyboard)
    0xa1, 0x01,       // Collection (Application)
    // Modifier keys, bit 0 = key code 224, bit 7 = keycode 231, 1/0 on/off
    0x95, 0x08,       //   Report Count............ (8)
    0x75, 0x01,       //   Report Size............. (1)
    0x15, 0x00,       //   Logical Minimum......... (0)
    0x25, 0x01,       //   Logical Maximum......... (1)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0xe0,       //   Usage Minimum........... (224)
    0x29, 0xe7,       //   Usage Maximum........... (231)
    0x81, 0x02,       //   Input................... (Data, Variable, Absolute)
    // Reserved (padding to make it 8 bytes total)
    0x95, 0x01,       //   Report Count............ (1)
    0x75, 0x08,       //   Report Size............. (8)
    0x81, 0x01,       //   Input................... (Constant)
    // Keys (6 keys can be sent at a time)
    0x95, 0x06,       //   Report Count............ (6)
    0x75, 0x08,       //   Report Size............. (8)
    0x15, 0x00,       //   Logical Minimum......... (0)
    0x26, 0xff, 0x00, //   Logical Maximum......... (255)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0x00,       //   Usage Minimum........... (0)
    0x29, 0xff,       //   Usage Maximum........... (255)
    0x81, 0x00,       //   Input................... (Data, Array, Absolute)
    0xc0              // End Collection
};

// Buffer for printing device information
static char buffer[256];

// Buffer for printing report information
static uint8_t reportBuffer[256];

/**
 * Creates the HID manager.
 */
void createHIDManager()
{
    // Create the HID manager
    hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    // Match all devices
    IOHIDManagerSetDeviceMatching(hidManager, NULL);
    // Set the run loop
    IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
}

int getKeyboardList(struct KeyboardInformation** keyboardsPointer)
{
    int index = 0;
    struct KeyboardInformation* keyboards = 0;
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
            keyboards = (struct KeyboardInformation*)realloc(keyboards, (index + 1) * sizeof(struct KeyboardInformation));
            keyboards[index].Name = CFStringCreateCopy(kCFAllocatorDefault, IOHIDDeviceGetProperty(devices[i], CFSTR(kIOHIDProductKey)));
            keyboards[index].ProductID = getProductID(devices[i]);
            keyboards[index].VendorID = getVendorID(devices[i]);
            ++index;
        }
    }
    free(devices);
    (*keyboardsPointer) = keyboards;
    return index;
}

/**
 * Gets a string property from the given HID reference.
 */
static int getStringProperty(IOHIDDeviceRef device, CFStringRef property, UInt8* buffer, size_t length)
{
    // Check for no length
    if (length <= 0)
    {
        return 0;
    }
    // Clear the buffer
    buffer[0] = '\0';
    // Get the string reference
    CFStringRef stringReference = (CFStringRef)IOHIDDeviceGetProperty(device, property);
    if (!stringReference)
    {
        return -1;
    }
    // Get the value length
    CFIndex valueLength = CFStringGetLength(stringReference);
    // Set up the range to copy
    CFRange range;
    range.location = 0;
    range.length = length;
    if (valueLength < length) range.length = valueLength;
    // Copy the value to the buffer
    CFIndex convertedCharacters = CFStringGetBytes(
        stringReference,
        range,
        kCFStringEncodingUTF32LE,
        '?',
        FALSE,
        (UInt8*)buffer,
        length * sizeof(wchar_t),
        NULL);
    buffer[convertedCharacters] = '\0';
    return (int)convertedCharacters;
}

/**
 * Gets binary property from the given HID reference.
 */
static int getDataProperty(IOHIDDeviceRef device, CFStringRef property, uint8_t* buffer, size_t length)
{
    // Check for no length
    if (length <= 0)
    {
        return 0;
    }
    // Clear the buffer
    buffer[0] = 0;
    // Get the data reference
    CFDataRef dataReference = (CFDataRef)IOHIDDeviceGetProperty(device, property);
    if (!dataReference)
    {
        return -1;
    }
    // Get the value length
    CFIndex valueLength = CFDataGetLength(dataReference);
    // Set up the range to copy
    CFRange range;
    range.location = 0;
    range.length = length;
    if (valueLength < length) range.length = valueLength;
    // Copy the value to the buffer
    CFDataGetBytes(dataReference, range, buffer);
    return (int)range.length;
}

/**
 * Gets an int from the given HID reference.
 */
static int32_t getIntProperty(IOHIDDeviceRef device, CFStringRef property)
{
    CFTypeRef typeReference = IOHIDDeviceGetProperty(device, property);
    if (typeReference) {
        if (CFGetTypeID(typeReference) == CFNumberGetTypeID()) {
            int32_t value;
            CFNumberGetValue((CFNumberRef)typeReference, kCFNumberSInt32Type, &value);
            return value;
        }
    }
    return 0;
}

/**
 * Gets an int from the given HID reference.
 */
static int64_t getLongProperty(IOHIDDeviceRef device, CFStringRef property)
{
    CFTypeRef typeReference = IOHIDDeviceGetProperty(device, property);
    if (typeReference) {
        if (CFGetTypeID(typeReference) == CFNumberGetTypeID()) {
            int64_t value;
            CFNumberGetValue((CFNumberRef)typeReference, kCFNumberSInt64Type, &value);
            return value;
        }
    }
    return 0;
}

/**
 * Gets the Product ID from the given HID reference.
 */
int getProductID(IOHIDDeviceRef device)
{
    return getIntProperty(device, CFSTR(kIOHIDProductIDKey));
}

/**
 * Gets the Vendor ID from the given HID reference.
 */
int getVendorID(IOHIDDeviceRef device)
{
    return getIntProperty(device, CFSTR(kIOHIDVendorIDKey));
}

/**
 * Gets the Manufacturer name from the given HID reference.
 */
static int getManufacturerName(IOHIDDeviceRef device, UInt8* buffer, size_t length)
{
    return getStringProperty(device, CFSTR(kIOHIDManufacturerKey), buffer, length);
}

/**
 * Gets the Product name from the given HID reference.
 */
int getProductName(IOHIDDeviceRef device, UInt8* buffer, size_t length)
{
    return getStringProperty(device, CFSTR(kIOHIDProductKey), buffer, length);
}

/**
 * Gets the Serial Number from the given HID reference.
 */
static int getSerialNumber(IOHIDDeviceRef device, UInt8* buffer, size_t length)
{
    return getStringProperty(device, CFSTR(kIOHIDSerialNumberKey), buffer, length);
}

/**
 * Gets the Location ID from the given HID reference.
 */
static int getLocationID(IOHIDDeviceRef device)
{
    return getIntProperty(device, CFSTR(kIOHIDLocationIDKey));
}

/**
 * Gets the Maximum Input Report Size from the given HID reference.
 */
static long getMaxInputReportSize(IOHIDDeviceRef device)
{
    return getLongProperty(device, CFSTR(kIOHIDMaxInputReportSizeKey));
}

/**
 * Gets the Maximum Input Report Size from the given HID reference.
 */
static long getMaxOutputReportSize(IOHIDDeviceRef device)
{
    return getLongProperty(device, CFSTR(kIOHIDMaxOutputReportSizeKey));
}

/**
 * Gets the Unique Device ID from the given HID reference.
 */
static int getPhysicalDeviceUniqueID(IOHIDDeviceRef device, UInt8* buffer, size_t length)
{
    return getStringProperty(device, CFSTR(kIOHIDPhysicalDeviceUniqueIDKey), buffer, length);
}

/**
 * Gets the report descriptor for the given HID device.
 */
static int getReportDescriptor(IOHIDDeviceRef device, UInt8* buffer, size_t length)
{
   return getDataProperty(device, CFSTR(kIOHIDReportDescriptorKey), buffer, length);
}

/**
 * Gets the Path for the given HID reference.
 */
static int getPath(IOHIDDeviceRef device, char* buffer, size_t length)
{
    io_string_t path;
    io_service_t service = IOHIDDeviceGetService(device);
    kern_return_t result = IORegistryEntryGetPath(service, kIOServicePlane, path);
    if (result == KERN_SUCCESS) {
        strcpy(buffer, path);
        return (int)strlen(buffer);
    }
    return 0;
}

/**
 * Prints the value elements for the device.
 */
static void printElements(IOHIDDeviceRef device)
{
    CFArrayRef elements = IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone);
    for (CFIndex i = 0; i < CFArrayGetCount(elements); i++)
    {
        IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, i);
        uint32_t usagePage = IOHIDElementGetUsagePage(element);
        uint32_t usage = IOHIDElementGetUsage(element);
        printf("element[%ld]: usagePage(%i) usage (%i)\n", i, usagePage, usage);
    }
    printf("\n");
}

/**
 * Creates a matching dictionary for the given device.
 */
static CFDictionaryRef createMatchingDictionary(IOHIDDeviceRef device)
{
    int vendorID = (int)getVendorID(device);
    int productID = (int)getProductID(device);
    int usagePage = kHIDPage_GenericDesktop;
    int usage = kHIDUsage_GD_Keyboard;
    CFStringRef keys[4] = {
        CFSTR(kIOHIDVendorIDKey),
        CFSTR(kIOHIDProductIDKey),
        CFSTR(kIOHIDDeviceUsagePageKey),
        CFSTR(kIOHIDDeviceUsageKey),
    };
    CFNumberRef values[4] = {
        CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &vendorID),
        CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &productID),
        CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usagePage),
        CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usage),
    };
    return CFDictionaryCreate(
        kCFAllocatorDefault,
        (const void **)keys,
        (const void **)values,
        4,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks);
}

/**
 * Prints the report data.
 */
static void printReport(uint8_t* report, CFIndex length)
{
    printf("Report Descriptor:\n");
    for (int i = 0; i < length; i++)
    {
        if (i > 0 && i % 8 == 0) printf("\n");
        printf("%02x ", report[i]);
    }
    printf("\n");
}

/**
 * Prints the return string.
 */
char* getIOReturnString(IOReturn ioReturn)
{
    switch (ioReturn)
    {
        case kIOReturnSuccess         : return "Success";
        case kIOReturnError           : return "General error";
        case kIOReturnNoMemory        : return "Can't allocate memory";
        case kIOReturnNoResources     : return "Resource shortage";
        case kIOReturnIPCError        : return "Error during IPC";
        case kIOReturnNoDevice        : return "No such device";
        case kIOReturnNotPrivileged   : return "Privilege violation";
        case kIOReturnBadArgument     : return "Invalid argument";
        case kIOReturnLockedRead      : return "Device read locked";
        case kIOReturnLockedWrite     : return "Device write locked";
        case kIOReturnExclusiveAccess : return "Exclusive access and device already open";
        case kIOReturnBadMessageID    : return "Sent/received messages had different msg_id";
        case kIOReturnUnsupported     : return "Unsupported function";
        case kIOReturnVMError         : return "Miscellaneous VM failure";
        case kIOReturnInternalError   : return "Internal error";
        case kIOReturnIOError         : return "General I/O error";
        case kIOReturnCannotLock      : return "Can't acquire lock";
        case kIOReturnNotOpen         : return "Device not open";
        case kIOReturnNotReadable     : return "Read not supported";
        case kIOReturnNotWritable     : return "Write not supported";
        case kIOReturnNotAligned      : return "Alignment error";
        case kIOReturnBadMedia        : return "Media Error";
        case kIOReturnStillOpen       : return "Device(s) still open";
        case kIOReturnRLDError        : return "RLD failure";
        case kIOReturnDMAError        : return "DMA failure";
        case kIOReturnBusy            : return "Device Busy";
        case kIOReturnTimeout         : return "I/O Timeout";
        case kIOReturnOffline         : return "Device offline";
        case kIOReturnNotReady        : return "Not ready";
        case kIOReturnNotAttached     : return "Device not attached";
        case kIOReturnNoChannels      : return "No DMA channels left";
        case kIOReturnNoSpace         : return "No space for data";
        case kIOReturnPortExists      : return "Port already exists";
        case kIOReturnCannotWire      : return "Can't wire down physical memory";
        case kIOReturnNoInterrupt     : return "No interrupt attached";
        case kIOReturnNoFrames        : return "No DMA frames enqueued";
        case kIOReturnMessageTooLarge : return "Oversized msg received on interrupt port";
        case kIOReturnNotPermitted    : return "Not permitted";
        case kIOReturnNoPower         : return "No power to device";
        case kIOReturnNoMedia         : return "Media not present";
        case kIOReturnUnformattedMedia: return "Media not formatted";
        case kIOReturnUnsupportedMode : return "No such mode";
        case kIOReturnUnderrun        : return "Data underrun";
        case kIOReturnOverrun         : return "Data overrun";
        case kIOReturnDeviceError     : return "The device is not working properly!";
        case kIOReturnNoCompletion    : return "A completion routine is required";
        case kIOReturnAborted         : return "Operation aborted";
        case kIOReturnNoBandwidth     : return "Bus bandwidth would be exceeded";
        case kIOReturnNotResponding   : return "Device not responding";
        case kIOReturnIsoTooOld       : return "Isochronous I/O request for distant past!";
        case kIOReturnIsoTooNew       : return "Isochronous I/O request for distant future";
        case kIOReturnNotFound        : return "Data was not found";
        case kIOReturnInvalid         : return "Should never be seen";
    }
    return "Unknown";
}

void printDeviceName(IOHIDDeviceRef device)
{
    if (getProductName(device, buffer, 256) > 0)
    {
        printf("name=%ls", buffer);
    }
}

/**
 * Prints information about the device.
 */
void printDeviceInformation(IOHIDDeviceRef device,
    bool path,
    bool reportSize,
    bool reportDescriptor,
    bool elements)
{
    int valueLength = 0;
    if ((valueLength = getProductName(device, buffer, 256)) > 0)
    {
        printf("Device: %ls\n", buffer);
    }
    if ((valueLength = getPhysicalDeviceUniqueID(device, buffer, 256)) > 0)
    {
        printf("ID: %ls\n", buffer);
    }
    if ((valueLength = getSerialNumber(device, buffer, 256)) > 0)
    {
        printf("Serial: %ls\n", buffer);
    }
    if ((valueLength = getManufacturerName(device, buffer, 256)) > 0)
    {
        printf("Manufacturer: %ls\n", buffer);
    }
    printf("ProductID: %d", getProductID(device));
    printf(", VendorID: %d", getVendorID(device));
    printf(", LocationID: %d\n", getLocationID(device));
    if (path)
    {
        if ((valueLength = getPath(device, (char*)buffer, 256)) > 0)
        {
            printf("Path: %s\n", (char*)buffer);
        }
    }
    if (reportSize)
    {
        printf("MaxInputReportSize: %ld\n", getMaxInputReportSize(device));
    }
    if (reportDescriptor)
    {
        if ((valueLength = getReportDescriptor(device, reportBuffer, 256)))
        {
            printReport(reportBuffer, valueLength);
        }
    }
    if (elements)
    {
        printElements(device);
    }
}
