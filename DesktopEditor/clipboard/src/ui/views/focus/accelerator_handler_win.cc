// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/focus/accelerator_handler.h"

#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_code_conversion_win.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/widget/widget.h"

namespace views {

AcceleratorHandler::AcceleratorHandler() {
}

bool AcceleratorHandler::Dispatch(const base::NativeEvent& msg) {
  if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) {
    Widget* widget = Widget::GetTopLevelWidgetForNativeView(msg.hwnd);
    FocusManager* focus_manager = widget ? widget->GetFocusManager() : NULL;
    if (focus_manager) {
      switch (msg.message) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
          ui::KeyEvent event(msg, false);
          if (!focus_manager->OnKeyEvent(event)) {
            // Record that this key is pressed so we can remember not to
            // translate and dispatch the associated WM_KEYUP.
            pressed_keys_.insert(msg.wParam);
            return true;
          }
          break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
          std::set<WPARAM>::iterator iter = pressed_keys_.find(msg.wParam);
          if (iter != pressed_keys_.end()) {
            // Don't translate/dispatch the KEYUP since we have eaten the
            // associated KEYDOWN.
            pressed_keys_.erase(iter);
            return true;
          }
          break;
        }
      }
    }
  }

  TranslateMessage(&msg);
  DispatchMessage(&msg);
  return true;
}

}  // namespace views
