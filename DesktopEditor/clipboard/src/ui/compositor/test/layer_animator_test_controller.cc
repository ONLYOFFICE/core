// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/animation/animation.h"
#include "ui/compositor/layer_animation_sequence.h"
#include "ui/compositor/test/layer_animator_test_controller.h"

namespace ui {

LayerAnimatorTestController::LayerAnimatorTestController(
    scoped_refptr<LayerAnimator> animator)
    : animator_(animator) {
}

LayerAnimatorTestController::~LayerAnimatorTestController() {
}

LayerAnimationSequence* LayerAnimatorTestController::GetRunningSequence(
    LayerAnimationElement::AnimatableProperty property) {
  LayerAnimator::RunningAnimation* running_animation =
      animator_->GetRunningAnimation(property);
  if (running_animation)
    return running_animation->sequence();
  else
    return NULL;
}

void LayerAnimatorTestController::StartThreadedAnimationsIfNeeded() {
  std::vector<cc::Animation::TargetProperty> threaded_properties;
  threaded_properties.push_back(cc::Animation::Opacity);
  threaded_properties.push_back(cc::Animation::Transform);

  for (size_t i = 0; i < threaded_properties.size(); i++) {
    LayerAnimationElement::AnimatableProperty animatable_property =
        LayerAnimationElement::ToAnimatableProperty(threaded_properties[i]);
    LayerAnimationSequence* sequence = GetRunningSequence(animatable_property);
    if (!sequence)
      continue;

    LayerAnimationElement* element = sequence->CurrentElement();
    if (element->properties().find(animatable_property) ==
        element->properties().end())
      continue;

    if (!element->Started() ||
        element->effective_start_time() != base::TimeTicks())
      continue;

    animator_->OnThreadedAnimationStarted(cc::AnimationEvent(
        cc::AnimationEvent::Started,
        0,
        element->animation_group_id(),
        threaded_properties[i],
        (base::TimeTicks::Now() - base::TimeTicks()).InSecondsF()));
  }
}

}  // namespace ui
