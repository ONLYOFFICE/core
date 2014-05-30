// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_UTIL_H_
#define UI_KEYBOARD_KEYBOARD_UTIL_H_

#include <string>

#include "ui/keyboard/keyboard_export.h"

namespace base {
class ListValue;
}

namespace ui {
class KeyEvent;
}

namespace keyboard {

// Returns true if the virtual keyboard is enabled.
KEYBOARD_EXPORT bool IsKeyboardEnabled();

// Creates a ui::KeyEvent from the argument base::ListValue that is passed to
// the synthetic input APIs (either the Extension API or the WebUI API).  The
// returned ui::KeyEvent is owned by the caller.  If an error occurs, NULL is
// returned and |error| will be populated with a description of the error.  For
// a description of the expected input, please see
// chrome/common/extensions/api/experimental_input_virtual_keyboard.json
KEYBOARD_EXPORT ui::KeyEvent* KeyEventFromArgs(const base::ListValue* args,
                                               std::string* error);

}  // namespace keyboard

#endif  // UI_KEYBOARD_KEYBOARD_UTIL_H_
