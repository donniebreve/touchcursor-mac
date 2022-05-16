/**
 * Touch Cursor for macOS
 * Replicates the touch cursor style movement under macOS.
 *
 * Special thanks to Thomas Bocek for the starting point for this application.
 * Special thanks to Martin Stone for the inspiration and Touch Cursor source.
 */

#include "keys.h"
#include "queue.h"
#include "binding.h"
#include "emit.h"
#include "touchcursor.h"

// The state machine state
enum states state = idle;


/**
 * Checks if the key has been mapped.
 */
static int isMapped(int code)
{
    // return keymap[code] != 0;
    return false;
}

/**
 * Converts input key to touch cursor key.
 */
static int convert(int code)
{
    // return keymap[code];
    return code;
}

/**
 * Processes a key input event. Converts and emits events as necessary.
 */
void processKey(int type, int code, int value)
{
    //printf("processKey: code=%i value=%i state=%i ... ", code, value, state);
    switch (state)
    {
        case idle: // 0
            emit(0, code, value);
            break;

        case delay: // 1
            if (isMapped(code))
            {
                state = map;
                if (isDown(value))
                {
                    if (lengthOfQueue() != 0)
                    {
                        emit(0, convert(peek()), 1);
                    }
                    enqueue(code);
                    emit(0, convert(code), value);
                }
                else
                {
                    int length = lengthOfQueue();
                    for (int i = 0; i < length; i++)
                    {
                        emit(0, convert(dequeue()), 1);
                    }
                    emit(0, convert(code), value);
                }
            }
            else
            {
                state = map;
                emit(0, code, value);
            }
            break;

        case map: // 2
            if (isMapped(code))
            {
                if (isDown(value))
                {
                    enqueue(code);
                    emit(0, convert(code), value);
                }
                else
                {
                    emit(0, convert(code), value);
                }
            }
            else
            {
                emit(0, code, value);
            }
            break;
    }
    //printf("processKey: state=%i\n", state);
}
