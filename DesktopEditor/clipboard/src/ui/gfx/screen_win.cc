// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/screen_win.h"

#include <windows.h>

#include "base/logging.h"
#include "ui/base/win/dpi.h"
#include "ui/gfx/display.h"

namespace {

MONITORINFO GetMonitorInfoForMonitor(HMONITOR monitor) {
  MONITORINFO monitor_info = { 0 };
  monitor_info.cbSize = sizeof(monitor_info);
  GetMonitorInfo(monitor, &monitor_info);
  return monitor_info;
}

gfx::Display GetDisplay(MONITORINFO& monitor_info) {
  // TODO(oshima): Implement ID and Observer.
  gfx::Rect bounds = gfx::Rect(monitor_info.rcMonitor);
  gfx::Display display(0, bounds);
  display.set_work_area(gfx::Rect(monitor_info.rcWork));
  display.SetScaleAndBounds(ui::win::GetDeviceScaleFactor(), bounds);
  return display;
}

}  // namespace

namespace gfx {

ScreenWin::ScreenWin() {
}

ScreenWin::~ScreenWin() {
}

bool ScreenWin::IsDIPEnabled() {
#if defined(ENABLE_HIDPI)
  return true;
#else
  return false;
#endif
}

gfx::Point ScreenWin::GetCursorScreenPoint() {
  POINT pt;
  GetCursorPos(&pt);
  return gfx::Point(pt);
}

gfx::NativeWindow ScreenWin::GetWindowAtCursorScreenPoint() {
  POINT location;
  HWND window_hwnd = GetCursorPos(&location) ? WindowFromPoint(location) : NULL;
  return GetNativeWindowFromHWND(window_hwnd);
}

int ScreenWin::GetNumDisplays() {
  return GetSystemMetrics(SM_CMONITORS);
}

gfx::Display ScreenWin::GetDisplayNearestWindow(gfx::NativeView window) const {
  HWND window_hwnd = GetHWNDFromNativeView(window);
  if (!window_hwnd) {
    // When |window| isn't rooted to a display, we should just return the
    // default display so we get some correct display information like the
    // scaling factor.
    return GetPrimaryDisplay();
  }

  MONITORINFO monitor_info;
  monitor_info.cbSize = sizeof(monitor_info);
  GetMonitorInfo(MonitorFromWindow(window_hwnd, MONITOR_DEFAULTTONEAREST),
                 &monitor_info);
  return GetDisplay(monitor_info);
}

gfx::Display ScreenWin::GetDisplayNearestPoint(const gfx::Point& point) const {
  POINT initial_loc = { point.x(), point.y() };
  HMONITOR monitor = MonitorFromPoint(initial_loc, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = {0};
  mi.cbSize = sizeof(mi);
  if (monitor && GetMonitorInfo(monitor, &mi))
    return GetDisplay(mi);
  return gfx::Display();
}

gfx::Display ScreenWin::GetDisplayMatching(const gfx::Rect& match_rect) const {
  RECT other_bounds_rect = match_rect.ToRECT();
  MONITORINFO monitor_info = GetMonitorInfoForMonitor(MonitorFromRect(
      &other_bounds_rect, MONITOR_DEFAULTTONEAREST));
  return GetDisplay(monitor_info);
}

gfx::Display ScreenWin::GetPrimaryDisplay() const {
  MONITORINFO mi = GetMonitorInfoForMonitor(
      MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY));
  gfx::Display display = GetDisplay(mi);
#if !defined(ENABLE_HIDPI)
  // TODO(kevers|girard): Test if these checks can be reintroduced for high-DIP
  // once more of the app is DIP-aware.
  DCHECK_EQ(GetSystemMetrics(SM_CXSCREEN), display.size().width());
  DCHECK_EQ(GetSystemMetrics(SM_CYSCREEN), display.size().height());
#endif
  return display;
}

void ScreenWin::AddObserver(DisplayObserver* observer) {
  // TODO(oshima): crbug.com/122863.
}

void ScreenWin::RemoveObserver(DisplayObserver* observer) {
  // TODO(oshima): crbug.com/122863.
}

HWND ScreenWin::GetHWNDFromNativeView(NativeView window) const {
#if defined(USE_AURA)
  NOTREACHED();
  return NULL;
#else
  return window;
#endif  // USE_AURA
}

NativeWindow ScreenWin::GetNativeWindowFromHWND(HWND hwnd) const {
#if defined(USE_AURA)
  NOTREACHED();
  return NULL;
#else
  return hwnd;
#endif  // USE_AURA
}

#if !defined(USE_AURA)
Screen* CreateNativeScreen() {
  return new ScreenWin;
}
#endif  // !USE_AURA

}  // namespace gfx
