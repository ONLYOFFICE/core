// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/checkbox.h"

#include "base/logging.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/gfx/canvas.h"
#include "ui/views/controls/label.h"

namespace views {

namespace {

const int kCheckboxLabelSpacing = 4;

const int kFocusBorderWidth = 1;

}  // namespace

// static
const char Checkbox::kViewClassName[] = "views/Checkbox";

////////////////////////////////////////////////////////////////////////////////
// CheckboxNativeThemeBorder, public:

gfx::Insets CheckboxNativeThemeBorder::GetInsets() const {
  if (use_custom_insets_)
    return custom_insets_;

  const gfx::Insets& insets = TextButtonNativeThemeBorder::GetInsets();
  return gfx::Insets(insets.top(), 0, insets.bottom(), 0);
}

void CheckboxNativeThemeBorder::SetCustomInsets(
    const gfx::Insets& custom_insets) {
 custom_insets_ = custom_insets;
 use_custom_insets_ = true;
}

void CheckboxNativeThemeBorder::UseDefaultInsets() {
  use_custom_insets_ = false;
}

////////////////////////////////////////////////////////////////////////////////
// Checkbox, public:

Checkbox::Checkbox(const string16& label)
    : TextButtonBase(NULL, label),
      checked_(false) {
  set_border(new CheckboxNativeThemeBorder(this));
  set_focusable(true);
}

Checkbox::~Checkbox() {
}

void Checkbox::SetChecked(bool checked) {
  checked_ = checked;
  SchedulePaint();
}

gfx::Size Checkbox::GetPreferredSize() {
  gfx::Size prefsize(TextButtonBase::GetPreferredSize());
  ui::NativeTheme::ExtraParams extra;
  ui::NativeTheme::State state = GetThemeState(&extra);
  gfx::Size size = GetNativeTheme()->GetPartSize(GetThemePart(), state, extra);
  prefsize.Enlarge(size.width() + kCheckboxLabelSpacing + kFocusBorderWidth, 0);
  prefsize.set_height(std::max(prefsize.height(), size.height()));

  if (max_width_ > 0)
    prefsize.set_width(std::min(max_width_, prefsize.width()));

  return prefsize;
}

std::string Checkbox::GetClassName() const {
  return kViewClassName;
}

void Checkbox::GetAccessibleState(ui::AccessibleViewState* state) {
  TextButtonBase::GetAccessibleState(state);
  state->role = ui::AccessibilityTypes::ROLE_CHECKBUTTON;
  state->state = checked() ? ui::AccessibilityTypes::STATE_CHECKED : 0;
}

void Checkbox::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    gfx::Rect bounds(GetTextBounds());
    // Increate the bounding box by one on each side so that that focus border
    // does not draw on top of the letters.
    bounds.Inset(-kFocusBorderWidth,
                 -kFocusBorderWidth,
                 -kFocusBorderWidth,
                 -kFocusBorderWidth);
    canvas->DrawFocusRect(bounds);
  }
}

void Checkbox::NotifyClick(const ui::Event& event) {
  SetChecked(!checked());
  RequestFocus();
  TextButtonBase::NotifyClick(event);
}

ui::NativeTheme::Part Checkbox::GetThemePart() const {
  return ui::NativeTheme::kCheckbox;
}

gfx::Rect Checkbox::GetThemePaintRect() const {
  ui::NativeTheme::ExtraParams extra;
  ui::NativeTheme::State state = GetThemeState(&extra);
  gfx::Size size(GetNativeTheme()->GetPartSize(GetThemePart(), state, extra));
  gfx::Insets insets = GetInsets();
  int y_offset = (height() - size.height()) / 2;
  gfx::Rect rect(insets.left(), y_offset, size.width(), size.height());
  rect.set_x(GetMirroredXForRect(rect));
  return rect;
}

void Checkbox::GetExtraParams(ui::NativeTheme::ExtraParams* params) const {
  TextButtonBase::GetExtraParams(params);
  params->button.checked = checked_;
}

gfx::Rect Checkbox::GetTextBounds() const {
  gfx::Rect bounds(TextButtonBase::GetTextBounds());
  ui::NativeTheme::ExtraParams extra;
  ui::NativeTheme::State state = GetThemeState(&extra);
  gfx::Size size(GetNativeTheme()->GetPartSize(GetThemePart(), state, extra));
  bounds.Offset(size.width() + kCheckboxLabelSpacing, 0);
  return bounds;
}

}  // namespace views
