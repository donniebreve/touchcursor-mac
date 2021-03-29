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
    readConfiguration();
    createHIDManager();
    if (!bindInput())
    {
        printf("error: Failed to capture the input device. Try allowing input monitoring for this executable in 'Security & Privacy'.\n");
        exit(1);
    }
    if (!bindOutput())
    {
        printf("error: Failed to connect to the output device. Maybe Karabiner Elements is not installed?\n");
        exit(1);
    }
    printf("info: running\n");
    CFRunLoopRun();
    exit(0);
}
