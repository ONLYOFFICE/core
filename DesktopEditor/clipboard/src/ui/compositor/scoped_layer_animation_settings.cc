// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/scoped_layer_animation_settings.h"

#include "ui/compositor/layer_animation_observer.h"
#include "ui/compositor/layer_animator.h"

namespace {

const int kDefaultTransitionDurationMs = 200;

}  // namespace

namespace ui {

ScopedLayerAnimationSettings::ScopedLayerAnimationSettings(
    LayerAnimator* animator)
    : animator_(animator),
      old_transition_duration_(animator->transition_duration_),
      old_tween_type_(animator->tween_type()),
      old_preemption_strategy_(animator->preemption_strategy()) {
  SetTransitionDuration(
      base::TimeDelta::FromMilliseconds(kDefaultTransitionDurationMs));
}

ScopedLayerAnimationSettings::~ScopedLayerAnimationSettings() {
  animator_->transition_duration_ = old_transition_duration_;
  animator_->set_tween_type(old_tween_type_);
  animator_->set_preemption_strategy(old_preemption_strategy_);

  for (std::set<ImplicitAnimationObserver*>::const_iterator i =
       observers_.begin(); i != observers_.end(); ++i) {
    animator_->observers_.RemoveObserver(*i);
    (*i)->SetActive(true);
  }
}

void ScopedLayerAnimationSettings::AddObserver(
    ImplicitAnimationObserver* observer) {
  observers_.insert(observer);
  animator_->AddObserver(observer);
}

void ScopedLayerAnimationSettings::SetTransitionDuration(
    base::TimeDelta duration) {
  animator_->transition_duration_ = duration;
}

base::TimeDelta ScopedLayerAnimationSettings::GetTransitionDuration() const {
  return animator_->transition_duration_;
}

void ScopedLayerAnimationSettings::SetTweenType(Tween::Type tween_type) {
  animator_->set_tween_type(tween_type);
}

Tween::Type ScopedLayerAnimationSettings::GetTweenType() const {
  return animator_->tween_type();
}

void ScopedLayerAnimationSettings::SetPreemptionStrategy(
    LayerAnimator::PreemptionStrategy strategy) {
  animator_->set_preemption_strategy(strategy);
}

LayerAnimator::PreemptionStrategy
ScopedLayerAnimationSettings::GetPreemptionStrategy() const {
  return animator_->preemption_strategy();
}

}  // namespace ui
