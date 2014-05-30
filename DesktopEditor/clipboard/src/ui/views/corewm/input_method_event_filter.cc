// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/input_method_event_filter.h"

#include "ui/aura/client/aura_constants.h"
#include "ui/aura/root_window.h"
#include "ui/base/events/event.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/ime/input_method_factory.h"

namespace views {
namespace corewm {

////////////////////////////////////////////////////////////////////////////////
// InputMethodEventFilter, public:

InputMethodEventFilter::InputMethodEventFilter(gfx::AcceleratedWidget widget)
    : input_method_(ui::CreateInputMethod(this, widget)),
      target_root_window_(NULL) {
  // TODO(yusukes): Check if the root window is currently focused and pass the
  // result to Init().
  input_method_->Init(true);
}

InputMethodEventFilter::~InputMethodEventFilter() {
}

void InputMethodEventFilter::SetInputMethodPropertyInRootWindow(
    aura::RootWindow* root_window) {
  root_window->SetProperty(aura::client::kRootWindowInputMethodKey,
                           input_method_.get());
}

////////////////////////////////////////////////////////////////////////////////
// InputMethodEventFilter, EventFilter implementation:

void InputMethodEventFilter::OnKeyEvent(ui::KeyEvent* event) {
  const ui::EventType type = event->type();
  if (type == ui::ET_TRANSLATED_KEY_PRESS ||
      type == ui::ET_TRANSLATED_KEY_RELEASE) {
    // The |event| is already handled by this object, change the type of the
    // event to ui::ET_KEY_* and pass it to the next filter.
    static_cast<ui::TranslatedKeyEvent*>(event)->ConvertToKeyEvent();
  } else {
    // If the focused window is changed, all requests to IME will be
    // discarded so it's safe to update the target_root_window_ here.
    aura::Window* target = static_cast<aura::Window*>(event->target());
    target_root_window_ = target->GetRootWindow();
    DCHECK(target_root_window_);
    bool handled = false;
    if (event->HasNativeEvent())
      handled = input_method_->DispatchKeyEvent(event->native_event());
    else
      handled = input_method_->DispatchFabricatedKeyEvent(*event);
    if (handled)
      event->StopPropagation();
  }
}

////////////////////////////////////////////////////////////////////////////////
// InputMethodEventFilter, ui::InputMethodDelegate implementation:

bool InputMethodEventFilter::DispatchKeyEventPostIME(
    const base::NativeEvent& event) {
#if defined(OS_WIN)
  DCHECK(event.message != WM_CHAR);
#endif
  ui::TranslatedKeyEvent aura_event(event, false /* is_char */);
  return target_root_window_->AsRootWindowHostDelegate()->OnHostKeyEvent(
      &aura_event);
}

bool InputMethodEventFilter::DispatchFabricatedKeyEventPostIME(
    ui::EventType type,
    ui::KeyboardCode key_code,
    int flags) {
  ui::TranslatedKeyEvent aura_event(type == ui::ET_KEY_PRESSED, key_code,
                                    flags);
  return target_root_window_->AsRootWindowHostDelegate()->OnHostKeyEvent(
      &aura_event);
}

}  // namespace corewm
}  // namespace views
