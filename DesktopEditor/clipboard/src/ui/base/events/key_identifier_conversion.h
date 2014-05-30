// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_EVENTS_KEY_IDENTIFIER_CONVERSION_H_
#define UI_BASE_EVENTS_KEY_IDENTIFIER_CONVERSION_H_

#include <string>

#include "ui/base/ui_export.h"

namespace ui {

class KeyEvent;

// Convert a KeyIdentifer (see Section 6.3.3 here:
// http://www.w3.org/TR/DOM-Level-3-Events/#keyset-keyidentifiers)
// to a ui::KeyEvent.
// This should only be called from the UI thread.
UI_EXPORT const ui::KeyEvent& KeyEventFromKeyIdentifier(
    const std::string& key_identifier);

}  // namespace ui

#endif  // UI_BASE_EVENTS_KEY_IDENTIFIER_CONVERSION_H_
