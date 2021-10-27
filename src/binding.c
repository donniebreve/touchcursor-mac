#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>

#include "hidInformation.h"
#include "hidKeyboard.h"
#include "cgEventVirtualKeyboard.h"
#include "binding.h"

// The Run Loop
CFRunLoopRef runLoop;

/**
 * Binds the input device.
 */
int bindInput(int productID, int vendorID)
{
    int result = bindKeyboard(productID, vendorID, hidManager);
    if (!result)
    {
        return 0;
    }
    return 1;
}

/**
 * Releases the input device.
 */
void releaseInput()
{
    releaseKeyboard();
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
 * Releases the output device.
 */
void releaseOutput()
{
    // TODO
}

/**
 * Starts the CFRunLoop.
 */
void startRunLoop()
{
    runLoop = CFRunLoopGetCurrent();
    CFRunLoopRun();
}

/**
 * Starts the CFRunLoop.
 */
void stopRunLoop()
{
    if ((long)runLoop > 1)
    {
        CFRunLoopStop(runLoop);
    }
}
