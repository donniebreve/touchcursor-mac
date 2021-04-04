#include <stdio.h>

#include "emit.h"
#include "cgEventVirtualKeyboard.h"

/**
 * Emits a key event.
 */
void emit(int type, int code, int value)
{
    sendCGEvent(0, code, value);
}
