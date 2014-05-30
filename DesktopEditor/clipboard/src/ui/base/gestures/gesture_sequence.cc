// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gestures/gesture_sequence.h"

#include <cmath>
#include <stdlib.h>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/string_number_conversions.h"
#include "base/time.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/gestures/gesture_configuration.h"
#include "ui/base/gestures/gesture_util.h"
#include "ui/gfx/rect.h"

namespace ui {

namespace {

// ui::EventType is mapped to TouchState so it can fit into 3 bits of
// Signature.
enum TouchState {
  TS_RELEASED,
  TS_PRESSED,
  TS_MOVED,
  TS_STATIONARY,
  TS_CANCELLED,
  TS_UNKNOWN,
};

// ui::EventResult is mapped to TouchStatusInternal to simply indicate whether a
// processed touch-event should affect gesture-recognition or not.
enum TouchStatusInternal {
  TSI_NOT_PROCESSED,  // The touch-event should take-part into
                      // gesture-recognition only if the touch-event has not
                      // been processed.

  TSI_PROCESSED,      // The touch-event should affect gesture-recognition only
                      // if the touch-event has been processed. For example,,
                      // this means that a JavaScript touch handler called
                      // |preventDefault| on the associated touch event
                      // or was processed by an aura-window or views-view.

  TSI_ALWAYS          // The touch-event should always affect gesture
                      // recognition.
};

// Get equivalent TouchState from EventType |type|.
TouchState TouchEventTypeToTouchState(ui::EventType type) {
  switch (type) {
    case ui::ET_TOUCH_RELEASED:
      return TS_RELEASED;
    case ui::ET_TOUCH_PRESSED:
      return TS_PRESSED;
    case ui::ET_TOUCH_MOVED:
      return TS_MOVED;
    case ui::ET_TOUCH_STATIONARY:
      return TS_STATIONARY;
    case ui::ET_TOUCH_CANCELLED:
      return TS_CANCELLED;
    default:
      DVLOG(1) << "Unknown Touch Event type";
  }
  return TS_UNKNOWN;
}

// Gesture signature types for different values of combination (GestureState,
// touch_id, ui::EventType, touch_handled), see Signature for more info.
//
// Note: New addition of types should be placed as per their Signature value.
#define G(gesture_state, id, touch_state, handled) 1 + ( \
  (((touch_state) & 0x7) << 1) |                         \
  ((handled & 0x3) << 4) |                               \
  (((id) & 0xfff) << 6) |                                \
  ((gesture_state) << 18))

enum EdgeStateSignatureType {
  GST_INVALID = -1,

  GST_NO_GESTURE_FIRST_PRESSED =
      G(GS_NO_GESTURE, 0, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_RELEASED, TSI_NOT_PROCESSED),

  GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED_HANDLED =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_RELEASED, TSI_PROCESSED),

  // Ignore processed touch-move events until gesture-scroll starts.
  GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_MOVED, TSI_NOT_PROCESSED),

  GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED_PROCESSED =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_MOVED, TSI_PROCESSED),

  GST_PENDING_SYNTHETIC_CLICK_FIRST_STATIONARY =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_STATIONARY, TSI_NOT_PROCESSED),

  GST_PENDING_SYNTHETIC_CLICK_FIRST_CANCELLED =
      G(GS_PENDING_SYNTHETIC_CLICK, 0, TS_CANCELLED, TSI_ALWAYS),

  GST_PENDING_SYNTHETIC_CLICK_SECOND_PRESSED =
      G(GS_PENDING_SYNTHETIC_CLICK, 1, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_SCROLL_FIRST_RELEASED =
      G(GS_SCROLL, 0, TS_RELEASED, TSI_ALWAYS),

  // Once scroll has started, process all touch-move events.
  GST_SCROLL_FIRST_MOVED =
      G(GS_SCROLL, 0, TS_MOVED, TSI_ALWAYS),

  GST_SCROLL_FIRST_CANCELLED =
      G(GS_SCROLL, 0, TS_CANCELLED, TSI_ALWAYS),

  GST_SCROLL_SECOND_PRESSED =
      G(GS_SCROLL, 1, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED =
      G(GS_PENDING_TWO_FINGER_TAP, 0, TS_RELEASED, TSI_NOT_PROCESSED),

  GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED_HANDLED =
      G(GS_PENDING_TWO_FINGER_TAP, 0, TS_RELEASED, TSI_PROCESSED),

  GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED =
      G(GS_PENDING_TWO_FINGER_TAP, 1, TS_RELEASED, TSI_NOT_PROCESSED),

  GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED_HANDLED =
      G(GS_PENDING_TWO_FINGER_TAP, 1, TS_RELEASED, TSI_PROCESSED),

  GST_PENDING_TWO_FINGER_TAP_FIRST_MOVED =
      G(GS_PENDING_TWO_FINGER_TAP, 0, TS_MOVED, TSI_ALWAYS),

  GST_PENDING_TWO_FINGER_TAP_SECOND_MOVED =
      G(GS_PENDING_TWO_FINGER_TAP, 1, TS_MOVED, TSI_ALWAYS),

  GST_PENDING_TWO_FINGER_TAP_FIRST_CANCELLED =
      G(GS_PENDING_TWO_FINGER_TAP, 0, TS_CANCELLED, TSI_ALWAYS),

  GST_PENDING_TWO_FINGER_TAP_SECOND_CANCELLED =
      G(GS_PENDING_TWO_FINGER_TAP, 1, TS_CANCELLED, TSI_ALWAYS),

  GST_PENDING_TWO_FINGER_TAP_THIRD_PRESSED =
      G(GS_PENDING_TWO_FINGER_TAP, 2, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PINCH_FIRST_MOVED =
      G(GS_PINCH, 0, TS_MOVED, TSI_ALWAYS),

  GST_PINCH_SECOND_MOVED =
      G(GS_PINCH, 1, TS_MOVED, TSI_ALWAYS),

  GST_PINCH_FIRST_RELEASED =
      G(GS_PINCH, 0, TS_RELEASED, TSI_ALWAYS),

  GST_PINCH_SECOND_RELEASED =
      G(GS_PINCH, 1, TS_RELEASED, TSI_ALWAYS),

  GST_PINCH_FIRST_CANCELLED =
      G(GS_PINCH, 0, TS_CANCELLED, TSI_ALWAYS),

  GST_PINCH_SECOND_CANCELLED =
      G(GS_PINCH, 1, TS_CANCELLED, TSI_ALWAYS),

  GST_PINCH_THIRD_PRESSED =
      G(GS_PINCH, 2, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PINCH_THIRD_MOVED =
      G(GS_PINCH, 2, TS_MOVED, TSI_ALWAYS),

  GST_PINCH_THIRD_RELEASED =
      G(GS_PINCH, 2, TS_RELEASED, TSI_ALWAYS),

  GST_PINCH_THIRD_CANCELLED =
      G(GS_PINCH, 2, TS_CANCELLED, TSI_ALWAYS),

  GST_PINCH_FOURTH_PRESSED =
      G(GS_PINCH, 3, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PINCH_FOURTH_MOVED =
      G(GS_PINCH, 3, TS_MOVED, TSI_ALWAYS),

  GST_PINCH_FOURTH_RELEASED =
      G(GS_PINCH, 3, TS_RELEASED, TSI_ALWAYS),

  GST_PINCH_FOURTH_CANCELLED =
      G(GS_PINCH, 3, TS_CANCELLED, TSI_ALWAYS),

  GST_PINCH_FIFTH_PRESSED =
      G(GS_PINCH, 4, TS_PRESSED, TSI_NOT_PROCESSED),

  GST_PINCH_FIFTH_MOVED =
      G(GS_PINCH, 4, TS_MOVED, TSI_ALWAYS),

  GST_PINCH_FIFTH_RELEASED =
      G(GS_PINCH, 4, TS_RELEASED, TSI_ALWAYS),

  GST_PINCH_FIFTH_CANCELLED =
      G(GS_PINCH, 4, TS_CANCELLED, TSI_ALWAYS),
};

// Builds a signature. Signatures are assembled by joining together
// multiple bits.
// 1 LSB bit so that the computed signature is always greater than 0
// 3 bits for the |type|.
// 2 bit for |touch_status|
// 12 bits for |touch_id|
// 14 bits for the |gesture_state|.
EdgeStateSignatureType Signature(GestureState gesture_state,
                                 unsigned int touch_id,
                                 ui::EventType type,
                                 TouchStatusInternal touch_status) {
  CHECK((touch_id & 0xfff) == touch_id);
  TouchState touch_state = TouchEventTypeToTouchState(type);
  EdgeStateSignatureType signature = static_cast<EdgeStateSignatureType>
      (G(gesture_state, touch_id, touch_state, touch_status));

  switch (signature) {
    case GST_NO_GESTURE_FIRST_PRESSED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED_HANDLED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED_PROCESSED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_STATIONARY:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_CANCELLED:
    case GST_PENDING_SYNTHETIC_CLICK_SECOND_PRESSED:
    case GST_SCROLL_FIRST_RELEASED:
    case GST_SCROLL_FIRST_MOVED:
    case GST_SCROLL_FIRST_CANCELLED:
    case GST_SCROLL_SECOND_PRESSED:
    case GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED:
    case GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED_HANDLED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED_HANDLED:
    case GST_PENDING_TWO_FINGER_TAP_FIRST_MOVED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_MOVED:
    case GST_PENDING_TWO_FINGER_TAP_FIRST_CANCELLED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_CANCELLED:
    case GST_PENDING_TWO_FINGER_TAP_THIRD_PRESSED:
    case GST_PINCH_FIRST_MOVED:
    case GST_PINCH_SECOND_MOVED:
    case GST_PINCH_FIRST_RELEASED:
    case GST_PINCH_SECOND_RELEASED:
    case GST_PINCH_FIRST_CANCELLED:
    case GST_PINCH_SECOND_CANCELLED:
    case GST_PINCH_THIRD_PRESSED:
    case GST_PINCH_THIRD_MOVED:
    case GST_PINCH_THIRD_RELEASED:
    case GST_PINCH_THIRD_CANCELLED:
    case GST_PINCH_FOURTH_PRESSED:
    case GST_PINCH_FOURTH_MOVED:
    case GST_PINCH_FOURTH_RELEASED:
    case GST_PINCH_FOURTH_CANCELLED:
    case GST_PINCH_FIFTH_PRESSED:
    case GST_PINCH_FIFTH_MOVED:
    case GST_PINCH_FIFTH_RELEASED:
    case GST_PINCH_FIFTH_CANCELLED:
      break;
    default:
      signature = GST_INVALID;
      break;
  }

  return signature;
}
#undef G

float BoundingBoxDiagonal(const gfx::Rect& rect) {
  float width = rect.width() * rect.width();
  float height = rect.height() * rect.height();
  return sqrt(width + height);
}

unsigned int ComputeTouchBitmask(const GesturePoint* points) {
  unsigned int touch_bitmask = 0;
  for (int i = 0; i < GestureSequence::kMaxGesturePoints; ++i) {
    if (points[i].in_use())
      touch_bitmask |= 1 << points[i].touch_id();
  }
  return touch_bitmask;
}

const float kFlingCurveNormalization = 1.0f / 1875.f;

float CalibrateFlingVelocity(float velocity) {
  const unsigned last_coefficient =
      GestureConfiguration::NumAccelParams - 1;
  float normalized_velocity = fabs(velocity * kFlingCurveNormalization);
  float nu = 0.0f, x = 1.f;

  for (int i = last_coefficient ; i >= 0; i--) {
    float a = GestureConfiguration::fling_acceleration_curve_coefficients(i);
    nu += x * a;
    x *= normalized_velocity;
  }
  if (velocity < 0.f)
    return std::max(nu * velocity, -GestureConfiguration::fling_velocity_cap());
  else
    return std::min(nu * velocity, GestureConfiguration::fling_velocity_cap());
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// GestureSequence Public:

GestureSequence::GestureSequence(GestureEventHelper* helper)
    : state_(GS_NO_GESTURE),
      flags_(0),
      pinch_distance_start_(0.f),
      pinch_distance_current_(0.f),
      scroll_type_(ST_FREE),
      point_count_(0),
      helper_(helper) {
}

GestureSequence::~GestureSequence() {
}

GestureSequence::Gestures* GestureSequence::ProcessTouchEventForGesture(
    const TouchEvent& event,
    EventResult result) {
  StopLongPressTimerIfRequired(event);
  last_touch_location_ = event.location();
  if (result & ER_CONSUMED)
    return NULL;

  // Set a limit on the number of simultaneous touches in a gesture.
  if (event.touch_id() >= kMaxGesturePoints)
    return NULL;

  if (event.type() == ui::ET_TOUCH_PRESSED) {
    if (point_count_ == kMaxGesturePoints)
      return NULL;
    GesturePoint* new_point = &points_[event.touch_id()];
    // We shouldn't be able to get two PRESSED events from the same
    // finger without either a RELEASE or CANCEL in between. But let's not crash
    // in a release build.
    if (new_point->in_use()) {
      LOG(ERROR) << "Received a second press for a point: " << event.touch_id();
      new_point->ResetVelocity();
      new_point->UpdateValues(event);
      return NULL;
    }
    new_point->set_point_id(point_count_++);
    new_point->set_touch_id(event.touch_id());
  }

  GestureState last_state = state_;

  // NOTE: when modifying these state transitions, also update gestures.dot
  scoped_ptr<Gestures> gestures(new Gestures());
  GesturePoint& point = GesturePointForEvent(event);
  point.UpdateValues(event);
  RecreateBoundingBox();
  flags_ = event.flags();
  const int point_id = point.point_id();
  if (point_id < 0)
    return NULL;

  // Send GESTURE_BEGIN for any touch pressed.
  if (event.type() == ui::ET_TOUCH_PRESSED)
    AppendBeginGestureEvent(point, gestures.get());

  TouchStatusInternal status_internal = (result == ER_UNHANDLED) ?
      TSI_NOT_PROCESSED : TSI_PROCESSED;

  EdgeStateSignatureType signature = Signature(state_, point_id,
      event.type(), status_internal);

  if (signature == GST_INVALID)
    signature = Signature(state_, point_id, event.type(), TSI_ALWAYS);

  switch (signature) {
    case GST_INVALID:
      break;

    case GST_NO_GESTURE_FIRST_PRESSED:
      TouchDown(event, point, gestures.get());
      set_state(GS_PENDING_SYNTHETIC_CLICK);
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED:
      if (Click(event, point, gestures.get()))
        point.UpdateForTap();
      else
        PrependTapCancelGestureEvent(point, gestures.get());
      set_state(GS_NO_GESTURE);
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_STATIONARY:
      if (ScrollStart(event, point, gestures.get())) {
        PrependTapCancelGestureEvent(point, gestures.get());
        set_state(GS_SCROLL);
        if (ScrollUpdate(event, point, gestures.get()))
          point.UpdateForScroll();
      }
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED_PROCESSED:
      // TODO(rbyers): This should be able to trigger a TapCancel
      // if we moved far enough. crbug.com/146397
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED_HANDLED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_CANCELLED:
      PrependTapCancelGestureEvent(point, gestures.get());
      set_state(GS_NO_GESTURE);
      break;
    case GST_SCROLL_FIRST_MOVED:
      if (scroll_type_ == ST_VERTICAL ||
          scroll_type_ == ST_HORIZONTAL)
        BreakRailScroll(event, point, gestures.get());
      if (ScrollUpdate(event, point, gestures.get()))
        point.UpdateForScroll();
      break;
    case GST_SCROLL_FIRST_RELEASED:
    case GST_SCROLL_FIRST_CANCELLED:
      ScrollEnd(event, point, gestures.get());
      set_state(GS_NO_GESTURE);
      break;
    case GST_PENDING_SYNTHETIC_CLICK_SECOND_PRESSED:
      PrependTapCancelGestureEvent(point, gestures.get());
      if (IsSecondTouchDownCloseEnoughForTwoFingerTap()) {
        TwoFingerTouchDown(event, point, gestures.get());
        set_state(GS_PENDING_TWO_FINGER_TAP);
        break;
      }
      // fall through
    case GST_SCROLL_SECOND_PRESSED:
      PinchStart(event, point, gestures.get());
      set_state(GS_PINCH);
      break;
    case GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED:
      TwoFingerTouchReleased(event, point, gestures.get());
      set_state(GS_SCROLL);
      break;
    case GST_PENDING_TWO_FINGER_TAP_FIRST_MOVED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_MOVED:
      if (TwoFingerTouchMove(event, point, gestures.get()))
        set_state(GS_PINCH);
      break;
    case GST_PENDING_TWO_FINGER_TAP_FIRST_RELEASED_HANDLED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_RELEASED_HANDLED:
    case GST_PENDING_TWO_FINGER_TAP_FIRST_CANCELLED:
    case GST_PENDING_TWO_FINGER_TAP_SECOND_CANCELLED:
      scroll_type_ = ST_FREE;
      set_state(GS_SCROLL);
      break;
    case GST_PENDING_TWO_FINGER_TAP_THIRD_PRESSED:
      PinchStart(event, point, gestures.get());
      set_state(GS_PINCH);
      break;
    case GST_PINCH_FIRST_MOVED:
    case GST_PINCH_SECOND_MOVED:
    case GST_PINCH_THIRD_MOVED:
    case GST_PINCH_FOURTH_MOVED:
    case GST_PINCH_FIFTH_MOVED:
      if (PinchUpdate(event, point, gestures.get())) {
        for (int i = 0; i < point_count_; ++i)
          GetPointByPointId(i)->UpdateForScroll();
      }
      break;
    case GST_PINCH_FIRST_RELEASED:
    case GST_PINCH_SECOND_RELEASED:
    case GST_PINCH_THIRD_RELEASED:
    case GST_PINCH_FOURTH_RELEASED:
    case GST_PINCH_FIFTH_RELEASED:
    case GST_PINCH_FIRST_CANCELLED:
    case GST_PINCH_SECOND_CANCELLED:
    case GST_PINCH_THIRD_CANCELLED:
    case GST_PINCH_FOURTH_CANCELLED:
    case GST_PINCH_FIFTH_CANCELLED:
      // Was it a swipe? i.e. were all the fingers moving in the same
      // direction?
      MaybeSwipe(event, point, gestures.get());

      if (point_count_ == 2) {
        PinchEnd(event, point, gestures.get());

        // Once pinch ends, it should still be possible to scroll with the
        // remaining finger on the screen.
        set_state(GS_SCROLL);
      } else {
        // Nothing else to do if we have more than 2 fingers active, since after
        // the release/cancel, there are still enough fingers to do pinch.
        // pinch_distance_current_ and pinch_distance_start_ will be updated
        // when the bounding-box is updated.
      }
      ResetVelocities();
      break;
    case GST_PINCH_THIRD_PRESSED:
    case GST_PINCH_FOURTH_PRESSED:
    case GST_PINCH_FIFTH_PRESSED:
      pinch_distance_current_ = BoundingBoxDiagonal(bounding_box_);
      pinch_distance_start_ = pinch_distance_current_;
      break;
  }

  if (event.type() == ui::ET_TOUCH_RELEASED ||
      event.type() == ui::ET_TOUCH_CANCELLED)
    AppendEndGestureEvent(point, gestures.get());

  if (state_ != last_state)
    DVLOG(4) << "Gesture Sequence"
             << " State: " << state_
             << " touch id: " << event.touch_id();

  if (last_state == GS_PENDING_SYNTHETIC_CLICK && state_ != last_state)
    GetLongPressTimer()->Stop();

  // The set of point_ids must be contiguous and include 0.
  // When a touch point is released, all points with ids greater than the
  // released point must have their ids decremented, or the set of point_ids
  // could end up with gaps.
  if (event.type() == ui::ET_TOUCH_RELEASED ||
      event.type() == ui::ET_TOUCH_CANCELLED) {
    for (int i = 0; i < kMaxGesturePoints; ++i) {
      GesturePoint& iter_point = points_[i];
      if (iter_point.point_id() > point.point_id())
        iter_point.set_point_id(iter_point.point_id() - 1);
    }

    point.Reset();
    --point_count_;
    CHECK_GE(point_count_, 0);
    RecreateBoundingBox();
    if (state_ == GS_PINCH) {
      pinch_distance_current_ = BoundingBoxDiagonal(bounding_box_);
      pinch_distance_start_ = pinch_distance_current_;
    }
  }
  return gestures.release();
}

void GestureSequence::RecreateBoundingBox() {
  // TODO(sad): Recreating the bounding box at every touch-event is not very
  // efficient. This should be made better.
  if (point_count_ == 0) {
    bounding_box_.SetRect(0, 0, 0, 0);
  } else if (point_count_ == 1) {
    bounding_box_ = GetPointByPointId(0)->enclosing_rectangle();
  } else {
    int left = INT_MAX / 20, top = INT_MAX / 20;
    int right = INT_MIN / 20, bottom = INT_MIN / 20;
    for (int i = 0; i < kMaxGesturePoints; ++i) {
      if (!points_[i].in_use())
        continue;
      // Using the |enclosing_rectangle()| for the touch-points would be ideal.
      // However, this becomes brittle especially when a finger is in motion
      // because the change in radius can overshadow the actual change in
      // position. So the actual position of the point is used instead.
      const gfx::Point& point = points_[i].last_touch_position();
      left = std::min(left, point.x());
      right = std::max(right, point.x());
      top = std::min(top, point.y());
      bottom = std::max(bottom, point.y());
    }
    bounding_box_.SetRect(left, top, right - left, bottom - top);
  }
}

void GestureSequence::ResetVelocities() {
  for (int i = 0; i < kMaxGesturePoints; ++i) {
    if (points_[i].in_use())
      points_[i].ResetVelocity();
  }
}

////////////////////////////////////////////////////////////////////////////////
// GestureSequence Protected:

base::OneShotTimer<GestureSequence>* GestureSequence::CreateTimer() {
  return new base::OneShotTimer<GestureSequence>();
}

base::OneShotTimer<GestureSequence>* GestureSequence::GetLongPressTimer() {
  if (!long_press_timer_.get())
    long_press_timer_.reset(CreateTimer());
  return long_press_timer_.get();
}

////////////////////////////////////////////////////////////////////////////////
// GestureSequence Private:

GesturePoint& GestureSequence::GesturePointForEvent(
    const TouchEvent& event) {
  return points_[event.touch_id()];
}

GesturePoint* GestureSequence::GetPointByPointId(int point_id) {
  DCHECK(0 <= point_id && point_id < kMaxGesturePoints);
  for (int i = 0; i < kMaxGesturePoints; ++i) {
    GesturePoint& point = points_[i];
    if (point.in_use() && point.point_id() == point_id)
      return &point;
  }
  NOTREACHED();
  return NULL;
}

bool GestureSequence::IsSecondTouchDownCloseEnoughForTwoFingerTap() {
  gfx::Point p1 = GetPointByPointId(0)->last_touch_position();
  gfx::Point p2 = GetPointByPointId(1)->last_touch_position();
  double max_distance =
      GestureConfiguration::max_distance_for_two_finger_tap_in_pixels();
  double distance = (p1.x() - p2.x()) * (p1.x() - p2.x()) +
      (p1.y() - p2.y()) * (p1.y() - p2.y());
  if (distance < max_distance * max_distance)
    return true;
  return false;
}

GestureEvent* GestureSequence::CreateGestureEvent(
    const GestureEventDetails& details,
    const gfx::Point& location,
    int flags,
    base::Time timestamp,
    unsigned int touch_id_bitmask) {
  GestureEventDetails gesture_details(details);
  gesture_details.set_touch_points(point_count_);
  gesture_details.set_bounding_box(bounding_box_);
  return new GestureEvent(gesture_details.type(), location.x(), location.y(),
      flags, base::TimeDelta::FromMilliseconds(timestamp.ToDoubleT() * 1000),
      gesture_details, touch_id_bitmask);
}

void GestureSequence::AppendTapDownGestureEvent(const GesturePoint& point,
                                                Gestures* gestures) {
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_TAP_DOWN, 0, 0),
      point.first_touch_position(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

void GestureSequence::PrependTapCancelGestureEvent(const GesturePoint& point,
                                            Gestures* gestures) {
  gestures->insert(gestures->begin(), CreateGestureEvent(
    GestureEventDetails(ui::ET_GESTURE_TAP_CANCEL, 0, 0),
    point.first_touch_position(),
    flags_,
    base::Time::FromDoubleT(point.last_touch_time()),
    1 << point.touch_id()));
}

void GestureSequence::AppendBeginGestureEvent(const GesturePoint& point,
                                              Gestures* gestures) {
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_BEGIN, 0, 0),
      point.first_touch_position(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

void GestureSequence::AppendEndGestureEvent(const GesturePoint& point,
                                              Gestures* gestures) {
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_END, 0, 0),
      point.first_touch_position(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

void GestureSequence::AppendClickGestureEvent(const GesturePoint& point,
                                              int tap_count,
                                              Gestures* gestures) {
  gfx::Rect er = point.enclosing_rectangle();
  gfx::Point center = er.CenterPoint();
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_TAP, tap_count, 0),
      center,
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

void GestureSequence::AppendScrollGestureBegin(const GesturePoint& point,
                                               const gfx::Point& location,
                                               Gestures* gestures) {
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_SCROLL_BEGIN, 0, 0),
      location,
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

void GestureSequence::AppendScrollGestureEnd(const GesturePoint& point,
                                             const gfx::Point& location,
                                             Gestures* gestures,
                                             float x_velocity,
                                             float y_velocity) {
  float railed_x_velocity = x_velocity;
  float railed_y_velocity = y_velocity;

  if (scroll_type_ == ST_HORIZONTAL)
    railed_y_velocity = 0;
  else if (scroll_type_ == ST_VERTICAL)
    railed_x_velocity = 0;

  if (railed_x_velocity != 0 || railed_y_velocity != 0) {

    gestures->push_back(CreateGestureEvent(
        GestureEventDetails(ui::ET_SCROLL_FLING_START,
            CalibrateFlingVelocity(railed_x_velocity),
            CalibrateFlingVelocity(railed_y_velocity)),
        location,
        flags_,
        base::Time::FromDoubleT(point.last_touch_time()),
        1 << point.touch_id()));
  } else {
    gestures->push_back(CreateGestureEvent(
        GestureEventDetails(ui::ET_GESTURE_SCROLL_END, 0, 0),
        location,
        flags_,
        base::Time::FromDoubleT(point.last_touch_time()),
        1 << point.touch_id()));
  }
}

void GestureSequence::AppendScrollGestureUpdate(GesturePoint& point,
                                                Gestures* gestures) {
  gfx::Vector2d d;
  gfx::Point location;
  if (point_count_ == 1) {
    d = point.ScrollDelta();
    location = point.last_touch_position();
  } else {
    location = bounding_box_.CenterPoint();
    d = location - latest_multi_scroll_update_location_;
    latest_multi_scroll_update_location_ = location;
  }
  if (scroll_type_ == ST_HORIZONTAL)
    d.set_y(0);
  else if (scroll_type_ == ST_VERTICAL)
    d.set_x(0);
  if (d.IsZero())
    return;

  GestureEventDetails details(ui::ET_GESTURE_SCROLL_UPDATE, d.x(), d.y());
  details.SetScrollVelocity(
      scroll_type_ == ST_VERTICAL ? 0 : point.XVelocity(),
      scroll_type_ == ST_HORIZONTAL ? 0 : point.YVelocity());
  gestures->push_back(CreateGestureEvent(
      details,
      location,
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      ComputeTouchBitmask(points_)));
}

void GestureSequence::AppendPinchGestureBegin(const GesturePoint& p1,
                                              const GesturePoint& p2,
                                              Gestures* gestures) {
  gfx::Point center = bounding_box_.CenterPoint();
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_PINCH_BEGIN, 0, 0),
      center,
      flags_,
      base::Time::FromDoubleT(p1.last_touch_time()),
      1 << p1.touch_id() | 1 << p2.touch_id()));
}

void GestureSequence::AppendPinchGestureEnd(const GesturePoint& p1,
                                            const GesturePoint& p2,
                                            float scale,
                                            Gestures* gestures) {
  gfx::Point center = bounding_box_.CenterPoint();
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_PINCH_END, 0, 0),
      center,
      flags_,
      base::Time::FromDoubleT(p1.last_touch_time()),
      1 << p1.touch_id() | 1 << p2.touch_id()));
}

void GestureSequence::AppendPinchGestureUpdate(const GesturePoint& point,
                                               float scale,
                                               Gestures* gestures) {
  // TODO(sad): Compute rotation and include it in delta_y.
  // http://crbug.com/113145
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_PINCH_UPDATE, scale, 0),
      bounding_box_.CenterPoint(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      ComputeTouchBitmask(points_)));
}

void GestureSequence::AppendSwipeGesture(const GesturePoint& point,
                                         int swipe_x,
                                         int swipe_y,
                                         Gestures* gestures) {
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_MULTIFINGER_SWIPE, swipe_x, swipe_y),
      bounding_box_.CenterPoint(),
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      ComputeTouchBitmask(points_)));
}

void GestureSequence::AppendTwoFingerTapGestureEvent(Gestures* gestures) {
  const GesturePoint* point = GetPointByPointId(0);
  const gfx::Rect rect = point->enclosing_rectangle();
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_TWO_FINGER_TAP,
                          rect.width(),
                          rect.height()),
      point->enclosing_rectangle().CenterPoint(),
      flags_,
      base::Time::FromDoubleT(point->last_touch_time()),
      1 << point->touch_id()));
}

bool GestureSequence::Click(const TouchEvent& event,
                            const GesturePoint& point,
                            Gestures* gestures) {
  DCHECK(state_ == GS_PENDING_SYNTHETIC_CLICK);
  if (point.IsInClickWindow(event)) {
    bool double_tap = point.IsInDoubleClickWindow(event);
    AppendClickGestureEvent(point, double_tap ? 2 : 1, gestures);
    return true;
  } else if (point.IsInsideManhattanSquare(event) &&
      !GetLongPressTimer()->IsRunning()) {
    AppendLongTapGestureEvent(point, gestures);
  }
  return false;
}

bool GestureSequence::ScrollStart(const TouchEvent& event,
                                  GesturePoint& point,
                                  Gestures* gestures) {
  DCHECK(state_ == GS_PENDING_SYNTHETIC_CLICK);
  if (!point.IsConsistentScrollingActionUnderway() &&
      !point.IsInScrollWindow(event))
    return false;
  AppendScrollGestureBegin(point, point.first_touch_position(), gestures);
  if (point.IsInHorizontalRailWindow())
    scroll_type_ = ST_HORIZONTAL;
  else if (point.IsInVerticalRailWindow())
    scroll_type_ = ST_VERTICAL;
  else
    scroll_type_ = ST_FREE;
  return true;
}

void GestureSequence::BreakRailScroll(const TouchEvent& event,
                                      GesturePoint& point,
                                      Gestures* gestures) {
  DCHECK(state_ == GS_SCROLL);
  if (scroll_type_ == ST_HORIZONTAL &&
      point.BreaksHorizontalRail())
    scroll_type_ = ST_FREE;
  else if (scroll_type_ == ST_VERTICAL &&
           point.BreaksVerticalRail())
    scroll_type_ = ST_FREE;
}

bool GestureSequence::ScrollUpdate(const TouchEvent& event,
                                   GesturePoint& point,
                                   Gestures* gestures) {
  DCHECK(state_ == GS_SCROLL);
  if (!point.DidScroll(event, 0))
    return false;
  AppendScrollGestureUpdate(point, gestures);
  return true;
}

bool GestureSequence::TouchDown(const TouchEvent& event,
                                const GesturePoint& point,
                                Gestures* gestures) {
  DCHECK(state_ == GS_NO_GESTURE);
  AppendTapDownGestureEvent(point, gestures);
  GetLongPressTimer()->Start(
      FROM_HERE,
      base::TimeDelta::FromMilliseconds(
          GestureConfiguration::long_press_time_in_seconds() * 1000),
      this,
      &GestureSequence::AppendLongPressGestureEvent);
  return true;
}

bool GestureSequence::TwoFingerTouchDown(const TouchEvent& event,
                                         const GesturePoint& point,
                                         Gestures* gestures) {
  DCHECK(state_ == GS_PENDING_SYNTHETIC_CLICK || state_ == GS_SCROLL);
  if (state_ == GS_SCROLL) {
    AppendScrollGestureEnd(point, point.last_touch_position(), gestures,
        0.f, 0.f);
  }
  second_touch_time_ = event.time_stamp();
  return true;
}

bool GestureSequence::TwoFingerTouchMove(const TouchEvent& event,
                                         const GesturePoint& point,
                                         Gestures* gestures) {
  DCHECK(state_ == GS_PENDING_TWO_FINGER_TAP);

  base::TimeDelta time_delta = event.time_stamp() - second_touch_time_;
  base::TimeDelta max_delta = base::TimeDelta::FromMilliseconds(1000 *
      ui::GestureConfiguration::max_touch_down_duration_in_seconds_for_click());
  if (time_delta > max_delta || !point.IsInsideManhattanSquare(event)) {
    PinchStart(event, point, gestures);
    return true;
  }
  return false;
}

bool GestureSequence::TwoFingerTouchReleased(const TouchEvent& event,
                                             const GesturePoint& point,
                                             Gestures* gestures) {
  DCHECK(state_ == GS_PENDING_TWO_FINGER_TAP);
  base::TimeDelta time_delta = event.time_stamp() - second_touch_time_;
  base::TimeDelta max_delta = base::TimeDelta::FromMilliseconds(1000 *
      ui::GestureConfiguration::max_touch_down_duration_in_seconds_for_click());
  if (time_delta < max_delta && point.IsInsideManhattanSquare(event))
    AppendTwoFingerTapGestureEvent(gestures);
  return true;
}

void GestureSequence::AppendLongPressGestureEvent() {
  const GesturePoint* point = GetPointByPointId(0);
  scoped_ptr<GestureEvent> gesture(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_LONG_PRESS, 0, 0),
      point->first_touch_position(),
      flags_,
      base::Time::FromDoubleT(point->last_touch_time()),
      1 << point->touch_id()));
  helper_->DispatchLongPressGestureEvent(gesture.get());
}

void GestureSequence::AppendLongTapGestureEvent(const GesturePoint& point,
                                                Gestures* gestures) {
  gfx::Rect er = point.enclosing_rectangle();
  gfx::Point center = er.CenterPoint();
  gestures->push_back(CreateGestureEvent(
      GestureEventDetails(ui::ET_GESTURE_LONG_TAP, 0, 0),
      center,
      flags_,
      base::Time::FromDoubleT(point.last_touch_time()),
      1 << point.touch_id()));
}

bool GestureSequence::ScrollEnd(const TouchEvent& event,
                                GesturePoint& point,
                                Gestures* gestures) {
  DCHECK(state_ == GS_SCROLL);
  if (point.IsInFlickWindow(event)) {
    AppendScrollGestureEnd(point, point.last_touch_position(), gestures,
        point.XVelocity(), point.YVelocity());
  } else {
    AppendScrollGestureEnd(point, point.last_touch_position(), gestures,
        0.f, 0.f);
  }
  return true;
}

bool GestureSequence::PinchStart(const TouchEvent& event,
                                 const GesturePoint& point,
                                 Gestures* gestures) {
  DCHECK(state_ == GS_SCROLL ||
         state_ == GS_PENDING_SYNTHETIC_CLICK ||
         state_ == GS_PENDING_TWO_FINGER_TAP);

  // Once pinch starts, we immediately break rail scroll.
  scroll_type_ = ST_FREE;

  const GesturePoint* point1 = GetPointByPointId(0);
  const GesturePoint* point2 = GetPointByPointId(1);

  pinch_distance_current_ = BoundingBoxDiagonal(bounding_box_);
  pinch_distance_start_ = pinch_distance_current_;
  latest_multi_scroll_update_location_ = bounding_box_.CenterPoint();
  AppendPinchGestureBegin(*point1, *point2, gestures);

  if (state_ == GS_PENDING_SYNTHETIC_CLICK ||
      state_ == GS_PENDING_TWO_FINGER_TAP) {
    gfx::Point center = bounding_box_.CenterPoint();
    AppendScrollGestureBegin(point, center, gestures);
  }

  return true;
}

bool GestureSequence::PinchUpdate(const TouchEvent& event,
                                  GesturePoint& point,
                                  Gestures* gestures) {
  DCHECK(state_ == GS_PINCH);

  // It is possible that the none of the touch-points changed their position,
  // but their radii changed, and that caused the bounding box to also change.
  // But in such cases, we do not want to either pinch or scroll.
  // To avoid small jiggles, it is also necessary to make sure that at least one
  // of the fingers moved enough before a pinch or scroll update is created.
  bool did_scroll = false;
  for (int i = 0; i < kMaxGesturePoints; ++i) {
    if (!points_[i].in_use() || !points_[i].DidScroll(event, 0))
      continue;
    did_scroll = true;
    break;
  }

  if (!did_scroll)
    return false;

  float distance = BoundingBoxDiagonal(bounding_box_);

  if (abs(distance - pinch_distance_current_) >=
      GestureConfiguration::min_pinch_update_distance_in_pixels()) {
    AppendPinchGestureUpdate(point,
        distance / pinch_distance_current_, gestures);
    pinch_distance_current_ = distance;
  }
  AppendScrollGestureUpdate(point, gestures);

  return true;
}

bool GestureSequence::PinchEnd(const TouchEvent& event,
                               const GesturePoint& point,
                               Gestures* gestures) {
  DCHECK(state_ == GS_PINCH);

  GesturePoint* point1 = GetPointByPointId(0);
  GesturePoint* point2 = GetPointByPointId(1);

  float distance = BoundingBoxDiagonal(bounding_box_);
  AppendPinchGestureEnd(*point1, *point2,
      distance / pinch_distance_start_, gestures);

  pinch_distance_start_ = 0;
  pinch_distance_current_ = 0;
  return true;
}

bool GestureSequence::MaybeSwipe(const TouchEvent& event,
                                 const GesturePoint& point,
                                 Gestures* gestures) {
  DCHECK(state_ == GS_PINCH);
  float velocity_x = 0.f, velocity_y = 0.f;
  bool swipe_x = true, swipe_y = true;
  int sign_x = 0, sign_y = 0;
  int i = 0;

  for (i = 0; i < kMaxGesturePoints; ++i) {
    if (points_[i].in_use())
      break;
  }
  DCHECK(i < kMaxGesturePoints);

  velocity_x = points_[i].XVelocity();
  velocity_y = points_[i].YVelocity();
  sign_x = velocity_x < 0.f ? -1 : 1;
  sign_y = velocity_y < 0.f ? -1 : 1;

  for (++i; i < kMaxGesturePoints; ++i) {
    if (!points_[i].in_use())
      continue;

    if (sign_x * points_[i].XVelocity() < 0)
      swipe_x = false;

    if (sign_y * points_[i].YVelocity() < 0)
      swipe_y = false;

    velocity_x += points_[i].XVelocity();
    velocity_y += points_[i].YVelocity();
  }

  float min_velocity = GestureConfiguration::min_swipe_speed();
  min_velocity *= min_velocity;

  velocity_x = fabs(velocity_x / point_count_);
  velocity_y = fabs(velocity_y / point_count_);
  if (velocity_x < min_velocity)
    swipe_x = false;
  if (velocity_y < min_velocity)
    swipe_y = false;

  if (!swipe_x && !swipe_y)
    return false;

  if (!swipe_x)
    velocity_x = 0.001f;
  if (!swipe_y)
    velocity_y = 0.001f;

  float ratio = velocity_x > velocity_y ? velocity_x / velocity_y :
                                          velocity_y / velocity_x;
  if (ratio < GestureConfiguration::max_swipe_deviation_ratio())
    return false;

  if (velocity_x > velocity_y)
    sign_y = 0;
  else
    sign_x = 0;

  AppendSwipeGesture(point, sign_x, sign_y, gestures);

  return true;
}

void GestureSequence::StopLongPressTimerIfRequired(const TouchEvent& event) {
  if (!GetLongPressTimer()->IsRunning() ||
      event.type() != ui::ET_TOUCH_MOVED)
    return;

  // Since long press timer has been started, there should be a non-NULL point.
  const GesturePoint* point = GetPointByPointId(0);
  if (!ui::gestures::IsInsideManhattanSquare(point->first_touch_position(),
      event.location()))
    GetLongPressTimer()->Stop();
}

}  // namespace ui
