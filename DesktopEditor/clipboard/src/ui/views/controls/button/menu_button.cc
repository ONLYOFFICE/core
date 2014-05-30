// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/menu_button.h"

#include "base/utf_string_conversions.h"
#include "grit/ui_resources.h"
#include "grit/ui_strings.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/screen.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/widget.h"

using base::Time;
using base::TimeDelta;

namespace views {

// The amount of time, in milliseconds, we wait before allowing another mouse
// pressed event to show the menu.
static const int64 kMinimumTimeBetweenButtonClicks = 100;

// Default menu offset.
static const int kDefaultMenuOffsetX = -2;
static const int kDefaultMenuOffsetY = -4;

// static
const int MenuButton::kMenuMarkerPaddingLeft = 3;
const int MenuButton::kMenuMarkerPaddingRight = -1;
const char MenuButton::kViewClassName[] = "views/MenuButton";

////////////////////////////////////////////////////////////////////////////////
//
// MenuButton - constructors, destructors, initialization
//
////////////////////////////////////////////////////////////////////////////////

MenuButton::MenuButton(ButtonListener* listener,
                       const string16& text,
                       MenuButtonListener* menu_button_listener,
                       bool show_menu_marker)
    : TextButton(listener, text),
      menu_visible_(false),
      menu_offset_(kDefaultMenuOffsetX, kDefaultMenuOffsetY),
      listener_(menu_button_listener),
      show_menu_marker_(show_menu_marker),
      menu_marker_(ui::ResourceBundle::GetSharedInstance().GetImageNamed(
          IDR_MENU_DROPARROW).ToImageSkia()),
      destroyed_flag_(NULL) {
  set_alignment(TextButton::ALIGN_LEFT);
}

MenuButton::~MenuButton() {
  if (destroyed_flag_)
    *destroyed_flag_ = true;
}

////////////////////////////////////////////////////////////////////////////////
//
// MenuButton - Public APIs
//
////////////////////////////////////////////////////////////////////////////////

bool MenuButton::Activate() {
  SetState(STATE_PRESSED);
  if (listener_) {
    gfx::Rect lb = GetLocalBounds();

    // The position of the menu depends on whether or not the locale is
    // right-to-left.
    gfx::Point menu_position(lb.right(), lb.bottom());
    if (base::i18n::IsRTL())
      menu_position.set_x(lb.x());

    View::ConvertPointToScreen(this, &menu_position);
    if (base::i18n::IsRTL())
      menu_position.Offset(-menu_offset_.x(), menu_offset_.y());
    else
      menu_position.Offset(menu_offset_.x(), menu_offset_.y());

    int max_x_coordinate = GetMaximumScreenXCoordinate();
    if (max_x_coordinate && max_x_coordinate <= menu_position.x())
      menu_position.set_x(max_x_coordinate - 1);

    // We're about to show the menu from a mouse press. By showing from the
    // mouse press event we block RootView in mouse dispatching. This also
    // appears to cause RootView to get a mouse pressed BEFORE the mouse
    // release is seen, which means RootView sends us another mouse press no
    // matter where the user pressed. To force RootView to recalculate the
    // mouse target during the mouse press we explicitly set the mouse handler
    // to NULL.
    static_cast<internal::RootView*>(GetWidget()->GetRootView())->
        SetMouseHandler(NULL);

    menu_visible_ = true;

    bool destroyed = false;
    destroyed_flag_ = &destroyed;

    listener_->OnMenuButtonClicked(this, menu_position);

    if (destroyed) {
      // The menu was deleted while showing. Don't attempt any processing.
      return false;
    }

    destroyed_flag_ = NULL;

    menu_visible_ = false;
    menu_closed_time_ = Time::Now();

    // Now that the menu has closed, we need to manually reset state to
    // "normal" since the menu modal loop will have prevented normal
    // mouse move messages from getting to this View. We set "normal"
    // and not "hot" because the likelihood is that the mouse is now
    // somewhere else (user clicked elsewhere on screen to close the menu
    // or selected an item) and we will inevitably refresh the hot state
    // in the event the mouse _is_ over the view.
    SetState(STATE_NORMAL);

    // We must return false here so that the RootView does not get stuck
    // sending all mouse pressed events to us instead of the appropriate
    // target.
    return false;
  }
  return true;
}

void MenuButton::PaintButton(gfx::Canvas* canvas, PaintButtonMode mode) {
  TextButton::PaintButton(canvas, mode);

  if (show_menu_marker_) {
    gfx::Insets insets = GetInsets();

    // We can not use the views' mirroring infrastructure for mirroring a
    // MenuButton control (see TextButton::OnPaint() for a detailed explanation
    // regarding why we can not flip the canvas). Therefore, we need to
    // manually mirror the position of the down arrow.
    gfx::Rect arrow_bounds(width() - insets.right() -
                           menu_marker_->width() - kMenuMarkerPaddingRight,
                           height() / 2 - menu_marker_->height() / 2,
                           menu_marker_->width(),
                           menu_marker_->height());
    arrow_bounds.set_x(GetMirroredXForRect(arrow_bounds));
    canvas->DrawImageInt(*menu_marker_, arrow_bounds.x(), arrow_bounds.y());
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// MenuButton - Events
//
////////////////////////////////////////////////////////////////////////////////

gfx::Size MenuButton::GetPreferredSize() {
  gfx::Size prefsize = TextButton::GetPreferredSize();
  if (show_menu_marker_) {
    prefsize.Enlarge(menu_marker_->width() + kMenuMarkerPaddingLeft +
                         kMenuMarkerPaddingRight,
                     0);
  }
  return prefsize;
}

std::string MenuButton::GetClassName() const {
  return kViewClassName;
}

bool MenuButton::OnMousePressed(const ui::MouseEvent& event) {
  RequestFocus();
  if (state() != STATE_DISABLED) {
    // If we're draggable (GetDragOperations returns a non-zero value), then
    // don't pop on press, instead wait for release.
    if (event.IsOnlyLeftMouseButton() &&
        HitTestPoint(event.location()) &&
        GetDragOperations(event.location()) == ui::DragDropTypes::DRAG_NONE) {
      TimeDelta delta = Time::Now() - menu_closed_time_;
      int64 delta_in_milliseconds = delta.InMilliseconds();
      if (delta_in_milliseconds > kMinimumTimeBetweenButtonClicks) {
        return Activate();
      }
    }
  }
  return true;
}

void MenuButton::OnMouseReleased(const ui::MouseEvent& event) {
  // Explicitly test for left mouse button to show the menu. If we tested for
  // !IsTriggerableEvent it could lead to a situation where we end up showing
  // the menu and context menu (this would happen if the right button is not
  // triggerable and there's a context menu).
  if (GetDragOperations(event.location()) != ui::DragDropTypes::DRAG_NONE &&
      state() != STATE_DISABLED && !InDrag() && event.IsOnlyLeftMouseButton() &&
      HitTestPoint(event.location())) {
    Activate();
  } else {
    TextButton::OnMouseReleased(event);
  }
}

// The reason we override View::OnMouseExited is because we get this event when
// we display the menu. If we don't override this method then
// BaseButton::OnMouseExited will get the event and will set the button's state
// to STATE_NORMAL instead of keeping the state BM_PUSHED. This, in turn, will
// cause the button to appear depressed while the menu is displayed.
void MenuButton::OnMouseExited(const ui::MouseEvent& event) {
  if ((state_ != STATE_DISABLED) && (!menu_visible_) && (!InDrag())) {
    SetState(STATE_NORMAL);
  }
}

void MenuButton::OnGestureEvent(ui::GestureEvent* event) {
  if (state() != STATE_DISABLED && event->type() == ui::ET_GESTURE_TAP) {
    if (Activate())
      event->StopPropagation();
    return;
  }
  TextButton::OnGestureEvent(event);
}

bool MenuButton::OnKeyPressed(const ui::KeyEvent& event) {
  switch (event.key_code()) {
    case ui::VKEY_SPACE:
      // Alt-space on windows should show the window menu.
      if (event.IsAltDown())
        break;
    case ui::VKEY_RETURN:
    case ui::VKEY_UP:
    case ui::VKEY_DOWN: {
      // WARNING: we may have been deleted by the time Activate returns.
      bool ret = Activate();
#if defined(USE_AURA)
      // This is to prevent the keyboard event from being dispatched twice.
      // The Activate function returns false in most cases. In AURA if the
      // keyboard event is not handled, we pass it to the default handler
      // which dispatches the event back to us causing the menu to get
      // displayed again.
      ret = true;
#endif
      return ret;
    }
    default:
      break;
  }
  return false;
}

bool MenuButton::OnKeyReleased(const ui::KeyEvent& event) {
  // Override CustomButton's implementation, which presses the button when
  // you press space and clicks it when you release space.  For a MenuButton
  // we always activate the menu on key press.
  return false;
}

void MenuButton::GetAccessibleState(ui::AccessibleViewState* state) {
  CustomButton::GetAccessibleState(state);
  state->role = ui::AccessibilityTypes::ROLE_BUTTONMENU;
  state->default_action = l10n_util::GetStringUTF16(IDS_APP_ACCACTION_PRESS);
  state->state = ui::AccessibilityTypes::STATE_HASPOPUP;
}

int MenuButton::GetMaximumScreenXCoordinate() {
  if (!GetWidget()) {
    NOTREACHED();
    return 0;
  }

  gfx::Rect monitor_bounds = GetWidget()->GetWorkAreaBoundsInScreen();
  return monitor_bounds.right() - 1;
}

}  // namespace views
