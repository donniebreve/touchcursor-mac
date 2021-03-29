#ifndef cgeventvirtualkeyboard_h
#define cgeventvirtualkeyboard_h

/**
 * Binds the CGEvent virtual keyboard.
 */
int bindCGEventVirtualKeyboard(void);
    
/**
 * Sends a key event using CGEventPost.
 */
void sendCGEventVirtualKeyboardEvent(int type, int code, int down);

#endif
