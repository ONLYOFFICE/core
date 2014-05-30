// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/fake_input_method.h"

#include "base/logging.h"
#include "base/string16.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/glib/glib_integers.h"
#include "ui/base/ime/input_method_delegate.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/keycodes/keyboard_code_conversion.h"

#if defined(USE_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ui/base/keycodes/keyboard_code_conversion_x.h"
#endif

namespace {

#if defined(USE_X11)
uint32 EventFlagsFromXFlags(unsigned int flags) {
  return (flags & LockMask ? ui::EF_CAPS_LOCK_DOWN : 0U) |
      (flags & ControlMask ? ui::EF_CONTROL_DOWN : 0U) |
      (flags & ShiftMask ? ui::EF_SHIFT_DOWN : 0U) |
      (flags & Mod1Mask ? ui::EF_ALT_DOWN : 0U);
}
#endif

}  // namespace

namespace ui {

FakeInputMethod::FakeInputMethod(internal::InputMethodDelegate* delegate)
    : delegate_(NULL),
      text_input_client_(NULL) {
  SetDelegate(delegate);
}

FakeInputMethod::~FakeInputMethod() {
}

void FakeInputMethod::SetDelegate(internal::InputMethodDelegate* delegate) {
  delegate_ = delegate;
}

void FakeInputMethod::SetFocusedTextInputClient(TextInputClient* client) {
  text_input_client_ = client;
  FOR_EACH_OBSERVER(InputMethodObserver, observers_,
                    OnTextInputStateChanged(client));
}

TextInputClient* FakeInputMethod::GetTextInputClient() const {
  return text_input_client_;
}

bool FakeInputMethod::DispatchKeyEvent(const base::NativeEvent& native_event) {
  bool handled = false;
#if defined(OS_WIN)
  if (native_event.message == WM_CHAR) {
    if (text_input_client_) {
      text_input_client_->InsertChar(ui::KeyboardCodeFromNative(native_event),
                                     ui::EventFlagsFromNative(native_event));
      handled = true;
    }
  } else {
    handled = delegate_->DispatchKeyEventPostIME(native_event);
  }
#elif defined(USE_X11)
  DCHECK(native_event);
  if (native_event->type == KeyRelease) {
    // On key release, just dispatch it.
    handled = delegate_->DispatchKeyEventPostIME(native_event);
  } else {
    const uint32 state = EventFlagsFromXFlags(native_event->xkey.state);
    // Send a RawKeyDown event first,
    handled = delegate_->DispatchKeyEventPostIME(native_event);
    if (text_input_client_) {
      // then send a Char event via ui::TextInputClient.
      const KeyboardCode key_code = ui::KeyboardCodeFromNative(native_event);
      uint16 ch = 0;
      if (!(state & ui::EF_CONTROL_DOWN))
        ch = ui::GetCharacterFromXEvent(native_event);
      if (!ch)
        ch = ui::GetCharacterFromKeyCode(key_code, state);
      if (ch)
        text_input_client_->InsertChar(ch, state);
    }
  }
#else
  // TODO(yusukes): Support other platforms. Call InsertChar() when necessary.
  handled = delegate_->DispatchKeyEventPostIME(native_event);
#endif
  return handled;
}

bool FakeInputMethod::DispatchFabricatedKeyEvent(const ui::KeyEvent& event) {
  bool handled = delegate_->DispatchFabricatedKeyEventPostIME(
      event.type(), event.key_code(), event.flags());
  if (event.type() == ET_KEY_PRESSED && text_input_client_) {
    uint16 ch = event.GetCharacter();
    if (ch)
      text_input_client_->InsertChar(ch, event.flags());
  }
  return handled;
}

void FakeInputMethod::Init(bool focused) {}
void FakeInputMethod::OnFocus() {}
void FakeInputMethod::OnBlur() {}
void FakeInputMethod::OnTextInputTypeChanged(const TextInputClient* client) {
  FOR_EACH_OBSERVER(InputMethodObserver, observers_,
                    OnTextInputStateChanged(client));
}
void FakeInputMethod::OnCaretBoundsChanged(const TextInputClient* client) {}
void FakeInputMethod::CancelComposition(const TextInputClient* client) {}

std::string FakeInputMethod::GetInputLocale() {
  return "";
}

base::i18n::TextDirection FakeInputMethod::GetInputTextDirection() {
  return base::i18n::UNKNOWN_DIRECTION;
}

bool FakeInputMethod::IsActive() {
  return true;
}

ui::TextInputType FakeInputMethod::GetTextInputType() const {
  return ui::TEXT_INPUT_TYPE_NONE;
}

bool FakeInputMethod::CanComposeInline() const {
  return true;
}

void FakeInputMethod::AddObserver(InputMethodObserver* observer) {
  observers_.AddObserver(observer);
}

void FakeInputMethod::RemoveObserver(InputMethodObserver* observer) {
  observers_.RemoveObserver(observer);
}

}  // namespace ui
