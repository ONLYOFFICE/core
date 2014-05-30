// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_MONITOR_WIN_H_
#define UI_VIEWS_WIDGET_MONITOR_WIN_H_

#include <windows.h>

#include "ui/views/views_export.h"

namespace gfx {
class Rect;
}

namespace views {

// Returns the bounds for the monitor that contains the largest area of
// intersection with the specified rectangle.
VIEWS_EXPORT gfx::Rect GetMonitorBoundsForRect(const gfx::Rect& rect);

// Returns the always-on-top auto-hiding taskbar for edge |edge| (one of
// ABE_LEFT, TOP, RIGHT, or BOTTOM) of monitor |monitor|. NULL is returned
// if nothing is found.
VIEWS_EXPORT HWND GetTopmostAutoHideTaskbarForEdge(UINT edge, HMONITOR monitor);

}  // namespace views

#endif  // UI_VIEWS_WIDGET_MONITOR_WIN_H_
