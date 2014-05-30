// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_FOCUS_WINDOW_SET_H_
#define UI_BASE_COCOA_FOCUS_WINDOW_SET_H_

#include <set>

#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {

// Brings a group of windows to the front without changing their order, and
// makes the frontmost one key and main.
UI_EXPORT void FocusWindowSet(std::set<gfx::NativeWindow> windows);

}  // namespace ui

#endif  // UI_BASE_COCOA_FOCUS_WINDOW_SET_H_
