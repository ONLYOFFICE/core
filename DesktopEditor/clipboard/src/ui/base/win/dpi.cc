// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/win/dpi.h"

#include <windows.h>

#include "base/win/scoped_hdc.h"
#include "ui/base/layout.h"
#include "base/win/registry.h"
#include "ui/gfx/display.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/rect_conversions.h"
#include "ui/gfx/size_conversions.h"

namespace {

int kDefaultDPIX = 96;
int kDefaultDPIY = 96;

float GetDeviceScaleFactorImpl() {
#if defined(ENABLE_HIDPI)
  float scale = gfx::Display::HasForceDeviceScaleFactor() ?
      gfx::Display::GetForcedDeviceScaleFactor() : ui::GetDPIScale();
  // Quantize to nearest supported scale factor.
  scale = ui::GetScaleFactorScale(ui::GetScaleFactorFromScale(scale));
  return scale;
#else
  return 1.0f;
#endif
}

BOOL IsProcessDPIAwareWrapper() {
  typedef BOOL(WINAPI *IsProcessDPIAwarePtr)(VOID);
  IsProcessDPIAwarePtr is_process_dpi_aware_func =
      reinterpret_cast<IsProcessDPIAwarePtr>(
          GetProcAddress(GetModuleHandleA("user32.dll"), "IsProcessDPIAware"));
  if (is_process_dpi_aware_func)
    return is_process_dpi_aware_func();
  return FALSE;
}

}  // namespace

namespace ui {

gfx::Size GetDPI() {
  static int dpi_x = 0;
  static int dpi_y = 0;
  static bool should_initialize = true;

  if (should_initialize) {
    should_initialize = false;
    base::win::ScopedGetDC screen_dc(NULL);
    // This value is safe to cache for the life time of the app since the
    // user must logout to change the DPI setting. This value also applies
    // to all screens.
    dpi_x = GetDeviceCaps(screen_dc, LOGPIXELSX);
    dpi_y = GetDeviceCaps(screen_dc, LOGPIXELSY);
  }
  return gfx::Size(dpi_x, dpi_y);
}

float GetDPIScale() {
  return static_cast<float>(GetDPI().width()) /
      static_cast<float>(kDefaultDPIX);
}

bool IsInHighDPIMode() {
  gfx::Size dpi(GetDPI());
  return dpi.width() > kDefaultDPIX || dpi.height() > kDefaultDPIY;
}

void EnableHighDPISupport() {
  typedef BOOL(WINAPI *SetProcessDPIAwarePtr)(VOID);
  SetProcessDPIAwarePtr set_process_dpi_aware_func =
      reinterpret_cast<SetProcessDPIAwarePtr>(
          GetProcAddress(GetModuleHandleA("user32.dll"), "SetProcessDPIAware"));
  if (set_process_dpi_aware_func)
    set_process_dpi_aware_func();
}

namespace win {

float GetDeviceScaleFactor() {
  static const float device_scale_factor = GetDeviceScaleFactorImpl();
  return device_scale_factor;
}

gfx::Point ScreenToDIPPoint(const gfx::Point& pixel_point) {
  return gfx::ToFlooredPoint(
      gfx::ScalePoint(pixel_point, 1.0f / GetDeviceScaleFactor()));
}

gfx::Rect ScreenToDIPRect(const gfx::Rect& pixel_bounds) {
  // TODO(kevers): Switch to non-deprecated method for float to int conversions.
  return gfx::ToFlooredRectDeprecated(
      gfx::ScaleRect(pixel_bounds, 1.0f / GetDeviceScaleFactor()));
}

gfx::Rect DIPToScreenRect(const gfx::Rect& dip_bounds) {
  // TODO(kevers): Switch to non-deprecated method for float to int conversions.
  return gfx::ToFlooredRectDeprecated(
      gfx::ScaleRect(dip_bounds, GetDeviceScaleFactor()));
}

gfx::Size ScreenToDIPSize(const gfx::Size& size_in_pixels) {
  return gfx::ToFlooredSize(
      gfx::ScaleSize(size_in_pixels, 1.0f / GetDeviceScaleFactor()));
}

gfx::Size DIPToScreenSize(const gfx::Size& dip_size) {
  return gfx::ToFlooredSize(gfx::ScaleSize(dip_size, GetDeviceScaleFactor()));
}

int GetSystemMetricsInDIP(int metric) {
  return static_cast<int>(GetSystemMetrics(metric) /
      GetDeviceScaleFactor() + 0.5);
}

double GetUndocumentedDPIScale() {
  // TODO(girard): Remove this code when chrome is DPIAware.
  static double scale = -1.0;
  if (scale == -1.0) {
    scale = 1.0;
    if (!IsProcessDPIAwareWrapper()) {
      base::win::RegKey key(HKEY_CURRENT_USER,
                            L"Control Panel\\Desktop\\WindowMetrics",
                            KEY_QUERY_VALUE);
      if (key.Valid()) {
        DWORD value = 0;
        if (key.ReadValueDW(L"AppliedDPI", &value) == ERROR_SUCCESS) {
          scale = static_cast<double>(value) / kDefaultDPIX;
        }
      }
    }
  }
  return scale;
}

}  // namespace win

}  // namespace ui
