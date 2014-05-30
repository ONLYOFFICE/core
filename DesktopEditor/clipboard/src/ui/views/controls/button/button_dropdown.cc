// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/button_dropdown.h"

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/message_loop.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_strings.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/menu_model.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/controls/menu/menu_model_adapter.h"
#include "ui/views/controls/menu/menu_runner.h"
#include "ui/views/widget/widget.h"

namespace views {

// static
const char ButtonDropDown::kViewClassName[] =
    "ui/views/controls/button/ButtonDropDown";

// How long to wait before showing the menu.
const int kMenuTimerDelay = 500;

////////////////////////////////////////////////////////////////////////////////
//
// ButtonDropDown - constructors, destructors, initialization, cleanup
//
////////////////////////////////////////////////////////////////////////////////

ButtonDropDown::ButtonDropDown(ButtonListener* listener, ui::MenuModel* model)
    : ImageButton(listener),
      model_(model),
      menu_showing_(false),
      y_position_on_lbuttondown_(0),
      show_menu_factory_(this) {
  set_context_menu_controller(this);
}

ButtonDropDown::~ButtonDropDown() {
}

void ButtonDropDown::ClearPendingMenu() {
  show_menu_factory_.InvalidateWeakPtrs();
}

bool ButtonDropDown::IsMenuShowing() const {
  return menu_showing_;
}

////////////////////////////////////////////////////////////////////////////////
//
// ButtonDropDown - Events
//
////////////////////////////////////////////////////////////////////////////////

bool ButtonDropDown::OnMousePressed(const ui::MouseEvent& event) {
  if (enabled() && ShouldShowMenu() &&
      IsTriggerableEvent(event) && HitTestPoint(event.location())) {
    // Store the y pos of the mouse coordinates so we can use them later to
    // determine if the user dragged the mouse down (which should pop up the
    // drag down menu immediately, instead of waiting for the timer)
    y_position_on_lbuttondown_ = event.y();

    // Schedule a task that will show the menu.
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&ButtonDropDown::ShowDropDownMenu,
                   show_menu_factory_.GetWeakPtr()),
        base::TimeDelta::FromMilliseconds(kMenuTimerDelay));
  }
  return ImageButton::OnMousePressed(event);
}

bool ButtonDropDown::OnMouseDragged(const ui::MouseEvent& event) {
  bool result = ImageButton::OnMouseDragged(event);

  if (show_menu_factory_.HasWeakPtrs()) {
    // If the mouse is dragged to a y position lower than where it was when
    // clicked then we should not wait for the menu to appear but show
    // it immediately.
    if (event.y() > y_position_on_lbuttondown_ + GetHorizontalDragThreshold()) {
      show_menu_factory_.InvalidateWeakPtrs();
      ShowDropDownMenu();
    }
  }

  return result;
}

void ButtonDropDown::OnMouseReleased(const ui::MouseEvent& event) {
  if (IsTriggerableEvent(event) ||
      (event.IsRightMouseButton() && !HitTestPoint(event.location()))) {
    ImageButton::OnMouseReleased(event);
  }

  if (IsTriggerableEvent(event))
    show_menu_factory_.InvalidateWeakPtrs();
}

std::string ButtonDropDown::GetClassName() const {
  return kViewClassName;
}

void ButtonDropDown::OnMouseExited(const ui::MouseEvent& event) {
  // Starting a drag results in a MouseExited, we need to ignore it.
  // A right click release triggers an exit event. We want to
  // remain in a PUSHED state until the drop down menu closes.
  if (state_ != STATE_DISABLED && !InDrag() && state_ != STATE_PRESSED)
    SetState(STATE_NORMAL);
}

void ButtonDropDown::OnGestureEvent(ui::GestureEvent* event) {
  if (menu_showing_) {
    // While dropdown menu is showing the button should not handle gestures.
    event->StopPropagation();
    return;
  }

  ImageButton::OnGestureEvent(event);
}

void ButtonDropDown::GetAccessibleState(ui::AccessibleViewState* state) {
  CustomButton::GetAccessibleState(state);
  state->role = ui::AccessibilityTypes::ROLE_BUTTONDROPDOWN;
  state->default_action = l10n_util::GetStringUTF16(IDS_APP_ACCACTION_PRESS);
  state->state = ui::AccessibilityTypes::STATE_HASPOPUP;
}

void ButtonDropDown::ShowContextMenuForView(View* source,
                                            const gfx::Point& point) {
  if (!enabled())
    return;

  show_menu_factory_.InvalidateWeakPtrs();
  ShowDropDownMenu();
}

bool ButtonDropDown::ShouldEnterPushedState(const ui::Event& event) {
  // Enter PUSHED state on press with Left or Right mouse button or on taps.
  // Remain in this state while the context menu is open.
  return event.type() == ui::ET_GESTURE_TAP ||
         event.type() == ui::ET_GESTURE_TAP_DOWN ||
         (event.IsMouseEvent() && ((ui::EF_LEFT_MOUSE_BUTTON |
             ui::EF_RIGHT_MOUSE_BUTTON) & event.flags()) != 0);
}

bool ButtonDropDown::ShouldShowMenu() {
  return true;
}

void ButtonDropDown::ShowDropDownMenu() {
  if (!ShouldShowMenu())
    return;

  gfx::Rect lb = GetLocalBounds();

  // Both the menu position and the menu anchor type change if the UI layout
  // is right-to-left.
  gfx::Point menu_position(lb.origin());
  menu_position.Offset(0, lb.height() - 1);
  if (base::i18n::IsRTL())
    menu_position.Offset(lb.width() - 1, 0);

  View::ConvertPointToScreen(this, &menu_position);

#if defined(OS_WIN)
  int left_bound = GetSystemMetrics(SM_XVIRTUALSCREEN);
#else
  int left_bound = 0;
  NOTIMPLEMENTED();
#endif
  if (menu_position.x() < left_bound)
    menu_position.set_x(left_bound);

  // Make the button look depressed while the menu is open.
  SetState(STATE_PRESSED);

  menu_showing_ = true;

  // Create and run menu.  Display an empty menu if model is NULL.
  if (model_.get()) {
    MenuModelAdapter menu_delegate(model_.get());
    menu_delegate.set_triggerable_event_flags(triggerable_event_flags());
    menu_runner_.reset(new MenuRunner(menu_delegate.CreateMenu()));
    MenuRunner::RunResult result =
        menu_runner_->RunMenuAt(GetWidget(), NULL,
                                gfx::Rect(menu_position, gfx::Size(0, 0)),
                                MenuItemView::TOPLEFT,
                                MenuRunner::HAS_MNEMONICS);
    if (result == MenuRunner::MENU_DELETED)
      return;
  } else {
    MenuDelegate menu_delegate;
    MenuItemView* menu = new MenuItemView(&menu_delegate);
    menu_runner_.reset(new MenuRunner(menu));
    MenuRunner::RunResult result =
        menu_runner_->RunMenuAt(GetWidget(), NULL,
                                gfx::Rect(menu_position, gfx::Size(0, 0)),
                                MenuItemView::TOPLEFT,
                                MenuRunner::HAS_MNEMONICS);
    if (result == MenuRunner::MENU_DELETED)
      return;
  }

  menu_showing_ = false;

  // Need to explicitly clear mouse handler so that events get sent
  // properly after the menu finishes running. If we don't do this, then
  // the first click to other parts of the UI is eaten.
  SetMouseHandler(NULL);

  // Set the state back to normal after the drop down menu is closed.
  if (state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
}

////////////////////////////////////////////////////////////////////////////////
//
// ButtonDropDown - Accessibility
//
////////////////////////////////////////////////////////////////////////////////

}  // namespace views
