#ifndef cgeventvirtualkeyboard_h
#define cgeventvirtualkeyboard_h

/**
 * Binds the CGEvent virtual keyboard.
 */
int createCGEventSource(void);
    
/**
 * Adds a modifier to the state.
 */
void setModifierDown(int code);

/**
 * Removes a modifier from the state.
 */
void setModifierUp(int code);
    
/**
 * Sends a key event using CGEventPost.
 */
void sendCGEvent(int type, int code, int down);

#endif
