// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TRANSFORM_ANIMATION_CURVE_ADAPTER_H_
#define UI_COMPOSITOR_TRANSFORM_ANIMATION_CURVE_ADAPTER_H_

#include "base/time.h"
#include "cc/animation/animation_curve.h"
#include "ui/base/animation/tween.h"
#include "ui/gfx/transform.h"
#include "ui/gfx/transform_util.h"

namespace ui {

class TransformAnimationCurveAdapter :public cc::TransformAnimationCurve {
 public:
  TransformAnimationCurveAdapter(Tween::Type tween_type,
                                 gfx::Transform intial_value,
                                 gfx::Transform target_value,
                                 base::TimeDelta duration);

  virtual ~TransformAnimationCurveAdapter();

  // TransformAnimationCurve implementation.
  virtual double Duration() const OVERRIDE;
  virtual scoped_ptr<AnimationCurve> Clone() const OVERRIDE;
  virtual gfx::Transform GetValue(double t) const OVERRIDE;

 private:
  Tween::Type tween_type_;
  gfx::Transform initial_value_;
  gfx::Transform target_value_;
  gfx::DecomposedTransform decomposed_initial_value_;
  gfx::DecomposedTransform decomposed_target_value_;
  base::TimeDelta duration_;
};

}  // namespace ui

#endif  // UI_COMPOSITOR_TRANSFORM_ANIMATION_CURVE_ADAPTER_H_

