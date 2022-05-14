#ifndef keys_header
#define keys_header

#include <IOKit/hid/IOHIDUsageTables.h>

// linux codes    https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
// windows codes  http://nehe.gamedev.net/article/msdn_virtualkey_codes/15009/
// mac IOHIDUsage https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-1035.41.2/IOHIDFamily/IOHIDUsageTables.h.auto.html
// mac events     https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX10.6.sdk/System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h

#define kHIDUsage_Csmr  0x0C00

#define KEY_RESERVED    0
#define KEY_ESC         kHIDUsage_KeyboardEscape
#define KEY_1           kHIDUsage_Keyboard1
#define KEY_2           kHIDUsage_Keyboard2
#define KEY_3           kHIDUsage_Keyboard3
#define KEY_4           kHIDUsage_Keyboard4
#define KEY_5           kHIDUsage_Keyboard5
#define KEY_6           kHIDUsage_Keyboard6
#define KEY_7           kHIDUsage_Keyboard7
#define KEY_8           kHIDUsage_Keyboard8
#define KEY_9           kHIDUsage_Keyboard9
#define KEY_0           kHIDUsage_Keyboard0
#define KEY_MINUS       kHIDUsage_KeyboardHyphen
#define KEY_EQUAL       kHIDUsage_KeyboardEqualSign
#define KEY_BACKSPACE   kHIDUsage_KeyboardDeleteOrBackspace
#define KEY_TAB         kHIDUsage_KeyboardTab
#define KEY_Q           kHIDUsage_KeyboardQ
#define KEY_W           kHIDUsage_KeyboardW
#define KEY_E           kHIDUsage_KeyboardE
#define KEY_R           kHIDUsage_KeyboardR
#define KEY_T           kHIDUsage_KeyboardT
#define KEY_Y           kHIDUsage_KeyboardY
#define KEY_U           kHIDUsage_KeyboardU
#define KEY_I           kHIDUsage_KeyboardI
#define KEY_O           kHIDUsage_KeyboardO
#define KEY_P           kHIDUsage_KeyboardP
#define KEY_LEFTBRACE   kHIDUsage_KeyboardOpenBracket
#define KEY_RIGHTBRACE  kHIDUsage_KeyboardCloseBracket
#define KEY_ENTER       kHIDUsage_KeyboardReturnOrEnter
#define KEY_LEFTCTRL    kHIDUsage_KeyboardLeftControl
#define KEY_A           kHIDUsage_KeyboardA
#define KEY_S           kHIDUsage_KeyboardS
#define KEY_D           kHIDUsage_KeyboardD
#define KEY_F           kHIDUsage_KeyboardF
#define KEY_G           kHIDUsage_KeyboardG
#define KEY_H           kHIDUsage_KeyboardH
#define KEY_J           kHIDUsage_KeyboardJ
#define KEY_K           kHIDUsage_KeyboardK
#define KEY_L           kHIDUsage_KeyboardL
#define KEY_SEMICOLON   kHIDUsage_KeyboardSemicolon
#define KEY_APOSTROPHE  kHIDUsage_KeyboardQuote
#define KEY_GRAVE       kHIDUsage_KeyboardGraveAccentAndTilde
#define KEY_LEFTSHIFT   kHIDUsage_KeyboardLeftShift
#define KEY_BACKSLASH   kHIDUsage_KeyboardBackslash
#define KEY_Z           kHIDUsage_KeyboardZ
#define KEY_X           kHIDUsage_KeyboardX
#define KEY_C           kHIDUsage_KeyboardC
#define KEY_V           kHIDUsage_KeyboardV
#define KEY_B           kHIDUsage_KeyboardB
#define KEY_N           kHIDUsage_KeyboardN
#define KEY_M           kHIDUsage_KeyboardM
#define KEY_COMMA       kHIDUsage_KeyboardComma
#define KEY_DOT         kHIDUsage_KeyboardPeriod
#define KEY_SLASH       kHIDUsage_KeyboardSlash
#define KEY_RIGHTSHIFT  kHIDUsage_KeyboardRightShift
#define KEY_KPASTERISK  kHIDUsage_KeypadAsterisk
#define KEY_LEFTALT     kHIDUsage_KeyboardLeftAlt
#define KEY_SPACE       kHIDUsage_KeyboardSpacebar
#define KEY_CAPSLOCK    kHIDUsage_KeyboardCapsLock
#define KEY_F1          kHIDUsage_KeyboardF1
#define KEY_F2          kHIDUsage_KeyboardF2
#define KEY_F3          kHIDUsage_KeyboardF3
#define KEY_F4          kHIDUsage_KeyboardF4
#define KEY_F5          kHIDUsage_KeyboardF5
#define KEY_F6          kHIDUsage_KeyboardF6
#define KEY_F7          kHIDUsage_KeyboardF7
#define KEY_F8          kHIDUsage_KeyboardF8
#define KEY_F9          kHIDUsage_KeyboardF9
#define KEY_F10         kHIDUsage_KeyboardF10
#define KEY_NUMLOCK     kHIDUsage_KeypadNumLock
#define KEY_SCROLLLOCK  kHIDUsage_KeyboardScrollLock
#define KEY_KP7         kHIDUsage_Keypad7
#define KEY_KP8         kHIDUsage_Keypad8
#define KEY_KP9         kHIDUsage_Keypad9
#define KEY_KPMINUS     kHIDUsage_KeypadHyphen
#define KEY_KP4         kHIDUsage_Keypad4
#define KEY_KP5         kHIDUsage_Keypad5
#define KEY_KP6         kHIDUsage_Keypad6
#define KEY_KPPLUS      kHIDUsage_KeypadPlus
#define KEY_KP1         kHIDUsage_Keypad1
#define KEY_KP2         kHIDUsage_Keypad2
#define KEY_KP3         kHIDUsage_Keypad3
#define KEY_KP0         kHIDUsage_Keypad0
#define KEY_KPDOT       kHIDUsage_KeypadPeriod

#define KEY_ZENKAKUHANKAKU 85
#define KEY_102ND     86

#define KEY_F11         kHIDUsage_KeyboardF11
#define KEY_F12         kHIDUsage_KeyboardF12

#define KEY_RO        89
#define KEY_KATAKANA  90
#define KEY_HIRAGANA  91
#define KEY_HENKAN    92
#define KEY_KATAKANAHIRAGANA 93
#define KEY_MUHENKAN  94
#define KEY_KPJPCOMMA 95
#define KEY_KPENTER   96

#define KEY_RIGHTCTRL   kHIDUsage_KeyboardRightControl
#define KEY_KPSLASH     kHIDUsage_KeypadSlash
#define KEY_SYSRQ       kHIDUsage_KeyboardSysReqOrAttention /* print screen, prtscr */
#define KEY_RIGHTALT    kHIDUsage_KeyboardRightAlt
#define KEY_LINEFEED    kHIDUsage_KeypadEnter
#define KEY_HOME        kHIDUsage_KeyboardHome
#define KEY_UP          kHIDUsage_KeyboardUpArrow
#define KEY_PAGEUP      kHIDUsage_KeyboardPageUp
#define KEY_LEFT        kHIDUsage_KeyboardLeftArrow
#define KEY_RIGHT       kHIDUsage_KeyboardRightArrow
#define KEY_END         kHIDUsage_KeyboardEnd
#define KEY_DOWN        kHIDUsage_KeyboardDownArrow
#define KEY_PAGEDOWN    kHIDUsage_KeyboardPageDown
#define KEY_INSERT      kHIDUsage_KeyboardInsert
#define KEY_DELETE      kHIDUsage_KeyboardDeleteForward

#define KEY_MACRO       112
#define KEY_MUTE        113
#define KEY_VOLUMEDOWN  114
#define KEY_VOLUMEUP    115
#define KEY_POWER       116    /* SC System Power Down */
#define KEY_KPEQUAL     117
#define KEY_KPPLUSMINUS 118
#define KEY_PAUSE       119
#define KEY_SCALE       120    /* AL Compiz Scale (Expose) */
#define KEY_KPCOMMA     121
#define KEY_HANGEUL     122
#define KEY_HANGUEL     KEY_HANGEUL
#define KEY_HANJA       123
#define KEY_YEN         124

#define KEY_LEFTMETA    kHIDUsage_KeyboardLeftGUI
#define KEY_RIGHTMETA   kHIDUsage_KeyboardRightGUI

#define KEY_COMPOSE   127

#define KEY_STOP      128    /* AC Stop */
#define KEY_AGAIN     129
#define KEY_PROPS     130    /* AC Properties */
#define KEY_UNDO      131    /* AC Undo */
#define KEY_FRONT     132
#define KEY_COPY      133    /* AC Copy */
#define KEY_OPEN      134    /* AC Open */
#define KEY_PASTE     135    /* AC Paste */
#define KEY_FIND      136    /* AC Search */
#define KEY_CUT       137    /* AC Cut */
#define KEY_HELP      138    /* AL Integrated Help Center */
#define KEY_MENU      139    /* Menu (show menu) */
#define KEY_CALC      140    /* AL Calculator */
#define KEY_SETUP     141
#define KEY_SLEEP     VK_SLEEP    /* SC System Sleep */
#define KEY_WAKEUP    143    /* System Wake Up */
#define KEY_FILE      144    /* AL Local Machine Browser */
#define KEY_SENDFILE  145
#define KEY_DELETEFILE 146
#define KEY_XFER      147
#define KEY_PROG1     148
#define KEY_PROG2     149
#define KEY_WWW       150    /* AL Internet Browser */
#define KEY_MSDOS     151
#define KEY_COFFEE    152    /* AL Terminal Lock/Screensaver */
#define KEY_SCREENLOCK KEY_COFFEE
#define KEY_ROTATE_DISPLAY 153    /* Display orientation for e.g. tablets */
#define KEY_DIRECTION KEY_ROTATE_DISPLAY
#define KEY_CYCLEWINDOWS 154
#define KEY_MAIL      155
#define KEY_BOOKMARKS 156    /* AC Bookmarks */
#define KEY_COMPUTER  157
#define KEY_BACK      158    /* AC Back */
#define KEY_FORWARD   159    /* AC Forward */
#define KEY_CLOSECD   160
#define KEY_EJECTCD   161
#define KEY_EJECTCLOSECD 162
#define KEY_NEXTSONG     163
#define KEY_PLAYPAUSE    164
#define KEY_PREVIOUSSONG 165
#define KEY_STOPCD    166
#define KEY_RECORD    167
#define KEY_REWIND    168
#define KEY_PHONE     169    /* Media Select Telephone */
#define KEY_ISO       170
#define KEY_CONFIG    171    /* AL Consumer Control Configuration */
#define KEY_HOMEPAGE  172    /* AC Home */
#define KEY_REFRESH   173    /* AC Refresh */
#define KEY_EXIT      174    /* AC Exit */
#define KEY_MOVE      175
#define KEY_EDIT      176
#define KEY_SCROLLUP  177
#define KEY_SCROLLDOWN 178
#define KEY_KPLEFTPAREN 179
#define KEY_KPRIGHTPAREN 180
#define KEY_NEW       181    /* AC New */
#define KEY_REDO      182    /* AC Redo/Repeat */

#define KEY_F13         kHIDUsage_KeyboardF13
#define KEY_F14         kHIDUsage_KeyboardF14
#define KEY_F15         kHIDUsage_KeyboardF15
#define KEY_F16         kHIDUsage_KeyboardF16
#define KEY_F17         kHIDUsage_KeyboardF17
#define KEY_F18         kHIDUsage_KeyboardF18
#define KEY_F19         kHIDUsage_KeyboardF19
#define KEY_F20         kHIDUsage_KeyboardF20
#define KEY_F21         kHIDUsage_KeyboardF21
#define KEY_F22         kHIDUsage_KeyboardF22
#define KEY_F23         kHIDUsage_KeyboardF23
#define KEY_F24         kHIDUsage_KeyboardF24

#define KEY_PLAYCD    200
#define KEY_PAUSECD   201
#define KEY_PROG3     202
#define KEY_PROG4     203
#define KEY_DASHBOARD 204    /* AL Dashboard */
#define KEY_SUSPEND   205
#define KEY_CLOSE     206    /* AC Close */
#define KEY_PLAY      207
#define KEY_FASTFORWARD 208
#define KEY_BASSBOOST   209
#define KEY_PRINT     210
#define KEY_HP        211
#define KEY_CAMERA    212
#define KEY_SOUND     213
#define KEY_QUESTION  214
#define KEY_EMAIL     215
#define KEY_CHAT      216
#define KEY_SEARCH    217
#define KEY_CONNECT   218
#define KEY_FINANCE   219    /* AL Checkbook/Finance */
#define KEY_SPORT     220
#define KEY_SHOP      221
#define KEY_ALTERASE  222
#define KEY_CANCEL    223    /* AC Cancel */
#define KEY_BRIGHTNESSDOWN 224
#define KEY_BRIGHTNESSUP   225
#define KEY_MEDIA     226
#define KEY_SWITCHVIDEOMODE 227    /* Cycle between available video outputs (Monitor/LCD/TV-out/etc) */
#define KEY_KBDILLUMTOGGLE  228
#define KEY_KBDILLUMDOWN 229
#define KEY_KBDILLUMUP   230
#define KEY_SEND      231    /* AC Send */
#define KEY_REPLY     232    /* AC Reply */
#define KEY_FORWARDMAIL     233    /* AC Forward Msg */
#define KEY_SAVE      234    /* AC Save */
#define KEY_DOCUMENTS 235
#define KEY_BATTERY   236
#define KEY_BLUETOOTH 237
#define KEY_WLAN      238
#define KEY_UWB       239
#define KEY_UNKNOWN   240
#define KEY_VIDEO_NEXT      241    /* drive next video source */
#define KEY_VIDEO_PREV      242    /* drive previous video source */
#define KEY_BRIGHTNESS_CYCLE 243    /* brightness up, after max is min */
#define KEY_BRIGHTNESS_AUTO 244    /* Set Auto Brightness: manual brightness control is off, rely on ambient */
#define KEY_BRIGHTNESS_ZERO KEY_BRIGHTNESS_AUTO
#define KEY_DISPLAY_OFF     245    /* display device to off state */
#define KEY_WWAN      246    /* Wireless WAN (LTE, UMTS, GSM, etc.) */
#define KEY_WIMAX     KEY_WWAN
#define KEY_RFKILL    247    /* Key that controls all radios */
#define KEY_MICMUTE   248

// MacOS specific
#define KEY_FN  3

/**
 * Checks if the event is a key down.
 */
int isDown(int value);

/**
 * Checks if the key is a modifier key.
 */
int isModifier(int code);

/**
 * Converts a key string "KEY_I" to its corresponding code.
 */
int convertKeyStringToCode(char* keyString);

#endif
