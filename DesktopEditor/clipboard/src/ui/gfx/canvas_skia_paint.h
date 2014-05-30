// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_SKIA_PAINT_H_
#define UI_GFX_CANVAS_SKIA_PAINT_H_

// This file provides an easy way to include the appropriate CanvasPaint
// header file on your platform.

#if defined(WIN32)
#include "ui/gfx/canvas_paint_win.h"
#elif defined(__APPLE__)
#include "ui/gfx/canvas_paint_mac.h"
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
#if defined(TOOLKIT_GTK)
#include "ui/gfx/canvas_paint_gtk.h"
#else
#error "No canvas paint for this platform"
#endif
#endif

#endif  // UI_GFX_CANVAS_SKIA_PAINT_H_
