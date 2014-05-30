// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/pulsing_block_view.h"

#include <vector>

#include "base/rand_util.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animation_element.h"
#include "ui/compositor/layer_animation_sequence.h"
#include "ui/compositor/layer_animator.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/transform_util.h"

namespace {

const SkColor kBlockColor = SkColorSetRGB(225, 225, 225);
const int kBlockSize = 64;

const int kAnimationDurationInMs = 600;
const float kAnimationOpacity[] = { 0.4f, 0.8f, 0.4f };
const float kAnimationScale[] = { 0.8f, 1.0f, 0.8f };

void SchedulePulsingAnimation(ui::Layer* layer) {
  DCHECK(layer);
  DCHECK_EQ(arraysize(kAnimationOpacity), arraysize(kAnimationScale));

  scoped_ptr<ui::LayerAnimationSequence> opacity_sequence(
      new ui::LayerAnimationSequence());
  scoped_ptr<ui::LayerAnimationSequence> transform_sequence(
      new ui::LayerAnimationSequence());

  // The animations loop infinitely.
  opacity_sequence->set_is_cyclic(true);
  transform_sequence->set_is_cyclic(true);

  const gfx::Rect local_bounds(layer->bounds().size());
  for (size_t i = 0; i < arraysize(kAnimationOpacity); ++i) {
    opacity_sequence->AddElement(
        ui::LayerAnimationElement::CreateOpacityElement(
            kAnimationOpacity[i],
            base::TimeDelta::FromMilliseconds(kAnimationDurationInMs)));
    transform_sequence->AddElement(
        ui::LayerAnimationElement::CreateTransformElement(
            gfx::GetScaleTransform(local_bounds.CenterPoint(),
                                   kAnimationScale[i]),
            base::TimeDelta::FromMilliseconds(kAnimationDurationInMs)));
  }

  ui::LayerAnimationElement::AnimatableProperties opacity_properties;
  opacity_properties.insert(ui::LayerAnimationElement::OPACITY);
  opacity_sequence->AddElement(
      ui::LayerAnimationElement::CreatePauseElement(
          opacity_properties,
           base::TimeDelta::FromMilliseconds(kAnimationDurationInMs)));

  ui::LayerAnimationElement::AnimatableProperties transform_properties;
  transform_properties.insert(ui::LayerAnimationElement::TRANSFORM);
  transform_sequence->AddElement(
      ui::LayerAnimationElement::CreatePauseElement(
          transform_properties,
          base::TimeDelta::FromMilliseconds(kAnimationDurationInMs)));

  std::vector<ui::LayerAnimationSequence*> animations;
  animations.push_back(opacity_sequence.release());
  animations.push_back(transform_sequence.release());
  layer->GetAnimator()->ScheduleTogether(animations);
}

}  // namespace

namespace app_list {

PulsingBlockView::PulsingBlockView(const gfx::Size& size, bool start_delay) {
#if defined(USE_AURA)
  SetPaintToLayer(true);
  SetFillsBoundsOpaquely(false);

  const int max_delay = kAnimationDurationInMs * arraysize(kAnimationOpacity);
  const int delay = start_delay ? base::RandInt(0, max_delay) : 0;
  start_delay_timer_.Start(
      FROM_HERE,
      base::TimeDelta::FromMilliseconds(delay),
      this, &PulsingBlockView::OnStartDelayTimer);
#else
  NOTREACHED() << "Pulsing animation is not supported on Windows";
#endif
}

PulsingBlockView::~PulsingBlockView() {
}

void PulsingBlockView::OnStartDelayTimer() {
  SchedulePulsingAnimation(layer());
}

void PulsingBlockView::OnPaint(gfx::Canvas* canvas) {
  gfx::Rect rect(GetContentsBounds());
  rect.ClampToCenteredSize(gfx::Size(kBlockSize, kBlockSize));
  canvas->FillRect(rect, kBlockColor);
}

}  // namespace app_list
