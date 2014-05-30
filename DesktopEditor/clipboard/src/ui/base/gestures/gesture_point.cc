// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gestures/gesture_point.h"

#include <cmath>

#include "base/basictypes.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/gestures/gesture_configuration.h"
#include "ui/base/gestures/gesture_types.h"
#include "ui/base/gestures/gesture_util.h"

namespace ui {

GesturePoint::GesturePoint()
    : first_touch_time_(0.0),
      second_last_touch_time_(0.0),
      last_touch_time_(0.0),
      last_tap_time_(0.0),
      velocity_calculator_(
          GestureConfiguration::points_buffered_for_velocity()),
      point_id_(-1),
      touch_id_(-1) {
}

GesturePoint::~GesturePoint() {}

void GesturePoint::Reset() {
  first_touch_time_ = second_last_touch_time_ = last_touch_time_ = 0.0;
  ResetVelocity();
  point_id_ = -1;
  clear_enclosing_rectangle();
}

void GesturePoint::ResetVelocity() {
  velocity_calculator_.ClearHistory();
  same_direction_count_ = gfx::Vector2d();
}

gfx::Vector2d GesturePoint::ScrollDelta() {
  return last_touch_position_ - second_last_touch_position_;
}

void GesturePoint::UpdateValues(const TouchEvent& event) {
  const int64 event_timestamp_microseconds =
      event.time_stamp().InMicroseconds();
  if (event.type() == ui::ET_TOUCH_MOVED) {
    velocity_calculator_.PointSeen(event.location().x(),
                                   event.location().y(),
                                   event_timestamp_microseconds);
    gfx::Vector2d sd(ScrollVelocityDirection(velocity_calculator_.XVelocity()),
                     ScrollVelocityDirection(velocity_calculator_.YVelocity()));
    same_direction_count_ = same_direction_count_ + sd;
  }

  last_touch_time_ = event.time_stamp().InSecondsF();
  last_touch_position_ = event.location();

  if (event.type() == ui::ET_TOUCH_PRESSED) {
    ResetVelocity();
    clear_enclosing_rectangle();
    first_touch_time_ = last_touch_time_;
    first_touch_position_ = event.location();
    second_last_touch_position_ = last_touch_position_;
    second_last_touch_time_ = last_touch_time_;
    velocity_calculator_.PointSeen(event.location().x(),
                                   event.location().y(),
                                   event_timestamp_microseconds);
  }

  UpdateEnclosingRectangle(event);
}

void GesturePoint::UpdateForTap() {
  // Update the tap-position and time, and reset every other state.
  last_tap_time_ = last_touch_time_;
  last_tap_position_ = last_touch_position_;
}

void GesturePoint::UpdateForScroll() {
  second_last_touch_position_ = last_touch_position_;
  second_last_touch_time_ = last_touch_time_;
  same_direction_count_ = gfx::Vector2d();
}

bool GesturePoint::IsInClickWindow(const TouchEvent& event) const {
  return IsInClickTimeWindow() && IsInsideManhattanSquare(event);
}

bool GesturePoint::IsInDoubleClickWindow(const TouchEvent& event) const {
  return IsInSecondClickTimeWindow() &&
         IsSecondClickInsideManhattanSquare(event);
}

bool GesturePoint::IsInScrollWindow(const TouchEvent& event) const {
  return event.type() == ui::ET_TOUCH_MOVED &&
         !IsInsideManhattanSquare(event);
}

bool GesturePoint::IsInFlickWindow(const TouchEvent& event) {
  return IsOverMinFlickSpeed() &&
         event.type() != ui::ET_TOUCH_CANCELLED;
}

int GesturePoint::ScrollVelocityDirection(float v) {
  if (v < -GestureConfiguration::min_scroll_velocity())
    return -1;
  else if (v > GestureConfiguration::min_scroll_velocity())
    return 1;
  else
    return 0;
}

bool GesturePoint::DidScroll(const TouchEvent& event, int dist) const {
  gfx::Vector2d d = last_touch_position_ - second_last_touch_position_;
  return abs(d.x()) > dist || abs(d.y()) > dist;
}

bool GesturePoint::IsConsistentScrollingActionUnderway() {
  int me = GestureConfiguration::min_scroll_successive_velocity_events();
  if (abs(same_direction_count_.x()) >= me ||
      abs(same_direction_count_.y()) >= me)
    return true;
  return false;
}

bool GesturePoint::IsInHorizontalRailWindow() const {
  gfx::Vector2d d = last_touch_position_ - second_last_touch_position_;
  return abs(d.x()) >
      GestureConfiguration::rail_start_proportion() * abs(d.y());
}

bool GesturePoint::IsInVerticalRailWindow() const {
  gfx::Vector2d d = last_touch_position_ - second_last_touch_position_;
  return abs(d.y()) >
      GestureConfiguration::rail_start_proportion() * abs(d.x());
}

bool GesturePoint::BreaksHorizontalRail() {
  float vx = XVelocity();
  float vy = YVelocity();
  return fabs(vy) > GestureConfiguration::rail_break_proportion() * fabs(vx) +
      GestureConfiguration::min_rail_break_velocity();
}

bool GesturePoint::BreaksVerticalRail() {
  float vx = XVelocity();
  float vy = YVelocity();
  return fabs(vx) > GestureConfiguration::rail_break_proportion() * fabs(vy) +
      GestureConfiguration::min_rail_break_velocity();
}

bool GesturePoint::IsInClickTimeWindow() const {
  double duration = last_touch_time_ - first_touch_time_;
  return duration >=
      GestureConfiguration::min_touch_down_duration_in_seconds_for_click() &&
      duration <
      GestureConfiguration::max_touch_down_duration_in_seconds_for_click();
}

bool GesturePoint::IsInSecondClickTimeWindow() const {
  double duration =  last_touch_time_ - last_tap_time_;
  return duration < GestureConfiguration::max_seconds_between_double_click();
}

bool GesturePoint::IsInsideManhattanSquare(const TouchEvent& event) const {
  return ui::gestures::IsInsideManhattanSquare(event.location(),
                                               first_touch_position_);
}

bool GesturePoint::IsSecondClickInsideManhattanSquare(
    const TouchEvent& event) const {
  int manhattan_distance = abs(event.location().x() - last_tap_position_.x()) +
                           abs(event.location().y() - last_tap_position_.y());
  return manhattan_distance <
      GestureConfiguration::max_distance_between_taps_for_double_tap();
}

bool GesturePoint::IsOverMinFlickSpeed() {
  return velocity_calculator_.VelocitySquared() >
      GestureConfiguration::min_flick_speed_squared();
}

void GesturePoint::UpdateEnclosingRectangle(const TouchEvent& event) {
  int radius;

  // Ignore this TouchEvent if it has a radius larger than the maximum
  // allowed radius size.
  if (event.radius_x() > GestureConfiguration::max_radius() ||
      event.radius_y() > GestureConfiguration::max_radius())
    return;

  // If the device provides at least one of the radius values, take the larger
  // of the two and use this as both the x radius and the y radius of the
  // touch region. Otherwise use the default radius value.
  // TODO(tdanderson): Implement a more specific check for the exact
  // information provided by the device (0-2 radii values, force, angle) and
  // use this to compute a more representative rectangular touch region.
  if (event.radius_x() || event.radius_y())
    radius = std::max(event.radius_x(), event.radius_y());
  else
    radius = GestureConfiguration::default_radius();

  gfx::Rect rect(event.location().x() - radius,
                 event.location().y() - radius,
                 radius * 2,
                 radius * 2);
  if (IsInClickWindow(event))
    enclosing_rect_.Union(rect);
  else
    enclosing_rect_ = rect;
}

}  // namespace ui
