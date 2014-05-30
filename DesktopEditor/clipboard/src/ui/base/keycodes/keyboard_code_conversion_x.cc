// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/keycodes/keyboard_code_conversion_x.h"

#define XK_3270  // for XK_3270_BackTab
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XF86keysym.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"

namespace ui {

// Get an ui::KeyboardCode from an X keyevent
KeyboardCode KeyboardCodeFromXKeyEvent(XEvent* xev) {
  // XLookupKeysym does not take into consideration the state of the lock/shift
  // etc. keys. So it is necessary to use XLookupString instead.
  KeySym keysym;
  XLookupString(&xev->xkey, NULL, 0, &keysym, NULL);
  KeyboardCode keycode = KeyboardCodeFromXKeysym(keysym);
  if (keycode == VKEY_UNKNOWN) {
    keysym = DefaultXKeysymFromHardwareKeycode(xev->xkey.keycode);
    keycode = KeyboardCodeFromXKeysym(keysym);
  }

  return keycode;
}

KeyboardCode KeyboardCodeFromXKeysym(unsigned int keysym) {
  // TODO(sad): Have |keysym| go through the X map list?

  switch (keysym) {
    case XK_BackSpace:
      return VKEY_BACK;
    case XK_Delete:
    case XK_KP_Delete:
      return VKEY_DELETE;
    case XK_Tab:
    case XK_KP_Tab:
    case XK_ISO_Left_Tab:
    case XK_3270_BackTab:
      return VKEY_TAB;
    case XK_Linefeed:
    case XK_Return:
    case XK_KP_Enter:
    case XK_ISO_Enter:
      return VKEY_RETURN;
    case XK_Clear:
    case XK_KP_Begin:  // NumPad 5 without Num Lock, for crosbug.com/29169.
      return VKEY_CLEAR;
    case XK_KP_Space:
    case XK_space:
      return VKEY_SPACE;
    case XK_Home:
    case XK_KP_Home:
      return VKEY_HOME;
    case XK_End:
    case XK_KP_End:
      return VKEY_END;
    case XK_Page_Up:
    case XK_KP_Page_Up:  // aka XK_KP_Prior
      return VKEY_PRIOR;
    case XK_Page_Down:
    case XK_KP_Page_Down:  // aka XK_KP_Next
      return VKEY_NEXT;
    case XK_Left:
    case XK_KP_Left:
      return VKEY_LEFT;
    case XK_Right:
    case XK_KP_Right:
      return VKEY_RIGHT;
    case XK_Down:
    case XK_KP_Down:
      return VKEY_DOWN;
    case XK_Up:
    case XK_KP_Up:
      return VKEY_UP;
    case XK_Escape:
      return VKEY_ESCAPE;
    case XK_Kana_Lock:
    case XK_Kana_Shift:
      return VKEY_KANA;
    case XK_Hangul:
      return VKEY_HANGUL;
    case XK_Hangul_Hanja:
      return VKEY_HANJA;
    case XK_Kanji:
      return VKEY_KANJI;
    case XK_Henkan:
      return VKEY_CONVERT;
    case XK_Muhenkan:
      return VKEY_NONCONVERT;
    case XK_Zenkaku_Hankaku:
      return VKEY_DBE_DBCSCHAR;
    case XK_A:
    case XK_a:
      return VKEY_A;
    case XK_B:
    case XK_b:
      return VKEY_B;
    case XK_C:
    case XK_c:
      return VKEY_C;
    case XK_D:
    case XK_d:
      return VKEY_D;
    case XK_E:
    case XK_e:
      return VKEY_E;
    case XK_F:
    case XK_f:
      return VKEY_F;
    case XK_G:
    case XK_g:
      return VKEY_G;
    case XK_H:
    case XK_h:
      return VKEY_H;
    case XK_I:
    case XK_i:
      return VKEY_I;
    case XK_J:
    case XK_j:
      return VKEY_J;
    case XK_K:
    case XK_k:
      return VKEY_K;
    case XK_L:
    case XK_l:
      return VKEY_L;
    case XK_M:
    case XK_m:
      return VKEY_M;
    case XK_N:
    case XK_n:
      return VKEY_N;
    case XK_O:
    case XK_o:
      return VKEY_O;
    case XK_P:
    case XK_p:
      return VKEY_P;
    case XK_Q:
    case XK_q:
      return VKEY_Q;
    case XK_R:
    case XK_r:
      return VKEY_R;
    case XK_S:
    case XK_s:
      return VKEY_S;
    case XK_T:
    case XK_t:
      return VKEY_T;
    case XK_U:
    case XK_u:
      return VKEY_U;
    case XK_V:
    case XK_v:
      return VKEY_V;
    case XK_W:
    case XK_w:
      return VKEY_W;
    case XK_X:
    case XK_x:
      return VKEY_X;
    case XK_Y:
    case XK_y:
      return VKEY_Y;
    case XK_Z:
    case XK_z:
      return VKEY_Z;

    case XK_0:
    case XK_1:
    case XK_2:
    case XK_3:
    case XK_4:
    case XK_5:
    case XK_6:
    case XK_7:
    case XK_8:
    case XK_9:
      return static_cast<KeyboardCode>(VKEY_0 + (keysym - XK_0));

    case XK_parenright:
      return VKEY_0;
    case XK_exclam:
      return VKEY_1;
    case XK_at:
      return VKEY_2;
    case XK_numbersign:
      return VKEY_3;
    case XK_dollar:
      return VKEY_4;
    case XK_percent:
      return VKEY_5;
    case XK_asciicircum:
      return VKEY_6;
    case XK_ampersand:
      return VKEY_7;
    case XK_asterisk:
      return VKEY_8;
    case XK_parenleft:
      return VKEY_9;

    case XK_KP_0:
    case XK_KP_1:
    case XK_KP_2:
    case XK_KP_3:
    case XK_KP_4:
    case XK_KP_5:
    case XK_KP_6:
    case XK_KP_7:
    case XK_KP_8:
    case XK_KP_9:
      return static_cast<KeyboardCode>(VKEY_NUMPAD0 + (keysym - XK_KP_0));

    case XK_multiply:
    case XK_KP_Multiply:
      return VKEY_MULTIPLY;
    case XK_KP_Add:
      return VKEY_ADD;
    case XK_KP_Separator:
      return VKEY_SEPARATOR;
    case XK_KP_Subtract:
      return VKEY_SUBTRACT;
    case XK_KP_Decimal:
      return VKEY_DECIMAL;
    case XK_KP_Divide:
      return VKEY_DIVIDE;
    case XK_KP_Equal:
    case XK_equal:
    case XK_plus:
      return VKEY_OEM_PLUS;
    case XK_comma:
    case XK_less:
      return VKEY_OEM_COMMA;
    case XK_minus:
    case XK_underscore:
      return VKEY_OEM_MINUS;
    case XK_greater:
    case XK_period:
      return VKEY_OEM_PERIOD;
    case XK_colon:
    case XK_semicolon:
      return VKEY_OEM_1;
    case XK_question:
    case XK_slash:
      return VKEY_OEM_2;
    case XK_asciitilde:
    case XK_quoteleft:
      return VKEY_OEM_3;
    case XK_bracketleft:
    case XK_braceleft:
      return VKEY_OEM_4;
    case XK_backslash:
    case XK_bar:
      return VKEY_OEM_5;
    case XK_bracketright:
    case XK_braceright:
      return VKEY_OEM_6;
    case XK_quoteright:
    case XK_quotedbl:
      return VKEY_OEM_7;
    case XK_Shift_L:
    case XK_Shift_R:
      return VKEY_SHIFT;
    case XK_Control_L:
    case XK_Control_R:
      return VKEY_CONTROL;
    case XK_Meta_L:
    case XK_Meta_R:
    case XK_Alt_L:
    case XK_Alt_R:
      return VKEY_MENU;
    case XK_Pause:
      return VKEY_PAUSE;
    case XK_Caps_Lock:
      return VKEY_CAPITAL;
    case XK_Num_Lock:
      return VKEY_NUMLOCK;
    case XK_Scroll_Lock:
      return VKEY_SCROLL;
    case XK_Select:
      return VKEY_SELECT;
    case XK_Print:
      return VKEY_PRINT;
    case XK_Execute:
      return VKEY_EXECUTE;
    case XK_Insert:
    case XK_KP_Insert:
      return VKEY_INSERT;
    case XK_Help:
      return VKEY_HELP;
    case XK_Super_L:
      return VKEY_LWIN;
    case XK_Super_R:
      return VKEY_RWIN;
    case XK_Menu:
      return VKEY_APPS;
    case XK_F1:
    case XK_F2:
    case XK_F3:
    case XK_F4:
    case XK_F5:
    case XK_F6:
    case XK_F7:
    case XK_F8:
    case XK_F9:
    case XK_F10:
    case XK_F11:
    case XK_F12:
    case XK_F13:
    case XK_F14:
    case XK_F15:
    case XK_F16:
    case XK_F17:
    case XK_F18:
    case XK_F19:
    case XK_F20:
    case XK_F21:
    case XK_F22:
    case XK_F23:
    case XK_F24:
      return static_cast<KeyboardCode>(VKEY_F1 + (keysym - XK_F1));
    case XK_KP_F1:
    case XK_KP_F2:
    case XK_KP_F3:
    case XK_KP_F4:
      return static_cast<KeyboardCode>(VKEY_F1 + (keysym - XK_KP_F1));

    case XK_guillemotleft:
    case XK_guillemotright:
    case XK_degree:
      return VKEY_OEM_102;  // international backslash key in 102 keyboard.

    // When evdev is in use, /usr/share/X11/xkb/symbols/inet maps F13-18 keys
    // to the special XF86XK symbols to support Microsoft Ergonomic keyboards:
    // https://bugs.freedesktop.org/show_bug.cgi?id=5783
    // In Chrome, we map these X key symbols back to F13-18 since we don't have
    // VKEYs for these XF86XK symbols.
    case XF86XK_Tools:
      return VKEY_F13;
    case XF86XK_Launch5:
      return VKEY_F14;
    case XF86XK_Launch6:
      return VKEY_F15;
    case XF86XK_Launch7:
      return VKEY_F16;
    case XF86XK_Launch8:
      return VKEY_F17;
    case XF86XK_Launch9:
      return VKEY_F18;

#if defined(TOOLKIT_GTK)
    case XF86XK_Refresh:
    case XF86XK_History:
    case XF86XK_OpenURL:
    case XF86XK_AddFavorite:
    case XF86XK_Go:
    case XF86XK_ZoomIn:
    case XF86XK_ZoomOut:
      // ui::AcceleratorGtk tries to convert the XF86XK_ keysyms on Chrome
      // startup. It's safe to return VKEY_UNKNOWN here since ui::AcceleratorGtk
      // also checks a Gdk keysym. http://crbug.com/109843
      return VKEY_UNKNOWN;
#endif

    // For supporting multimedia buttons on a USB keyboard.
    case XF86XK_Back:
      return VKEY_BROWSER_BACK;
    case XF86XK_Forward:
      return VKEY_BROWSER_FORWARD;
    case XF86XK_Reload:
      return VKEY_BROWSER_REFRESH;
    case XF86XK_Stop:
      return VKEY_BROWSER_STOP;
    case XF86XK_Search:
      return VKEY_BROWSER_SEARCH;
    case XF86XK_Favorites:
      return VKEY_BROWSER_FAVORITES;
    case XF86XK_HomePage:
      return VKEY_BROWSER_HOME;
    case XF86XK_AudioMute:
      return VKEY_VOLUME_MUTE;
    case XF86XK_AudioLowerVolume:
      return VKEY_VOLUME_DOWN;
    case XF86XK_AudioRaiseVolume:
      return VKEY_VOLUME_UP;
    case XF86XK_AudioNext:
      return VKEY_MEDIA_NEXT_TRACK;
    case XF86XK_AudioPrev:
      return VKEY_MEDIA_PREV_TRACK;
    case XF86XK_AudioStop:
      return VKEY_MEDIA_STOP;
    case XF86XK_AudioPlay:
      return VKEY_MEDIA_PLAY_PAUSE;
    case XF86XK_Mail:
      return VKEY_MEDIA_LAUNCH_MAIL;
    case XF86XK_LaunchA:  // F3 on an Apple keyboard.
      return VKEY_MEDIA_LAUNCH_APP1;
    case XF86XK_LaunchB:  // F4 on an Apple keyboard.
    case XF86XK_Calculator:
      return VKEY_MEDIA_LAUNCH_APP2;
    case XF86XK_WLAN:
      return VKEY_WLAN;
    case XF86XK_PowerOff:
      return VKEY_POWER;
    case XF86XK_MonBrightnessDown:
      return VKEY_BRIGHTNESS_DOWN;
    case XF86XK_MonBrightnessUp:
      return VKEY_BRIGHTNESS_UP;
    case XF86XK_KbdBrightnessDown:
      return VKEY_KBD_BRIGHTNESS_DOWN;
    case XF86XK_KbdBrightnessUp:
      return VKEY_KBD_BRIGHTNESS_UP;

    // TODO(sad): some keycodes are still missing.
  }
  DLOG(WARNING) << "Unknown keysym: " << base::StringPrintf("0x%x", keysym);
  return VKEY_UNKNOWN;
}

uint16 GetCharacterFromXEvent(XEvent* xev) {
  char buf[6];
  int bytes_written = XLookupString(&xev->xkey, buf, 6, NULL, NULL);
  DCHECK_LE(bytes_written, 6);

  string16 result;
  return (bytes_written > 0 && UTF8ToUTF16(buf, bytes_written, &result) &&
          result.length() == 1) ? result[0] : 0;
}

unsigned int DefaultXKeysymFromHardwareKeycode(unsigned int hardware_code) {
  static const unsigned int kHardwareKeycodeMap[] = {
    0,                // 0x00:
    0,                // 0x01:
    0,                // 0x02:
    0,                // 0x03:
    0,                // 0x04:
    0,                // 0x05:
    0,                // 0x06:
    0,                // 0x07:
    0,                // 0x08:
    XK_Escape,        // 0x09: XK_Escape
    XK_1,             // 0x0A: XK_1
    XK_2,             // 0x0B: XK_2
    XK_3,             // 0x0C: XK_3
    XK_4,             // 0x0D: XK_4
    XK_5,             // 0x0E: XK_5
    XK_6,             // 0x0F: XK_6
    XK_7,             // 0x10: XK_7
    XK_8,             // 0x11: XK_8
    XK_9,             // 0x12: XK_9
    XK_0,             // 0x13: XK_0
    XK_minus,         // 0x14: XK_minus
    XK_equal,         // 0x15: XK_equal
    XK_BackSpace,     // 0x16: XK_BackSpace
    XK_Tab,           // 0x17: XK_Tab
    XK_q,             // 0x18: XK_q
    XK_w,             // 0x19: XK_w
    XK_e,             // 0x1A: XK_e
    XK_r,             // 0x1B: XK_r
    XK_t,             // 0x1C: XK_t
    XK_y,             // 0x1D: XK_y
    XK_u,             // 0x1E: XK_u
    XK_i,             // 0x1F: XK_i
    XK_o,             // 0x20: XK_o
    XK_p,             // 0x21: XK_p
    XK_bracketleft,   // 0x22: XK_bracketleft
    XK_bracketright,  // 0x23: XK_bracketright
    XK_Return,        // 0x24: XK_Return
    XK_Control_L,     // 0x25: XK_Control_L
    XK_a,             // 0x26: XK_a
    XK_s,             // 0x27: XK_s
    XK_d,             // 0x28: XK_d
    XK_f,             // 0x29: XK_f
    XK_g,             // 0x2A: XK_g
    XK_h,             // 0x2B: XK_h
    XK_j,             // 0x2C: XK_j
    XK_k,             // 0x2D: XK_k
    XK_l,             // 0x2E: XK_l
    XK_semicolon,     // 0x2F: XK_semicolon
    XK_apostrophe,    // 0x30: XK_apostrophe
    XK_grave,         // 0x31: XK_grave
    XK_Shift_L,       // 0x32: XK_Shift_L
    XK_backslash,     // 0x33: XK_backslash
    XK_z,             // 0x34: XK_z
    XK_x,             // 0x35: XK_x
    XK_c,             // 0x36: XK_c
    XK_v,             // 0x37: XK_v
    XK_b,             // 0x38: XK_b
    XK_n,             // 0x39: XK_n
    XK_m,             // 0x3A: XK_m
    XK_comma,         // 0x3B: XK_comma
    XK_period,        // 0x3C: XK_period
    XK_slash,         // 0x3D: XK_slash
    XK_Shift_R,       // 0x3E: XK_Shift_R
    0,                // 0x3F: XK_KP_Multiply
    XK_Alt_L,         // 0x40: XK_Alt_L
    XK_space,         // 0x41: XK_space
    XK_Caps_Lock,     // 0x42: XK_Caps_Lock
    XK_F1,            // 0x43: XK_F1
    XK_F2,            // 0x44: XK_F2
    XK_F3,            // 0x45: XK_F3
    XK_F4,            // 0x46: XK_F4
    XK_F5,            // 0x47: XK_F5
    XK_F6,            // 0x48: XK_F6
    XK_F7,            // 0x49: XK_F7
    XK_F8,            // 0x4A: XK_F8
    XK_F9,            // 0x4B: XK_F9
    XK_F10,           // 0x4C: XK_F10
    XK_Num_Lock,      // 0x4D: XK_Num_Lock
    XK_Scroll_Lock,   // 0x4E: XK_Scroll_Lock
  };

  return hardware_code < arraysize(kHardwareKeycodeMap) ?
      kHardwareKeycodeMap[hardware_code] : 0;
}

// TODO(jcampan): this method might be incomplete.
int XKeysymForWindowsKeyCode(KeyboardCode keycode, bool shift) {
  switch (keycode) {
    case VKEY_NUMPAD0:
      return XK_KP_0;
    case VKEY_NUMPAD1:
      return XK_KP_1;
    case VKEY_NUMPAD2:
      return XK_KP_2;
    case VKEY_NUMPAD3:
      return XK_KP_3;
    case VKEY_NUMPAD4:
      return XK_KP_4;
    case VKEY_NUMPAD5:
      return XK_KP_5;
    case VKEY_NUMPAD6:
      return XK_KP_6;
    case VKEY_NUMPAD7:
      return XK_KP_7;
    case VKEY_NUMPAD8:
      return XK_KP_8;
    case VKEY_NUMPAD9:
      return XK_KP_9;
    case VKEY_MULTIPLY:
      return XK_KP_Multiply;
    case VKEY_ADD:
      return XK_KP_Add;
    case VKEY_SUBTRACT:
      return XK_KP_Subtract;
    case VKEY_DECIMAL:
      return XK_KP_Decimal;
    case VKEY_DIVIDE:
      return XK_KP_Divide;

    case VKEY_BACK:
      return XK_BackSpace;
    case VKEY_TAB:
      return shift ? XK_ISO_Left_Tab : XK_Tab;
    case VKEY_CLEAR:
      return XK_Clear;
    case VKEY_RETURN:
      return XK_Return;
    case VKEY_SHIFT:
      return XK_Shift_L;
    case VKEY_CONTROL:
      return XK_Control_L;
    case VKEY_MENU:
      return XK_Alt_L;
    case VKEY_APPS:
      return XK_Menu;

    case VKEY_PAUSE:
      return XK_Pause;
    case VKEY_CAPITAL:
      return XK_Caps_Lock;
    case VKEY_KANA:
      return XK_Kana_Lock;
    case VKEY_HANJA:
      return XK_Hangul_Hanja;
    case VKEY_CONVERT:
      return XK_Henkan;
    case VKEY_NONCONVERT:
      return XK_Muhenkan;
    case VKEY_DBE_SBCSCHAR:
      return XK_Zenkaku_Hankaku;
    case VKEY_DBE_DBCSCHAR:
      return XK_Zenkaku_Hankaku;
    case VKEY_ESCAPE:
      return XK_Escape;
    case VKEY_SPACE:
      return XK_space;
    case VKEY_PRIOR:
      return XK_Page_Up;
    case VKEY_NEXT:
      return XK_Page_Down;
    case VKEY_END:
      return XK_End;
    case VKEY_HOME:
      return XK_Home;
    case VKEY_LEFT:
      return XK_Left;
    case VKEY_UP:
      return XK_Up;
    case VKEY_RIGHT:
      return XK_Right;
    case VKEY_DOWN:
      return XK_Down;
    case VKEY_SELECT:
      return XK_Select;
    case VKEY_PRINT:
      return XK_Print;
    case VKEY_EXECUTE:
      return XK_Execute;
    case VKEY_INSERT:
      return XK_Insert;
    case VKEY_DELETE:
      return XK_Delete;
    case VKEY_HELP:
      return XK_Help;
    case VKEY_0:
      return shift ? XK_parenright : XK_0;
    case VKEY_1:
      return shift ? XK_exclam : XK_1;
    case VKEY_2:
      return shift ? XK_at : XK_2;
    case VKEY_3:
      return shift ? XK_numbersign : XK_3;
    case VKEY_4:
      return shift ? XK_dollar : XK_4;
    case VKEY_5:
      return shift ? XK_percent : XK_5;
    case VKEY_6:
      return shift ? XK_asciicircum : XK_6;
    case VKEY_7:
      return shift ? XK_ampersand : XK_7;
    case VKEY_8:
      return shift ? XK_asterisk : XK_8;
    case VKEY_9:
      return shift ? XK_parenleft : XK_9;

    case VKEY_A:
    case VKEY_B:
    case VKEY_C:
    case VKEY_D:
    case VKEY_E:
    case VKEY_F:
    case VKEY_G:
    case VKEY_H:
    case VKEY_I:
    case VKEY_J:
    case VKEY_K:
    case VKEY_L:
    case VKEY_M:
    case VKEY_N:
    case VKEY_O:
    case VKEY_P:
    case VKEY_Q:
    case VKEY_R:
    case VKEY_S:
    case VKEY_T:
    case VKEY_U:
    case VKEY_V:
    case VKEY_W:
    case VKEY_X:
    case VKEY_Y:
    case VKEY_Z:
      return (shift ? XK_A : XK_a) + (keycode - VKEY_A);

    case VKEY_LWIN:
      return XK_Super_L;
    case VKEY_RWIN:
      return XK_Super_R;

    case VKEY_NUMLOCK:
      return XK_Num_Lock;

    case VKEY_SCROLL:
      return XK_Scroll_Lock;

    case VKEY_OEM_1:
      return shift ? XK_colon : XK_semicolon;
    case VKEY_OEM_PLUS:
      return shift ? XK_plus : XK_equal;
    case VKEY_OEM_COMMA:
      return shift ? XK_less : XK_comma;
    case VKEY_OEM_MINUS:
      return shift ? XK_underscore : XK_minus;
    case VKEY_OEM_PERIOD:
      return shift ? XK_greater : XK_period;
    case VKEY_OEM_2:
      return shift ? XK_question : XK_slash;
    case VKEY_OEM_3:
      return shift ? XK_asciitilde : XK_quoteleft;
    case VKEY_OEM_4:
      return shift ? XK_braceleft : XK_bracketleft;
    case VKEY_OEM_5:
      return shift ? XK_bar : XK_backslash;
    case VKEY_OEM_6:
      return shift ? XK_braceright : XK_bracketright;
    case VKEY_OEM_7:
      return shift ? XK_quotedbl : XK_quoteright;
    case VKEY_OEM_102:
      return shift ? XK_guillemotleft : XK_guillemotright;

    case VKEY_F1:
    case VKEY_F2:
    case VKEY_F3:
    case VKEY_F4:
    case VKEY_F5:
    case VKEY_F6:
    case VKEY_F7:
    case VKEY_F8:
    case VKEY_F9:
    case VKEY_F10:
    case VKEY_F11:
    case VKEY_F12:
    case VKEY_F13:
    case VKEY_F14:
    case VKEY_F15:
    case VKEY_F16:
    case VKEY_F17:
    case VKEY_F18:
    case VKEY_F19:
    case VKEY_F20:
    case VKEY_F21:
    case VKEY_F22:
    case VKEY_F23:
    case VKEY_F24:
      return XK_F1 + (keycode - VKEY_F1);

    case VKEY_BROWSER_BACK:
      return XF86XK_Back;
    case VKEY_BROWSER_FORWARD:
      return XF86XK_Forward;
    case VKEY_BROWSER_REFRESH:
      return XF86XK_Reload;
    case VKEY_BROWSER_STOP:
      return XF86XK_Stop;
    case VKEY_BROWSER_SEARCH:
      return XF86XK_Search;
    case VKEY_BROWSER_FAVORITES:
      return XF86XK_Favorites;
    case VKEY_BROWSER_HOME:
      return XF86XK_HomePage;
    case VKEY_VOLUME_MUTE:
      return XF86XK_AudioMute;
    case VKEY_VOLUME_DOWN:
      return XF86XK_AudioLowerVolume;
    case VKEY_VOLUME_UP:
      return XF86XK_AudioRaiseVolume;
    case VKEY_MEDIA_NEXT_TRACK:
      return XF86XK_AudioNext;
    case VKEY_MEDIA_PREV_TRACK:
      return XF86XK_AudioPrev;
    case VKEY_MEDIA_STOP:
      return XF86XK_AudioStop;
    case VKEY_MEDIA_PLAY_PAUSE:
      return XF86XK_AudioPlay;
    case VKEY_MEDIA_LAUNCH_MAIL:
      return XF86XK_Mail;
    case VKEY_MEDIA_LAUNCH_APP1:
      return XF86XK_LaunchA;
    case VKEY_MEDIA_LAUNCH_APP2:
      return XF86XK_LaunchB;
    case VKEY_WLAN:
      return XF86XK_WLAN;
    case VKEY_POWER:
      return XF86XK_PowerOff;
    case VKEY_BRIGHTNESS_DOWN:
      return XF86XK_MonBrightnessDown;
    case VKEY_BRIGHTNESS_UP:
      return XF86XK_MonBrightnessUp;
    case VKEY_KBD_BRIGHTNESS_DOWN:
      return XF86XK_KbdBrightnessDown;
    case VKEY_KBD_BRIGHTNESS_UP:
      return XF86XK_KbdBrightnessUp;

    default:
      LOG(WARNING) << "Unknown keycode:" << keycode;
      return 0;
    }
}

}  // namespace ui
