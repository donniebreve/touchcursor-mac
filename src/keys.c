#include <string.h>
#include <Carbon/Carbon.h>

#include "keys.h"

/**
 * Checks if the event is key down.
 * Linux input sends value=2 for repeated key down.
 * We treat them as keydown events for processing.
 */
int isDown(int value)
{
    return value == 1 || value == 2;
}

/**
 * Checks if the key is a modifier key.
 */
int isModifier(int code)
{
    switch (code)
    {
        case KEY_ESC:
        case KEY_BACKSPACE:
        case KEY_TAB:
        case KEY_ENTER:
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT:
        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL:
        case KEY_LEFTALT:
        case KEY_RIGHTALT:
        case KEY_LEFTMETA:
        case KEY_RIGHTMETA:
        case KEY_CAPSLOCK:
        case KEY_NUMLOCK:
        case KEY_SCROLLLOCK:
        case KEY_FN:
            return 1;
        default:
            return 0;
    }
}
