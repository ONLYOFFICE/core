// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/tooltip_manager_win.h"

#include <windowsx.h>

#include <limits>

#include "base/bind.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/message_loop.h"
#include "base/string_util.h"
#include "base/win/scoped_hdc.h"
#include "base/win/scoped_select_object.h"
#include "ui/base/l10n/l10n_util_win.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/base/win/scoped_set_map_mode.h"
#include "ui/gfx/font.h"
#include "ui/gfx/screen.h"
#include "ui/views/view.h"
#include "ui/views/widget/monitor_win.h"
#include "ui/views/widget/widget.h"

namespace views {

static int tooltip_height_ = 0;

// Default timeout for the tooltip displayed using keyboard.
// Timeout is mentioned in milliseconds.
static const int kDefaultTimeout = 4000;

// static
int TooltipManager::GetTooltipHeight() {
  DCHECK_GT(tooltip_height_, 0);
  return tooltip_height_;
}

static gfx::Font DetermineDefaultFont() {
  HWND window = CreateWindowEx(
      WS_EX_TRANSPARENT | l10n_util::GetExtendedTooltipStyles(),
      TOOLTIPS_CLASS, NULL, 0 , 0, 0, 0, 0, NULL, NULL, NULL, NULL);
  if (!window)
    return gfx::Font();
  HFONT hfont = reinterpret_cast<HFONT>(SendMessage(window, WM_GETFONT, 0, 0));
  gfx::Font font = hfont ? gfx::Font(hfont) : gfx::Font();
  DestroyWindow(window);
  return font;
}

// static
gfx::Font TooltipManager::GetDefaultFont() {
  static gfx::Font* font = NULL;
  if (!font)
    font = new gfx::Font(DetermineDefaultFont());
  return *font;
}

// static
int TooltipManager::GetMaxWidth(int x, int y, gfx::NativeView context) {
  gfx::Rect monitor_bounds =
      gfx::Screen::GetScreenFor(context)->GetDisplayNearestPoint(
          gfx::Point(x, y)).bounds();
  // Allow the tooltip to be almost as wide as the screen.
  // Otherwise, we would truncate important text, since we're not word-wrapping
  // the text onto multiple lines.
  return monitor_bounds.width() == 0 ? 800 : monitor_bounds.width() - 30;
}

TooltipManagerWin::TooltipManagerWin(Widget* widget)
    : widget_(widget),
      tooltip_hwnd_(NULL),
      last_mouse_pos_(-1, -1),
      tooltip_showing_(false),
      last_tooltip_view_(NULL),
      last_view_out_of_sync_(false),
      tooltip_width_(0),
      keyboard_tooltip_hwnd_(NULL),
      keyboard_tooltip_factory_(this) {
  DCHECK(widget);
  DCHECK(widget->GetNativeView());
}

TooltipManagerWin::~TooltipManagerWin() {
  if (tooltip_hwnd_)
    DestroyWindow(tooltip_hwnd_);
  if (keyboard_tooltip_hwnd_)
    DestroyWindow(keyboard_tooltip_hwnd_);
}

bool TooltipManagerWin::Init() {
  DCHECK(!tooltip_hwnd_);
  // Create the tooltip control.
  tooltip_hwnd_ = CreateWindowEx(
      WS_EX_TRANSPARENT | l10n_util::GetExtendedTooltipStyles(),
      TOOLTIPS_CLASS, NULL, TTS_NOPREFIX, 0, 0, 0, 0,
      GetParent(), NULL, NULL, NULL);
  if (!tooltip_hwnd_)
    return false;

  l10n_util::AdjustUIFontForWindow(tooltip_hwnd_);

  // This effectively turns off clipping of tooltips. We need this otherwise
  // multi-line text (\r\n) won't work right. The size doesn't really matter
  // (just as long as its bigger than the monitor's width) as we clip to the
  // screen size before rendering.
  SendMessage(tooltip_hwnd_, TTM_SETMAXTIPWIDTH, 0,
              std::numeric_limits<int16>::max());

  // Add one tool that is used for all tooltips.
  toolinfo_.cbSize = sizeof(toolinfo_);
  toolinfo_.uFlags = TTF_TRANSPARENT | TTF_IDISHWND;
  toolinfo_.hwnd = GetParent();
  toolinfo_.uId = reinterpret_cast<UINT_PTR>(GetParent());
  // Setting this tells windows to call GetParent() back (using a WM_NOTIFY
  // message) for the actual tooltip contents.
  toolinfo_.lpszText = LPSTR_TEXTCALLBACK;
  toolinfo_.lpReserved = NULL;
  SetRectEmpty(&toolinfo_.rect);
  SendMessage(tooltip_hwnd_, TTM_ADDTOOL, 0, (LPARAM)&toolinfo_);
  return true;
}

gfx::NativeView TooltipManagerWin::GetParent() {
  return widget_->GetNativeView();
}

void TooltipManagerWin::UpdateTooltip() {
  // Set last_view_out_of_sync_ to indicate the view is currently out of sync.
  // This doesn't update the view under the mouse immediately as it may cause
  // timing problems.
  last_view_out_of_sync_ = true;
  last_tooltip_view_ = NULL;
  // Hide the tooltip.
  SendMessage(tooltip_hwnd_, TTM_POP, 0, 0);
}

void TooltipManagerWin::TooltipTextChanged(View* view) {
  if (view == last_tooltip_view_)
    UpdateTooltip(last_mouse_pos_);
}

LRESULT TooltipManagerWin::OnNotify(int w_param,
                                    NMHDR* l_param,
                                    bool* handled) {
  *handled = false;
  if (l_param->hwndFrom == tooltip_hwnd_ && keyboard_tooltip_hwnd_ == NULL) {
    switch (l_param->code) {
      case TTN_GETDISPINFO: {
        if (last_view_out_of_sync_) {
          // View under the mouse is out of sync, determine it now.
          View* root_view = widget_->GetRootView();
          last_tooltip_view_ =
              root_view->GetTooltipHandlerForPoint(last_mouse_pos_);
          last_view_out_of_sync_ = false;
        }
        // Tooltip control is asking for the tooltip to display.
        NMTTDISPINFOW* tooltip_info =
            reinterpret_cast<NMTTDISPINFOW*>(l_param);
        // Initialize the string, if we have a valid tooltip the string will
        // get reset below.
        tooltip_info->szText[0] = TEXT('\0');
        tooltip_text_.clear();
        tooltip_info->lpszText = NULL;
        clipped_text_.clear();
        if (last_tooltip_view_ != NULL) {
          tooltip_text_.clear();
          // Mouse is over a View, ask the View for its tooltip.
          gfx::Point view_loc = last_mouse_pos_;
          View::ConvertPointToTarget(widget_->GetRootView(),
                                     last_tooltip_view_, &view_loc);
          if (last_tooltip_view_->GetTooltipText(view_loc, &tooltip_text_) &&
              !tooltip_text_.empty()) {
            // View has a valid tip, copy it into TOOLTIPINFO.
            clipped_text_ = tooltip_text_;
            gfx::Point screen_loc = last_mouse_pos_;
            View::ConvertPointToScreen(widget_->GetRootView(), &screen_loc);
            TrimTooltipToFit(&clipped_text_, &tooltip_width_, &line_count_,
                             screen_loc.x(), screen_loc.y(),
                             widget_->GetNativeView());
            // Adjust the clipped tooltip text for locale direction.
            base::i18n::AdjustStringForLocaleDirection(&clipped_text_);
            tooltip_info->lpszText = const_cast<WCHAR*>(clipped_text_.c_str());
          } else {
            tooltip_text_.clear();
          }
        }
        *handled = true;
        return 0;
      }
      case TTN_POP:
        tooltip_showing_ = false;
        *handled = true;
        return 0;
      case TTN_SHOW: {
        *handled = true;
        tooltip_showing_ = true;
        // The tooltip is about to show, allow the view to position it
        gfx::Point text_origin;
        if (tooltip_height_ == 0)
          tooltip_height_ = CalcTooltipHeight();
        gfx::Point view_loc = last_mouse_pos_;
        View::ConvertPointToTarget(widget_->GetRootView(),
                                   last_tooltip_view_, &view_loc);
        if (last_tooltip_view_->GetTooltipTextOrigin(view_loc, &text_origin) &&
            SetTooltipPosition(text_origin.x(), text_origin.y())) {
          // Return true, otherwise the rectangle we specified is ignored.
          return TRUE;
        }
        return 0;
      }
      default:
        // Fall through.
        break;
    }
  }
  return 0;
}

bool TooltipManagerWin::SetTooltipPosition(int text_x, int text_y) {
  // NOTE: this really only tests that the y location fits on screen, but that
  // is good enough for our usage.

  // Calculate the bounds the tooltip will get.
  gfx::Point view_loc;
  View::ConvertPointToScreen(last_tooltip_view_, &view_loc);
  RECT bounds = { view_loc.x() + text_x,
                  view_loc.y() + text_y,
                  view_loc.x() + text_x + tooltip_width_,
                  view_loc.y() + line_count_ * GetTooltipHeight() };
  SendMessage(tooltip_hwnd_, TTM_ADJUSTRECT, TRUE, (LPARAM)&bounds);

  // Make sure the rectangle completely fits on the current monitor. If it
  // doesn't, return false so that windows positions the tooltip at the
  // default location.
  gfx::Rect monitor_bounds =
      views::GetMonitorBoundsForRect(gfx::Rect(bounds.left, bounds.right,
                                                  0, 0));
  if (!monitor_bounds.Contains(gfx::Rect(bounds))) {
    return false;
  }

  ::SetWindowPos(tooltip_hwnd_, NULL, bounds.left, bounds.top, 0, 0,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
  return true;
}

int TooltipManagerWin::CalcTooltipHeight() {
  // Ask the tooltip for its font.
  int height;
  HFONT hfont = reinterpret_cast<HFONT>(
      SendMessage(tooltip_hwnd_, WM_GETFONT, 0, 0));
  if (hfont != NULL) {
    base::win::ScopedGetDC dc(tooltip_hwnd_);
    base::win::ScopedSelectObject font(dc, hfont);
    ui::ScopedSetMapMode mode(dc, MM_TEXT);
    TEXTMETRIC font_metrics;
    GetTextMetrics(dc, &font_metrics);
    height = font_metrics.tmHeight;
  } else {
    // Tooltip is using the system font. Use gfx::Font, which should pick
    // up the system font.
    height = gfx::Font().GetHeight();
  }
  // Get the margins from the tooltip
  RECT tooltip_margin;
  SendMessage(tooltip_hwnd_, TTM_GETMARGIN, 0, (LPARAM)&tooltip_margin);
  return height + tooltip_margin.top + tooltip_margin.bottom;
}

void TooltipManagerWin::UpdateTooltip(const gfx::Point& mouse_pos) {
  View* root_view = widget_->GetRootView();
  View* view = root_view->GetTooltipHandlerForPoint(mouse_pos);
  if (view != last_tooltip_view_) {
    // NOTE: This *must* be sent regardless of the visibility of the tooltip.
    // It triggers Windows to ask for the tooltip again.
    SendMessage(tooltip_hwnd_, TTM_POP, 0, 0);
    last_tooltip_view_ = view;
  } else if (last_tooltip_view_ != NULL) {
    // Tooltip is showing, and mouse is over the same view. See if the tooltip
    // text has changed.
    gfx::Point view_point = mouse_pos;
    View::ConvertPointToTarget(root_view, last_tooltip_view_, &view_point);
    string16 new_tooltip_text;
    bool has_tooltip_text =
        last_tooltip_view_->GetTooltipText(view_point, &new_tooltip_text);
    if (!has_tooltip_text || (new_tooltip_text != tooltip_text_)) {
      // The text has changed, hide the popup.
      SendMessage(tooltip_hwnd_, TTM_POP, 0, 0);
      if (has_tooltip_text && !new_tooltip_text.empty() && tooltip_showing_) {
        // New text is valid, show the popup.
        SendMessage(tooltip_hwnd_, TTM_POPUP, 0, 0);
      }
    }
  }
}

void TooltipManagerWin::OnMouse(UINT u_msg, WPARAM w_param, LPARAM l_param) {
  gfx::Point mouse_pos(l_param);

  if (u_msg >= WM_NCMOUSEMOVE && u_msg <= WM_NCXBUTTONDBLCLK) {
    // NC message coordinates are in screen coordinates.
    POINT temp = mouse_pos.ToPOINT();
    ::MapWindowPoints(HWND_DESKTOP, GetParent(), &temp, 1);
    mouse_pos.SetPoint(temp.x, temp.y);
  }

  if (u_msg != WM_MOUSEMOVE || last_mouse_pos_ != mouse_pos) {
    last_mouse_pos_ = mouse_pos;
    HideKeyboardTooltip();
    UpdateTooltip(mouse_pos);
  }
  // Forward the message onto the tooltip.
  MSG msg;
  msg.hwnd = GetParent();
  msg.message = u_msg;
  msg.wParam = w_param;
  msg.lParam = l_param;
  SendMessage(tooltip_hwnd_, TTM_RELAYEVENT, 0, (LPARAM)&msg);
}

void TooltipManagerWin::ShowKeyboardTooltip(View* focused_view) {
  if (tooltip_showing_) {
    SendMessage(tooltip_hwnd_, TTM_POP, 0, 0);
    tooltip_text_.clear();
  }
  HideKeyboardTooltip();
  string16 tooltip_text;
  if (!focused_view->GetTooltipText(gfx::Point(), &tooltip_text))
    return;
  gfx::Rect focused_bounds = focused_view->bounds();
  gfx::Point screen_point;
  focused_view->ConvertPointToScreen(focused_view, &screen_point);
  keyboard_tooltip_hwnd_ = CreateWindowEx(
      WS_EX_TRANSPARENT | l10n_util::GetExtendedTooltipStyles(),
      TOOLTIPS_CLASS, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
  if (!keyboard_tooltip_hwnd_)
    return;

  SendMessage(keyboard_tooltip_hwnd_, TTM_SETMAXTIPWIDTH, 0,
              std::numeric_limits<int16>::max());
  int tooltip_width;
  int line_count;
  TrimTooltipToFit(&tooltip_text, &tooltip_width, &line_count,
                   screen_point.x(), screen_point.y(),
                   widget_->GetNativeView());
  ReplaceSubstringsAfterOffset(&tooltip_text, 0, L"\n", L"\r\n");
  TOOLINFO keyboard_toolinfo;
  memset(&keyboard_toolinfo, 0, sizeof(keyboard_toolinfo));
  keyboard_toolinfo.cbSize = sizeof(keyboard_toolinfo);
  keyboard_toolinfo.hwnd = GetParent();
  keyboard_toolinfo.uFlags = TTF_TRACK | TTF_TRANSPARENT | TTF_IDISHWND;
  keyboard_toolinfo.lpszText = const_cast<WCHAR*>(tooltip_text.c_str());
  SendMessage(keyboard_tooltip_hwnd_, TTM_ADDTOOL, 0,
              reinterpret_cast<LPARAM>(&keyboard_toolinfo));
  SendMessage(keyboard_tooltip_hwnd_, TTM_TRACKACTIVATE,  TRUE,
              reinterpret_cast<LPARAM>(&keyboard_toolinfo));
  if (!tooltip_height_)
    tooltip_height_ = CalcTooltipHeight();
  RECT rect_bounds = {screen_point.x(),
                      screen_point.y() + focused_bounds.height(),
                      screen_point.x() + tooltip_width,
                      screen_point.y() + focused_bounds.height() +
                      line_count * tooltip_height_ };
  gfx::Rect monitor_bounds =
      views::GetMonitorBoundsForRect(gfx::Rect(rect_bounds));
  gfx::Rect fitted_bounds = gfx::Rect(rect_bounds);
  fitted_bounds.AdjustToFit(monitor_bounds);
  rect_bounds = fitted_bounds.ToRECT();
  ::SetWindowPos(keyboard_tooltip_hwnd_, NULL, rect_bounds.left,
                 rect_bounds.top, 0, 0,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&TooltipManagerWin::DestroyKeyboardTooltipWindow,
                 keyboard_tooltip_factory_.GetWeakPtr(),
                 keyboard_tooltip_hwnd_),
      base::TimeDelta::FromMilliseconds(kDefaultTimeout));
}

void TooltipManagerWin::HideKeyboardTooltip() {
  if (keyboard_tooltip_hwnd_ != NULL) {
    SendMessage(keyboard_tooltip_hwnd_, WM_CLOSE, 0, 0);
    keyboard_tooltip_hwnd_ = NULL;
  }
}

void TooltipManagerWin::DestroyKeyboardTooltipWindow(HWND window_to_destroy) {
  if (keyboard_tooltip_hwnd_ == window_to_destroy)
    HideKeyboardTooltip();
}

}  // namespace views
