// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_WIN_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_WIN_H_

#include <windows.h>
#include <commctrl.h>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/tooltip_manager.h"

namespace gfx {
class Point;
}

namespace views {

class View;
class Widget;

// TooltipManager implementation for Windows.
//
// This class is intended to be used by NativeWidgetWin. To use this, you must
// do the following:
// Add the following to your MSG_MAP:
//
//   MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseRange)
//   MESSAGE_RANGE_HANDLER(WM_NCMOUSEMOVE, WM_NCMOUSEMOVE, OnMouseRange)
//   MSG_WM_NOTIFY(OnNotify)
//
// With the following implementations:
//   LRESULT XXX::OnMouseRange(UINT u_msg, WPARAM w_param, LPARAM l_param,
//                             BOOL& handled) {
//     tooltip_manager_->OnMouse(u_msg, w_param, l_param);
//     handled = FALSE;
//     return 0;
//   }
//
//   LRESULT XXX::OnNotify(int w_param, NMHDR* l_param) {
//     bool handled;
//     LRESULT result = tooltip_manager_->OnNotify(w_param, l_param, &handled);
//     SetMsgHandled(handled);
//     return result;
//   }
//
// And of course you'll need to create the TooltipManager!
//
// Lastly, you'll need to override GetTooltipManager.
//
// See NativeWidgetWin for an example of this in action.
class TooltipManagerWin : public TooltipManager {
 public:
  // Creates a TooltipManager for the specified Widget and parent window.
  explicit TooltipManagerWin(Widget* widget);
  virtual ~TooltipManagerWin();

  // Initializes the TooltipManager returning whether initialization was
  // successful. If this returns false the TooltipManager should be destroyed
  // and not used.
  bool Init();

  // Notification that the view hierarchy has changed in some way.
  virtual void UpdateTooltip() OVERRIDE;

  // Invoked when the tooltip text changes for the specified views.
  virtual void TooltipTextChanged(View* view) OVERRIDE;

  // Invoked when toolbar icon gets focus.
  virtual void ShowKeyboardTooltip(View* view) OVERRIDE;

  // Invoked when toolbar loses focus.
  virtual void HideKeyboardTooltip() OVERRIDE;

  // Message handlers. These forward to the tooltip control.
  virtual void OnMouse(UINT u_msg, WPARAM w_param, LPARAM l_param);
  LRESULT OnNotify(int w_param, NMHDR* l_param, bool* handled);

 protected:
  // Returns the Widget we're showing tooltips for.
  gfx::NativeView GetParent();

  // Updates the tooltip for the specified location.
  void UpdateTooltip(const gfx::Point& location);

  // Tooltip control window.
  HWND tooltip_hwnd_;

  // Tooltip information.
  TOOLINFO toolinfo_;

  // Last location of the mouse. This is in the coordinates of the rootview.
  gfx::Point last_mouse_pos_;

  // Whether or not the tooltip is showing.
  bool tooltip_showing_;

 private:
  // Sets the tooltip position based on the x/y position of the text. If the
  // tooltip fits, true is returned.
  bool SetTooltipPosition(int text_x, int text_y);

  // Calculates the preferred height for tooltips. This always returns a
  // positive value.
  int CalcTooltipHeight();

  // Invoked when the timer elapses and tooltip has to be destroyed.
  void DestroyKeyboardTooltipWindow(HWND window_to_destroy);

  // Hosting Widget.
  Widget* widget_;

  // The View the mouse is under. This is null if the mouse isn't under a
  // View.
  View* last_tooltip_view_;

  // Whether or not the view under the mouse needs to be refreshed. If this
  // is true, when the tooltip is asked for the view under the mouse is
  // refreshed.
  bool last_view_out_of_sync_;

  // Text for tooltip from the view.
  string16 tooltip_text_;

  // The clipped tooltip.
  string16 clipped_text_;

  // Number of lines in the tooltip.
  int line_count_;

  // Width of the last tooltip.
  int tooltip_width_;

  // control window for tooltip displayed using keyboard.
  HWND keyboard_tooltip_hwnd_;

  // Used to register DestroyTooltipWindow function with PostDelayedTask
  // function.
  base::WeakPtrFactory<TooltipManagerWin> keyboard_tooltip_factory_;

  DISALLOW_COPY_AND_ASSIGN(TooltipManagerWin);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_TOOLTIP_MANAGER_WIN_H_
