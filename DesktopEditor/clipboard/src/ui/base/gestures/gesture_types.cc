// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gestures/gesture_types.h"

namespace ui {

GestureEventDetails::GestureEventDetails(ui::EventType type,
                                         float delta_x,
                                         float delta_y)
    : type_(type),
      touch_points_(1) {
  switch (type_) {
    case ui::ET_GESTURE_SCROLL_UPDATE:
      data.scroll_update.x = delta_x;
      data.scroll_update.y = delta_y;
      break;

    case ui::ET_SCROLL_FLING_START:
      data.fling_velocity.x = delta_x;
      data.fling_velocity.y = delta_y;
      break;

    case ui::ET_GESTURE_LONG_PRESS:
      data.touch_id = static_cast<int>(delta_x);
      CHECK_EQ(0.f, delta_y) << "Unknown data in delta_y for long press.";
      break;

    case ui::ET_GESTURE_TWO_FINGER_TAP:
      data.first_finger_enclosing_rectangle.width = delta_x;
      data.first_finger_enclosing_rectangle.height = delta_y;
      break;

    case ui::ET_GESTURE_PINCH_UPDATE:
      data.scale = delta_x;
      CHECK_EQ(0.f, delta_y) << "Unknown data in delta_y for pinch";
      break;

    case ui::ET_GESTURE_MULTIFINGER_SWIPE:
      data.swipe.left = delta_x < 0;
      data.swipe.right = delta_x > 0;
      data.swipe.up = delta_y < 0;
      data.swipe.down = delta_y > 0;
      break;

    case ui::ET_GESTURE_TAP:
      data.tap_count = static_cast<int>(delta_x);
      CHECK_EQ(0.f, delta_y) << "Unknown data in delta_y for tap.";
      break;

    default:
      if (delta_x != 0.f || delta_y != 0.f) {
        DLOG(WARNING) << "A gesture event (" << type << ") had unknown data: ("
                      << delta_x << "," << delta_y;
      }
      break;
  }
}

void GestureEventDetails::SetScrollVelocity(float velocity_x,
                                            float velocity_y) {
  CHECK_EQ(ui::ET_GESTURE_SCROLL_UPDATE, type_);
  data.scroll_update.velocity_x = velocity_x;
  data.scroll_update.velocity_y = velocity_y;
}

}  // namespace ui
