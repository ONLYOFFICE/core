// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_WIN_H_
#define UI_BASE_IME_INPUT_METHOD_WIN_H_

#include <windows.h>

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/input_method_base.h"
#include "ui/base/win/ime_input.h"

namespace ui {

// An InputMethod implementation based on Windows IMM32 API.
class UI_EXPORT InputMethodWin : public InputMethodBase {
 public:
  explicit InputMethodWin(internal::InputMethodDelegate* delegate, HWND hwnd);
  virtual ~InputMethodWin();

  // Overridden from InputMethod:
  virtual void Init(bool focused) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual bool DispatchKeyEvent(
      const base::NativeEvent& native_key_event) OVERRIDE;
  virtual bool DispatchFabricatedKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;
  virtual void SetFocusedTextInputClient(TextInputClient* client) OVERRIDE;
  virtual TextInputClient* GetTextInputClient() const OVERRIDE;

  // Handles IME messages.
  LRESULT OnImeMessages(UINT message,
                        WPARAM wparam,
                        LPARAM lparam,
                        BOOL* handled);

  // Message handlers. The native widget is responsible for forwarding following
  // messages to the input method.
  void OnInputLangChange(DWORD character_set, HKL input_language_id);

 protected:
  // Overridden from InputMethodBase:
  virtual void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                         TextInputClient* focused) OVERRIDE;
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

 private:
  LRESULT OnImeSetContext(UINT message,
                          WPARAM wparam,
                          LPARAM lparam,
                          BOOL* handled);
  LRESULT OnImeStartComposition(UINT message,
                                WPARAM wparam,
                                LPARAM lparam,
                                BOOL* handled);
  LRESULT OnImeComposition(UINT message,
                           WPARAM wparam,
                           LPARAM lparam,
                           BOOL* handled);
  LRESULT OnImeEndComposition(UINT message,
                              WPARAM wparam,
                              LPARAM lparam,
                              BOOL* handled);
  LRESULT OnImeRequest(UINT message,
                       WPARAM wparam,
                       LPARAM lparam,
                       BOOL* handled);
  // For both WM_CHAR and WM_SYSCHAR
  LRESULT OnChar(UINT message, WPARAM wparam, LPARAM lparam, BOOL* handled);
  // For both WM_DEADCHAR and WM_SYSDEADCHAR
  LRESULT OnDeadChar(UINT message, WPARAM wparam, LPARAM lparam, BOOL* handled);

  LRESULT OnDocumentFeed(RECONVERTSTRING *reconv);
  LRESULT OnReconvertString(RECONVERTSTRING *reconv);

  // Asks the client to confirm current composition text.
  void ConfirmCompositionText();

  // Enables or disables the IME according to the current text input type.
  void UpdateIMEState();

  // The HWND this InputMethod is bound to.
  HWND hwnd_;

  // Indicates if the current input locale has an IME.
  bool active_;

  // Name of the current input locale.
  std::string locale_;

  // The current input text direction.
  base::i18n::TextDirection direction_;

  // The new text direction and layout alignment requested by the user by
  // pressing ctrl-shift. It'll be sent to the text input client when the key
  // is released.
  base::i18n::TextDirection pending_requested_direction_;

  // Windows IMM32 wrapper.
  // (See "ui/base/win/ime_input.h" for its details.)
  ui::ImeInput ime_input_;

  bool enabled_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodWin);
};

}  // namespace ui

#endif  // UI_BASE_IME_INPUT_METHOD_WIN_H_
