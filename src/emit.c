#include "emit.h"
#include "keys.h"
#include "karabinerVirtualKeyboard.h"

/**
 * Emits a key event.
 */
void emit(int type, int code, int value)
{
    printf("emit code: %d value: %d\n", code, value);
    if (isModifier(code))
    {
        if (value)
        {
            setKarabinerVirtualKeyboardModifierDown(code);
        }
        else 
        {
            setKarabinerVirtualKeyboardModifierUp(code);
        }
    }
    else 
    {
        if (value) 
        {
            setKarabinerVirtualKeyboardKeyDown(code);
        }
        else
        {
            setKarabinerVirtualKeyboardKeyUp(code);
        }
    }
    sendKarabinerVirtualKeyboardReport();
}
