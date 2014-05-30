// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GESTURES_GESTURE_TYPES_H_
#define UI_BASE_GESTURES_GESTURE_TYPES_H_

#include "base/logging.h"
#include "ui/base/events/event_constants.h"
#include "ui/gfx/rect.h"

namespace ui {

class GestureEvent;
class TouchEvent;

struct UI_EXPORT GestureEventDetails {
 public:
  GestureEventDetails(EventType type, float delta_x, float delta_y);

  EventType type() const { return type_; }

  int touch_points() const { return touch_points_; }
  void set_touch_points(int touch_points) { touch_points_ = touch_points; }

  const gfx::Rect& bounding_box() const { return bounding_box_; }
  void set_bounding_box(const gfx::Rect& box) { bounding_box_ = box; }

  void SetScrollVelocity(float velocity_x, float velocity_y);

  float scroll_x() const {
    CHECK_EQ(ui::ET_GESTURE_SCROLL_UPDATE, type_);
    return data.scroll_update.x;
  }

  float scroll_y() const {
    CHECK_EQ(ui::ET_GESTURE_SCROLL_UPDATE, type_);
    return data.scroll_update.y;
  }

  float velocity_x() const {
    CHECK(type_ == ui::ET_GESTURE_SCROLL_UPDATE ||
          type_ == ui::ET_SCROLL_FLING_START);
    return type_ == ui::ET_SCROLL_FLING_START ? data.fling_velocity.x :
                                                data.scroll_update.velocity_x;
  }

  float velocity_y() const {
    CHECK(type_ == ui::ET_GESTURE_SCROLL_UPDATE ||
          type_ == ui::ET_SCROLL_FLING_START);
    return type_ == ui::ET_SCROLL_FLING_START ? data.fling_velocity.y :
                                                data.scroll_update.velocity_y;
  }

  int touch_id() const {
    CHECK_EQ(ui::ET_GESTURE_LONG_PRESS, type_);
    return data.touch_id;
  }

  float first_finger_width() const {
    CHECK_EQ(ui::ET_GESTURE_TWO_FINGER_TAP, type_);
    return data.first_finger_enclosing_rectangle.width;
  }

  float first_finger_height() const {
    CHECK_EQ(ui::ET_GESTURE_TWO_FINGER_TAP, type_);
    return data.first_finger_enclosing_rectangle.height;
  }

  float scale() const {
    CHECK_EQ(ui::ET_GESTURE_PINCH_UPDATE, type_);
    return data.scale;
  }

  bool swipe_left() const {
    CHECK_EQ(ui::ET_GESTURE_MULTIFINGER_SWIPE, type_);
    return data.swipe.left;
  }

  bool swipe_right() const {
    CHECK_EQ(ui::ET_GESTURE_MULTIFINGER_SWIPE, type_);
    return data.swipe.right;
  }

  bool swipe_up() const {
    CHECK_EQ(ui::ET_GESTURE_MULTIFINGER_SWIPE, type_);
    return data.swipe.up;
  }

  bool swipe_down() const {
    CHECK_EQ(ui::ET_GESTURE_MULTIFINGER_SWIPE, type_);
    return data.swipe.down;
  }

  int tap_count() const {
    CHECK_EQ(ui::ET_GESTURE_TAP, type_);
    return data.tap_count;
  }

 private:
  ui::EventType type_;
  union {
    struct {  // SCROLL delta.
      float x;
      float y;
      float velocity_x;
      float velocity_y;
    } scroll_update;

    float scale;  // PINCH scale.

    struct {  // FLING velocity.
      float x;
      float y;
    } fling_velocity;

    int touch_id;  // LONG_PRESS touch-id.

    // Dimensions of the first finger's enclosing rectangle for TWO_FINGER_TAP.
    struct {
      float width;
      float height;
    } first_finger_enclosing_rectangle;

    struct {  // SWIPE direction.
      bool left;
      bool right;
      bool up;
      bool down;
    } swipe;

    int tap_count;  // TAP repeat count.
  } data;

  int touch_points_;  // Number of active touch points in the gesture.

  // Bounding box is an axis-aligned rectangle that contains all the
  // enclosing rectangles of the touch-points in the gesture.
  gfx::Rect bounding_box_;
};

// An abstract type for consumers of gesture-events created by the
// gesture-recognizer.
class UI_EXPORT GestureConsumer {
 public:
  GestureConsumer()
      : ignores_events_(false) {
  }

  explicit GestureConsumer(bool ignores_events)
      : ignores_events_(ignores_events) {
  }

  virtual ~GestureConsumer() {}

  // TODO: this is a hack! GestureRecognizer should never expose the internal
  // marker object that implements this.
  bool ignores_events() { return ignores_events_; }

 private:
  const bool ignores_events_;
};

// GestureEventHelper creates implementation-specific gesture events and
// can dispatch them.
class UI_EXPORT GestureEventHelper {
 public:
  virtual ~GestureEventHelper() {
  }

  virtual bool DispatchLongPressGestureEvent(GestureEvent* event) = 0;
  virtual bool DispatchCancelTouchEvent(TouchEvent* event) = 0;
};

}  // namespace ui

#endif  // UI_BASE_GESTURES_GESTURE_TYPES_H_
