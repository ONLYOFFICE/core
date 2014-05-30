// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"

#include "base/logging.h"
#include "content/public/browser/native_web_keyboard_event.h"
#include "ui/base/events/event.h"
#include "ui/views/focus/focus_manager.h"

using content::NativeWebKeyboardEvent;

namespace views {

UnhandledKeyboardEventHandler::UnhandledKeyboardEventHandler() {
}

void UnhandledKeyboardEventHandler::HandleKeyboardEvent(
    const NativeWebKeyboardEvent& event,
    FocusManager* focus_manager) {
  if (!focus_manager) {
    NOTREACHED();
    return;
  }
  if (event.os_event && !event.skip_in_browser)
    focus_manager->OnKeyEvent(*static_cast<ui::KeyEvent*>(event.os_event));
}

}  // namespace views
