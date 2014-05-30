// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/view.h"

namespace {

// Default horizontal drag threshold in pixels.
// Same as what gtk uses.
const int kDefaultHorizontalDragThreshold = 8;

// Default vertical drag threshold in pixels.
// Same as what gtk uses.
const int kDefaultVerticalDragThreshold = 8;

}  // namespace

namespace views {

int View::GetHorizontalDragThreshold() {
  // TODO(jennyz): This value may need to be adjusted for different platforms
  // and for different display density.
  return kDefaultHorizontalDragThreshold;
}

int View::GetVerticalDragThreshold() {
  // TODO(jennyz): This value may need to be adjusted for different platforms
  // and for different display density.
  return kDefaultVerticalDragThreshold;
}

}  // namespace views
