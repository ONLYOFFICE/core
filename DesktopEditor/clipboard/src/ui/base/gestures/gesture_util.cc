// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gestures/gesture_util.h"

#include <stdlib.h>

#include "ui/base/gestures/gesture_configuration.h"
#include "ui/gfx/point.h"

namespace ui {
namespace gestures {

bool IsInsideManhattanSquare(const gfx::Point& p1,
                             const gfx::Point& p2) {
  int manhattan_distance = abs(p1.x() - p2.x()) + abs(p1.y() - p2.y());
  return manhattan_distance <
      GestureConfiguration::max_touch_move_in_pixels_for_click();
}

}  // namespace gestures
}  // namespace ui
