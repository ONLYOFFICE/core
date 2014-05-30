// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/native_control_win.h"

#include <windowsx.h>

#include "base/logging.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/l10n/l10n_util_win.h"
#include "ui/base/view_prop.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/widget/widget.h"

using ui::ViewProp;

const char kNativeControlWinKey[] = "__NATIVE_CONTROL_WIN__";

namespace views {

////////////////////////////////////////////////////////////////////////////////
// NativeControlWin, public:

NativeControlWin::NativeControlWin() : original_wndproc_(NULL) {
}

NativeControlWin::~NativeControlWin() {
  HWND hwnd = native_view();
  if (hwnd) {
    // Destroy the hwnd if it still exists. Otherwise we won't have shut things
    // down correctly, leading to leaking and crashing if another message
    // comes in for the hwnd.
    Detach();
    DestroyWindow(hwnd);
  }
}

bool NativeControlWin::ProcessMessage(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param,
                                      LRESULT* result) {
  switch (message) {
    case WM_CONTEXTMENU:
      ShowContextMenu(gfx::Point(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param)));
      *result = 0;
      return true;
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
      *result = GetControlColor(message, reinterpret_cast<HDC>(w_param),
                                native_view());
      return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
// NativeControlWin, View overrides:

void NativeControlWin::OnEnabledChanged() {
  View::OnEnabledChanged();
  if (native_view())
    EnableWindow(native_view(), enabled());
}

void NativeControlWin::ViewHierarchyChanged(bool is_add, View* parent,
                                            View* child) {
  // Call the base class to hide the view if we're being removed.
  NativeViewHost::ViewHierarchyChanged(is_add, parent, child);

  // Create the HWND when we're added to a valid Widget. Many controls need a
  // parent HWND to function properly.
  if (is_add && GetWidget() && !native_view())
    CreateNativeControl();
}

void NativeControlWin::VisibilityChanged(View* starting_from, bool is_visible) {
  // We might get called due to visibility changes at any point in the
  // hierarchy, lets check whether we are really visible or not.
  bool is_drawn = IsDrawn();
  if (!is_drawn && native_view()) {
    // We destroy the child control HWND when we become invisible because of the
    // performance cost of maintaining many HWNDs.
    HWND hwnd = native_view();
    Detach();
    DestroyWindow(hwnd);
  } else if (is_drawn && !native_view()) {
    if (GetWidget())
      CreateNativeControl();
  }
  if (is_drawn) {
    // The view becomes visible after native control is created.
    // Layout now.
    Layout();
  }
}

void NativeControlWin::OnFocus() {
  DCHECK(native_view());
  SetFocus(native_view());

  // Since we are being wrapped by a view, accessibility should receive
  // the super class as the focused view.
  View* parent_view = parent();

  // Due to some controls not behaving as expected without having
  // a native win32 control, we don't always send a native (MSAA)
  // focus notification.
  bool send_native_event =
      parent_view->GetClassName() != Combobox::kViewClassName &&
      parent_view->HasFocus();

  // Send the accessibility focus notification.
  parent_view->NotifyAccessibilityEvent(
      ui::AccessibilityTypes::EVENT_FOCUS, send_native_event);
}

////////////////////////////////////////////////////////////////////////////////
// NativeControlWin, protected:

void NativeControlWin::ShowContextMenu(const gfx::Point& location) {
  if (!context_menu_controller())
    return;

  if (location.x() == -1 && location.y() == -1)
    View::ShowContextMenu(GetKeyboardContextMenuLocation(), false);
  else
    View::ShowContextMenu(location, true);
}

void NativeControlWin::NativeControlCreated(HWND native_control) {
  // Associate this object with the control's HWND so that NativeWidgetWin can
  // find this object when it receives messages from it.
  props_.push_back(new ViewProp(native_control, kNativeControlWinKey, this));
  props_.push_back(ChildWindowMessageProcessor::Register(native_control, this));

  // Subclass so we get WM_KEYDOWN and WM_SETFOCUS messages.
  original_wndproc_ = ui::SetWindowProc(
      native_control, &NativeControlWin::NativeControlWndProc);

  Attach(native_control);
  // native_view() is now valid.

  // Update the newly created HWND with any resident enabled state.
  EnableWindow(native_view(), enabled());

  // This message ensures that the focus border is shown.
  SendMessage(native_view(), WM_CHANGEUISTATE,
              MAKEWPARAM(UIS_CLEAR, UISF_HIDEFOCUS), 0);
}

DWORD NativeControlWin::GetAdditionalExStyle() const {
  // If the UI for the view is mirrored, we should make sure we add the
  // extended window style for a right-to-left layout so the subclass creates
  // a mirrored HWND for the underlying control.
  DWORD ex_style = 0;
  if (base::i18n::IsRTL())
    ex_style |= l10n_util::GetExtendedStyles();

  return ex_style;
}

DWORD NativeControlWin::GetAdditionalRTLStyle() const {
  // If the UI for the view is mirrored, we should make sure we add the
  // extended window style for a right-to-left layout so the subclass creates
  // a mirrored HWND for the underlying control.
  DWORD ex_style = 0;
  if (base::i18n::IsRTL())
    ex_style |= l10n_util::GetExtendedTooltipStyles();

  return ex_style;
}

////////////////////////////////////////////////////////////////////////////////
// NativeControlWin, private:

LRESULT NativeControlWin::GetControlColor(UINT message, HDC dc, HWND sender) {
  View *ancestor = this;
  while (ancestor) {
    const Background* background = ancestor->background();
    if (background) {
      HBRUSH brush = background->GetNativeControlBrush();
      if (brush)
        return reinterpret_cast<LRESULT>(brush);
    }
    ancestor = ancestor->parent();
  }

  // COLOR_BTNFACE is the default for dialog box backgrounds.
  return reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_BTNFACE));
}

// static
LRESULT NativeControlWin::NativeControlWndProc(HWND window,
                                               UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param) {
  NativeControlWin* native_control = reinterpret_cast<NativeControlWin*>(
      ViewProp::GetValue(window, kNativeControlWinKey));
  DCHECK(native_control);

  if (message == WM_KEYDOWN &&
      native_control->OnKeyDown(static_cast<int>(w_param))) {
      return 0;
  } else if (message == WM_SETFOCUS) {
    // Let the focus manager know that the focus changed.
    FocusManager* focus_manager = native_control->GetFocusManager();
    if (focus_manager) {
      focus_manager->SetFocusedView(native_control->focus_view());
    } else {
      NOTREACHED();
    }
  } else if (message == WM_DESTROY) {
    native_control->props_.clear();
    ui::SetWindowProc(window, native_control->original_wndproc_);
  }

  return CallWindowProc(native_control->original_wndproc_, window, message,
                        w_param, l_param);
}

}  // namespace views
