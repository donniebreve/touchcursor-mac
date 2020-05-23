#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOReturn.h>
#include <IOKit/hid/IOHIDBase.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>

#include "hidInformation.h"
#include "macOSInternalKeyboard.h"
#include "karabinerVirtualKeyboard.h"
#include "touchcursor.h"
#include "binding.h"

// The HID manager object
static IOHIDManagerRef hidManager;

// The input and output devices
static IOHIDDeviceRef inputDevice;
static IOHIDDeviceRef outputDevice;

/**
 * Emits a key event.
 */
void emit(int code, int value)
{
    KarabinerVirtualKeyboardReport report;
    uint32_t post_keyboard_input_report_method = 3;
    report.id = 1;
    report.modifiers = 0;
    report.reserved = 0;
    if (value) report.keys[0] = code;
    else report.keys[0] = 0;
    for (int i = 1; i < 32; i++) report.keys[i] = 0;
    IOReturn virtualKeyboardResult = IOConnectCallStructMethod(outputConnect, post_keyboard_input_report_method, &report, sizeof(report), NULL, 0);
    if (virtualKeyboardResult != kIOReturnSuccess)
    {
        printf("%s\n", getIOReturnString(virtualKeyboardResult));
    }
}

/*
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
    if (3 < code && code < 232)
    {
        /*
        printf("code: %d down: %ld\n", usage, down);
        
        // Invalid argument
        IOReturn sendResult = IOHIDDeviceSetValue(outputDevice, element, value);
        if (sendResult != kIOReturnSuccess)
        {
            printf("%s\n", getIOReturnString(sendResult));
        }
        
        // General error
        karabinerKeyboardReport report;
        report.id = 1;
        report.keys[0] = 7;
        IOReturn sendResult = IOHIDDeviceSetReport(outputDevice, kIOHIDReportTypeInput, 1, (uint8_t*)(&report), sizeof(report));
        if (sendResult != kIOReturnSuccess)
        {
            printf("%s\n", getIOReturnString(sendResult));
        }
        */
        
        // Process the input
        processKey(code, down);
    }
    else
    {
        // Emit the key
        emit(code, down);
    }
}

/*
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
    // IOHIDManagerOpen opens all the matching devices, but we will iterate and open the desired device
    // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
    // Use kIOHIDOptionsTypeSeizeDevice to capture the device(s)
    //IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
}

/*
 * Binds the input device.
 */
int bindInput()
{
    inputDevice = bindMacOSInternalKeyboard(hidManager);
    if (!inputDevice)
    {
        return 0;
    }
    // Register the input value callback (key down/up)
    IOHIDDeviceRegisterInputValueCallback(
        inputDevice,
        macOSKeyboardInputValueCallback,
        NULL);
    // Register the input report callback (total key state, not working)
    //IOHIDDeviceRegisterInputReportCallback(
    //    inputDevice,
    //    macOSKeyboardReportBuffer,
    //    macOSKeyboardMaxInputReportSize,
    //    macOSKeyboardInputReportCallback,
    //    NULL);
    return 1;
}

/*
 * Binds the output device.
 */
int bindOutput()
{
    outputDevice = bindKarabinerVirtualKeyboard();
    if (!outputDevice)
    {
        return 0;
    }
    return 1;
}
