#include <stdio.h>
#include <stdlib.h>
#include <CoreFoundation/CFRunLoop.h>

#include "keys.h"
#include "config.h"
#include "binding.h"

/**
 * Main method.
 */
int main(int argc, char* argv[])
{
    /*
    readConfiguration();
    if (eventPath[0] == '\0')
    {
        fprintf(stderr, "error: please specify the keyboard device name in the configuration file\n");
        return EXIT_FAILURE;
    }
    */

    // testing
    hyperKey = KEY_SPACE;
    keymap[KEY_I] = KEY_UP;
    keymap[KEY_J] = KEY_LEFT;
    keymap[KEY_K] = KEY_DOWN;
    keymap[KEY_L] = KEY_RIGHT;
    keymap[KEY_U] = KEY_HOME;
    keymap[KEY_O] = KEY_END;
    keymap[KEY_P] = KEY_BACKSPACE;
    keymap[KEY_H] = KEY_PAGEUP;
    keymap[KEY_N] = KEY_PAGEDOWN;
    keymap[KEY_M] = KEY_DELETE;
    keymap[KEY_Y] = KEY_INSERT;
    keymap[KEY_E] = KEY_ESC;
    
    createHIDManager();
    if (!bindInput())
    {
        printf("error: could not capture the keyboard device\n");
        exit(1);
    }
    if (!bindOutput())
    {
        printf("error: could not create the virtual keyboard device\n");
        exit(1);
    }
    printf("info: running\n");
    CFRunLoopRun();
    exit(0);
}
