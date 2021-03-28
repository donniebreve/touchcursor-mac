#ifndef emit_h
#define emit_h

/**
 * Emits a key event.
 */
void emit(int type, int code, int value);

/**
 * Emits a key event (delayed).
 */
void emitDelayed(int type, int code, int value);

#endif
