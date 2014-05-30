// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GESTURES_GESTURE_SEQUENCE_H_
#define UI_BASE_GESTURES_GESTURE_SEQUENCE_H_

#include "base/timer.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/gestures/gesture_point.h"
#include "ui/base/gestures/gesture_recognizer.h"
#include "ui/gfx/rect.h"

namespace ui {
class TouchEvent;
class GestureEvent;

// Gesture state.
enum GestureState {
  GS_NO_GESTURE,
  GS_PENDING_SYNTHETIC_CLICK,
  GS_SCROLL,
  GS_PINCH,
  GS_PENDING_TWO_FINGER_TAP,
};

enum ScrollType {
  ST_FREE,
  ST_HORIZONTAL,
  ST_VERTICAL,
};

// A GestureSequence recognizes gestures from touch sequences.
class UI_EXPORT GestureSequence {
 public:
  // Maximum number of points in a single gesture.
  static const int kMaxGesturePoints = 12;

  explicit GestureSequence(GestureEventHelper* consumer);
  virtual ~GestureSequence();

  typedef GestureRecognizer::Gestures Gestures;

  // Invoked for each touch event that could contribute to the current gesture.
  // Returns list of  zero or more GestureEvents identified after processing
  // TouchEvent.
  // Caller would be responsible for freeing up Gestures.
  virtual Gestures* ProcessTouchEventForGesture(const TouchEvent& event,
                                                EventResult status);
  const GesturePoint* points() const { return points_; }
  int point_count() const { return point_count_; }

  const gfx::Point& last_touch_location() const { return last_touch_location_; }

 protected:
  virtual base::OneShotTimer<GestureSequence>* CreateTimer();
  base::OneShotTimer<GestureSequence>* GetLongPressTimer();

 private:
  // Recreates the axis-aligned bounding box that contains all the touch-points
  // at their most recent position.
  void RecreateBoundingBox();

  void ResetVelocities();

  GesturePoint& GesturePointForEvent(const TouchEvent& event);

  // Do a linear scan through points_ to find the GesturePoint
  // with id |point_id|.
  GesturePoint* GetPointByPointId(int point_id);

  bool IsSecondTouchDownCloseEnoughForTwoFingerTap();

  // Creates a gesture event with the specified parameters. The function
  // includes some common information (e.g. number of touch-points in the
  // gesture etc.) in the gesture event as well.
  GestureEvent* CreateGestureEvent(const GestureEventDetails& details,
                                   const gfx::Point& location,
                                   int flags,
                                   base::Time timestamp,
                                   unsigned int touch_id_bitmask);

  // Functions to be called to add GestureEvents, after successful recognition.

  // Tap gestures.
  void AppendTapDownGestureEvent(const GesturePoint& point, Gestures* gestures);
  void PrependTapCancelGestureEvent(const GesturePoint& point,
                                   Gestures* gestures);
  void AppendBeginGestureEvent(const GesturePoint& point, Gestures* gestures);
  void AppendEndGestureEvent(const GesturePoint& point, Gestures* gestures);
  void AppendClickGestureEvent(const GesturePoint& point,
                               int tap_count,
                               Gestures* gestures);
  void AppendDoubleClickGestureEvent(const GesturePoint& point,
                                     Gestures* gestures);
  void AppendLongPressGestureEvent();
  void AppendLongTapGestureEvent(const GesturePoint& point,
                                 Gestures* gestures);

  // Scroll gestures.
  void AppendScrollGestureBegin(const GesturePoint& point,
                                const gfx::Point& location,
                                Gestures* gestures);
  void AppendScrollGestureEnd(const GesturePoint& point,
                              const gfx::Point& location,
                              Gestures* gestures,
                              float x_velocity,
                              float y_velocity);
  void AppendScrollGestureUpdate(GesturePoint& point,
                                 Gestures* gestures);

  // Pinch gestures.
  void AppendPinchGestureBegin(const GesturePoint& p1,
                               const GesturePoint& p2,
                               Gestures* gestures);
  void AppendPinchGestureEnd(const GesturePoint& p1,
                             const GesturePoint& p2,
                             float scale,
                             Gestures* gestures);
  void AppendPinchGestureUpdate(const GesturePoint& point,
                                float scale,
                                Gestures* gestures);
  void AppendSwipeGesture(const GesturePoint& point,
                          int swipe_x,
                          int swipe_y,
                          Gestures* gestures);
  void AppendTwoFingerTapGestureEvent(Gestures* gestures);

  void set_state(const GestureState state) { state_ = state; }

  // Various GestureTransitionFunctions for a signature.
  // There is, 1:many mapping from GestureTransitionFunction to Signature
  // But a Signature have only one GestureTransitionFunction.
  bool Click(const TouchEvent& event,
             const GesturePoint& point,
             Gestures* gestures);
  bool ScrollStart(const TouchEvent& event,
                   GesturePoint& point,
                   Gestures* gestures);
  void BreakRailScroll(const TouchEvent& event,
                       GesturePoint& point,
                       Gestures* gestures);
  bool ScrollUpdate(const TouchEvent& event,
                    GesturePoint& point,
                    Gestures* gestures);
  bool TouchDown(const TouchEvent& event,
                 const GesturePoint& point,
                 Gestures* gestures);
  bool TwoFingerTouchDown(const TouchEvent& event,
                          const GesturePoint& point,
                          Gestures* gestures);
  bool TwoFingerTouchMove(const TouchEvent& event,
                          const GesturePoint& point,
                          Gestures* gestures);
  bool TwoFingerTouchReleased(const TouchEvent& event,
                              const GesturePoint& point,
                              Gestures* gestures);
  bool ScrollEnd(const TouchEvent& event,
                 GesturePoint& point,
                 Gestures* gestures);
  bool PinchStart(const TouchEvent& event,
                  const GesturePoint& point,
                  Gestures* gestures);
  bool PinchUpdate(const TouchEvent& event,
                   GesturePoint& point,
                   Gestures* gestures);
  bool PinchEnd(const TouchEvent& event,
                const GesturePoint& point,
                Gestures* gestures);
  bool MaybeSwipe(const TouchEvent& event,
                  const GesturePoint& point,
                  Gestures* gestures);

  void StopLongPressTimerIfRequired(const TouchEvent& event);

  // Current state of gesture recognizer.
  GestureState state_;

  // ui::EventFlags.
  int flags_;

  // We maintain the smallest axis-aligned rectangle that contains all the
  // current touch-points. This box is updated after every touch-event.
  gfx::Rect bounding_box_;

  // The center of the bounding box used in the latest multi-finger scroll
  // update gesture.
  gfx::Point latest_multi_scroll_update_location_;

  // For pinch, the 'distance' represents the diagonal distance of
  // |bounding_box_|.

  // The distance between the two points at PINCH_START.
  float pinch_distance_start_;

  // This distance is updated after each PINCH_UPDATE.
  float pinch_distance_current_;

  // This is the time when second touch down was received. Used for determining
  // if a two finger double tap has happened.
  base::TimeDelta second_touch_time_;

  ScrollType scroll_type_;
  scoped_ptr<base::OneShotTimer<GestureSequence> > long_press_timer_;

  GesturePoint points_[kMaxGesturePoints];
  int point_count_;

  // Location of the last touch event.
  gfx::Point last_touch_location_;

  GestureEventHelper* helper_;

  DISALLOW_COPY_AND_ASSIGN(GestureSequence);
};

}  // namespace ui

#endif  // UI_BASE_GESTURES_GESTURE_SEQUENCE_H_
