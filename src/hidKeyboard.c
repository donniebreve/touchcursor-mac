#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hidsystem/IOHIDEventSystemClient.h>

#include "keys.h"
#include "hidInformation.h"
#include "hidKeyboard.h"
#include "touchcursor.h"

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
    // Create an event system client
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
 * If the key is a repeatable key (basically non-modifiers).
 */
static int isRepeatable(int code)
{
    if (isModifier(code))
    {
        return 0;
    }
    return 1;
}

/**
 * After the initial delay, loops until superseded, sending repeated key down events.
 */
static void* repeatLoop(void* arg)
{
    int code = *(int*)arg;
    usleep(initialKeyRepeatDelay); // microseconds
    while (repeatThread == pthread_self())
    {
        processKey(0, code, 2);
        usleep(keyRepeatDelay);
    }
    free(arg);
    pthread_exit(NULL);
    return 0;
}

/**
 * Stops the current key repeating thread.
 */
static void stopRepeat(int code)
{
    if (repeatThread > 0 && repeatCode == code)
    {
        repeatThread = 0;
        repeatCode = -1;
    }
}

/**
 * Starts a key repeat thread.
 * Spawns new threads for every key down, the repeat loop will exit if the thread has been superseded.
 */
static void startRepeat(int code)
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
void inputValueCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDValueRef value)
{
    IOHIDElementRef element = IOHIDValueGetElement(value);
    //uint32_t page = IOHIDElementGetUsagePage(element);
    uint32_t code = IOHIDElementGetUsage(element);
    uint32_t down = (int)IOHIDValueGetIntegerValue(value);
    //printf("input value callback: code=%d value=%d\n", code, down);
    // If the HID Element Usage is outside the standard keyboard values, ignore it
    // See IOKit/hid/IOHIDUsageTables.h
    // Not entirely sure if this is correct, Fn is code 3, which is not in the usage tables...
    if (code <= 2 || 232 <= code)
    {
        return;
    }
    //printf("processing: code=%d\n", code);
    processKey(0, code, down);
    // It seems like since we have captured the device, key repeat functionality is lost.
    // Here is my *probably bad* implementation of a key repeat.
    if (isRepeatable(code))
    {
        if (!down)
        {
            stopRepeat(code);
        }
        if (down)
        {
            startRepeat(code);
        }
    }
}

/**
 * Binds the macOS internal keyboard.
 */
int bindKeyboard(uint32_t productID, uint32_t vendorID, IOHIDManagerRef hidManager)
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
            uint32_t product = getProductID(devices[i]);
            uint32_t vendor = getVendorID(devices[i]);
            if (product == productID && vendor == vendorID)
            {
                // Open the device and capture all input
                printf("info: capturing the keyboard... ");
                // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
                // Use kIOHIDOptionsTypeSeizeDevice to capture the device and all inputs
                IOReturn result = IOHIDDeviceOpen(devices[i], kIOHIDOptionsTypeSeizeDevice);
                if (result != kIOReturnSuccess)
                {
                    printf("\nerror: IOHIDDeviceOpen: %s\n", getIOReturnString(result));
                    return 0;
                }
                device = devices[i];
                // Register the input value callback
                IOHIDDeviceRegisterInputValueCallback(
                    device,
                    inputValueCallback,
                    NULL);
                getKeyDelays();
                printf("success\n");
                return 1;
            }
        }
    }
    return 0;
}
