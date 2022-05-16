extern "C"
{

#import <Foundation/Foundation.h>
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSEvent.h>

// NX media key definitions
#import <IOKit/hidsystem/ev_keymap.h>

// Other special key definitions
#define kVK_Launchpad 130
#define kVK_Expose 160

#import "keys.h"
#import "cgEventVirtualKeyboard.h"

static int (*currentFilterCallback)(int, int);

// The virtual keyboard source
CGEventSourceRef cgEventSource;

// The mouse event tap
CFMachPortRef cgEventTap;

// The modifier flags
static int fn = 0;
static int modifiers = 0;

/**
 * Creates the virtual CGEventSource keyboard.
 */
int createCGEventSource(void)
{
    printf("info: creating the CGEventSource... ");
    cgEventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    //CFRelease(cgEventSource);
    printf("success\n");
    return 1;
}

/**
 * Call back for mouse events. Adds modifier flags to the events.
 */
static CGEventRef mouseCGEventCallback(
    CGEventTapProxy proxy,
    CGEventType type,
    CGEventRef event,
    void *refcon)
{
    if (modifiers > 0)
    {
        int mouseModifiers = modifiers &= ~NX_NONCOALSESCEDMASK;
        CGEventSetFlags(event, mouseModifiers);
    }
    return event;
}

/**
 * Creates the mouse CGEventTap.
 */
int createCGEventTap(void)
{
    printf("info: creating the CGEventTap... ");
    cgEventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        kCGEventLeftMouseDown
            | kCGEventLeftMouseUp
            | kCGEventRightMouseDown
            | kCGEventRightMouseUp,
        mouseCGEventCallback,
        NULL);
    if ((long)cgEventTap <= 0)
    {
        printf("failed\n");
        return 0;
    }
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault,
        cgEventTap,
        0);
    CFRunLoopAddSource(
        CFRunLoopGetCurrent(),
        runLoopSource,
        kCFRunLoopCommonModes);
    CGEventTapEnable(
        cgEventTap,
        1);
    printf("success\n");
    return 1;
}

/**
 * Creates a media CGEvent using the NSEvent structure.
 *
 * This method was causing an error "Detected over-release of a CFTypeRef".
 * I believe this is because the NSEvent is reaped when leaving this scope, which in turn
 * reaps the CGEvent returned. Later on we would attempt to free the CGEvent again.
 *
 * This method has been changed to return a copy of the CGEvent so as not to be
 * dependent on the NSEvent.
 */
static CGEventRef CGEventCreateMediaEvent(int code, int down)
{
    NSEvent* event = [NSEvent
        otherEventWithType:NSEventTypeSystemDefined
        location: NSMakePoint(0, 0)
        modifierFlags:0xa00
        timestamp:0
        windowNumber:0
        context:0
        subtype:8
        data1:(code << 16) | (down ? (0xa << 8) : (0xb << 8))
        data2:-1];
	return CGEventCreateCopy([event CGEvent]);
}

/**
 * Converts a key KEY_ to its corresponding virtual keycode kVK_. See Events.h.
 */
static int convertCodeToQuartzKeycode(int code)
{
    switch (code)
    {
        case KEY_ESC: return kVK_Escape;
        case KEY_1: return kVK_ANSI_1;
        case KEY_2: return kVK_ANSI_2;
        case KEY_3: return kVK_ANSI_3;
        case KEY_4: return kVK_ANSI_4;
        case KEY_5: return kVK_ANSI_5;
        case KEY_6: return kVK_ANSI_6;
        case KEY_7: return kVK_ANSI_7;
        case KEY_8: return kVK_ANSI_8;
        case KEY_9: return kVK_ANSI_9;
        case KEY_0: return kVK_ANSI_0;
        case KEY_MINUS: return kVK_ANSI_Minus;
        case KEY_EQUAL: return kVK_ANSI_Equal;
        case KEY_BACKSPACE: return kVK_Delete;
        case KEY_TAB: return kVK_Tab;
        case KEY_Q: return kVK_ANSI_Q;
        case KEY_W: return kVK_ANSI_W;
        case KEY_E: return kVK_ANSI_E;
        case KEY_R: return kVK_ANSI_R;
        case KEY_T: return kVK_ANSI_T;
        case KEY_Y: return kVK_ANSI_Y;
        case KEY_U: return kVK_ANSI_U;
        case KEY_I: return kVK_ANSI_I;
        case KEY_O: return kVK_ANSI_O;
        case KEY_P: return kVK_ANSI_P;
        case KEY_LEFTBRACE: return kVK_ANSI_LeftBracket;
        case KEY_RIGHTBRACE: return kVK_ANSI_RightBracket;
        case KEY_ENTER: return kVK_Return;
        case KEY_LEFTCTRL: return kVK_Control;
        case KEY_A: return kVK_ANSI_A;
        case KEY_S: return kVK_ANSI_S;
        case KEY_D: return kVK_ANSI_D;
        case KEY_F: return kVK_ANSI_F;
        case KEY_G: return kVK_ANSI_G;
        case KEY_H: return kVK_ANSI_H;
        case KEY_J: return kVK_ANSI_J;
        case KEY_K: return kVK_ANSI_K;
        case KEY_L: return kVK_ANSI_L;
        case KEY_SEMICOLON: return kVK_ANSI_Semicolon;
        case KEY_APOSTROPHE: return kVK_ANSI_Quote;
        case KEY_GRAVE: return kVK_ANSI_Grave;
        case KEY_LEFTSHIFT: return kVK_Shift;
        case KEY_BACKSLASH: return kVK_ANSI_Backslash;
        case KEY_Z: return kVK_ANSI_Z;
        case KEY_X: return kVK_ANSI_X;
        case KEY_C: return kVK_ANSI_C;
        case KEY_V: return kVK_ANSI_V;
        case KEY_B: return kVK_ANSI_B;
        case KEY_N: return kVK_ANSI_N;
        case KEY_M: return kVK_ANSI_M;
        case KEY_COMMA: return kVK_ANSI_Comma;
        case KEY_DOT: return kVK_ANSI_Period;
        case KEY_SLASH: return kVK_ANSI_Slash;
        case KEY_RIGHTSHIFT: return kVK_RightShift;
        case KEY_KPASTERISK: return kVK_ANSI_KeypadMultiply;
        case KEY_LEFTALT: return kVK_Option;
        case KEY_SPACE: return kVK_Space;
        case KEY_CAPSLOCK: return kVK_CapsLock;
        case KEY_F1: return kVK_F1;
        case KEY_F2: return kVK_F2;
        case KEY_F3: return kVK_F3;
        case KEY_F4: return kVK_F4;
        case KEY_F5: return kVK_F5;
        case KEY_F6: return kVK_F6;
        case KEY_F7: return kVK_F7;
        case KEY_F8: return kVK_F8;
        case KEY_F9: return kVK_F9;
        case KEY_F10: return kVK_F10;
        case KEY_KP7: return kVK_ANSI_Keypad7;
        case KEY_KP8: return kVK_ANSI_Keypad8;
        case KEY_KP9: return kVK_ANSI_Keypad9;
        case KEY_KPMINUS: return kVK_ANSI_KeypadMinus;
        case KEY_KP4: return kVK_ANSI_Keypad4;
        case KEY_KP5: return kVK_ANSI_Keypad5;
        case KEY_KP6: return kVK_ANSI_Keypad6;
        case KEY_KPPLUS: return kVK_ANSI_KeypadPlus;
        case KEY_KP1: return kVK_ANSI_Keypad1;
        case KEY_KP2: return kVK_ANSI_Keypad2;
        case KEY_KP3: return kVK_ANSI_Keypad3;
        case KEY_KP0: return kVK_ANSI_Keypad0;
        case KEY_KPDOT: return kVK_ANSI_KeypadDecimal;
        case KEY_F11: return kVK_F11;
        case KEY_F12: return kVK_F12;
        case KEY_RIGHTCTRL: return kVK_RightControl;
        case KEY_KPSLASH: return kVK_ANSI_Slash;
        case KEY_RIGHTALT: return kVK_RightOption;
        case KEY_HOME: return kVK_Home;
        case KEY_UP: return kVK_UpArrow;
        case KEY_PAGEUP: return kVK_PageUp;
        case KEY_LEFT: return kVK_LeftArrow;
        case KEY_RIGHT: return kVK_RightArrow;
        case KEY_END: return kVK_End;
        case KEY_DOWN: return kVK_DownArrow;
        case KEY_PAGEDOWN: return kVK_PageDown;
        case KEY_DELETE: return kVK_ForwardDelete;
        case KEY_LEFTMETA: return kVK_Command;
        case KEY_RIGHTMETA: return kVK_RightCommand;
        case KEY_F13: return kVK_F13;
        case KEY_F14: return kVK_F14;
        case KEY_F15: return kVK_F15;
        case KEY_F16: return kVK_F16;
        case KEY_F17: return kVK_F17;
        case KEY_F18: return kVK_F18;
        case KEY_F19: return kVK_F19;
        case KEY_F20: return kVK_F20;
        case KEY_FN: return kVK_Function;
        /*
        Not defined in Events.h
        case KEY_NUMLOCK: return -1;
        case KEY_SCROLLLOCK: return -1;
        case KEY_SYSRQ: return -1;
        case KEY_INSERT: return -1;
        case KEY_PAUSE: return -1;
        */
    }
    return -1;
}

/**
 * Adds a modifier to the state.
 */
void setModifierDown(int code)
{
    switch (code)
    {
        case KEY_LEFTCTRL:
        {
            modifiers |= kCGEventFlagMaskControl;
            break;
        }
        case KEY_RIGHTCTRL:
        {
            modifiers |= kCGEventFlagMaskControl;
            break;
        }
        case KEY_LEFTSHIFT:
        {
            modifiers |= NX_DEVICERSHIFTKEYMASK;
            modifiers |= NX_SHIFTMASK;
            break;
        }
        case KEY_RIGHTSHIFT:
        {
            modifiers |= NX_DEVICELSHIFTKEYMASK;
            modifiers |= NX_SHIFTMASK;
            break;
        }
        case KEY_LEFTALT:
        {
            modifiers |= NX_ALTERNATEMASK;
            modifiers |= NX_DEVICELALTKEYMASK;
            break;
        }
        case KEY_RIGHTALT:
        {
            modifiers |= NX_ALTERNATEMASK;
            modifiers |= NX_DEVICERALTKEYMASK;
            break;
        }
        case KEY_LEFTMETA:
        {
            modifiers |= NX_COMMANDMASK;
            modifiers |= NX_DEVICELCMDKEYMASK;
            break;
        }
        case KEY_RIGHTMETA:
        {
            modifiers |= NX_COMMANDMASK;
            modifiers |= NX_DEVICERCMDKEYMASK;
            break;
        }
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
        {
            modifiers |= NX_NUMERICPADMASK;
            break;
        }
        case KEY_FN:
            {
                fn = 1;
                break;
            }
    }
}

/**
 * Removes a modifier from the state.
 * This is very naive and isn't going to work if two modifier keys are pressed, and only one released.
 */
void setModifierUp(int code)
{
    switch (code)
    {
        case KEY_LEFTCTRL:
        {
            modifiers &= ~kCGEventFlagMaskControl;
            break;
        }
        case KEY_RIGHTCTRL:
        {
            modifiers &= ~kCGEventFlagMaskControl;
            break;
        }
        case KEY_LEFTSHIFT: {
            modifiers &= ~NX_DEVICELSHIFTKEYMASK;
            modifiers &= ~NX_SHIFTMASK;
        }
        case KEY_RIGHTSHIFT:
        {
            modifiers &= ~NX_DEVICERSHIFTKEYMASK;
            modifiers &= ~NX_SHIFTMASK;
            break;
        }
        case KEY_LEFTALT:
        {
            modifiers &= ~NX_ALTERNATEMASK;
            modifiers &= ~NX_DEVICELALTKEYMASK;
            break;
        }
        case KEY_RIGHTALT:
        {
            modifiers &= ~NX_ALTERNATEMASK;
            modifiers &= ~NX_DEVICERALTKEYMASK;
            break;
        }
        case KEY_LEFTMETA:
        {
            modifiers &= ~NX_COMMANDMASK;
            modifiers &= ~NX_DEVICELCMDKEYMASK;
            break;
        }
        case KEY_RIGHTMETA:
        {
            modifiers &= ~NX_COMMANDMASK;
            modifiers &= ~NX_DEVICERCMDKEYMASK;
            break;
        }
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
        {
            modifiers &= ~NX_NUMERICPADMASK;
            break;
        }
        case KEY_FN:
            {
                fn = 0;
                break;
            }
    }
}

void setFilterCallback(int filterCallback(int, int)) {
    currentFilterCallback = filterCallback;
}

/**
 * Sends a key event using CGEventPost.
 */
void sendCGEvent(int type, int code, int value)
{
    // check modifiers ok
    if (!cgEventSource)
    {
        // CGEventPost will send the event even if the source is null :\
        return;
    }

    //printf("sendCGEvent: type=%i code=%i value=%i\n", type, code, value);
    // Update the modifier state
    if (isModifier(code) || code == KEY_UP || code == KEY_DOWN || code == KEY_LEFT || code == KEY_RIGHT)
    {
        if (value)
        {
            setModifierDown(code);
        }
        else
        {
            setModifierUp(code);
        }
    }

    // Check if client wants to filter first.
    if (currentFilterCallback(code, modifiers) != 1) {
        return;
    }

    CGEventRef event = 0;
    switch (code)
    {
        case KEY_F1:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_BRIGHTNESS_DOWN, isDown(value));
                break;
            }
        case KEY_F2:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_BRIGHTNESS_UP, isDown(value));
                break;
            }
        case KEY_F3:
            {
                event = CGEventCreateKeyboardEvent(cgEventSource, kVK_Expose, isDown(value));
                break;
            }
        case KEY_F4:
            {
                event = CGEventCreateKeyboardEvent(cgEventSource, kVK_Launchpad, isDown(value));
                break;
            }
        case KEY_F5:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_ILLUMINATION_DOWN, isDown(value));
                break;
            }
        case KEY_F6:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_ILLUMINATION_UP, isDown(value));
                break;
            }
        case KEY_F7:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_PREVIOUS, isDown(value));
                break;
            }
        case KEY_F8:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_PLAY, isDown(value));
                break;
            }
        case KEY_F9:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_NEXT, isDown(value));
                break;
            }
        case KEY_F10:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_MUTE, isDown(value));
                break;
            }
        case KEY_F11:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_SOUND_DOWN, isDown(value));
                break;
            }
        case KEY_F12:
            {
                event = CGEventCreateMediaEvent(NX_KEYTYPE_SOUND_UP, isDown(value));
                break;
            }
        default:
            break;
    }
    // Otherwise send normal keys
    if ((long)event <= 0)
    {
        int quartzCode = convertCodeToQuartzKeycode(code);
        if (quartzCode < 0)
        {
            return;
        }
        event = CGEventCreateKeyboardEvent(cgEventSource, quartzCode, isDown(value));
    }
    if (value == 2)
    {
        CGEventSetIntegerValueField(event, kCGKeyboardEventAutorepeat, 1);
    }

    // to indicate mouse is not involved, we will reset the bit later if mouse event occurs
    modifiers |= NX_NONCOALSESCEDMASK;
    CGEventSetFlags(event, modifiers);
    // printf("sendCGEvent: type=%i code=%i value=%i modifiers=%i\n", type, code, value, modifiers);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}

}
