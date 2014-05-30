// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "build/build_config.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/font.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"
#include "ui/views/view.h"

#if !defined(OS_LINUX)
#include "base/logging.h"
#endif

namespace gfx {
class ImageSkia;
}

namespace ui {
class Range;
class TextInputClient;
}  // namespace ui

namespace views {

class ImageView;

class TextfieldController;

// This class implements a View that wraps a native text (edit) field.
class VIEWS_EXPORT Textfield : public View {
 public:
  // The button's class name.
  static const char kViewClassName[];

  enum StyleFlags {
    STYLE_DEFAULT   = 0,
    STYLE_OBSCURED  = 1 << 0,
    STYLE_LOWERCASE = 1 << 1
  };

  // Returns true if the build or commandline dictates NativeTextfieldViews use.
  static bool IsViewsTextfieldEnabled();

  Textfield();
  explicit Textfield(StyleFlags style);
  virtual ~Textfield();

  // TextfieldController accessors
  void SetController(TextfieldController* controller);
  TextfieldController* GetController() const;

  // Gets/Sets whether or not the Textfield is read-only.
  bool read_only() const { return read_only_; }
  void SetReadOnly(bool read_only);

  // Gets/sets the STYLE_OBSCURED bit, controlling whether characters in this
  // Textfield are displayed as asterisks/bullets.
  bool IsObscured() const;
  void SetObscured(bool obscured);

  // Gets/Sets the input type of this textfield.
  ui::TextInputType GetTextInputType() const;
  void SetTextInputType(ui::TextInputType type);

  // Gets/Sets the text currently displayed in the Textfield.
  const string16& text() const { return text_; }

  // Sets the text currently displayed in the Textfield.  This doesn't
  // change the cursor position if the current cursor is within the
  // new text's range, or moves the cursor to the end if the cursor is
  // out of the new text's range.
  void SetText(const string16& text);

  // Appends the given string to the previously-existing text in the field.
  void AppendText(const string16& text);

  // Replaces the selected text with |text|.
  void ReplaceSelection(const string16& text);

  // Returns the text direction.
  base::i18n::TextDirection GetTextDirection() const;

  // Returns the text that is currently selected.
  string16 GetSelectedText() const;

  // Select the entire text range. If |reversed| is true, the range will end at
  // the logical beginning of the text; this generally shows the leading portion
  // of text that overflows its display area.
  void SelectAll(bool reversed);

  // Clears the selection within the edit field and sets the caret to the end.
  void ClearSelection() const;

  // Checks if there is any selected text.
  bool HasSelection() const;

  // Accessor for |style_|.
  StyleFlags style() const { return style_; }

  // Gets/Sets the text color to be used when painting the Textfield.
  // Call |UseDefaultTextColor| to restore the default system color.
  SkColor GetTextColor() const;
  void SetTextColor(SkColor color);
  void UseDefaultTextColor();

  // Gets/Sets the background color to be used when painting the Textfield.
  // Call |UseDefaultBackgroundColor| to restore the default system color.
  SkColor GetBackgroundColor() const;
  void SetBackgroundColor(SkColor color);
  void UseDefaultBackgroundColor();

  // Gets/Sets whether or not the cursor is enabled.
  bool GetCursorEnabled() const;
  void SetCursorEnabled(bool enabled);

  // Gets/Sets the font used when rendering the text within the Textfield.
  const gfx::Font& font() const { return font_; }
  void SetFont(const gfx::Font& font);

  // Sets the left and right margin (in pixels) within the text box. On Windows
  // this is accomplished by packing the left and right margin into a single
  // 32 bit number, so the left and right margins are effectively 16 bits.
  void SetHorizontalMargins(int left, int right);

  // Sets the top and bottom margins (in pixels) within the textfield.
  // NOTE: in most cases height could be changed instead.
  void SetVerticalMargins(int top, int bottom);

  // Sets the default width of the text control. See default_width_in_chars_.
  void set_default_width_in_chars(int default_width) {
    default_width_in_chars_ = default_width;
  }

  // Removes the border from the edit box, giving it a 2D look.
  bool draw_border() const { return draw_border_; }
  void RemoveBorder();

  // Sets the border color (if one is in use).
  void SetBorderColor(SkColor color);
  // Reverts the textfield to the system default border color.
  void UseDefaultBorderColor();
  SkColor border_color() const { return border_color_; }
  bool use_default_border_color() const { return use_default_border_color_; }

  // Sets the text to display when empty.
  void set_placeholder_text(const string16& text) {
    placeholder_text_ = text;
#if !defined(OS_LINUX)
    NOTIMPLEMENTED();
#endif
  }
  const string16& placeholder_text() const {
    return placeholder_text_;
  }

  SkColor placeholder_text_color() const { return placeholder_text_color_; }
  void set_placeholder_text_color(SkColor color) {
    placeholder_text_color_ = color;
  }

  // Adds an icon which displays inside the border on the right side of the view
  // (left in RTL).
  void SetIcon(const gfx::ImageSkia& icon);

  // Getter for the horizontal margins that were set. Returns false if
  // horizontal margins weren't set.
  bool GetHorizontalMargins(int* left, int* right);

  // Getter for the vertical margins that were set. Returns false if vertical
  // margins weren't set.
  bool GetVerticalMargins(int* top, int* bottom);

  // Updates all properties on the textfield. This is invoked internally.
  // Users of Textfield never need to invoke this directly.
  void UpdateAllProperties();

  // Invoked by the edit control when the value changes. This method set
  // the text_ member variable to the value contained in edit control.
  // This is important because the edit control can be replaced if it has
  // been deleted during a window close.
  void SyncText();

  // Returns whether or not an IME is composing text.
  bool IsIMEComposing() const;

  // Gets the selected range. This is views-implementation only and
  // has to be called after the wrapper is created.
  // TODO(msw): Return a const reference when NativeTextfieldWin is gone.
  ui::Range GetSelectedRange() const;

  // Selects the text given by |range|. This is views-implementation only and
  // has to be called after the wrapper is created.
  void SelectRange(const ui::Range& range);

  // Gets the selection model. This is views-implementation only and
  // has to be called after the wrapper is created.
  // TODO(msw): Return a const reference when NativeTextfieldWin is gone.
  gfx::SelectionModel GetSelectionModel() const;

  // Selects the text given by |sel|. This is views-implementation only and
  // has to be called after the wrapper is created.
  void SelectSelectionModel(const gfx::SelectionModel& sel);

  // Returns the current cursor position. This is views-implementation
  // only and has to be called after the wrapper is created.
  size_t GetCursorPosition() const;

  // Set the text color over the entire text or a logical character range.
  // Empty and invalid ranges are ignored. This is views-implementation only and
  // has to be called after the wrapper is created.
  void SetColor(SkColor value);
  void ApplyColor(SkColor value, const ui::Range& range);

  // Set various text styles over the entire text or a logical character range.
  // The respective |style| is applied if |value| is true, or removed if false.
  // Empty and invalid ranges are ignored. This is views-implementation only and
  // has to be called after the wrapper is created.
  void SetStyle(gfx::TextStyle style, bool value);
  void ApplyStyle(gfx::TextStyle style, bool value, const ui::Range& range);

  // Clears Edit history.
  void ClearEditHistory();

  // Set the accessible name of the text field.
  void SetAccessibleName(const string16& name);

  // Performs the action associated with the specified command id.
  void ExecuteCommand(int command_id);

  // Provided only for testing:
  gfx::NativeView GetTestingHandle() const {
    return native_wrapper_ ? native_wrapper_->GetTestingHandle() : NULL;
  }
  NativeTextfieldWrapper* GetNativeWrapperForTesting() const {
    return native_wrapper_;
  }

  // Returns whether there is a drag operation originating from the textfield.
  bool HasTextBeingDragged();

  // Overridden from View:
  virtual void Layout() OVERRIDE;
  virtual int GetBaseline() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnEnabledChanged() OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& e) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;

 protected:
  virtual void ViewHierarchyChanged(bool is_add, View* parent,
                                    View* child) OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;

  // The object that actually implements the native text field.
  NativeTextfieldWrapper* native_wrapper_;

 private:
  // This is the current listener for events from this Textfield.
  TextfieldController* controller_;

  // The mask of style options for this Textfield.
  StyleFlags style_;

  // The font used to render the text in the Textfield.
  gfx::Font font_;

  // The text displayed in the Textfield.
  string16 text_;

  // True if this Textfield cannot accept input and is read-only.
  bool read_only_;

  // The default number of average characters for the width of this text field.
  // This will be reported as the "desired size". Defaults to 0.
  int default_width_in_chars_;

  // Whether the border is drawn.
  bool draw_border_;

  // Text color.  Only used if |use_default_text_color_| is false.
  SkColor text_color_;

  // Should we use the system text color instead of |text_color_|?
  bool use_default_text_color_;

  // Background color.  Only used if |use_default_background_color_| is false.
  SkColor background_color_;

  // Should we use the system background color instead of |background_color_|?
  bool use_default_background_color_;

  // Border color.  Only used if |use_default_border_color_| is false.
  SkColor border_color_;

  // Should we use the system border color instead of |border_color_|?
  bool use_default_border_color_;

  // TODO(beng): remove this once NativeTextfieldWin subclasses
  //             NativeControlWin.
  bool initialized_;

  // Holds inner textfield margins.
  gfx::Insets margins_;

  // Holds whether margins were set.
  bool horizontal_margins_were_set_;
  bool vertical_margins_were_set_;

  // Text to display when empty.
  string16 placeholder_text_;

  // Placeholder text color.
  SkColor placeholder_text_color_;

  // When non-NULL, an icon to display inside the border of the textfield.
  views::ImageView* icon_view_;

  // The accessible name of the text field.
  string16 accessible_name_;

  // The input type of this text field.
  ui::TextInputType text_input_type_;

  DISALLOW_COPY_AND_ASSIGN(Textfield);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_H_
