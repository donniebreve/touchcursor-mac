/**
 * Touch Cursor for macOS
 * Replicates the touch cursor style movement under macOS.
 *
 * Special thanks to Thomas Bocek for the starting point for this application.
 * Special thanks to Martin Stone for the inspiration and Touch Cursor source.
 */

#include "keys.h"
#include "queue.h"
#include "config.h"
#include "binding.h"
#include "touchcursor.h"

// The state machine states
enum states
{
    idle,
    hyper,
    delay,
    map
} state;

// Flag if the hyper key has been emitted
static int hyperEmitted;

/**
 * Checks if the key is the hyper key.
 */
static int isHyper(int code)
{
    return code == hyperKey;
}

/**
 * Checks if the event is key down.
 * Linux input sends value=2 for repeated key down.
 * We treat them as keydown events for processing.
 */
static int isDown(int value)
{
    return value == 1 || value == 2;
}

/**
 * Checks if the key has been mapped.
 */
static int isMapped(int code)
{
    return keymap[code] != 0;
}

/**
 * Converts input key to touch cursor key
 * To do: make this configurable
 */
static int convert(int code)
{
    return keymap[code];
}

/**
 * Processes a key input event. Converts and emits events as necessary.
 */
void processKey(int type, int code, int value)
{
    // printf("processKey: code=%i value=%i state=%i\n", code, value, state);
    switch (state)
    {
        case idle: // 0
            if (isHyper(code))
            {
                if (isDown(value))
                {
                    state = hyper;
                    hyperEmitted = 0;
                    clearQueue();
                }
                else
                {
                    emit(0, code, value);
                }
            }
            else
            {
                emit(0, code, value);
            }
            break;

        case hyper: // 1
            if (isHyper(code))
            {
                if (!isDown(value))
                {
                    state = idle;
                    if (!hyperEmitted)
                    {
                        emit(0, hyperKey, 1);
                    }
                    emit(0, hyperKey, 0);
                }
            }
            else if (isMapped(code))
            {
                if (isDown(value))
                {
                    state = delay;
                    enqueue(code);
                }
                else
                {
                    emit(0, code, value);
                }
            }
            else
            {
                if (!isModifier(code) && isDown(value))
                {
                    if (!hyperEmitted)
                    {
                        emit(0, hyperKey, 1);
                        hyperEmitted = 1;
                    }
                    emit(0, code, value);
                }
                else
                {
                    emit(0, code, value);
                }
            }
            break;

        case delay: // 2
            if (isHyper(code))
            {
                if (!isDown(value))
                {
                    state = idle;
                    if (!hyperEmitted)
                    {
                        emit(0, hyperKey, 1);
                    }
                    int length = lengthOfQueue();
                    for (int i = 0; i < length; i++)
                    {
                        emit(0, dequeue(), 1);
                    }
                    emit(0, hyperKey, 0);
                }
            }
            else if (isMapped(code))
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

        case map: // 3
            if (isHyper(code))
            {
                if (!isDown(value))
                {
                    state = idle;
                    int length = lengthOfQueue();
                    for (int i = 0; i < length; i++)
                    {
                        emit(0, convert(dequeue()), 0);
                    }
                }
            }
            else if (isMapped(code))
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
}
