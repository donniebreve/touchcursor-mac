#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDEventServiceKeys.h>
#include <IOKit/hid/IOHIDProperties.h>
#include <IOKit/hidsystem/IOHIDServiceClient.h>
#include <IOKit/hidsystem/IOHIDEventSystemClient.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"
#include "touchcursor.h"
#include "emit.h"

static IOHIDDeviceRef device;

static useconds_t initialKeyRepeatDelay = 250000;
static useconds_t keyRepeatDelay = 250000;

static pthread_t repeatThread;
static int repeatCode;

/**
 * Gets the system default key delays.
 */
static void getKeyDelays(void)
{
    // Get the service client
    IOHIDEventSystemClientRef eventSystemClient = IOHIDEventSystemClientCreateSimpleClient(kCFAllocatorDefault);
    // Get the initial key repeat delay
    CFTypeRef property = IOHIDEventSystemClientCopyProperty(eventSystemClient, CFSTR(kIOHIDServiceInitialKeyRepeatDelayKey));
    if (property)
    {
        uint64_t value;
        CFNumberGetValue((CFNumberRef)property, kCFNumberSInt64Type, &value);
        initialKeyRepeatDelay = (useconds_t)(value / 1000);
    }
    // Get the key repeat delay
    property = IOHIDEventSystemClientCopyProperty(eventSystemClient, CFSTR(kIOHIDServiceKeyRepeatDelayKey));
    if (property)
    {
        uint64_t value;
        CFNumberGetValue((CFNumberRef)property, kCFNumberSInt64Type, &value);
        keyRepeatDelay = (useconds_t)(value / 1000);
    }
}

/**
 * Binds the macOS internal keyboard.
 */
int bindMacOSInternalKeyboard(IOHIDManagerRef hidManager)
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
                printDeviceInformation(devices[i], false, false, false, false);
                // Open the device and capture all input
                printf("Capturing the MacOS Internal Keyboard... ");
                // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
                // Use kIOHIDOptionsTypeSeizeDevice to capture the device and all inputs
                IOReturn result = IOHIDDeviceOpen(devices[i], kIOHIDOptionsTypeSeizeDevice);
                if (result != kIOReturnSuccess)
                {
                    printf("IOHIDDeviceOpen: %s\n", getIOReturnString(result));
                    return 0;
                }
                printf("Success\n");
                device = devices[i];
                // Register the input value callback
                IOHIDDeviceRegisterInputValueCallback(
                    device,
                    macOSKeyboardInputValueCallback,
                    NULL);
                getKeyDelays();
                return 1;
            }
        }
    }
    return 0;
}

static int isProcessable(int code)
{
    if (4 <= code && code <= 231)
    {
        return 1;
    }
    return 0;
}

/**
 * Processes a key event after a short delay.
 */
static void* repeatLoop(void* arg)
{
    int code = *(int*)arg;
    useconds_t delay = (useconds_t)initialKeyRepeatDelay;
    usleep(delay); // milliseconds * microseconds
    while (repeatThread == pthread_self() && repeatCode == code)
    {
        processKey(2, code, 1);
        usleep((useconds_t)keyRepeatDelay);
    }
    free(arg);
    pthread_exit(NULL);
    return 0;
}

/**
 * Stops the current key repeating thread.
 */
void stopRepeat(int code)
{
    if (repeatThread > 0 && repeatCode == code)
    {
        repeatThread = 0;
        repeatCode = -1;
    }
}

/**
 * Starts a key repeat thread.
 * Spawns new threads for every key down, the repeat loop will exit if the repeat thread has been superseded.
 */
void startRepeat(int code)
{
    int* arg = malloc(sizeof(int));
    *arg = code;
    repeatCode = code;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, repeatLoop, arg);
    repeatThread = thread_id;
    pthread_detach(thread_id);
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
    
    //printf("macOSKeyboardInputValueCallback: code=%d value=%d\n", code, down);
    
    // If the HID Element Usage is outside the standard keyboard values, ignore it
    if (code <= 3 || 232 <= code)
    {
        return;
    }
    
    processKey(0, code, down);
    
    // It seems like since we have captured the device, key repeat functionality is lost.
    // Here is my *probably bad* implementation of a key repeat.
    if (!down)
    {
        stopRepeat(code);
    }
    if (down)
    {
        startRepeat(code);
    }
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
