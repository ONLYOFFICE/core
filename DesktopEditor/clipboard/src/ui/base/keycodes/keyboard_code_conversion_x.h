// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_X_H_
#define UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_X_H_

#include "base/basictypes.h"
#include "ui/base/keycodes/keyboard_codes_posix.h"
#include "ui/base/ui_export.h"

typedef union _XEvent XEvent;

namespace ui {

UI_EXPORT KeyboardCode KeyboardCodeFromXKeyEvent(XEvent* xev);

UI_EXPORT KeyboardCode KeyboardCodeFromXKeysym(unsigned int keysym);

// Returns a character on a standard US PC keyboard from an XEvent.
UI_EXPORT uint16 GetCharacterFromXEvent(XEvent* xev);

// Converts a KeyboardCode into an X KeySym.
UI_EXPORT int XKeysymForWindowsKeyCode(KeyboardCode keycode, bool shift);

// Converts an X keycode into an X KeySym.
unsigned int DefaultXKeysymFromHardwareKeycode(unsigned int keycode);

}  // namespace ui

#endif  // UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_X_H_
