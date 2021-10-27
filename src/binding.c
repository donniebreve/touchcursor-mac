#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>

#include "hidInformation.h"
#include "hidKeyboard.h"
#include "cgEventVirtualKeyboard.h"
#include "binding.h"

/**
 * Binds the input device.
 */
int bindInput()
{
    int result = bindKeyboard(630, 1452, hidManager);
    if (!result)
    {
        return 0;
    }
    return 1;
}

/**
 * Binds the output device.
 */
int bindOutput()
{
    int result = createCGEventSource();
    if (!result)
    {
        return 0;
    }
    result = createCGEventTap();
    if (!result)
    {
        return 0;
    }
    
    return 1;
}

/**
 * Starts the CFRunLoop.
 */
void startRunLoop()
{
    CFRunLoopRun();
}
