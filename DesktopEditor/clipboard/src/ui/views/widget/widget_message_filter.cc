// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/widget_message_filter.h"

#include "base/event_types.h"
#include "base/win/win_util.h"
#include "base/win/windows_version.h"
#include "ui/aura/root_window.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/gfx/canvas_skia_paint.h"
#include "ui/gfx/gdi_util.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/path.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_hwnd_utils.h"
#include "ui/views/window/non_client_view.h"

#include <atltypes.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <windowsx.h>

#pragma comment(lib, "dwmapi.lib")

namespace views {

// The thickness of an auto-hide taskbar in pixels.
static const int kAutoHideTaskbarThicknessPx = 2;
HWND GetTopmostAutoHideTaskbarForEdge(UINT edge, HMONITOR monitor) {
  APPBARDATA taskbar_data =  { sizeof(APPBARDATA), NULL, 0, edge };
  HWND taskbar = reinterpret_cast<HWND>(SHAppBarMessage(ABM_GETAUTOHIDEBAR,
                                                        &taskbar_data));
  return (::IsWindow(taskbar) && (monitor != NULL) &&
         (MonitorFromWindow(taskbar, MONITOR_DEFAULTTONULL) == monitor) &&
         (GetWindowLong(taskbar, GWL_EXSTYLE) & WS_EX_TOPMOST)) ?
      taskbar : NULL;
}

WidgetMessageFilter::WidgetMessageFilter(aura::RootWindow* root_window,
                                         Widget* widget)
    : root_window_(root_window),
      widget_(widget),
      lock_updates_count_(0),
      is_right_mouse_pressed_on_caption_(false),
      destroyed_(NULL) {
  hwnd_ = root_window_->GetAcceleratedWidget();
  ClientAreaSizeChanged();
}

WidgetMessageFilter::~WidgetMessageFilter() {
  if (destroyed_)
    *destroyed_ = true;
}

bool WidgetMessageFilter::FilterMessage(HWND hwnd,
                                        UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param,
                                        LRESULT* l_result) {
  switch (message) {
    case WM_NCCALCSIZE:
      return OnNCCalcSize(w_param, l_param, l_result);
    case WM_NCHITTEST:
      return OnNCHitTest(GET_X_LPARAM(l_param),
                         GET_Y_LPARAM(l_param),
                         l_result);
    case WM_NCPAINT:
      return OnNCPaint(reinterpret_cast<HRGN>(w_param));
    case WM_NCACTIVATE:
      return OnNCActivate(!!w_param, reinterpret_cast<HRGN>(l_param));
    case WM_ERASEBKGND:
      return true;
    case WM_SIZE:
      OnSize();
      return false;
    case WM_WINDOWPOSCHANGED:
      OnWindowPosChanged(reinterpret_cast<WINDOWPOS*>(l_param));
      return false;
  }
  return false;
}

bool WidgetMessageFilter::OnNCCalcSize(WPARAM w_param,
                                       LPARAM l_param,
                                       LRESULT* l_result) {
  gfx::Insets insets;
  if (!GetClientAreaInsets(&insets)) {
    *l_result = 0;
    return false;
  }

  RECT* client_rect = w_param ?
      &(reinterpret_cast<NCCALCSIZE_PARAMS*>(l_param)->rgrc[0]) :
      reinterpret_cast<RECT*>(l_param);
  client_rect->left += insets.left();
  client_rect->top += insets.top();
  client_rect->bottom -= insets.bottom();
  client_rect->right -= insets.right();

  if (IsZoomed(hwnd_)) {
    // Find all auto-hide taskbars along the screen edges and adjust in by the
    // thickness of the auto-hide taskbar on each such edge, so the window isn't
    // treated as a "fullscreen app", which would cause the taskbars to
    // disappear.
    HMONITOR monitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONULL);
    if (!monitor) {
      // We might end up here if the window was previously minimized and the
      // user clicks on the taskbar button to restore it in the previously
      // maximized position. In that case WM_NCCALCSIZE is sent before the
      // window coordinates are restored to their previous values, so our
      // (left,top) would probably be (-32000,-32000) like all minimized
      // windows. So the above MonitorFromWindow call fails, but if we check
      // the window rect given with WM_NCCALCSIZE (which is our previous
      // restored window position) we will get the correct monitor handle.
      monitor = MonitorFromRect(client_rect, MONITOR_DEFAULTTONULL);
      if (!monitor) {
        // This is probably an extreme case that we won't hit, but if we don't
        // intersect any monitor, let us not adjust the client rect since our
        // window will not be visible anyway.
        return 0;
      }
    }
    if (GetTopmostAutoHideTaskbarForEdge(ABE_LEFT, monitor))
      client_rect->left += kAutoHideTaskbarThicknessPx;
    if (GetTopmostAutoHideTaskbarForEdge(ABE_TOP, monitor)) {
      if (widget_->ShouldUseNativeFrame()) {
        // Tricky bit.  Due to a bug in DwmDefWindowProc()'s handling of
        // WM_NCHITTEST, having any nonclient area atop the window causes the
        // caption buttons to draw onscreen but not respond to mouse
        // hover/clicks.
        // So for a taskbar at the screen top, we can't push the
        // client_rect->top down; instead, we move the bottom up by one pixel,
        // which is the smallest change we can make and still get a client area
        // less than the screen size. This is visibly ugly, but there seems to
        // be no better solution.
        --client_rect->bottom;
      } else {
        client_rect->top += kAutoHideTaskbarThicknessPx;
      }
    }
    if (GetTopmostAutoHideTaskbarForEdge(ABE_RIGHT, monitor))
      client_rect->right -= kAutoHideTaskbarThicknessPx;
    if (GetTopmostAutoHideTaskbarForEdge(ABE_BOTTOM, monitor))
      client_rect->bottom -= kAutoHideTaskbarThicknessPx;

    // We cannot return WVR_REDRAW when there is nonclient area, or Windows
    // exhibits bugs where client pixels and child HWNDs are mispositioned by
    // the width/height of the upper-left nonclient area.
    *l_result = 0;
    return true;
  }


  if (insets.left() == 0 || insets.top() == 0)
    *l_result = 0;
  else
    *l_result = w_param ? WVR_REDRAW : 0;
  return true;
}

bool WidgetMessageFilter::OnNCHitTest(int x, int y, LRESULT* l_result) {
  POINT temp = { x, y };
  MapWindowPoints(HWND_DESKTOP, hwnd_, &temp, 1);
  int component = widget_->GetNonClientComponent(gfx::Point(temp));
  if (component != HTNOWHERE) {
    *l_result = component;
    return true;
  }
  return false;
}

bool WidgetMessageFilter::OnNCPaint(HRGN update_region) {
  CRect window_rect;
  GetWindowRect(hwnd_, &window_rect);

  CRect dirty_region;
  // A value of 1 indicates paint all.
  if (!update_region || update_region == reinterpret_cast<HRGN>(1)) {
    dirty_region = CRect(0, 0, window_rect.Width(), window_rect.Height());
  } else {
    RECT rgn_bounding_box;
    GetRgnBox(update_region, &rgn_bounding_box);
    if (!IntersectRect(&dirty_region, &rgn_bounding_box, &window_rect))
      return true;

    // rgn_bounding_box is in screen coordinates. Map it to window coordinates.
    OffsetRect(&dirty_region, -window_rect.left, -window_rect.top);
  }

  HDC dc = GetWindowDC(hwnd_);

  widget_->SchedulePaintInRect(gfx::Rect(dirty_region));

  // gfx::CanvasSkiaPaint's destructor does the actual painting. As such, wrap
  // the following in a block to force paint to occur so that we can release
  // the dc.
  {
    gfx::CanvasSkiaPaint canvas(dc, true, dirty_region.left,
                                dirty_region.top, dirty_region.Width(),
                                dirty_region.Height());
  }

  ReleaseDC(hwnd_, dc);
  return true;
}

bool WidgetMessageFilter::OnNCActivate(bool active, HRGN update_region) {
  if (!widget_->non_client_view())
    return false;

  if (!widget_->ShouldUseNativeFrame()) {
    RedrawWindow(hwnd_, NULL, NULL,
                 RDW_NOCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW);
  }

  return true;
}

void WidgetMessageFilter::OnSize() {
  RedrawWindow(hwnd_, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
  // ResetWindowRegion is going to trigger WM_NCPAINT. By doing it after we've
  // invoked OnSize we ensure the RootView has been laid out.
  ResetWindowRegion(false);
}

void WidgetMessageFilter::OnWindowPosChanged(WINDOWPOS* window_pos) {
  if (DidClientAreaSizeChange(window_pos))
    ClientAreaSizeChanged();
}

HWND WidgetMessageFilter::hwnd() const {
  return hwnd_;
}

bool WidgetMessageFilter::IsVisible() const {
  return !!::IsWindowVisible(hwnd_);
}

bool WidgetMessageFilter::HasCapture() const {
  return ::GetCapture() == hwnd_;
}

bool WidgetMessageFilter::GetClientAreaInsets(gfx::Insets* insets) const {
  // TODO(beng): native frame
  *insets = gfx::Insets(0, 0, 0, 0);
  return true;
}

bool WidgetMessageFilter::WidgetSizeIsClientSize() const {
  return IsZoomed(hwnd_) || widget_->ShouldUseNativeFrame();
}

void WidgetMessageFilter::ClientAreaSizeChanged() {
  RECT r;
  if (WidgetSizeIsClientSize())
    GetClientRect(hwnd_, &r);
  else
    GetWindowRect(hwnd_, &r);
  gfx::Size s(std::max(0, static_cast<int>(r.right - r.left)),
              std::max(0, static_cast<int>(r.bottom - r.top)));
  widget_->OnNativeWidgetSizeChanged(s);
}

void WidgetMessageFilter::ResetWindowRegion(bool force) {
  // A native frame uses the native window region, and we don't want to mess
  // with it.
  if (widget_->ShouldUseNativeFrame() || !widget_->non_client_view()) {
    if (force)
      SetWindowRgn(hwnd_, NULL, TRUE);
    return;
  }

  // Changing the window region is going to force a paint. Only change the
  // window region if the region really differs.
  HRGN current_rgn = CreateRectRgn(0, 0, 0, 0);
  int current_rgn_result = GetWindowRgn(hwnd_, current_rgn);

  CRect window_rect;
  GetWindowRect(hwnd_, &window_rect);
  HRGN new_region;
  if (IsZoomed(hwnd_)) {
    HMONITOR monitor =
        MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof mi;
    GetMonitorInfo(monitor, &mi);
    CRect work_rect = mi.rcWork;
    work_rect.OffsetRect(-window_rect.left, -window_rect.top);
    new_region = CreateRectRgnIndirect(&work_rect);
  } else {
    gfx::Path window_mask;
    widget_->non_client_view()->GetWindowMask(
        gfx::Size(window_rect.Width(), window_rect.Height()), &window_mask);
    new_region = ConvertPathToHRGN(window_mask);
  }

  if (current_rgn_result == ERROR || !EqualRgn(current_rgn, new_region)) {
    // SetWindowRgn takes ownership of the HRGN created by CreateNativeRegion.
    SetWindowRgn(hwnd_, new_region, TRUE);
  } else {
    DeleteObject(new_region);
  }

  DeleteObject(current_rgn);
}

}  // namespace views
