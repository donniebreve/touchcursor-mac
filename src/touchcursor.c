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
static enum states
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
void processKey(int code, int value)
{
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
                    emit(code, value);
                }
            }
            else
            {
                emit(code, value);
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
                        emit(hyperKey, 1);
                    }
                    emit(hyperKey, 0);
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
                    emit(code, value);
                }
            }
            else
            {
                if (isDown(value))
                {
                    if (!hyperEmitted)
                    {
                        emit(hyperKey, 1);
                        hyperEmitted = 1;
                    }
                    emit(code, value);
                }
                else
                {
                    emit(code, value);
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
                        emit(hyperKey, 1);
                    }
                    int length = lengthOfQueue();
                    for (int i = 0; i < length; i++)
                    {
                        emit(dequeue(), 1);
                    }
                    emit(hyperKey, 0);
                }
            }
            else if (isMapped(code))
            {
                state = map;
                if (isDown(value))
                {
                    if (lengthOfQueue() != 0)
                    {
                        emit(convert(peek()), 1);
                    }
                    enqueue(code);
                    emit(convert(code), value);
                }
                else
                {
                    int length = lengthOfQueue();
                    for (int i = 0; i < length; i++)
                    {
                        emit(convert(dequeue()), 1);
                    }
                    emit(convert(code), value);
                }
            }
            else
            {
                state = map;
                emit(code, value);
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
                        emit(convert(dequeue()), 0);
                    }
                }
            }
            else if (isMapped(code))
            {
                if (isDown(value))
                {
                    enqueue(code);
                    emit(convert(code), value);
                }
                else
                {
                    emit(convert(code), value);
                }
            }
            else
            {
                emit(code, value);
            }
            break;
    }
}

/**
 * Drops all the currently held keys.
 */
void dropKeys()
{
    int length = lengthOfQueue();
    for (int i = 0; i < length; i++)
    {
        int code = dequeue();
        emit(code, 0);
        emit(convert(code), 0);
    }
}
