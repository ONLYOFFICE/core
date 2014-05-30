// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_
#define UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_

#include <map>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/gestures/gesture_recognizer.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/point.h"

namespace ui {
class GestureConsumer;
class GestureEvent;
class GestureEventHelper;
class GestureSequence;
class TouchEvent;

class UI_EXPORT GestureRecognizerImpl : public GestureRecognizer {
 public:
  typedef std::map<int, GestureConsumer*> TouchIdToConsumerMap;

  explicit GestureRecognizerImpl(GestureEventHelper* helper);
  virtual ~GestureRecognizerImpl();

  // Overridden from GestureRecognizer
  virtual GestureConsumer* GetTouchLockedTarget(TouchEvent* event) OVERRIDE;
  virtual GestureConsumer* GetTargetForGestureEvent(
      GestureEvent* event) OVERRIDE;
  virtual GestureConsumer* GetTargetForLocation(
      const gfx::Point& location) OVERRIDE;
  virtual void TransferEventsTo(GestureConsumer* current_consumer,
                                GestureConsumer* new_consumer) OVERRIDE;
  virtual bool GetLastTouchPointForTarget(GestureConsumer* consumer,
                                          gfx::Point* point) OVERRIDE;

 protected:
  virtual GestureSequence* CreateSequence(GestureEventHelper* helper);
  virtual GestureSequence* GetGestureSequenceForConsumer(GestureConsumer* c);

 private:
  // Sets up the target consumer for gestures based on the touch-event.
  void SetupTargets(const TouchEvent& event, GestureConsumer* consumer);

  // Overridden from GestureRecognizer
  virtual Gestures* ProcessTouchEventForGesture(
      const TouchEvent& event,
      ui::EventResult result,
      GestureConsumer* target) OVERRIDE;
  virtual void CleanupStateForConsumer(GestureConsumer* consumer) OVERRIDE;

  std::map<GestureConsumer*, GestureSequence*> consumer_sequence_;

  // Both |touch_id_target_| and |touch_id_target_for_gestures_| map a touch-id
  // to its target window.  touch-ids are removed from |touch_id_target_| on
  // ET_TOUCH_RELEASE and ET_TOUCH_CANCEL. |touch_id_target_for_gestures_| are
  // removed in ConsumerDestroyed().
  TouchIdToConsumerMap touch_id_target_;
  TouchIdToConsumerMap touch_id_target_for_gestures_;

  // Touches cancelled by touch capture are routed to the
  // gesture_consumer_ignorer_.
  scoped_ptr<GestureConsumer> gesture_consumer_ignorer_;
  GestureEventHelper* helper_;

  DISALLOW_COPY_AND_ASSIGN(GestureRecognizerImpl);
};

}  // namespace ui

#endif  // UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_
