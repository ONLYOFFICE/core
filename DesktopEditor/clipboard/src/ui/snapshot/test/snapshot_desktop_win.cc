// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/snapshot/test/snapshot_desktop.h"

#include "ui/snapshot/snapshot_win.h"

namespace ui {

bool GrabDesktopSnapshot(const gfx::Rect& snapshot_bounds,
                         std::vector<unsigned char>* png_representation) {
  return internal::GrabHwndSnapshot(NULL, snapshot_bounds, png_representation);
}

}  // namespace ui
