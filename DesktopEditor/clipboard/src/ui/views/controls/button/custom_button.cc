// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/custom_button.h"

#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/screen.h"
#include "ui/views/widget/widget.h"

namespace views {

// How long the hover animation takes if uninterrupted.
static const int kHoverFadeDurationMs = 150;

// static
const char CustomButton::kViewClassName[] = "views/CustomButton";

////////////////////////////////////////////////////////////////////////////////
// CustomButton, public:

CustomButton::~CustomButton() {
}

void CustomButton::SetState(ButtonState state) {
  if (state == state_)
    return;

  if (animate_on_state_change_ &&
      (!is_throbbing_ || !hover_animation_->is_animating())) {
    is_throbbing_ = false;
    if (state_ == STATE_NORMAL && state == STATE_HOVERED) {
      // Button is hovered from a normal state, start hover animation.
      hover_animation_->Show();
    } else if ((state_ == STATE_HOVERED || state_ == STATE_PRESSED)
          && state == STATE_NORMAL) {
      // Button is returning to a normal state from hover, start hover
      // fade animation.
      hover_animation_->Hide();
    } else {
      hover_animation_->Stop();
    }
  }

  state_ = state;
  StateChanged();
  if (state_changed_delegate_.get())
    state_changed_delegate_->StateChanged(state_);
  SchedulePaint();
}

void CustomButton::StartThrobbing(int cycles_til_stop) {
  is_throbbing_ = true;
  hover_animation_->StartThrobbing(cycles_til_stop);
}

void CustomButton::StopThrobbing() {
  if (hover_animation_->is_animating()) {
    hover_animation_->Stop();
    SchedulePaint();
  }
}

void CustomButton::SetAnimationDuration(int duration) {
  hover_animation_->SetSlideDuration(duration);
}

bool CustomButton::IsMouseHovered() const {
  // If we haven't yet been placed in an onscreen view hierarchy, we can't be
  // hovered.
  if (!GetWidget())
    return false;

  gfx::Point cursor_pos(gfx::Screen::GetScreenFor(
      GetWidget()->GetNativeView())->GetCursorScreenPoint());
  ConvertPointToTarget(NULL, this, &cursor_pos);
  return HitTestPoint(cursor_pos);
}

void CustomButton::SetHotTracked(bool is_hot_tracked) {
  if (state_ != STATE_DISABLED)
    SetState(is_hot_tracked ? STATE_HOVERED : STATE_NORMAL);

  if (is_hot_tracked)
    NotifyAccessibilityEvent(ui::AccessibilityTypes::EVENT_FOCUS, true);
}

bool CustomButton::IsHotTracked() const {
  return state_ == STATE_HOVERED;
}

////////////////////////////////////////////////////////////////////////////////
// CustomButton, View overrides:

void CustomButton::OnEnabledChanged() {
  if (enabled() ? (state_ != STATE_DISABLED) : (state_ == STATE_DISABLED))
    return;

  if (enabled())
    SetState(IsMouseHovered() ? STATE_HOVERED : STATE_NORMAL);
  else
    SetState(STATE_DISABLED);
}

std::string CustomButton::GetClassName() const {
  return kViewClassName;
}

bool CustomButton::OnMousePressed(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (ShouldEnterPushedState(event) && HitTestPoint(event.location()))
      SetState(STATE_PRESSED);
    if (request_focus_on_press_)
      RequestFocus();
  }
  return true;
}

bool CustomButton::OnMouseDragged(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (HitTestPoint(event.location()))
      SetState(ShouldEnterPushedState(event) ? STATE_PRESSED : STATE_HOVERED);
    else
      SetState(STATE_NORMAL);
  }
  return true;
}

void CustomButton::OnMouseReleased(const ui::MouseEvent& event) {
  if (state_ == STATE_DISABLED)
    return;

  if (!HitTestPoint(event.location())) {
    SetState(STATE_NORMAL);
    return;
  }

  SetState(STATE_HOVERED);
  if (IsTriggerableEvent(event)) {
    NotifyClick(event);
    // NOTE: We may be deleted at this point (by the listener's notification
    // handler).
  }
}

void CustomButton::OnMouseCaptureLost() {
  // Starting a drag results in a MouseCaptureLost, we need to ignore it.
  if (state_ != STATE_DISABLED && !InDrag())
    SetState(STATE_NORMAL);
}

void CustomButton::OnMouseEntered(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED)
    SetState(STATE_HOVERED);
}

void CustomButton::OnMouseExited(const ui::MouseEvent& event) {
  // Starting a drag results in a MouseExited, we need to ignore it.
  if (state_ != STATE_DISABLED && !InDrag())
    SetState(STATE_NORMAL);
}

void CustomButton::OnMouseMoved(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED)
    SetState(HitTestPoint(event.location()) ? STATE_HOVERED : STATE_NORMAL);
}

bool CustomButton::OnKeyPressed(const ui::KeyEvent& event) {
  if (state_ == STATE_DISABLED)
    return false;

  // Space sets button state to pushed. Enter clicks the button. This matches
  // the Windows native behavior of buttons, where Space clicks the button on
  // KeyRelease and Enter clicks the button on KeyPressed.
  if (event.key_code() == ui::VKEY_SPACE) {
    SetState(STATE_PRESSED);
  } else if (event.key_code() == ui::VKEY_RETURN) {
    SetState(STATE_NORMAL);
    // TODO(beng): remove once NotifyClick takes ui::Event.
    ui::MouseEvent synthetic_event(ui::ET_MOUSE_RELEASED,
                                   gfx::Point(),
                                   gfx::Point(),
                                   ui::EF_LEFT_MOUSE_BUTTON);
    NotifyClick(synthetic_event);
  } else {
    return false;
  }
  return true;
}

bool CustomButton::OnKeyReleased(const ui::KeyEvent& event) {
  if ((state_ == STATE_DISABLED) || (event.key_code() != ui::VKEY_SPACE))
    return false;

  SetState(STATE_NORMAL);
  // TODO(beng): remove once NotifyClick takes ui::Event.
  ui::MouseEvent synthetic_event(ui::ET_MOUSE_RELEASED,
                                 gfx::Point(),
                                 gfx::Point(),
                                 ui::EF_LEFT_MOUSE_BUTTON);
  NotifyClick(synthetic_event);
  return true;
}

void CustomButton::OnGestureEvent(ui::GestureEvent* event) {
  if (state_ == STATE_DISABLED) {
    Button::OnGestureEvent(event);
    return;
  }

  if (event->type() == ui::ET_GESTURE_TAP && IsTriggerableEvent(*event)) {
    // Set the button state to hot and start the animation fully faded in. The
    // TAP_UP event issued immediately after will set the state to STATE_NORMAL
    // beginning the fade out animation. See http://crbug.com/131184.
    SetState(STATE_HOVERED);
    hover_animation_->Reset(1.0);
    NotifyClick(*event);
    event->StopPropagation();
  } else if (event->type() == ui::ET_GESTURE_TAP_DOWN &&
             ShouldEnterPushedState(*event)) {
    SetState(STATE_PRESSED);
    if (request_focus_on_press_)
      RequestFocus();
    event->StopPropagation();
  } else {
    SetState(STATE_NORMAL);
  }
  if (!event->handled())
    Button::OnGestureEvent(event);
}

bool CustomButton::AcceleratorPressed(const ui::Accelerator& accelerator) {
  SetState(STATE_NORMAL);
  /*
  ui::KeyEvent key_event(ui::ET_KEY_RELEASED, accelerator.key_code(),
                         accelerator.modifiers());
                         */
  // TODO(beng): remove once NotifyClick takes ui::Event.
  ui::MouseEvent synthetic_event(ui::ET_MOUSE_RELEASED,
                                 gfx::Point(),
                                 gfx::Point(),
                                 ui::EF_LEFT_MOUSE_BUTTON);
  NotifyClick(synthetic_event);
  return true;
}

void CustomButton::ShowContextMenu(const gfx::Point& p, bool is_mouse_gesture) {
  if (!context_menu_controller())
    return;

  // We're about to show the context menu. Showing the context menu likely means
  // we won't get a mouse exited and reset state. Reset it now to be sure.
  if (state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
  View::ShowContextMenu(p, is_mouse_gesture);
}

void CustomButton::OnDragDone() {
  SetState(STATE_NORMAL);
}

void CustomButton::GetAccessibleState(ui::AccessibleViewState* state) {
  Button::GetAccessibleState(state);
  switch (state_) {
    case STATE_HOVERED:
      state->state = ui::AccessibilityTypes::STATE_HOTTRACKED;
      break;
    case STATE_PRESSED:
      state->state = ui::AccessibilityTypes::STATE_PRESSED;
      break;
    case STATE_DISABLED:
      state->state = ui::AccessibilityTypes::STATE_UNAVAILABLE;
      break;
    case STATE_NORMAL:
    case STATE_COUNT:
      // No additional accessibility state set for this button state.
      break;
  }
}

void CustomButton::VisibilityChanged(View* starting_from, bool visible) {
  if (state_ == STATE_DISABLED)
    return;
  SetState(visible && IsMouseHovered() ? STATE_HOVERED : STATE_NORMAL);
}

////////////////////////////////////////////////////////////////////////////////
// CustomButton, ui::AnimationDelegate implementation:

void CustomButton::AnimationProgressed(const ui::Animation* animation) {
  SchedulePaint();
}

////////////////////////////////////////////////////////////////////////////////
// CustomButton, protected:

CustomButton::CustomButton(ButtonListener* listener)
    : Button(listener),
      state_(STATE_NORMAL),
      animate_on_state_change_(true),
      is_throbbing_(false),
      triggerable_event_flags_(ui::EF_LEFT_MOUSE_BUTTON),
      request_focus_on_press_(true) {
  hover_animation_.reset(new ui::ThrobAnimation(this));
  hover_animation_->SetSlideDuration(kHoverFadeDurationMs);
}

void CustomButton::StateChanged() {
}

bool CustomButton::IsTriggerableEvent(const ui::Event& event) {
  return event.type() == ui::ET_GESTURE_TAP_DOWN ||
         event.type() == ui::ET_GESTURE_TAP ||
         (event.IsMouseEvent() &&
             (triggerable_event_flags_ & event.flags()) != 0);
}

bool CustomButton::ShouldEnterPushedState(const ui::Event& event) {
  return IsTriggerableEvent(event);
}

////////////////////////////////////////////////////////////////////////////////
// CustomButton, View overrides (protected):

void CustomButton::ViewHierarchyChanged(bool is_add,
                                        View* parent,
                                        View* child) {
  if (!is_add && state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
}

void CustomButton::OnBlur() {
  if (IsHotTracked())
    SetState(STATE_NORMAL);
}

}  // namespace views
