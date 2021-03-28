#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDDeviceTypes.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"
#include "touchcursor.h"
#include "emit.h"

static IOHIDDeviceRef device;

static pthread_t repeatThread;
static int repeat = 0;
static int repeatCode = 0;

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
                IOHIDDeviceRef inputDevice = devices[i];
                printDeviceInformation(inputDevice, false, false, false, false);
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
                device = inputDevice;
                // Register the input value callback
                IOHIDDeviceRegisterInputValueCallback(
                    inputDevice,
                    macOSKeyboardInputValueCallback,
                    NULL);
                printf("Success\n");
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Processes a key event after a short delay.
 */
static void* repeatLoop(void* arg)
{
    int code = *(int*)arg;
    usleep(500 * 1000); // milliseconds * microseconds
    while (repeat)
    {
        processKey(1, code, 1);
        usleep(100 * 1000);
    }
    free(arg);
    return 0;
}

void stopRepeatThread(void)
{
    if (repeatThread != 0)
    {
        repeat = 0;
        pthread_join(repeatThread, (void*)-1);
        printf("stopped %i\n", repeatThread);
        repeatThread = 0;
    }
}

void stopRepeat(int code)
{
    if (repeatCode == code)
    {
        repeatCode = -1;
        stopRepeatThread();
    }
}

/**
 * Starts a key repeat thread.
 * Every key down needs to kill the previous thread and start a new one.
 */
void startRepeat(int code)
{
    stopRepeatThread();
    repeat = 1;
    
    int* arg = malloc(sizeof(int));
    *arg = code;
    
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, repeatLoop, NULL);
    repeatThread = thread_id;
    printf("thread: %i\n", repeatThread);
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
    if (code == 3) // I've forgotten the relevance of code 3
    {
        emit(0, code, down);
    }
    if (4 <= code && code <= 231) // Most normal key codes lie in this range
    {
        processKey(0, code, down);
        // It seems like since we have captured the device, key repeat functionality is lost.
        // Here is my *probably bad* implementation of a key repeat.
        // Every key down needs to kill the previous thread and start a new one.
        // Every key up needs to kill the thread, if the code matches the current repeatCode.
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
