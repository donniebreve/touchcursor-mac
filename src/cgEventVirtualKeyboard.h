#ifndef cgeventvirtualkeyboard_h
#define cgeventvirtualkeyboard_h

#import <CoreFoundation/CFMachPort.h>
#import <CoreGraphics/CGEventTypes.h>

// The virtual keyboard source
extern CGEventSourceRef cgEventSource;

// The mouse event tap
extern CFMachPortRef cgEventTap;

/**
 * Creates the mouse CGEventTap.
 */
int createCGEventSource(void);

/**
 * Creates the mouse CGEventTap.
 */
int createCGEventTap(void);
    
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
