// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/button.h"

#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"

namespace views {

////////////////////////////////////////////////////////////////////////////////
// Button, public:

Button::~Button() {
}

void Button::SetTooltipText(const string16& tooltip_text) {
  tooltip_text_ = tooltip_text;
  if (accessible_name_.empty())
    accessible_name_ = tooltip_text_;
  TooltipTextChanged();
}

void Button::SetAccessibleName(const string16& name) {
  accessible_name_ = name;
}

////////////////////////////////////////////////////////////////////////////////
// Button, View overrides:

bool Button::GetTooltipText(const gfx::Point& p, string16* tooltip) const {
  if (tooltip_text_.empty())
    return false;

  *tooltip = tooltip_text_;
  return true;
}

void Button::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_PUSHBUTTON;
  state->name = accessible_name_;
}

////////////////////////////////////////////////////////////////////////////////
// Button, protected:

Button::Button(ButtonListener* listener)
    : listener_(listener),
      tag_(-1) {
  set_accessibility_focusable(true);
}

void Button::NotifyClick(const ui::Event& event) {
  // We can be called when there is no listener, in cases like double clicks on
  // menu buttons etc.
  if (listener_)
    listener_->ButtonPressed(this, event);
}

}  // namespace views
