// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/animation/animation.h"

#include "ui/base/animation/animation_container.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/tween.h"
#include "ui/gfx/rect.h"

#if defined(OS_WIN)
#include "base/win/windows_version.h"
#endif

namespace ui {

Animation::Animation(base::TimeDelta timer_interval)
    : timer_interval_(timer_interval),
      is_animating_(false),
      delegate_(NULL) {
}

Animation::~Animation() {
  // Don't send out notification from the destructor. Chances are the delegate
  // owns us and is being deleted as well.
  if (is_animating_)
    container_->Stop(this);
}

void Animation::Start() {
  if (is_animating_)
    return;

  if (!container_.get())
    container_ = new AnimationContainer();

  is_animating_ = true;

  container_->Start(this);

  AnimationStarted();
}

void Animation::Stop() {
  if (!is_animating_)
    return;

  is_animating_ = false;

  // Notify the container first as the delegate may delete us.
  container_->Stop(this);

  AnimationStopped();

  if (delegate_) {
    if (ShouldSendCanceledFromStop())
      delegate_->AnimationCanceled(this);
    else
      delegate_->AnimationEnded(this);
  }
}

double Animation::CurrentValueBetween(double start, double target) const {
  return Tween::ValueBetween(GetCurrentValue(), start, target);
}

int Animation::CurrentValueBetween(int start, int target) const {
  return Tween::ValueBetween(GetCurrentValue(), start, target);
}

gfx::Rect Animation::CurrentValueBetween(const gfx::Rect& start_bounds,
                                         const gfx::Rect& target_bounds) const {
  return Tween::ValueBetween(GetCurrentValue(), start_bounds, target_bounds);
}

void Animation::SetContainer(AnimationContainer* container) {
  if (container == container_.get())
    return;

  if (is_animating_)
    container_->Stop(this);

  if (container)
    container_ = container;
  else
    container_ = new AnimationContainer();

  if (is_animating_)
    container_->Start(this);
}

// static
bool Animation::ShouldRenderRichAnimation() {
#if defined(OS_WIN)
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    BOOL result;
    // Get "Turn off all unnecessary animations" value.
    if (::SystemParametersInfo(SPI_GETCLIENTAREAANIMATION, 0, &result, 0)) {
      // There seems to be a typo in the MSDN document (as of May 2009):
      //   http://msdn.microsoft.com/en-us/library/ms724947(VS.85).aspx
      // The document states that the result is TRUE when animations are
      // _disabled_, but in fact, it is TRUE when they are _enabled_.
      return !!result;
    }
  }
  return !::GetSystemMetrics(SM_REMOTESESSION);
#endif
  return true;
}

bool Animation::ShouldSendCanceledFromStop() {
  return false;
}

void Animation::SetStartTime(base::TimeTicks start_time) {
  start_time_ = start_time;
}

base::TimeDelta Animation::GetTimerInterval() const {
  return timer_interval_;
}

}  // namespace ui
