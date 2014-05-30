// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ANIMATION_ANIMATION_CONTAINER_OBSERVER_H_
#define UI_BASE_ANIMATION_ANIMATION_CONTAINER_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class AnimationContainer;

// The observer is notified after every update of the animations managed by
// the container.
class UI_EXPORT AnimationContainerObserver {
 public:
  // Invoked on every tick of the timer managed by the container and after
  // all the animations have updated.
  virtual void AnimationContainerProgressed(
      AnimationContainer* container) = 0;

  // Invoked when no more animations are being managed by this container.
  virtual void AnimationContainerEmpty(AnimationContainer* container) = 0;

 protected:
  virtual ~AnimationContainerObserver() {}
};

}  // namespace ui

#endif  // UI_BASE_ANIMATION_ANIMATION_CONTAINER_OBSERVER_H_
