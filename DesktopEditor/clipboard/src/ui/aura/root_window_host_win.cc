// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/root_window_host_win.h"

#include <windows.h>

#include <algorithm>

#include "base/message_loop.h"
#include "ui/aura/client/capture_client.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/root_window.h"
#include "ui/base/cursor/cursor_loader_win.h"
#include "ui/base/events/event.h"
#include "ui/base/view_prop.h"
#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/screen.h"

using std::max;
using std::min;

namespace aura {
namespace {

bool use_popup_as_root_window_for_test = false;

}  // namespace

// static
RootWindowHost* RootWindowHost::Create(const gfx::Rect& bounds) {
  return new RootWindowHostWin(bounds);
}

// static
gfx::Size RootWindowHost::GetNativeScreenSize() {
  return gfx::Size(GetSystemMetrics(SM_CXSCREEN),
                   GetSystemMetrics(SM_CYSCREEN));
}

RootWindowHostWin::RootWindowHostWin(const gfx::Rect& bounds)
    : delegate_(NULL),
      fullscreen_(false),
      has_capture_(false),
      saved_window_style_(0),
      saved_window_ex_style_(0) {
  if (use_popup_as_root_window_for_test)
    set_window_style(WS_POPUP);
  Init(NULL, bounds);
  SetWindowText(hwnd(), L"aura::RootWindow!");
}

RootWindowHostWin::~RootWindowHostWin() {
  DestroyWindow(hwnd());
}

void RootWindowHostWin::SetDelegate(RootWindowHostDelegate* delegate) {
  delegate_ = delegate;
}

RootWindow* RootWindowHostWin::GetRootWindow() {
  return delegate_->AsRootWindow();
}

gfx::AcceleratedWidget RootWindowHostWin::GetAcceleratedWidget() {
  return hwnd();
}

void RootWindowHostWin::Show() {
  ShowWindow(hwnd(), SW_SHOWNORMAL);
}

void RootWindowHostWin::Hide() {
  NOTIMPLEMENTED();
}

void RootWindowHostWin::ToggleFullScreen() {
  gfx::Rect target_rect;
  if (!fullscreen_) {
    fullscreen_ = true;
    saved_window_style_ = GetWindowLong(hwnd(), GWL_STYLE);
    saved_window_ex_style_ = GetWindowLong(hwnd(), GWL_EXSTYLE);
    GetWindowRect(hwnd(), &saved_window_rect_);
    SetWindowLong(hwnd(), GWL_STYLE,
                  saved_window_style_ & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowLong(hwnd(), GWL_EXSTYLE,
                  saved_window_ex_style_ & ~(WS_EX_DLGMODALFRAME |
                      WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi);
    target_rect = gfx::Rect(mi.rcMonitor);
  } else {
    fullscreen_ = false;
    SetWindowLong(hwnd(), GWL_STYLE, saved_window_style_);
    SetWindowLong(hwnd(), GWL_EXSTYLE, saved_window_ex_style_);
    target_rect = gfx::Rect(saved_window_rect_);
  }
  SetWindowPos(hwnd(),
               NULL,
               target_rect.x(),
               target_rect.y(),
               target_rect.width(),
               target_rect.height(),
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}

gfx::Rect RootWindowHostWin::GetBounds() const {
  RECT r;
  GetClientRect(hwnd(), &r);
  return gfx::Rect(r);
}

void RootWindowHostWin::SetBounds(const gfx::Rect& bounds) {
  if (fullscreen_) {
    saved_window_rect_.right = saved_window_rect_.left + bounds.width();
    saved_window_rect_.bottom = saved_window_rect_.top + bounds.height();
    return;
  }
  RECT window_rect;
  window_rect.left = bounds.x();
  window_rect.top = bounds.y();
  window_rect.right = bounds.right() ;
  window_rect.bottom = bounds.bottom();
  AdjustWindowRectEx(&window_rect,
                     GetWindowLong(hwnd(), GWL_STYLE),
                     FALSE,
                     GetWindowLong(hwnd(), GWL_EXSTYLE));
  SetWindowPos(
      hwnd(),
      NULL,
      window_rect.left,
      window_rect.top,
      window_rect.right - window_rect.left,
      window_rect.bottom - window_rect.top,
      SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOREPOSITION);

  // Explicity call OnHostResized when the scale has changed because
  // the window size may not have changed.
  float current_scale = delegate_->GetDeviceScaleFactor();
  float new_scale = gfx::Screen::GetScreenFor(delegate_->AsRootWindow())->
      GetDisplayNearestWindow(delegate_->AsRootWindow()).device_scale_factor();
  if (current_scale != new_scale)
    delegate_->OnHostResized(bounds.size());
}

gfx::Insets RootWindowHostWin::GetInsets() const {
  return gfx::Insets();
}

void RootWindowHostWin::SetInsets(const gfx::Insets& insets) {
}

gfx::Point RootWindowHostWin::GetLocationOnNativeScreen() const {
  RECT r;
  GetClientRect(hwnd(), &r);
  return gfx::Point(r.left, r.top);
}


void RootWindowHostWin::SetCursor(gfx::NativeCursor native_cursor) {
  // Custom web cursors are handled directly.
  if (native_cursor == ui::kCursorCustom)
    return;

  ui::CursorLoaderWin cursor_loader;
  cursor_loader.SetPlatformCursor(&native_cursor);
  ::SetCursor(native_cursor.platform());
}

void RootWindowHostWin::SetCapture() {
  if (!has_capture_) {
    has_capture_ = true;
    ::SetCapture(hwnd());
  }
}

void RootWindowHostWin::ReleaseCapture() {
  if (has_capture_) {
    has_capture_ = false;
    ::ReleaseCapture();
  }
}

bool RootWindowHostWin::QueryMouseLocation(gfx::Point* location_return) {
  client::CursorClient* cursor_client =
      client::GetCursorClient(GetRootWindow());
  if (cursor_client && !cursor_client->IsMouseEventsEnabled()) {
    *location_return = gfx::Point(0, 0);
    return false;
  }

  POINT pt;
  GetCursorPos(&pt);
  ScreenToClient(hwnd(), &pt);
  const gfx::Size size = GetBounds().size();
  *location_return =
      gfx::Point(max(0, min(size.width(), static_cast<int>(pt.x))),
                 max(0, min(size.height(), static_cast<int>(pt.y))));
  return (pt.x >= 0 && static_cast<int>(pt.x) < size.width() &&
          pt.y >= 0 && static_cast<int>(pt.y) < size.height());
}

bool RootWindowHostWin::ConfineCursorToRootWindow() {
  RECT window_rect;
  GetWindowRect(hwnd(), &window_rect);
  return ClipCursor(&window_rect) != 0;
}

bool RootWindowHostWin::CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                           const gfx::Point& dest_offset,
                                           SkCanvas* canvas) {
  NOTIMPLEMENTED();
  return false;
}

void RootWindowHostWin::UnConfineCursor() {
  ClipCursor(NULL);
}

void RootWindowHostWin::OnCursorVisibilityChanged(bool show) {
  NOTIMPLEMENTED();
}

void RootWindowHostWin::MoveCursorTo(const gfx::Point& location) {
  // Deliberately not implemented.
}

void RootWindowHostWin::SetFocusWhenShown(bool focus_when_shown) {
  NOTIMPLEMENTED();
}

void RootWindowHostWin::PostNativeEvent(const base::NativeEvent& native_event) {
  ::PostMessage(
      hwnd(), native_event.message, native_event.wParam, native_event.lParam);
}

void RootWindowHostWin::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
  NOTIMPLEMENTED();
}

void RootWindowHostWin::PrepareForShutdown() {
  NOTIMPLEMENTED();
}

void RootWindowHostWin::OnClose() {
  // TODO: this obviously shouldn't be here.
  base::MessageLoopForUI::current()->Quit();
}

LRESULT RootWindowHostWin::OnKeyEvent(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param };
  ui::KeyEvent keyev(msg, message == WM_CHAR);
  SetMsgHandled(delegate_->OnHostKeyEvent(&keyev));
  return 0;
}

LRESULT RootWindowHostWin::OnMouseRange(UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param, 0,
              { GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) } };
  ui::MouseEvent event(msg);
  bool handled = false;
  if (!(event.flags() & ui::EF_IS_NON_CLIENT))
    handled = delegate_->OnHostMouseEvent(&event);
  SetMsgHandled(handled);
  return 0;
}

LRESULT RootWindowHostWin::OnCaptureChanged(UINT message,
                                            WPARAM w_param,
                                            LPARAM l_param) {
  if (has_capture_) {
    has_capture_ = false;
    delegate_->OnHostLostWindowCapture();
  }
  return 0;
}

LRESULT RootWindowHostWin::OnNCActivate(UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param) {
  if (!!w_param)
    delegate_->OnHostActivated();
  return DefWindowProc(hwnd(), message, w_param, l_param);
}

void RootWindowHostWin::OnMove(const CPoint& point) {
  if (delegate_)
    delegate_->OnHostMoved(gfx::Point(point.x, point.y));
}

void RootWindowHostWin::OnPaint(HDC dc) {
  gfx::Rect damage_rect;
  RECT update_rect = {0};
  if (GetUpdateRect(hwnd(), &update_rect, FALSE))
    damage_rect = gfx::Rect(update_rect);
  delegate_->OnHostPaint(damage_rect);
  ValidateRect(hwnd(), NULL);
}

void RootWindowHostWin::OnSize(UINT param, const CSize& size) {
  // Minimizing resizes the window to 0x0 which causes our layout to go all
  // screwy, so we just ignore it.
  if (delegate_ && param != SIZE_MINIMIZED)
    delegate_->OnHostResized(gfx::Size(size.cx, size.cy));
}

namespace test {

// static
void SetUsePopupAsRootWindowForTest(bool use) {
  use_popup_as_root_window_for_test = use;
}

}  // namespace test

}  // namespace aura
