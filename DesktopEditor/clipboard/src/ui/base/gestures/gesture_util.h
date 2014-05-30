// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GESTURES_GESTURE_UTIL_H_
#define UI_BASE_GESTURES_GESTURE_UTIL_H_

#include "ui/base/ui_export.h"

namespace gfx {
class Point;
}  // namespace gfx

namespace ui {
namespace gestures {

// Returns true if the distance between points |p1| and |p2| is less than a
// threshold. This is generally used to determine if a touch point has moved
// enough to be no longer considered a tap.
UI_EXPORT bool IsInsideManhattanSquare(const gfx::Point& p1,
                                       const gfx::Point& p2);

}  // namespace gestures
}  // namespace ui

#endif  // UI_BASE_GESTURES_GESTURE_UTIL_H_
