// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_IME_INPUT_METHOD_BRIDGE_H_
#define UI_VIEWS_IME_INPUT_METHOD_BRIDGE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/gfx/rect.h"
#include "ui/views/ime/input_method_base.h"

namespace ui {
class InputMethod;
}  // namespace ui

namespace views {

class View;

// A "bridge" InputMethod implementation for views top-level widgets, which just
// sends/receives IME related events to/from a system-wide ui::InputMethod
// object.
class InputMethodBridge : public InputMethodBase,
                          public ui::TextInputClient {
 public:
  // |shared_input_method| indicates if |host| is shared among other top level
  // widgets.
  InputMethodBridge(internal::InputMethodDelegate* delegate,
                    ui::InputMethod* host,
                    bool shared_input_method);
  virtual ~InputMethodBridge();

  // Overridden from InputMethod:
  virtual void Init(Widget* widget) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void DispatchKeyEvent(const ui::KeyEvent& key) OVERRIDE;
  virtual void OnTextInputTypeChanged(View* view) OVERRIDE;
  virtual void OnCaretBoundsChanged(View* view) OVERRIDE;
  virtual void CancelComposition(View* view) OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;

  // Overridden from TextInputClient:
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) OVERRIDE;
  virtual bool HasCompositionText() OVERRIDE;
  virtual bool GetTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetCompositionTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetSelectionRange(ui::Range* range) OVERRIDE;
  virtual bool SetSelectionRange(const ui::Range& range) OVERRIDE;
  virtual bool DeleteRange(const ui::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(
      const ui::Range& range, string16* text) OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;

  // Overridden from FocusChangeListener.
  virtual void OnWillChangeFocus(View* focused_before, View* focused) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before, View* focused) OVERRIDE;

 private:
  void UpdateViewFocusState();

  ui::InputMethod* const host_;

  const bool shared_input_method_;

  bool context_focused_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodBridge);
};

}  // namespace views

#endif  // UI_VIEWS_IME_INPUT_METHOD_BRIDGE_H_
