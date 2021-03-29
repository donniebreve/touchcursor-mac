extern "C"
{

#import <Foundation/Foundation.h>
#import <Carbon/Carbon.h>

#import "keys.h"

// The virtual keyboard source
static CGEventSourceRef eventSource;

/**
 * "Binds" the virtual CGEventSource keyboard.
 */
void bindCGEventVirtualKeyboard(void)
{
    eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    //CFRelease(source);
}

/**
 * Sends a key event using CGEventPost.
 */
void sendCGEventVirtualKeyboardEvent(int type, int code, int value)
{
    code = convertKeyToVirtualKeycode(code);
    CGEventRef eventReference = CGEventCreateKeyboardEvent(eventSource, code, value);
    if (value == 2)
    {
        CGEventSetIntegerValueField(eventReference, kCGKeyboardEventAutorepeat, 1);
    }
    CGEventPost(kCGHIDEventTap, eventReference);
    CFRelease(eventReference);
}

}
