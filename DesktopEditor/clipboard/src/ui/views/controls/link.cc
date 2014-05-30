// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/link.h"

#include "build/build_config.h"

#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/font.h"
#include "ui/views/controls/link_listener.h"

#if defined(USE_AURA)
#include "ui/base/cursor/cursor.h"
#endif

namespace views {

const char Link::kViewClassName[] = "views/Link";

Link::Link() : Label(string16()) {
  Init();
}

Link::Link(const string16& title) : Label(title) {
  Init();
}

Link::~Link() {
}

void Link::OnEnabledChanged() {
  RecalculateFont();
  View::OnEnabledChanged();
}

std::string Link::GetClassName() const {
  return kViewClassName;
}

gfx::NativeCursor Link::GetCursor(const ui::MouseEvent& event) {
  if (!enabled())
    return gfx::kNullCursor;
#if defined(USE_AURA)
  return ui::kCursorHand;
#elif defined(OS_WIN)
  static HCURSOR g_hand_cursor = LoadCursor(NULL, IDC_HAND);
  return g_hand_cursor;
#endif
}

bool Link::HitTestRect(const gfx::Rect& rect) const {
  // We need to allow clicks on the link. So we override the implementation in
  // Label and use the default implementation of View.
  return View::HitTestRect(rect);
}

bool Link::OnMousePressed(const ui::MouseEvent& event) {
  if (!enabled() ||
      (!event.IsLeftMouseButton() && !event.IsMiddleMouseButton()))
    return false;
  SetPressed(true);
  return true;
}

bool Link::OnMouseDragged(const ui::MouseEvent& event) {
  SetPressed(enabled() &&
             (event.IsLeftMouseButton() || event.IsMiddleMouseButton()) &&
             HitTestPoint(event.location()));
  return true;
}

void Link::OnMouseReleased(const ui::MouseEvent& event) {
  // Change the highlight first just in case this instance is deleted
  // while calling the controller
  OnMouseCaptureLost();
  if (enabled() &&
      (event.IsLeftMouseButton() || event.IsMiddleMouseButton()) &&
      HitTestPoint(event.location())) {
    // Focus the link on click.
    RequestFocus();

    if (listener_)
      listener_->LinkClicked(this, event.flags());
  }
}

void Link::OnMouseCaptureLost() {
  SetPressed(false);
}

bool Link::OnKeyPressed(const ui::KeyEvent& event) {
  bool activate = ((event.key_code() == ui::VKEY_SPACE) ||
                   (event.key_code() == ui::VKEY_RETURN));
  if (!activate)
    return false;

  SetPressed(false);

  // Focus the link on key pressed.
  RequestFocus();

  if (listener_)
    listener_->LinkClicked(this, event.flags());

  return true;
}

bool Link::SkipDefaultKeyEventProcessing(const ui::KeyEvent& event) {
  // Make sure we don't process space or enter as accelerators.
  return (event.key_code() == ui::VKEY_SPACE) ||
      (event.key_code() == ui::VKEY_RETURN);
}

void Link::GetAccessibleState(ui::AccessibleViewState* state) {
  Label::GetAccessibleState(state);
  state->role = ui::AccessibilityTypes::ROLE_LINK;
}

void Link::OnGestureEvent(ui::GestureEvent* event) {
  if (!enabled())
    return;

  if (event->type() == ui::ET_GESTURE_TAP_DOWN) {
    SetPressed(true);
  } else if (event->type() == ui::ET_GESTURE_TAP) {
    RequestFocus();
    if (listener_)
      listener_->LinkClicked(this, event->flags());
  } else {
    SetPressed(false);
    return;
  }
  event->SetHandled();
}

void Link::SetFont(const gfx::Font& font) {
  Label::SetFont(font);
  RecalculateFont();
}

void Link::SetEnabledColor(SkColor color) {
  requested_enabled_color_ = color;
  if (!pressed_)
    Label::SetEnabledColor(requested_enabled_color_);
}

void Link::SetPressedColor(SkColor color) {
  requested_pressed_color_ = color;
  if (pressed_)
    Label::SetEnabledColor(requested_pressed_color_);
}

void Link::SetUnderline(bool underline) {
  if (underline_ == underline)
    return;
  underline_ = underline;
  RecalculateFont();
}

void Link::Init() {
  static bool initialized = false;
  static SkColor kDefaultEnabledColor;
  static SkColor kDefaultDisabledColor;
  static SkColor kDefaultPressedColor;
  if (!initialized) {
#if defined(OS_WIN)
    kDefaultEnabledColor = color_utils::GetSysSkColor(COLOR_HOTLIGHT);
    kDefaultDisabledColor = color_utils::GetSysSkColor(COLOR_WINDOWTEXT);
    kDefaultPressedColor = SkColorSetRGB(200, 0, 0);
#else
    // TODO(beng): source from theme provider.
    kDefaultEnabledColor = SkColorSetRGB(0, 51, 153);
    kDefaultDisabledColor = SK_ColorBLACK;
    kDefaultPressedColor = SK_ColorRED;
#endif

    initialized = true;
  }

  listener_ = NULL;
  pressed_ = false;
  underline_ = true;
  SetEnabledColor(kDefaultEnabledColor);
  SetDisabledColor(kDefaultDisabledColor);
  SetPressedColor(kDefaultPressedColor);
  RecalculateFont();
  set_focusable(true);
}

void Link::SetPressed(bool pressed) {
  if (pressed_ != pressed) {
    pressed_ = pressed;
    Label::SetEnabledColor(pressed_ ?
        requested_pressed_color_ : requested_enabled_color_);
    RecalculateFont();
    SchedulePaint();
  }
}

void Link::RecalculateFont() {
  // Underline the link iff it is enabled and |underline_| is true.
  const int style = font().GetStyle();
  const int intended_style = (enabled() && underline_) ?
      (style | gfx::Font::UNDERLINE) : (style & ~gfx::Font::UNDERLINE);
  if (style != intended_style)
    Label::SetFont(font().DeriveFont(0, intended_style));
}

}  // namespace views
