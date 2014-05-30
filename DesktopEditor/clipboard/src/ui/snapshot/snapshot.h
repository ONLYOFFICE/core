// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SNAPSHOT_SNAPSHOT_H_
#define UI_SNAPSHOT_SNAPSHOT_H_

#include <vector>

#include "ui/gfx/native_widget_types.h"
#include "ui/snapshot/snapshot_export.h"

namespace gfx {
class Rect;
}

namespace ui {

// Grabs a snapshot of the window/view. No security checks are done.
// This is intended to be used for debugging purposes where no BrowserProcess
// instance is available (ie. tests). DO NOT use in a result of user action.
SNAPSHOT_EXPORT bool GrabWindowSnapshot(
    gfx::NativeWindow window,
    std::vector<unsigned char>* png_representation,
    const gfx::Rect& snapshot_bounds);

SNAPSHOT_EXPORT bool GrabViewSnapshot(
    gfx::NativeView view,
    std::vector<unsigned char>* png_representation,
    const gfx::Rect& snapshot_bounds);

}  // namespace ui

#endif  // UI_SNAPSHOT_SNAPSHOT_H_
