// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/transform_animation_curve_adapter.h"

namespace ui {

TransformAnimationCurveAdapter::TransformAnimationCurveAdapter(
    Tween::Type tween_type,
    gfx::Transform initial_value,
    gfx::Transform target_value,
    base::TimeDelta duration)
    : tween_type_(tween_type),
      initial_value_(initial_value),
      target_value_(target_value),
      duration_(duration) {
  gfx::DecomposeTransform(&decomposed_initial_value_, initial_value_);
  gfx::DecomposeTransform(&decomposed_target_value_, target_value_);
}

TransformAnimationCurveAdapter::~TransformAnimationCurveAdapter() {
}

double TransformAnimationCurveAdapter::Duration() const {
  return duration_.InSecondsF();
}

scoped_ptr<cc::AnimationCurve> TransformAnimationCurveAdapter::Clone() const {
  scoped_ptr<TransformAnimationCurveAdapter> to_return(
      new TransformAnimationCurveAdapter(tween_type_,
                                         initial_value_,
                                         target_value_,
                                         duration_));
  return to_return.PassAs<cc::AnimationCurve>();
}

gfx::Transform TransformAnimationCurveAdapter::GetValue(
    double t) const {
  if (t >= duration_.InSecondsF())
    return target_value_;
  if (t <= 0.0)
    return initial_value_;
  double progress = t / duration_.InSecondsF();

  gfx::DecomposedTransform to_return;
  gfx::BlendDecomposedTransforms(&to_return,
                                 decomposed_target_value_,
                                 decomposed_initial_value_,
                                 Tween::CalculateValue(tween_type_, progress));
  return gfx::ComposeTransform(to_return);
}

}  // namespace ui
