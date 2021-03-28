#include <stdio.h>

#include "emit.h"
#include "cgEventVirtualKeyboard.h"

/**
 * Emits a key event.
 */
void emit(int type, int code, int value)
{
    //printf("emit code: %d value: %d\n", code, value);
    sendCGEventVirtualKeyboardEvent(0, code, value);
}
