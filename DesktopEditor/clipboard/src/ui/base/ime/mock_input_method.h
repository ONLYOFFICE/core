// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_MOCK_INPUT_METHOD_H_
#define UI_BASE_IME_MOCK_INPUT_METHOD_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/ime/input_method_observer.h"
#include "ui/base/ui_export.h"

namespace ui {

class KeyEvent;
class TextInputClient;

// A mock ui::InputMethod implementation for testing. You can get the instance
// of this class as the global input method with calling
// SetUpInputMethodFactoryForTesting() which is declared in
// ui/base/ime/input_method_factory.h
class UI_EXPORT MockInputMethod : NON_EXPORTED_BASE(public InputMethod) {
 public:
  class Observer : public InputMethodObserver {
   public:
    virtual void OnTextInputTypeChanged(const TextInputClient* client) = 0;
    virtual void OnFocus() = 0;
    virtual void OnBlur() = 0;
    virtual void OnCaretBoundsChanged(const TextInputClient* client) = 0;

    // InputMethodObserver overrides
    virtual void OnTextInputStateChanged(const TextInputClient* client) = 0;
    virtual void OnInputMethodDestroyed(const InputMethod* input_method) = 0;
  };
  explicit MockInputMethod(internal::InputMethodDelegate* delegate);
  virtual ~MockInputMethod();

  // Overriden from InputMethod.
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) OVERRIDE;
  virtual void Init(bool focused) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void SetFocusedTextInputClient(TextInputClient* client) OVERRIDE;
  virtual TextInputClient* GetTextInputClient() const OVERRIDE;
  virtual bool DispatchKeyEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual bool DispatchFabricatedKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;
  virtual TextInputType GetTextInputType() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual void AddObserver(InputMethodObserver* observer) OVERRIDE;
  virtual void RemoveObserver(InputMethodObserver* observer) OVERRIDE;

 private:
  TextInputClient* text_input_client_;
  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(MockInputMethod);
};

}  // namespace ui

#endif  // UI_BASE_IME_MOCK_INPUT_METHOD_H_
