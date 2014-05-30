// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_WIN_H_
#define UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_WIN_H_

#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/ui_export.h"

namespace ui {

// Methods to convert ui::KeyboardCode/Windows virtual key type methods.
UI_EXPORT WORD WindowsKeyCodeForKeyboardCode(KeyboardCode keycode);
UI_EXPORT KeyboardCode KeyboardCodeForWindowsKeyCode(WORD keycode);

}  // namespace ui

#endif  // UI_BASE_KEYCODES_KEYBOARD_CODE_CONVERSION_WIN_H_
