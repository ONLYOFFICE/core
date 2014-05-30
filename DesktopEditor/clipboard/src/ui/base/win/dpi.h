// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_DPI_H_
#define UI_BASE_WIN_DPI_H_

#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT gfx::Size GetDPI();

// Gets the scale factor of the display. For example, if the display DPI is
// 96 then the scale factor is 1.0.
UI_EXPORT float GetDPIScale();

UI_EXPORT bool IsInHighDPIMode();

UI_EXPORT void EnableHighDPISupport();

// TODO(kevers|girard):  Move above methods into win namespace.

namespace win {

UI_EXPORT float GetDeviceScaleFactor();

UI_EXPORT gfx::Point ScreenToDIPPoint(const gfx::Point& pixel_point);

UI_EXPORT gfx::Rect ScreenToDIPRect(const gfx::Rect& pixel_bounds);

UI_EXPORT gfx::Rect DIPToScreenRect(const gfx::Rect& dip_bounds);

UI_EXPORT gfx::Size ScreenToDIPSize(const gfx::Size& size_in_pixels);

UI_EXPORT gfx::Size DIPToScreenSize(const gfx::Size& dip_size);

// Win32's GetSystemMetrics uses pixel measures. This function calls
// GetSystemMetrics for the given |metric|, then converts the result to DIP.
UI_EXPORT int GetSystemMetricsInDIP(int metric);

// The OS secretly scales apps that are not DPIAware. This is not visible
// through standard OS calls like GetWindowPos(), or through GetDPIScale().
// Returns the scale factor of the display, where 96 DPI is 1.0.
// (Avoid this function... use GetDPIScale() instead.)
// TODO(girard): Remove this once DPIAware is enabled - http://crbug.com/149881
UI_EXPORT double GetUndocumentedDPIScale();

}  // namespace win

}  // namespace ui

#endif  // UI_BASE_WIN_DPI_H_
