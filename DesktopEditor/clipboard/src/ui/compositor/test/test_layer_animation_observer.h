// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TEST_TEST_LAYER_ANIMATION_OBSERVER_H_
#define UI_COMPOSITOR_TEST_TEST_LAYER_ANIMATION_OBSERVER_H_

#include "base/compiler_specific.h"
#include "ui/compositor/layer_animation_observer.h"

namespace ui {

class LayerAnimationSequence;

// Listens to animation ended notifications. Remembers the last sequence that
// it was notified about.
class TestLayerAnimationObserver : public LayerAnimationObserver {
 public:
  TestLayerAnimationObserver();
  virtual ~TestLayerAnimationObserver();

  virtual void OnLayerAnimationEnded(
      LayerAnimationSequence* sequence) OVERRIDE;

  virtual void OnLayerAnimationAborted(
      LayerAnimationSequence* sequence) OVERRIDE;

  virtual void OnLayerAnimationScheduled(
      LayerAnimationSequence* sequence) OVERRIDE;

  virtual bool RequiresNotificationWhenAnimatorDestroyed() const OVERRIDE;

  const LayerAnimationSequence* last_ended_sequence() const {
    return last_ended_sequence_;
  }

  const LayerAnimationSequence* last_scheduled_sequence() const {
    return last_scheduled_sequence_;
  }

  const LayerAnimationSequence* last_aborted_sequence() const {
    return last_aborted_sequence_;
  }

  void set_requires_notification_when_animator_destroyed(bool value) {
    requires_notification_when_animator_destroyed_ = value;
  }

 private:
  const LayerAnimationSequence* last_ended_sequence_;
  const LayerAnimationSequence* last_scheduled_sequence_;
  const LayerAnimationSequence* last_aborted_sequence_;
  bool requires_notification_when_animator_destroyed_;

  // Copy and assign are allowed.
};

}  // namespace ui

#endif  // UI_COMPOSITOR_TEST_TEST_LAYER_ANIMATION_OBSERVER_H_
