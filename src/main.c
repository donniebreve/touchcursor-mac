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
