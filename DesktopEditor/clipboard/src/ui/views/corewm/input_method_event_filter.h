// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_INPUT_METHOD_EVENT_FILTER_H_
#define UI_VIEWS_COREWM_INPUT_METHOD_EVENT_FILTER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/events/event_handler.h"
#include "ui/base/ime/input_method_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
}

namespace ui {
class InputMethod;
}

namespace views {
namespace corewm {

// An event filter that forwards a KeyEvent to a system IME, and dispatches a
// TranslatedKeyEvent to the root window as needed.
class VIEWS_EXPORT InputMethodEventFilter
    : public ui::EventHandler,
      public ui::internal::InputMethodDelegate {
 public:
  explicit InputMethodEventFilter(gfx::AcceleratedWidget widget);
  virtual ~InputMethodEventFilter();

  void SetInputMethodPropertyInRootWindow(aura::RootWindow* root_window);

  ui::InputMethod* input_method() const { return input_method_.get(); }

 private:
  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;

  // Overridden from ui::internal::InputMethodDelegate.
  virtual bool DispatchKeyEventPostIME(const base::NativeEvent& event) OVERRIDE;
  virtual bool DispatchFabricatedKeyEventPostIME(ui::EventType type,
                                                 ui::KeyboardCode key_code,
                                                 int flags) OVERRIDE;

  scoped_ptr<ui::InputMethod> input_method_;

  // The target root window to which the key event translated by IME will
  // be dispatched.
  aura::RootWindow* target_root_window_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodEventFilter);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_INPUT_METHOD_EVENT_FILTER_H_
