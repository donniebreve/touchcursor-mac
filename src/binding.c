#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOReturn.h>
#include <IOKit/hid/IOHIDBase.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>

#include "hidInformation.h"
#include "keys.h"
#include "macOSInternalKeyboard.h"
#include "karabinerVirtualKeyboard.h"
#include "touchcursor.h"
#include "binding.h"

// The HID manager object
static IOHIDManagerRef hidManager;

// The input and output devices
static IOHIDDeviceRef inputDevice;
static IOHIDDeviceRef outputDevice;

KarabinerVirtualKeyboardReport kReport;

void emit_report(KarabinerVirtualKeyboardReport* report)
{
    printKarabinerVirtualKeyboardInputReport(report);
    uint32_t post_keyboard_input_report_method = 3;
    IOReturn virtualKeyboardResult = IOConnectCallStructMethod(outputConnect, post_keyboard_input_report_method, report, sizeof(karabinerVirtualKeyboardReport), NULL, 0);
    if (virtualKeyboardResult != kIOReturnSuccess)
    {
        printf("%s\n", getIOReturnString(virtualKeyboardResult));
    }
}

/**
 * Emits a key event.
 */
void emit(int type, int code, int value)
{
    //printf("emit code: %d value: %d\n", code, value);
    if (isModifier(code))
    {
        if (value) modifierDown(code);
        else modifierUp(code);
    }
    if (value) keyDown(code);
    else keyUp(code);
    emit_report(&karabinerVirtualKeyboardReport);
}

/**
 * The input value callback method.
 */
void keyboardInputValueCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDValueRef value)
{
    IOHIDElementRef element = IOHIDValueGetElement(value);
    uint32_t code = IOHIDElementGetUsage(element);
    uint32_t down = (int)IOHIDValueGetIntegerValue(value);
    
    printf("received code: %d value: %d\n", code, down);
    
    if (code == 3)
    {
        emit(0, code, down);
    }
    if (3 < code && code < 232)
    {
        processKey(0, code, down);
    }
}

/*
 * The input report callback method.
 */
static void keyboardInputReportCallback(
    void* context,
    IOReturn result,
    void* sender,
    IOHIDReportType reportType,
    uint32_t reportID,
    uint8_t* report,
    CFIndex reportLength)
{
    printf("Received report: %i\n", reportID);
    printf("Report type: %i\n", reportType);
    printf("Report length: %ld\n", reportLength);
    // if (reportID == 1)
    // {
    //     MacOSInternalKeyboardReport* macReport = (MacOSInternalKeyboardReport*)report;
    //     printMacOSInternalKeyboardInputReport(macReport);
        
    //     kReport.id = 1;
    //     kReport.reserved = 0;
    //     kReport.modifiers = macReport->modifiers;
    //     for (int i = 0; i < 6; i++)
    //     {
    //         kReport.keys[i] = macReport->keys[i];
    //     }
        
    //     emit_report(&kReport);
    // }
}

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
    // IOHIDManagerOpen opens all the matching devices, but we will iterate and open the desired device
    // Use kIOHIDOptionsTypeNone to capture events without interrupting the device
    // Use kIOHIDOptionsTypeSeizeDevice to capture the device(s)
    //IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
}

/**
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
    //IOHIDDeviceRegisterInputValueCallback(
    //    inputDevice,
    //    keyboardInputValueCallback,
    //    NULL);
    // Register the input report callback (total key state, not working, might be easier in the long run)
    IOHIDDeviceRegisterInputReportCallback(
        inputDevice,
        macOSKeyboardReportBuffer,
        65,
        keyboardInputReportCallback,
        NULL);
    return 1;
}

/**
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
