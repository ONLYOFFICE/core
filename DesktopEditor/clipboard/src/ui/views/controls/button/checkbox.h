// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_CHECKBOX_H_
#define UI_VIEWS_CONTROLS_BUTTON_CHECKBOX_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "ui/views/controls/button/text_button.h"

namespace views {

// A border with zero left inset.
class VIEWS_EXPORT CheckboxNativeThemeBorder
    : public TextButtonNativeThemeBorder {
 public:
  explicit CheckboxNativeThemeBorder(views::NativeThemeDelegate* delegate)
      : TextButtonNativeThemeBorder(delegate),
        use_custom_insets_(false) {}
  virtual ~CheckboxNativeThemeBorder() {}

  // The insets apply to the whole view (checkbox + text), not just the square
  // with the checkmark in it. The insets do not visibly affect the checkbox,
  // except to ensure that there is enough padding between this and other
  // elements.
  virtual gfx::Insets GetInsets() const OVERRIDE;

  // Use the |custom_insets_| provided instead of those from the theme.
  void SetCustomInsets(const gfx::Insets& custom_insets);

  // Use the default insets and ignore any |custom_insets_| that may be set.
  void UseDefaultInsets();

 private:
  // Only used if |use_custom_insets_| is true.
  gfx::Insets custom_insets_;

  // Whether |custom_insets_| should be used in |GetInsets()|.
  bool use_custom_insets_;

  DISALLOW_COPY_AND_ASSIGN(CheckboxNativeThemeBorder);
};

// A native themed class representing a checkbox.  This class does not use
// platform specific objects to replicate the native platforms looks and feel.
class VIEWS_EXPORT Checkbox : public TextButtonBase {
 public:
  static const char kViewClassName[];

  explicit Checkbox(const string16& label);
  virtual ~Checkbox();

  // Sets a listener for this checkbox. Checkboxes aren't required to have them
  // since their state can be read independently of them being toggled.
  void set_listener(ButtonListener* listener) { listener_ = listener; }

  // Sets/Gets whether or not the checkbox is checked.
  virtual void SetChecked(bool checked);
  bool checked() const { return checked_; }

 protected:
  // Overridden from View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;

 private:
  // Overridden from Button:
  virtual void NotifyClick(const ui::Event& event) OVERRIDE;

  // Overridden from TextButtonBase:
  virtual ui::NativeTheme::Part GetThemePart() const OVERRIDE;
  virtual gfx::Rect GetThemePaintRect() const OVERRIDE;
  virtual void GetExtraParams(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual gfx::Rect GetTextBounds() const OVERRIDE;

  // True if the checkbox is checked.
  bool checked_;

  DISALLOW_COPY_AND_ASSIGN(Checkbox);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_BUTTON_CHECKBOX_H_
