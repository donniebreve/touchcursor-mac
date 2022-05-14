#include <string.h>
#include <Carbon/Carbon.h>

#include "keys.h"

/**
 * Checks if the event is key down.
 * Linux input sends value=2 for repeated key down.
 * We treat them as keydown events for processing.
 */
int isDown(int value)
{
    return value == 1 || value == 2;
}

/**
 * Checks if the key is a modifier key.
 */
int isModifier(int code)
{
    switch (code)
    {
        case KEY_ESC:
        case KEY_BACKSPACE:
        case KEY_TAB:
        case KEY_ENTER:
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT:
        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL:
        case KEY_LEFTALT:
        case KEY_RIGHTALT:
        case KEY_LEFTMETA:
        case KEY_RIGHTMETA:
        case KEY_CAPSLOCK:
        case KEY_NUMLOCK:
        case KEY_SCROLLLOCK:
        case KEY_FN:
            return 1;
        default:
            return 0;
    }
}

/**
 * Converts a key string (e.g. "KEY_I") to its corresponding code.
 */
int convertKeyStringToCode(char* keyString)
{
    if (strcmp(keyString, "KEY_ESC") == 0) return KEY_ESC;
    else if (strcmp(keyString, "KEY_1") == 0) return KEY_1;
    else if (strcmp(keyString, "KEY_2") == 0) return KEY_2;
    else if (strcmp(keyString, "KEY_3") == 0) return KEY_3;
    else if (strcmp(keyString, "KEY_4") == 0) return KEY_4;
    else if (strcmp(keyString, "KEY_5") == 0) return KEY_5;
    else if (strcmp(keyString, "KEY_6") == 0) return KEY_6;
    else if (strcmp(keyString, "KEY_7") == 0) return KEY_7;
    else if (strcmp(keyString, "KEY_8") == 0) return KEY_8;
    else if (strcmp(keyString, "KEY_9") == 0) return KEY_9;
    else if (strcmp(keyString, "KEY_0") == 0) return KEY_0;
    else if (strcmp(keyString, "KEY_MINUS") == 0) return KEY_MINUS;
    else if (strcmp(keyString, "KEY_EQUAL") == 0) return KEY_EQUAL;
    else if (strcmp(keyString, "KEY_BACKSPACE") == 0) return KEY_BACKSPACE;
    else if (strcmp(keyString, "KEY_TAB") == 0) return KEY_TAB;
    else if (strcmp(keyString, "KEY_Q") == 0) return KEY_Q;
    else if (strcmp(keyString, "KEY_W") == 0) return KEY_W;
    else if (strcmp(keyString, "KEY_E") == 0) return KEY_E;
    else if (strcmp(keyString, "KEY_R") == 0) return KEY_R;
    else if (strcmp(keyString, "KEY_T") == 0) return KEY_T;
    else if (strcmp(keyString, "KEY_Y") == 0) return KEY_Y;
    else if (strcmp(keyString, "KEY_U") == 0) return KEY_U;
    else if (strcmp(keyString, "KEY_I") == 0) return KEY_I;
    else if (strcmp(keyString, "KEY_O") == 0) return KEY_O;
    else if (strcmp(keyString, "KEY_P") == 0) return KEY_P;
    else if (strcmp(keyString, "KEY_LEFTBRACE") == 0) return KEY_LEFTBRACE;
    else if (strcmp(keyString, "KEY_RIGHTBRACE") == 0) return KEY_RIGHTBRACE;
    else if (strcmp(keyString, "KEY_ENTER") == 0) return KEY_ENTER;
    else if (strcmp(keyString, "KEY_LEFTCTRL") == 0) return KEY_LEFTCTRL;
    else if (strcmp(keyString, "KEY_A") == 0) return KEY_A;
    else if (strcmp(keyString, "KEY_S") == 0) return KEY_S;
    else if (strcmp(keyString, "KEY_D") == 0) return KEY_D;
    else if (strcmp(keyString, "KEY_F") == 0) return KEY_F;
    else if (strcmp(keyString, "KEY_G") == 0) return KEY_G;
    else if (strcmp(keyString, "KEY_H") == 0) return KEY_H;
    else if (strcmp(keyString, "KEY_J") == 0) return KEY_J;
    else if (strcmp(keyString, "KEY_K") == 0) return KEY_K;
    else if (strcmp(keyString, "KEY_L") == 0) return KEY_L;
    else if (strcmp(keyString, "KEY_SEMICOLON") == 0) return KEY_SEMICOLON;
    else if (strcmp(keyString, "KEY_APOSTROPHE") == 0) return KEY_APOSTROPHE;
    else if (strcmp(keyString, "KEY_GRAVE") == 0) return KEY_GRAVE;
    else if (strcmp(keyString, "KEY_LEFTSHIFT") == 0) return KEY_LEFTSHIFT;
    else if (strcmp(keyString, "KEY_BACKSLASH") == 0) return KEY_BACKSLASH;
    else if (strcmp(keyString, "KEY_Z") == 0) return KEY_Z;
    else if (strcmp(keyString, "KEY_X") == 0) return KEY_X;
    else if (strcmp(keyString, "KEY_C") == 0) return KEY_C;
    else if (strcmp(keyString, "KEY_V") == 0) return KEY_V;
    else if (strcmp(keyString, "KEY_B") == 0) return KEY_B;
    else if (strcmp(keyString, "KEY_N") == 0) return KEY_N;
    else if (strcmp(keyString, "KEY_M") == 0) return KEY_M;
    else if (strcmp(keyString, "KEY_COMMA") == 0) return KEY_COMMA;
    else if (strcmp(keyString, "KEY_DOT") == 0) return KEY_DOT;
    else if (strcmp(keyString, "KEY_SLASH") == 0) return KEY_SLASH;
    else if (strcmp(keyString, "KEY_RIGHTSHIFT") == 0) return KEY_RIGHTSHIFT;
    else if (strcmp(keyString, "KEY_KPASTERISK") == 0) return KEY_KPASTERISK;
    else if (strcmp(keyString, "KEY_LEFTALT") == 0) return KEY_LEFTALT;
    else if (strcmp(keyString, "KEY_SPACE") == 0) return KEY_SPACE;
    else if (strcmp(keyString, "KEY_CAPSLOCK") == 0) return KEY_CAPSLOCK;
    else if (strcmp(keyString, "KEY_F1") == 0) return KEY_F1;
    else if (strcmp(keyString, "KEY_F2") == 0) return KEY_F2;
    else if (strcmp(keyString, "KEY_F3") == 0) return KEY_F3;
    else if (strcmp(keyString, "KEY_F4") == 0) return KEY_F4;
    else if (strcmp(keyString, "KEY_F5") == 0) return KEY_F5;
    else if (strcmp(keyString, "KEY_F6") == 0) return KEY_F6;
    else if (strcmp(keyString, "KEY_F7") == 0) return KEY_F7;
    else if (strcmp(keyString, "KEY_F8") == 0) return KEY_F8;
    else if (strcmp(keyString, "KEY_F9") == 0) return KEY_F9;
    else if (strcmp(keyString, "KEY_F10") == 0) return KEY_F10;
    else if (strcmp(keyString, "KEY_NUMLOCK") == 0) return KEY_NUMLOCK;
    else if (strcmp(keyString, "KEY_SCROLLLOCK") == 0) return KEY_SCROLLLOCK;
    else if (strcmp(keyString, "KEY_KP7") == 0) return KEY_KP7;
    else if (strcmp(keyString, "KEY_KP8") == 0) return KEY_KP8;
    else if (strcmp(keyString, "KEY_KP9") == 0) return KEY_KP9;
    else if (strcmp(keyString, "KEY_KPMINUS") == 0) return KEY_KPMINUS;
    else if (strcmp(keyString, "KEY_KP4") == 0) return KEY_KP4;
    else if (strcmp(keyString, "KEY_KP5") == 0) return KEY_KP5;
    else if (strcmp(keyString, "KEY_KP6") == 0) return KEY_KP6;
    else if (strcmp(keyString, "KEY_KPPLUS") == 0) return KEY_KPPLUS;
    else if (strcmp(keyString, "KEY_KP1") == 0) return KEY_KP1;
    else if (strcmp(keyString, "KEY_KP2") == 0) return KEY_KP2;
    else if (strcmp(keyString, "KEY_KP3") == 0) return KEY_KP3;
    else if (strcmp(keyString, "KEY_KP0") == 0) return KEY_KP0;
    else if (strcmp(keyString, "KEY_KPDOT") == 0) return KEY_KPDOT;
    else if (strcmp(keyString, "KEY_ZENKAKUHANKAKU") == 0) return KEY_ZENKAKUHANKAKU;
    else if (strcmp(keyString, "KEY_102ND") == 0) return KEY_102ND;
    else if (strcmp(keyString, "KEY_F11") == 0) return KEY_F11;
    else if (strcmp(keyString, "KEY_F12") == 0) return KEY_F12;
    else if (strcmp(keyString, "KEY_RO") == 0) return KEY_RO;
    else if (strcmp(keyString, "KEY_KATAKANA") == 0) return KEY_KATAKANA;
    else if (strcmp(keyString, "KEY_HIRAGANA") == 0) return KEY_HIRAGANA;
    else if (strcmp(keyString, "KEY_HENKAN") == 0) return KEY_HENKAN;
    else if (strcmp(keyString, "KEY_KATAKANAHIRAGANA") == 0) return KEY_KATAKANAHIRAGANA;
    else if (strcmp(keyString, "KEY_MUHENKAN") == 0) return KEY_MUHENKAN;
    else if (strcmp(keyString, "KEY_KPJPCOMMA") == 0) return KEY_KPJPCOMMA;
    else if (strcmp(keyString, "KEY_KPENTER") == 0) return KEY_KPENTER;
    else if (strcmp(keyString, "KEY_RIGHTCTRL") == 0) return KEY_RIGHTCTRL;
    else if (strcmp(keyString, "KEY_KPSLASH") == 0) return KEY_KPSLASH;
    else if (strcmp(keyString, "KEY_SYSRQ") == 0) return KEY_SYSRQ;
    else if (strcmp(keyString, "KEY_RIGHTALT") == 0) return KEY_RIGHTALT;
    else if (strcmp(keyString, "KEY_LINEFEED") == 0) return KEY_LINEFEED;
    else if (strcmp(keyString, "KEY_HOME") == 0) return KEY_HOME;
    else if (strcmp(keyString, "KEY_UP") == 0) return KEY_UP;
    else if (strcmp(keyString, "KEY_PAGEUP") == 0) return KEY_PAGEUP;
    else if (strcmp(keyString, "KEY_LEFT") == 0) return KEY_LEFT;
    else if (strcmp(keyString, "KEY_RIGHT") == 0) return KEY_RIGHT;
    else if (strcmp(keyString, "KEY_END") == 0) return KEY_END;
    else if (strcmp(keyString, "KEY_DOWN") == 0) return KEY_DOWN;
    else if (strcmp(keyString, "KEY_PAGEDOWN") == 0) return KEY_PAGEDOWN;
    else if (strcmp(keyString, "KEY_INSERT") == 0) return KEY_INSERT;
    else if (strcmp(keyString, "KEY_DELETE") == 0) return KEY_DELETE;
    else if (strcmp(keyString, "KEY_MACRO") == 0) return KEY_MACRO;
    else if (strcmp(keyString, "KEY_MUTE") == 0) return KEY_MUTE;
    else if (strcmp(keyString, "KEY_VOLUMEDOWN") == 0) return KEY_VOLUMEDOWN;
    else if (strcmp(keyString, "KEY_VOLUMEUP") == 0) return KEY_VOLUMEUP;
    else if (strcmp(keyString, "KEY_POWER") == 0) return KEY_POWER;
    else if (strcmp(keyString, "KEY_KPEQUAL") == 0) return KEY_KPEQUAL;
    else if (strcmp(keyString, "KEY_KPPLUSMINUS") == 0) return KEY_KPPLUSMINUS;
    else if (strcmp(keyString, "KEY_PAUSE") == 0) return KEY_PAUSE;
    else if (strcmp(keyString, "KEY_SCALE") == 0) return KEY_SCALE;
    else if (strcmp(keyString, "KEY_KPCOMMA") == 0) return KEY_KPCOMMA;
    else if (strcmp(keyString, "KEY_HANGEUL") == 0) return KEY_HANGEUL;
    else if (strcmp(keyString, "KEY_HANGUEL") == 0) return KEY_HANGUEL;
    else if (strcmp(keyString, "KEY_HANJA") == 0) return KEY_HANJA;
    else if (strcmp(keyString, "KEY_YEN") == 0) return KEY_YEN;
    else if (strcmp(keyString, "KEY_LEFTMETA") == 0) return KEY_LEFTMETA;
    else if (strcmp(keyString, "KEY_RIGHTMETA") == 0) return KEY_RIGHTMETA;
    else if (strcmp(keyString, "KEY_COMPOSE") == 0) return KEY_COMPOSE;
    else if (strcmp(keyString, "KEY_STOP") == 0) return KEY_STOP;
    else if (strcmp(keyString, "KEY_AGAIN") == 0) return KEY_AGAIN;
    else if (strcmp(keyString, "KEY_PROPS") == 0) return KEY_PROPS;
    else if (strcmp(keyString, "KEY_UNDO") == 0) return KEY_UNDO;
    else if (strcmp(keyString, "KEY_FRONT") == 0) return KEY_FRONT;
    else if (strcmp(keyString, "KEY_COPY") == 0) return KEY_COPY;
    else if (strcmp(keyString, "KEY_OPEN") == 0) return KEY_OPEN;
    else if (strcmp(keyString, "KEY_PASTE") == 0) return KEY_PASTE;
    else if (strcmp(keyString, "KEY_FIND") == 0) return KEY_FIND;
    else if (strcmp(keyString, "KEY_CUT") == 0) return KEY_CUT;
    else if (strcmp(keyString, "KEY_HELP") == 0) return KEY_HELP;
    else if (strcmp(keyString, "KEY_MENU") == 0) return KEY_MENU;
    else if (strcmp(keyString, "KEY_CALC") == 0) return KEY_CALC;
    else if (strcmp(keyString, "KEY_SETUP") == 0) return KEY_SETUP;
    else if (strcmp(keyString, "KEY_WAKEUP") == 0) return KEY_WAKEUP;
    else if (strcmp(keyString, "KEY_FILE") == 0) return KEY_FILE;
    else if (strcmp(keyString, "KEY_SENDFILE") == 0) return KEY_SENDFILE;
    else if (strcmp(keyString, "KEY_DELETEFILE") == 0) return KEY_DELETEFILE;
    else if (strcmp(keyString, "KEY_XFER") == 0) return KEY_XFER;
    else if (strcmp(keyString, "KEY_PROG1") == 0) return KEY_PROG1;
    else if (strcmp(keyString, "KEY_PROG2") == 0) return KEY_PROG2;
    else if (strcmp(keyString, "KEY_WWW") == 0) return KEY_WWW;
    else if (strcmp(keyString, "KEY_MSDOS") == 0) return KEY_MSDOS;
    else if (strcmp(keyString, "KEY_COFFEE") == 0) return KEY_COFFEE;
    else if (strcmp(keyString, "KEY_SCREENLOCK") == 0) return KEY_SCREENLOCK;
    else if (strcmp(keyString, "KEY_ROTATE_DISPLAY") == 0) return KEY_ROTATE_DISPLAY;
    else if (strcmp(keyString, "KEY_DIRECTION") == 0) return KEY_DIRECTION;
    else if (strcmp(keyString, "KEY_CYCLEWINDOWS") == 0) return KEY_CYCLEWINDOWS;
    else if (strcmp(keyString, "KEY_MAIL") == 0) return KEY_MAIL;
    else if (strcmp(keyString, "KEY_BOOKMARKS") == 0) return KEY_BOOKMARKS;
    else if (strcmp(keyString, "KEY_COMPUTER") == 0) return KEY_COMPUTER;
    else if (strcmp(keyString, "KEY_BACK") == 0) return KEY_BACK;
    else if (strcmp(keyString, "KEY_FORWARD") == 0) return KEY_FORWARD;
    else if (strcmp(keyString, "KEY_CLOSECD") == 0) return KEY_CLOSECD;
    else if (strcmp(keyString, "KEY_EJECTCD") == 0) return KEY_EJECTCD;
    else if (strcmp(keyString, "KEY_EJECTCLOSECD") == 0) return KEY_EJECTCLOSECD;
    else if (strcmp(keyString, "KEY_NEXTSONG") == 0) return KEY_NEXTSONG;
    else if (strcmp(keyString, "KEY_PLAYPAUSE") == 0) return KEY_PLAYPAUSE;
    else if (strcmp(keyString, "KEY_PREVIOUSSONG") == 0) return KEY_PREVIOUSSONG;
    else if (strcmp(keyString, "KEY_STOPCD") == 0) return KEY_STOPCD;
    else if (strcmp(keyString, "KEY_RECORD") == 0) return KEY_RECORD;
    else if (strcmp(keyString, "KEY_REWIND") == 0) return KEY_REWIND;
    else if (strcmp(keyString, "KEY_PHONE") == 0) return KEY_PHONE;
    else if (strcmp(keyString, "KEY_ISO") == 0) return KEY_ISO;
    else if (strcmp(keyString, "KEY_CONFIG") == 0) return KEY_CONFIG;
    else if (strcmp(keyString, "KEY_HOMEPAGE") == 0) return KEY_HOMEPAGE;
    else if (strcmp(keyString, "KEY_REFRESH") == 0) return KEY_REFRESH;
    else if (strcmp(keyString, "KEY_EXIT") == 0) return KEY_EXIT;
    else if (strcmp(keyString, "KEY_MOVE") == 0) return KEY_MOVE;
    else if (strcmp(keyString, "KEY_EDIT") == 0) return KEY_EDIT;
    else if (strcmp(keyString, "KEY_SCROLLUP") == 0) return KEY_SCROLLUP;
    else if (strcmp(keyString, "KEY_SCROLLDOWN") == 0) return KEY_SCROLLDOWN;
    else if (strcmp(keyString, "KEY_KPLEFTPAREN") == 0) return KEY_KPLEFTPAREN;
    else if (strcmp(keyString, "KEY_KPRIGHTPAREN") == 0) return KEY_KPRIGHTPAREN;
    else if (strcmp(keyString, "KEY_NEW") == 0) return KEY_NEW;
    else if (strcmp(keyString, "KEY_REDO") == 0) return KEY_REDO;
    else if (strcmp(keyString, "KEY_F13") == 0) return KEY_F13;
    else if (strcmp(keyString, "KEY_F14") == 0) return KEY_F14;
    else if (strcmp(keyString, "KEY_F15") == 0) return KEY_F15;
    else if (strcmp(keyString, "KEY_F16") == 0) return KEY_F16;
    else if (strcmp(keyString, "KEY_F17") == 0) return KEY_F17;
    else if (strcmp(keyString, "KEY_F18") == 0) return KEY_F18;
    else if (strcmp(keyString, "KEY_F19") == 0) return KEY_F19;
    else if (strcmp(keyString, "KEY_F20") == 0) return KEY_F20;
    else if (strcmp(keyString, "KEY_F21") == 0) return KEY_F21;
    else if (strcmp(keyString, "KEY_F22") == 0) return KEY_F22;
    else if (strcmp(keyString, "KEY_F23") == 0) return KEY_F23;
    else if (strcmp(keyString, "KEY_F24") == 0) return KEY_F24;
    else if (strcmp(keyString, "KEY_PLAYCD") == 0) return KEY_PLAYCD;
    else if (strcmp(keyString, "KEY_PAUSECD") == 0) return KEY_PAUSECD;
    else if (strcmp(keyString, "KEY_PROG3") == 0) return KEY_PROG3;
    else if (strcmp(keyString, "KEY_PROG4") == 0) return KEY_PROG4;
    else if (strcmp(keyString, "KEY_DASHBOARD") == 0) return KEY_DASHBOARD;
    else if (strcmp(keyString, "KEY_SUSPEND") == 0) return KEY_SUSPEND;
    else if (strcmp(keyString, "KEY_CLOSE") == 0) return KEY_CLOSE;
    else if (strcmp(keyString, "KEY_PLAY") == 0) return KEY_PLAY;
    else if (strcmp(keyString, "KEY_FASTFORWARD") == 0) return KEY_FASTFORWARD;
    else if (strcmp(keyString, "KEY_BASSBOOST") == 0) return KEY_BASSBOOST;
    else if (strcmp(keyString, "KEY_PRINT") == 0) return KEY_PRINT;
    else if (strcmp(keyString, "KEY_HP") == 0) return KEY_HP;
    else if (strcmp(keyString, "KEY_CAMERA") == 0) return KEY_CAMERA;
    else if (strcmp(keyString, "KEY_SOUND") == 0) return KEY_SOUND;
    else if (strcmp(keyString, "KEY_QUESTION") == 0) return KEY_QUESTION;
    else if (strcmp(keyString, "KEY_EMAIL") == 0) return KEY_EMAIL;
    else if (strcmp(keyString, "KEY_CHAT") == 0) return KEY_CHAT;
    else if (strcmp(keyString, "KEY_SEARCH") == 0) return KEY_SEARCH;
    else if (strcmp(keyString, "KEY_CONNECT") == 0) return KEY_CONNECT;
    else if (strcmp(keyString, "KEY_FINANCE") == 0) return KEY_FINANCE;
    else if (strcmp(keyString, "KEY_SPORT") == 0) return KEY_SPORT;
    else if (strcmp(keyString, "KEY_SHOP") == 0) return KEY_SHOP;
    else if (strcmp(keyString, "KEY_ALTERASE") == 0) return KEY_ALTERASE;
    else if (strcmp(keyString, "KEY_CANCEL") == 0) return KEY_CANCEL;
    else if (strcmp(keyString, "KEY_BRIGHTNESSDOWN") == 0) return KEY_BRIGHTNESSDOWN;
    else if (strcmp(keyString, "KEY_BRIGHTNESSUP") == 0) return KEY_BRIGHTNESSUP;
    else if (strcmp(keyString, "KEY_MEDIA") == 0) return KEY_MEDIA;
    else if (strcmp(keyString, "KEY_SWITCHVIDEOMODE") == 0) return KEY_SWITCHVIDEOMODE;
    else if (strcmp(keyString, "KEY_KBDILLUMTOGGLE") == 0) return KEY_KBDILLUMTOGGLE;
    else if (strcmp(keyString, "KEY_KBDILLUMDOWN") == 0) return KEY_KBDILLUMDOWN;
    else if (strcmp(keyString, "KEY_KBDILLUMUP") == 0) return KEY_KBDILLUMUP;
    else if (strcmp(keyString, "KEY_SEND") == 0) return KEY_SEND;
    else if (strcmp(keyString, "KEY_REPLY") == 0) return KEY_REPLY;
    else if (strcmp(keyString, "KEY_FORWARDMAIL") == 0) return KEY_FORWARDMAIL;
    else if (strcmp(keyString, "KEY_SAVE") == 0) return KEY_SAVE;
    else if (strcmp(keyString, "KEY_DOCUMENTS") == 0) return KEY_DOCUMENTS;
    else if (strcmp(keyString, "KEY_BATTERY") == 0) return KEY_BATTERY;
    else if (strcmp(keyString, "KEY_BLUETOOTH") == 0) return KEY_BLUETOOTH;
    else if (strcmp(keyString, "KEY_WLAN") == 0) return KEY_WLAN;
    else if (strcmp(keyString, "KEY_UWB") == 0) return KEY_UWB;
    else if (strcmp(keyString, "KEY_UNKNOWN") == 0) return KEY_UNKNOWN;
    else if (strcmp(keyString, "KEY_VIDEO_NEXT") == 0) return KEY_VIDEO_NEXT;
    else if (strcmp(keyString, "KEY_VIDEO_PREV") == 0) return KEY_VIDEO_PREV;
    else if (strcmp(keyString, "KEY_BRIGHTNESS_CYCLE") == 0) return KEY_BRIGHTNESS_CYCLE;
    else if (strcmp(keyString, "KEY_BRIGHTNESS_AUTO") == 0) return KEY_BRIGHTNESS_AUTO;
    else if (strcmp(keyString, "KEY_BRIGHTNESS_ZERO") == 0) return KEY_BRIGHTNESS_ZERO;
    else if (strcmp(keyString, "KEY_DISPLAY_OFF") == 0) return KEY_DISPLAY_OFF;
    else if (strcmp(keyString, "KEY_WWAN") == 0) return KEY_WWAN;
    else if (strcmp(keyString, "KEY_WIMAX") == 0) return KEY_WIMAX;
    else if (strcmp(keyString, "KEY_RFKILL") == 0) return KEY_RFKILL;
    else if (strcmp(keyString, "KEY_MICMUTE") == 0) return KEY_MICMUTE;
    else if (strcmp(keyString, "KEY_FN") == 0) return KEY_FN;
    else return 0;
}
