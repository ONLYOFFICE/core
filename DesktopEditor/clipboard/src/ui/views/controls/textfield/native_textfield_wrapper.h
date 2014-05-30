// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WRAPPER_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WRAPPER_H_

#include "base/string16.h"
#include "base/i18n/rtl.h"
#include "ui/base/range/range.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/selection_model.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class Insets;
}  // namespace gfx

namespace ui {
class KeyEvent;
class TextInputClient;
}  // namespace ui

namespace views {

class Textfield;
class View;

// An interface implemented by an object that provides a platform-native
// text field.
class VIEWS_EXPORT NativeTextfieldWrapper {
 public:
  // The Textfield calls this when it is destroyed to clean up the wrapper
  // object.
  virtual ~NativeTextfieldWrapper() {}

  // Gets the text displayed in the wrapped native text field.
  virtual string16 GetText() const = 0;

  // Updates the text displayed with the text held by the Textfield.
  virtual void UpdateText() = 0;

  // Adds the specified text to the text already displayed by the wrapped native
  // text field.
  virtual void AppendText(const string16& text) = 0;

  // Replaces the selected text with |text|.
  virtual void ReplaceSelection(const string16& text) = 0;

  // Returns the text direction.
  virtual base::i18n::TextDirection GetTextDirection() const = 0;

  // Gets the text that is selected in the wrapped native text field.
  virtual string16 GetSelectedText() const = 0;

  // Select the entire text range. If |reversed| is true, the range will end at
  // the logical beginning of the text; this generally shows the leading portion
  // of text that overflows its display area.
  virtual void SelectAll(bool reversed) = 0;

  // Clears the selection within the edit field and sets the caret to the end.
  virtual void ClearSelection() = 0;

  // Updates the border display for the native text field with the state desired
  // by the Textfield.
  virtual void UpdateBorder() = 0;

  // Updates the color of the border with the state desired by the Textfield.
  virtual void UpdateBorderColor() = 0;

  // Updates the text color used when painting the native text field.
  virtual void UpdateTextColor() = 0;

  // Updates the background color used when painting the native text field.
  virtual void UpdateBackgroundColor() = 0;

  // Updates the read-only state of the native text field.
  virtual void UpdateReadOnly() = 0;

  // Updates the font used to render text in the native text field.
  virtual void UpdateFont() = 0;

  // Updates the visibility of the text in the native text field.
  virtual void UpdateIsObscured() = 0;

  // Updates the enabled state of the native text field.
  virtual void UpdateEnabled() = 0;

  // Returns the insets for the text field.
  virtual gfx::Insets CalculateInsets() = 0;

  // Updates the horizontal margins for the native text field.
  virtual void UpdateHorizontalMargins() = 0;

  // Updates the vertical margins for the native text field.
  virtual void UpdateVerticalMargins() = 0;

  // Sets the focus to the text field. Returns false if the wrapper
  // didn't take focus.
  virtual bool SetFocus() = 0;

  // Retrieves the views::View that hosts the native control.
  virtual View* GetView() = 0;

  // Returns a handle to the underlying native view for testing.
  virtual gfx::NativeView GetTestingHandle() const = 0;

  // Returns whether or not an IME is composing text.
  virtual bool IsIMEComposing() const = 0;

  // Gets the selected range.
  virtual ui::Range GetSelectedRange() const = 0;

  // Selects the text given by |range|.
  virtual void SelectRange(const ui::Range& range) = 0;

  // Gets the selection model.
  virtual gfx::SelectionModel GetSelectionModel() const = 0;

  // Selects the text given by |sel|.
  virtual void SelectSelectionModel(const gfx::SelectionModel& sel) = 0;

  // Returns the currnet cursor position.
  virtual size_t GetCursorPosition() const = 0;

  // Get or set whether or not the cursor is enabled.
  virtual bool GetCursorEnabled() const = 0;
  virtual void SetCursorEnabled(bool enabled) = 0;

  // Following methods are to forward key/focus related events to the
  // views wrapper so that TextfieldViews can handle key inputs without
  // having focus.

  // Invoked when a key is pressed/release on Textfield.  Subclasser
  // should return true if the event has been processed and false
  // otherwise.
  // See also View::OnKeyPressed/OnKeyReleased.
  virtual bool HandleKeyPressed(const ui::KeyEvent& e) = 0;
  virtual bool HandleKeyReleased(const ui::KeyEvent& e) = 0;

  // Invoked when focus is being moved from or to the Textfield.
  // See also View::OnFocus/OnBlur.
  virtual void HandleFocus() = 0;
  virtual void HandleBlur() = 0;

  // Returns the View's TextInputClient instance or NULL if the View doesn't
  // support text input.
  virtual ui::TextInputClient* GetTextInputClient() = 0;

  // Set the text colors; see the corresponding Textfield functions for details.
  virtual void SetColor(SkColor value) = 0;
  virtual void ApplyColor(SkColor value, const ui::Range& range) = 0;

  // Set the text styles; see the corresponding Textfield functions for details.
  virtual void SetStyle(gfx::TextStyle style, bool value) = 0;
  virtual void ApplyStyle(gfx::TextStyle style,
                          bool value,
                          const ui::Range& range) = 0;

  // Clears Edit history.
  virtual void ClearEditHistory() = 0;

  // Get the height in pixels of the first font used in this textfield.
  virtual int GetFontHeight() = 0;

  // Returns the baseline of the textfield. This should not take into account
  // any insets.
  virtual int GetTextfieldBaseline() const = 0;

  // Returns the width necessary to display the current text, including any
  // necessary space for the cursor or border/margin.
  virtual int GetWidthNeededForText() const = 0;

  // Performs the action associated with the specified command id. Not called
  // ExecuteCommand to avoid name clash.
  virtual void ExecuteTextCommand(int command_id) = 0;

  // Returns whether there is a drag operation originating from the textfield.
  virtual bool HasTextBeingDragged() = 0;

  // Creates an appropriate NativeTextfieldWrapper for the platform.
  static NativeTextfieldWrapper* CreateWrapper(Textfield* field);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WRAPPER_H_
