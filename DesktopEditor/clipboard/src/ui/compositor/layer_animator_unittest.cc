// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/layer_animator.h"

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/stringprintf.h"
#include "base/time.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/compositor/layer_animation_delegate.h"
#include "ui/compositor/layer_animation_element.h"
#include "ui/compositor/layer_animation_sequence.h"
#include "ui/compositor/scoped_animation_duration_scale_mode.h"
#include "ui/compositor/scoped_layer_animation_settings.h"
#include "ui/compositor/test/layer_animator_test_controller.h"
#include "ui/compositor/test/test_layer_animation_delegate.h"
#include "ui/compositor/test/test_layer_animation_observer.h"
#include "ui/compositor/test/test_utils.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

namespace ui {

namespace {

// Converts |color| to a string. Each component of the color is separated by a
// space and the order if A R G B.
std::string ColorToString(SkColor color) {
  return base::StringPrintf("%d %d %d %d", SkColorGetA(color),
                            SkColorGetR(color), SkColorGetG(color),
                            SkColorGetB(color));
}

// Creates vector with two LayerAnimationSequences, based on |first| and
// |second| layer animation elements.
std::vector<LayerAnimationSequence*> CreateMultiSequence(
    LayerAnimationElement* first,
    LayerAnimationElement* second) {
  LayerAnimationSequence* first_sequence = new LayerAnimationSequence();
  first_sequence->AddElement(first);
  LayerAnimationSequence* second_sequence = new LayerAnimationSequence();
  second_sequence->AddElement(second);

  std::vector<ui::LayerAnimationSequence*> animations;
  animations.push_back(first_sequence);
  animations.push_back(second_sequence);
  return animations;
}

class TestImplicitAnimationObserver : public ImplicitAnimationObserver {
 public:
  explicit TestImplicitAnimationObserver(bool notify_when_animator_destructed)
    : animations_completed_(false),
      notify_when_animator_destructed_(notify_when_animator_destructed) {
  }

  bool animations_completed() const { return animations_completed_; }
  void set_animations_completed(bool completed) {
    animations_completed_ = completed;
  }

 private:
  // ImplicitAnimationObserver implementation
  virtual void OnImplicitAnimationsCompleted() OVERRIDE {
    animations_completed_ = true;
  }

  virtual bool RequiresNotificationWhenAnimatorDestroyed() const OVERRIDE {
    return notify_when_animator_destructed_;
  }

  bool animations_completed_;
  bool notify_when_animator_destructed_;

  DISALLOW_COPY_AND_ASSIGN(TestImplicitAnimationObserver);
};

// When notified that an animation has ended, stops all other animations.
class DeletingLayerAnimationObserver : public LayerAnimationObserver {
 public:
  DeletingLayerAnimationObserver(LayerAnimator* animator,
                                 LayerAnimationSequence* sequence)
    : animator_(animator),
      sequence_(sequence) {
  }

  virtual void OnLayerAnimationEnded(
      LayerAnimationSequence* sequence) OVERRIDE {
    animator_->StopAnimating();
  }

  virtual void OnLayerAnimationAborted(
      LayerAnimationSequence* sequence) OVERRIDE {
  }

  virtual void OnLayerAnimationScheduled(
      LayerAnimationSequence* sequence) OVERRIDE {
  }

 private:
  LayerAnimator* animator_;
  LayerAnimationSequence* sequence_;

  DISALLOW_COPY_AND_ASSIGN(DeletingLayerAnimationObserver);
};

class TestLayerAnimator : public LayerAnimator {
 public:
  TestLayerAnimator() : LayerAnimator(base::TimeDelta::FromSeconds(0)) {}

 protected:
  virtual ~TestLayerAnimator() {}

  virtual void ProgressAnimation(LayerAnimationSequence* sequence,
                                 base::TimeTicks now) OVERRIDE {
    EXPECT_TRUE(HasAnimation(sequence));
    LayerAnimator::ProgressAnimation(sequence, now);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestLayerAnimator);
};

// The test layer animation sequence updates a live instances count when it is
// created and destroyed.
class TestLayerAnimationSequence : public LayerAnimationSequence {
 public:
  TestLayerAnimationSequence(LayerAnimationElement* element,
                             int* num_live_instances)
      : LayerAnimationSequence(element),
        num_live_instances_(num_live_instances) {
    (*num_live_instances_)++;
  }

  virtual ~TestLayerAnimationSequence() {
    (*num_live_instances_)--;
  }

 private:
  int* num_live_instances_;

  DISALLOW_COPY_AND_ASSIGN(TestLayerAnimationSequence);
};

} // namespace

// Checks that setting a property on an implicit animator causes an animation to
// happen.
TEST(LayerAnimatorTest, ImplicitAnimation) {
  scoped_refptr<LayerAnimator> animator(
      LayerAnimator::CreateImplicitAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);
  base::TimeTicks now = base::TimeTicks::Now();
  animator->SetBrightness(0.5);
  EXPECT_TRUE(animator->is_animating());
  element->Step(now + base::TimeDelta::FromSeconds(1));
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), 0.5);
}

// Checks that if the animator is a default animator, that implicit animations
// are not started.
TEST(LayerAnimatorTest, NoImplicitAnimation) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);
  animator->SetBrightness(0.5);
  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), 0.5);
}

// Checks that StopAnimatingProperty stops animation for that property, and also
// skips the stopped animation to the end.
TEST(LayerAnimatorTest, StopAnimatingProperty) {
  scoped_refptr<LayerAnimator> animator(
      LayerAnimator::CreateImplicitAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);
  double target_opacity(0.5);
  gfx::Rect target_bounds(0, 0, 50, 50);
  animator->SetOpacity(target_opacity);
  animator->SetBounds(target_bounds);
  animator->StopAnimatingProperty(LayerAnimationElement::OPACITY);
  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), 0.5);
  animator->StopAnimatingProperty(LayerAnimationElement::BOUNDS);
  EXPECT_FALSE(animator->is_animating());
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Checks that multiple running animation for separate properties can be stopped
// simultaneously and that all animations are advanced to their target values.
TEST(LayerAnimatorTest, StopAnimating) {
  scoped_refptr<LayerAnimator> animator(
      LayerAnimator::CreateImplicitAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);
  double target_opacity(0.5);
  gfx::Rect target_bounds(0, 0, 50, 50);
  animator->SetOpacity(target_opacity);
  animator->SetBounds(target_bounds);
  EXPECT_TRUE(animator->is_animating());
  animator->StopAnimating();
  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), 0.5);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Checks that multiple running animation for separate properties can be stopped
// simultaneously and that all animations are advanced to their target values.
TEST(LayerAnimatorTest, AbortAllAnimations) {
  scoped_refptr<LayerAnimator> animator(
      LayerAnimator::CreateImplicitAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  double initial_opacity(1.0);
  gfx::Rect initial_bounds(0, 0, 10, 10);
  delegate.SetOpacityFromAnimation(initial_opacity);
  delegate.SetBoundsFromAnimation(initial_bounds);
  animator->SetDelegate(&delegate);
  double target_opacity(0.5);
  gfx::Rect target_bounds(0, 0, 50, 50);
  animator->SetOpacity(target_opacity);
  animator->SetBounds(target_bounds);
  EXPECT_TRUE(animator->is_animating());
  animator->AbortAllAnimations();
  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(initial_opacity, delegate.GetOpacityForAnimation());
  CheckApproximatelyEqual(initial_bounds, delegate.GetBoundsForAnimation());
}

// Schedule a non-threaded animation that can run immediately. This is the
// trivial case and should result in the animation being started immediately.
TEST(LayerAnimatorTest, ScheduleAnimationThatCanRunImmediately) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);
}

// Schedule a threaded animation that can run immediately.
TEST(LayerAnimatorTest, ScheduleThreadedAnimationThatCanRunImmediately) {
  double epsilon = 0.00001;
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  test_controller.animator()->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta/2);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(
      0.5,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          last_progressed_fraction(),
      epsilon);

  element->Step(effective_start + delta);

  EXPECT_FALSE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);
}

// Schedule two non-threaded animations on separate properties. Both animations
// should start immediately and should progress in lock step.
TEST(LayerAnimatorTest, ScheduleTwoAnimationsThatCanRunImmediately) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  gfx::Rect start_bounds, target_bounds, middle_bounds;
  start_bounds = target_bounds = middle_bounds = gfx::Rect(0, 0, 50, 50);
  start_bounds.set_x(-90);
  target_bounds.set_x(90);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);
  delegate.SetBoundsFromAnimation(start_bounds);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBoundsElement(target_bounds, delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), middle_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Schedule a threaded and a non-threaded animation on separate properties. Both
// animations should progress in lock step.
TEST(LayerAnimatorTest, ScheduleThreadedAndNonThreadedAnimations) {
  double epsilon = 0.00001;
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  gfx::Rect start_bounds, target_bounds, middle_bounds;
  start_bounds = target_bounds = middle_bounds = gfx::Rect(0, 0, 50, 50);
  start_bounds.set_x(-90);
  target_bounds.set_x(90);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);
  delegate.SetBoundsFromAnimation(start_bounds);

  std::vector<LayerAnimationSequence*> animations;
  animations.push_back(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  animations.push_back(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBoundsElement(target_bounds, delta)));

  test_controller.animator()->ScheduleTogether(animations);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta/2);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(
      0.5,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          last_progressed_fraction(),
      epsilon);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), middle_bounds);

  element->Step(effective_start + delta);

  EXPECT_FALSE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Schedule two animations on the same property. In this case, the two
// animations should run one after another.
TEST(LayerAnimatorTest, ScheduleTwoAnimationsOnSameProperty) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Schedule [{g}, {g,b}, {b}] and ensure that {b} doesn't run right away. That
// is, ensure that all animations targetting a particular property are run in
// order.
TEST(LayerAnimatorTest, ScheduleBlockedAnimation) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double middle_grayscale(0.5);
  double target_grayscale(1.0);

  gfx::Rect start_bounds, target_bounds, middle_bounds;
  start_bounds = target_bounds = middle_bounds = gfx::Rect(0, 0, 50, 50);
  start_bounds.set_x(-90);
  target_bounds.set_x(90);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBoundsFromAnimation(start_bounds);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta)));

  scoped_ptr<LayerAnimationSequence> bounds_and_grayscale(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateGrayscaleElement(start_grayscale,
                                                        delta)));

  bounds_and_grayscale->AddElement(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta));

  animator->ScheduleAnimation(bounds_and_grayscale.release());

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBoundsElement(start_bounds, delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), target_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(3000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(4000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);
}

// Schedule {g} and then schedule {g} and {b} together. In this case, since
// ScheduleTogether is being used, the bounds animation should not start until
// the second grayscale animation starts.
TEST(LayerAnimatorTest, ScheduleTogether) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double target_grayscale(1.0);

  gfx::Rect start_bounds, target_bounds, middle_bounds;
  start_bounds = target_bounds = gfx::Rect(0, 0, 50, 50);
  start_bounds.set_x(-90);
  target_bounds.set_x(90);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBoundsFromAnimation(start_bounds);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta)));

  std::vector<LayerAnimationSequence*> sequences;
  sequences.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateGrayscaleElement(start_grayscale, delta)));
  sequences.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta)));

  animator->ScheduleTogether(sequences);

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), target_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), start_bounds);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Start non-threaded animation (that can run immediately). This is the trivial
// case (see the trival case for ScheduleAnimation).
TEST(LayerAnimatorTest, StartAnimationThatCanRunImmediately) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);
}

// Start threaded animation (that can run immediately).
TEST(LayerAnimatorTest, StartThreadedAnimationThatCanRunImmediately) {
  double epsilon = 0.00001;
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  test_controller.animator()->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta/2);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(
      0.5,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          last_progressed_fraction(),
      epsilon);

  element->Step(effective_start + delta);
  EXPECT_FALSE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);
}

// Preempt by immediately setting new target.
TEST(LayerAnimatorTest, PreemptBySettingNewTarget) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  animator->set_preemption_strategy(LayerAnimator::IMMEDIATELY_SET_NEW_TARGET);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta)));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);
}

// Preempt by animating to new target, with a non-threaded animation.
TEST(LayerAnimatorTest, PreemptByImmediatelyAnimatingToNewTarget) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(),
                  0.5 * (start_brightness + middle_brightness));

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Preempt by animating to new target, with a threaded animation.
TEST(LayerAnimatorTest, PreemptThreadedByImmediatelyAnimatingToNewTarget) {
  double epsilon = 0.00001;
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double middle_opacity(0.5);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  test_controller.animator()->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  test_controller.animator()->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta/2);

  test_controller.animator()->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(delegate.GetOpacityForAnimation(), middle_opacity, epsilon);

  test_controller.animator()->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());

  base::TimeTicks second_effective_start = effective_start + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (second_effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(second_effective_start + delta/2);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(
      0.5,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          last_progressed_fraction(),
      epsilon);

  element->Step(second_effective_start + delta);

  EXPECT_FALSE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);
}

// Preempt by enqueuing the new animation.
TEST(LayerAnimatorTest, PreemptEnqueueNewAnimation) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                          delta)));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  EXPECT_TRUE(animator->is_animating());

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Start an animation when there are sequences waiting in the queue. In this
// case, all pending and running animations should be finished, and the new
// animation started.
TEST(LayerAnimatorTest, PreemptyByReplacingQueuedAnimations) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double middle_brightness(0.5);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(LayerAnimator::REPLACE_QUEUED_ANIMATIONS);

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(middle_brightness,
                                                         delta)));

  // Queue should now have two animations. Starting a third should replace the
  // second.
  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

TEST(LayerAnimatorTest, StartTogetherSetsLastStepTime) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double target_grayscale(1.0);
  double start_brightness(0.1);
  double target_brightness(0.9);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  animator->set_last_step_time(base::TimeTicks());

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)
      ));

  // If last step time was not set correctly, the resulting delta should be
  // miniscule (fractions of a millisecond). If set correctly, then the delta
  // should be enormous. Arbitrarily choosing 1 minute as the threshold,
  // though a much smaller value would probably have sufficed.
  delta = base::TimeTicks::Now() - animator->last_step_time();
  EXPECT_GT(60.0, delta.InSecondsF());
}

//-------------------------------------------------------
// Preempt by immediately setting new target.
TEST(LayerAnimatorTest, MultiPreemptBySettingNewTarget) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);
  double start_brightness(0.1);
  double target_brightness(0.9);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);
  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(LayerAnimator::IMMEDIATELY_SET_NEW_TARGET);

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)
      ));

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)
      ));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Preempt by animating to new target.
TEST(LayerAnimatorTest, MultiPreemptByImmediatelyAnimatingToNewTarget) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double middle_grayscale(0.5);
  double target_grayscale(1.0);

  double start_brightness(0.1);
  double middle_brightness(0.2);
  double target_brightness(0.3);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)
      ));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(start_grayscale, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(start_grayscale, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(),
                  0.5 * (start_grayscale + middle_grayscale));
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(),
                  0.5 * (start_brightness + middle_brightness));

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Preempt a threaded animation by animating to new target.
TEST(LayerAnimatorTest, MultiPreemptThreadedByImmediatelyAnimatingToNewTarget) {
  double epsilon = 0.00001;
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double middle_opacity(0.5);
  double target_opacity(1.0);

  double start_brightness(0.1);
  double middle_brightness(0.2);
  double target_brightness(0.3);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);
  delegate.SetBrightnessFromAnimation(start_brightness);

  test_controller.animator()->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  test_controller.animator()->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)
      ));

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta/2);

  test_controller.animator()->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(delegate.GetOpacityForAnimation(), middle_opacity, epsilon);
  EXPECT_NEAR(delegate.GetBrightnessForAnimation(), middle_brightness, epsilon);

  test_controller.animator()->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateOpacityElement(start_opacity, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(test_controller.animator()->is_animating());

  base::TimeTicks second_effective_start = effective_start + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (second_effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(second_effective_start + delta/2);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_NEAR(
      0.5,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          last_progressed_fraction(),
      epsilon);
  EXPECT_NEAR(delegate.GetBrightnessForAnimation(),
              0.5 * (start_brightness + middle_brightness),
              epsilon);

  element->Step(second_effective_start + delta);

  EXPECT_FALSE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Preempt by enqueuing the new animation.
TEST(LayerAnimatorTest, MultiPreemptEnqueueNewAnimation) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double middle_grayscale(0.5);
  double target_grayscale(1.0);

  double start_brightness(0.1);
  double middle_brightness(0.2);
  double target_brightness(0.3);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(start_grayscale, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  EXPECT_TRUE(animator->is_animating());

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), target_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}

// Start an animation when there are sequences waiting in the queue. In this
// case, all pending and running animations should be finished, and the new
// animation started.
TEST(LayerAnimatorTest, MultiPreemptByReplacingQueuedAnimations) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_grayscale(0.0);
  double middle_grayscale(0.5);
  double target_grayscale(1.0);

  double start_brightness(0.1);
  double middle_brightness(0.2);
  double target_brightness(0.3);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetGrayscaleFromAnimation(start_grayscale);
  delegate.SetBrightnessFromAnimation(start_brightness);

  animator->set_preemption_strategy(LayerAnimator::REPLACE_QUEUED_ANIMATIONS);

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(target_grayscale,
                                                        delta),
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(middle_grayscale,
                                                        delta),
          LayerAnimationElement::CreateBrightnessElement(middle_brightness,
                                                         delta)));

  // Queue should now have two animations. Starting a third should replace the
  // second.
  animator->StartTogether(
      CreateMultiSequence(
          LayerAnimationElement::CreateGrayscaleElement(start_grayscale, delta),
          LayerAnimationElement::CreateBrightnessElement(start_brightness,
                                                         delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), target_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), middle_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), middle_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetGrayscaleForAnimation(), start_grayscale);
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);
}
//-------------------------------------------------------
// Test that non-threaded cyclic sequences continue to animate.
TEST(LayerAnimatorTest, CyclicSequences) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double target_brightness(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(start_brightness);

  scoped_ptr<LayerAnimationSequence> sequence(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(target_brightness,
                                                         delta)));

  sequence->AddElement(
      LayerAnimationElement::CreateBrightnessElement(start_brightness, delta));

  sequence->set_is_cyclic(true);

  animator->StartAnimation(sequence.release());

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(2000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);

  element->Step(start_time + base::TimeDelta::FromMilliseconds(3000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  // Skip ahead by a lot.
  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000000000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), start_brightness);

  // Skip ahead by a lot.
  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000001000));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetBrightnessForAnimation(), target_brightness);

  animator->StopAnimatingProperty(LayerAnimationElement::BRIGHTNESS);

  EXPECT_FALSE(animator->is_animating());
}

// Test that threaded cyclic sequences continue to animate.
TEST(LayerAnimatorTest, ThreadedCyclicSequences) {
  LayerAnimatorTestController test_controller(
      LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = test_controller.animator();
  test_controller.animator()->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  test_controller.animator()->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  scoped_ptr<LayerAnimationSequence> sequence(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  sequence->AddElement(
      LayerAnimationElement::CreateOpacityElement(start_opacity, delta));

  sequence->set_is_cyclic(true);

  test_controller.animator()->StartAnimation(sequence.release());

  base::TimeTicks start_time = test_controller.animator()->last_step_time();
  base::TimeTicks effective_start = start_time + delta;

  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(effective_start + delta);
  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);

  base::TimeTicks second_effective_start = effective_start + 2 * delta;
  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (second_effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(second_effective_start + delta);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);

  base::TimeTicks third_effective_start = second_effective_start + 2 * delta;
  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (third_effective_start - base::TimeTicks()).InSecondsF()));

  element->Step(third_effective_start + delta);
  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);

  base::TimeTicks fourth_effective_start = third_effective_start + 2 * delta;
  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (fourth_effective_start - base::TimeTicks()).InSecondsF()));

  // Skip ahead by a lot.
  element->Step(fourth_effective_start + 1000 * delta);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), target_opacity);

  base::TimeTicks fifth_effective_start = fourth_effective_start + 1001 * delta;
  test_controller.animator()->OnThreadedAnimationStarted(cc::AnimationEvent(
      cc::AnimationEvent::Started,
      0,
      test_controller.GetRunningSequence(LayerAnimationElement::OPACITY)->
          animation_group_id(),
      cc::Animation::Opacity,
      (fifth_effective_start - base::TimeTicks()).InSecondsF()));

  // Skip ahead by a lot.
  element->Step(fifth_effective_start + 999 * delta);

  EXPECT_TRUE(test_controller.animator()->is_animating());
  EXPECT_FLOAT_EQ(delegate.GetOpacityForAnimation(), start_opacity);

  test_controller.animator()->StopAnimatingProperty(
      LayerAnimationElement::OPACITY);

  EXPECT_FALSE(test_controller.animator()->is_animating());
}

TEST(LayerAnimatorTest, AddObserverExplicit) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationObserver observer;
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);
  animator->AddObserver(&observer);
  observer.set_requires_notification_when_animator_destroyed(true);

  EXPECT_TRUE(!observer.last_ended_sequence());

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetBrightnessFromAnimation(0.0f);

  LayerAnimationSequence* sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBrightnessElement(1.0f, delta));

  animator->StartAnimation(sequence);

  EXPECT_EQ(observer.last_scheduled_sequence(), sequence);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_EQ(observer.last_ended_sequence(), sequence);

  // |sequence| has been destroyed. Recreate it to test abort.
  sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBrightnessElement(1.0f, delta));

  animator->StartAnimation(sequence);

  animator = NULL;

  EXPECT_EQ(observer.last_aborted_sequence(), sequence);
}

// Tests that an observer added to a scoped settings object is still notified
// when the object goes out of scope.
TEST(LayerAnimatorTest, ImplicitAnimationObservers) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestImplicitAnimationObserver observer(false);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  EXPECT_FALSE(observer.animations_completed());
  animator->SetBrightness(1.0f);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    settings.AddObserver(&observer);
    animator->SetBrightness(0.0f);
  }

  EXPECT_FALSE(observer.animations_completed());
  base::TimeTicks start_time = animator->last_step_time();
  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));
  EXPECT_TRUE(observer.animations_completed());
  EXPECT_FLOAT_EQ(0.0f, delegate.GetBrightnessForAnimation());
}

// Tests that an observer added to a scoped settings object is still notified
// when the object goes out of scope due to the animation being interrupted.
TEST(LayerAnimatorTest, InterruptedImplicitAnimationObservers) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestImplicitAnimationObserver observer(false);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  EXPECT_FALSE(observer.animations_completed());
  animator->SetBrightness(1.0f);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    settings.AddObserver(&observer);
    animator->SetBrightness(0.0f);
  }

  EXPECT_FALSE(observer.animations_completed());
  // This should interrupt the implicit animation causing the observer to be
  // notified immediately.
  animator->SetBrightness(1.0f);
  EXPECT_TRUE(observer.animations_completed());
  EXPECT_FLOAT_EQ(1.0f, delegate.GetBrightnessForAnimation());
}

// Tests that an observer added to a scoped settings object is not notified
// when the animator is destroyed unless explicitly requested.
TEST(LayerAnimatorTest, ImplicitObserversAtAnimatorDestruction) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestImplicitAnimationObserver observer_notify(true);
  TestImplicitAnimationObserver observer_do_not_notify(false);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  EXPECT_FALSE(observer_notify.animations_completed());
  EXPECT_FALSE(observer_do_not_notify.animations_completed());

  animator->SetBrightness(1.0f);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    settings.AddObserver(&observer_notify);
    settings.AddObserver(&observer_do_not_notify);
    animator->SetBrightness(0.0f);
  }

  EXPECT_FALSE(observer_notify.animations_completed());
  EXPECT_FALSE(observer_do_not_notify.animations_completed());
  animator = NULL;
  EXPECT_TRUE(observer_notify.animations_completed());
  EXPECT_FALSE(observer_do_not_notify.animations_completed());
}


TEST(LayerAnimatorTest, RemoveObserverShouldRemoveFromSequences) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationObserver observer;
  TestLayerAnimationObserver removed_observer;
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  LayerAnimationSequence* sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBrightnessElement(1.0f, delta));

  sequence->AddObserver(&observer);
  sequence->AddObserver(&removed_observer);

  animator->StartAnimation(sequence);

  EXPECT_EQ(observer.last_scheduled_sequence(), sequence);
  EXPECT_TRUE(!observer.last_ended_sequence());
  EXPECT_EQ(removed_observer.last_scheduled_sequence(), sequence);
  EXPECT_TRUE(!removed_observer.last_ended_sequence());

  // This should stop the observer from observing sequence.
  animator->RemoveObserver(&removed_observer);

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_EQ(observer.last_ended_sequence(), sequence);
  EXPECT_TRUE(!removed_observer.last_ended_sequence());
}

TEST(LayerAnimatorTest, ObserverReleasedBeforeAnimationSequenceEnds) {
  TestLayerAnimationDelegate delegate;
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);

  scoped_ptr<TestLayerAnimationObserver> observer(
      new TestLayerAnimationObserver);
  animator->SetDelegate(&delegate);
  animator->AddObserver(observer.get());

  delegate.SetOpacityFromAnimation(0.0f);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  LayerAnimationSequence* sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateOpacityElement(1.0f, delta));

  animator->StartAnimation(sequence);

  // |observer| should be attached to |sequence|.
  EXPECT_EQ(static_cast<size_t>(1), sequence->observers_.size());

  // Now, release |observer|
  observer.reset();

  // And |sequence| should no longer be attached to |observer|.
  EXPECT_EQ(static_cast<size_t>(0), sequence->observers_.size());
}

TEST(LayerAnimatorTest, ObserverAttachedAfterAnimationStarted) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);

  TestImplicitAnimationObserver observer(false);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetBrightnessFromAnimation(0.0f);

  {
    ScopedLayerAnimationSettings setter(animator.get());

    base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
    LayerAnimationSequence* sequence = new LayerAnimationSequence(
        LayerAnimationElement::CreateBrightnessElement(1.0f, delta));

    animator->StartAnimation(sequence);
    base::TimeTicks start_time = animator->last_step_time();
    element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

    setter.AddObserver(&observer);

    // Start observing an in-flight animation.
    sequence->AddObserver(&observer);

    element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));
  }

  EXPECT_TRUE(observer.animations_completed());
}

TEST(LayerAnimatorTest, ObserverDetachedBeforeAnimationFinished) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);

  TestImplicitAnimationObserver observer(false);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetBrightnessFromAnimation(0.0f);
  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  LayerAnimationSequence* sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBrightnessElement(1.0f, delta));

  {
    ScopedLayerAnimationSettings setter(animator.get());
    setter.AddObserver(&observer);

    animator->StartAnimation(sequence);
    base::TimeTicks start_time = animator->last_step_time();
    element->Step(start_time + base::TimeDelta::FromMilliseconds(500));
  }

  EXPECT_FALSE(observer.animations_completed());

  // Stop observing an in-flight animation.
  sequence->RemoveObserver(&observer);

  EXPECT_TRUE(observer.animations_completed());
}

// This checks that if an animation is deleted due to a callback, that the
// animator does not try to use the deleted animation. For example, if we have
// two running animations, and the first finishes and the resulting callback
// causes the second to be deleted, we should not attempt to animate the second
// animation.
TEST(LayerAnimatorTest, ObserverDeletesAnimations) {
  ScopedAnimationDurationScaleMode normal_duration_mode(
      ScopedAnimationDurationScaleMode::NORMAL_DURATION);
  scoped_refptr<LayerAnimator> animator(new TestLayerAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_brightness(0.0);
  double target_brightness(1.0);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect target_bounds(5, 5, 5, 5);

  delegate.SetBrightnessFromAnimation(start_brightness);
  delegate.SetBoundsFromAnimation(start_bounds);

  base::TimeDelta brightness_delta = base::TimeDelta::FromSeconds(1);
  base::TimeDelta halfway_delta = base::TimeDelta::FromSeconds(2);
  base::TimeDelta bounds_delta = base::TimeDelta::FromSeconds(3);

  LayerAnimationSequence* to_delete = new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, bounds_delta));

  scoped_ptr<DeletingLayerAnimationObserver> observer(
      new DeletingLayerAnimationObserver(animator.get(), to_delete));

  animator->AddObserver(observer.get());

  animator->StartAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateBrightnessElement(
              target_brightness, brightness_delta)));

  animator->StartAnimation(to_delete);

  base::TimeTicks start_time = animator->last_step_time();
  element->Step(start_time + halfway_delta);

  animator->RemoveObserver(observer.get());
}

// Check that setting a property during an animation with a default animator
// cancels the original animation.
TEST(LayerAnimatorTest, SettingPropertyDuringAnAnimation) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  double start_opacity(0.0);
  double target_opacity(1.0);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetOpacityFromAnimation(start_opacity);

  scoped_ptr<LayerAnimationSequence> sequence(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateOpacityElement(target_opacity, delta)));

  animator->StartAnimation(sequence.release());

  animator->SetOpacity(0.5);

  EXPECT_FALSE(animator->is_animating());
  EXPECT_EQ(0.5, animator->GetTargetOpacity());
}

// Tests that the preemption mode IMMEDIATELY_SET_NEW_TARGET, doesn't cause the
// second sequence to be leaked.
TEST(LayerAnimatorTest, ImmediatelySettingNewTargetDoesNotLeak) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_preemption_strategy(LayerAnimator::IMMEDIATELY_SET_NEW_TARGET);
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect middle_bounds(10, 10, 100, 100);
  gfx::Rect target_bounds(5, 5, 5, 5);

  delegate.SetBoundsFromAnimation(start_bounds);

  {
    // start an implicit bounds animation.
    ScopedLayerAnimationSettings settings(animator.get());
    animator->SetBounds(middle_bounds);
  }

  EXPECT_TRUE(animator->IsAnimatingProperty(LayerAnimationElement::BOUNDS));

  int num_live_instances = 0;
  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  scoped_ptr<TestLayerAnimationSequence> sequence(
      new TestLayerAnimationSequence(
          LayerAnimationElement::CreateBoundsElement(target_bounds, delta),
          &num_live_instances));

  EXPECT_EQ(1, num_live_instances);

  // This should interrupt the running sequence causing us to immediately set
  // the target value. The sequence should alse be destructed.
  animator->StartAnimation(sequence.release());

  EXPECT_FALSE(animator->IsAnimatingProperty(LayerAnimationElement::BOUNDS));
  EXPECT_EQ(0, num_live_instances);
  CheckApproximatelyEqual(delegate.GetBoundsForAnimation(), target_bounds);
}

// Verifies GetTargetOpacity() works when multiple sequences are scheduled.
TEST(LayerAnimatorTest, GetTargetOpacity) {
  TestLayerAnimationDelegate delegate;
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);
  animator->set_disable_timer_for_test(true);
  animator->SetDelegate(&delegate);

  delegate.SetOpacityFromAnimation(0.0);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    animator->SetOpacity(0.5);
    EXPECT_EQ(0.5, animator->GetTargetOpacity());

    // Because the strategy is ENQUEUE_NEW_ANIMATION the target should now be 1.
    animator->SetOpacity(1.0);
    EXPECT_EQ(1.0, animator->GetTargetOpacity());
  }
}

// Verifies GetTargetBrightness() works when multiple sequences are scheduled.
TEST(LayerAnimatorTest, GetTargetBrightness) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetBrightnessFromAnimation(0.0);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    animator->SetBrightness(0.5);
    EXPECT_EQ(0.5, animator->GetTargetBrightness());

    // Because the strategy is ENQUEUE_NEW_ANIMATION the target should now be 1.
    animator->SetBrightness(1.0);
    EXPECT_EQ(1.0, animator->GetTargetBrightness());
  }
}

// Verifies GetTargetGrayscale() works when multiple sequences are scheduled.
TEST(LayerAnimatorTest, GetTargetGrayscale) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetGrayscaleFromAnimation(0.0);

  {
    ScopedLayerAnimationSettings settings(animator.get());
    animator->SetGrayscale(0.5);
    EXPECT_EQ(0.5, animator->GetTargetGrayscale());

    // Because the strategy is ENQUEUE_NEW_ANIMATION the target should now be 1.
    animator->SetGrayscale(1.0);
    EXPECT_EQ(1.0, animator->GetTargetGrayscale());
  }
}

// Verifies color property is modified appropriately.
TEST(LayerAnimatorTest, Color) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  AnimationContainerElement* element = animator.get();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  SkColor start_color  = SkColorSetARGB(  0, 20, 40,  60);
  SkColor middle_color = SkColorSetARGB(127, 30, 60, 100);
  SkColor target_color = SkColorSetARGB(254, 40, 80, 140);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);

  delegate.SetColorFromAnimation(start_color);

  animator->ScheduleAnimation(
      new LayerAnimationSequence(
          LayerAnimationElement::CreateColorElement(target_color, delta)));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_EQ(ColorToString(start_color),
            ColorToString(delegate.GetColorForAnimation()));

  base::TimeTicks start_time = animator->last_step_time();

  element->Step(start_time + base::TimeDelta::FromMilliseconds(500));

  EXPECT_TRUE(animator->is_animating());
  EXPECT_EQ(ColorToString(middle_color),
            ColorToString(delegate.GetColorForAnimation()));

  element->Step(start_time + base::TimeDelta::FromMilliseconds(1000));

  EXPECT_FALSE(animator->is_animating());
  EXPECT_EQ(ColorToString(target_color),
            ColorToString(delegate.GetColorForAnimation()));
}

// Verifies SchedulePauseForProperties().
TEST(LayerAnimatorTest, SchedulePauseForProperties) {
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_preemption_strategy(LayerAnimator::ENQUEUE_NEW_ANIMATION);
  animator->SchedulePauseForProperties(base::TimeDelta::FromMilliseconds(100),
                                       LayerAnimationElement::TRANSFORM,
                                       LayerAnimationElement::BOUNDS, -1);
  EXPECT_TRUE(animator->IsAnimatingProperty(LayerAnimationElement::TRANSFORM));
  EXPECT_TRUE(animator->IsAnimatingProperty(LayerAnimationElement::BOUNDS));
  EXPECT_FALSE(animator->IsAnimatingProperty(LayerAnimationElement::OPACITY));
}


class AnimatorOwner {
public:
  AnimatorOwner()
      : animator_(LayerAnimator::CreateDefaultAnimator()) {
  }

  LayerAnimator* animator() { return animator_.get(); }

private:
  scoped_refptr<LayerAnimator> animator_;

  DISALLOW_COPY_AND_ASSIGN(AnimatorOwner);
};

class DeletingObserver : public LayerAnimationObserver {
public:
  DeletingObserver(bool* was_deleted)
      : animator_owner_(new AnimatorOwner),
        delete_on_animation_ended_(false),
        delete_on_animation_aborted_(false),
        delete_on_animation_scheduled_(false),
        was_deleted_(was_deleted) {
    animator()->AddObserver(this);
  }

  virtual ~DeletingObserver() {
    animator()->RemoveObserver(this);
    *was_deleted_ = true;
  }

  LayerAnimator* animator() { return animator_owner_->animator(); }

  bool delete_on_animation_ended() const {
    return delete_on_animation_ended_;
  }
  void set_delete_on_animation_ended(bool enabled) {
    delete_on_animation_ended_ = enabled;
  }

  bool delete_on_animation_aborted() const {
    return delete_on_animation_aborted_;
  }
  void set_delete_on_animation_aborted(bool enabled) {
    delete_on_animation_aborted_ = enabled;
  }

  bool delete_on_animation_scheduled() const {
    return delete_on_animation_scheduled_;
  }
  void set_delete_on_animation_scheduled(bool enabled) {
    delete_on_animation_scheduled_ = enabled;
  }

  // LayerAnimationObserver implementation.
  virtual void OnLayerAnimationEnded(
      LayerAnimationSequence* sequence) OVERRIDE {
    if (delete_on_animation_ended_)
      delete this;
  }

  virtual void OnLayerAnimationAborted(
      LayerAnimationSequence* sequence) OVERRIDE {
    if (delete_on_animation_aborted_)
      delete this;
  }

  virtual void OnLayerAnimationScheduled(
      LayerAnimationSequence* sequence) OVERRIDE {
    if (delete_on_animation_scheduled_)
      delete this;
  }

private:
  scoped_ptr<AnimatorOwner> animator_owner_;
  bool delete_on_animation_ended_;
  bool delete_on_animation_aborted_;
  bool delete_on_animation_scheduled_;
  bool* was_deleted_;

  DISALLOW_COPY_AND_ASSIGN(DeletingObserver);
};

TEST(LayerAnimatorTest, ObserverDeletesAnimatorAfterFinishingAnimation) {
  bool observer_was_deleted = false;
  DeletingObserver* observer = new DeletingObserver(&observer_was_deleted);
  observer->set_delete_on_animation_ended(true);
  observer->set_delete_on_animation_aborted(true);
  LayerAnimator* animator = observer->animator();
  AnimationContainerElement* element = observer->animator();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetBrightnessFromAnimation(0.0f);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect target_bounds(10, 10, 100, 100);

  delegate.SetBoundsFromAnimation(start_bounds);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  LayerAnimationSequence* brightness_sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBrightnessElement(1.0f, delta));
  animator->StartAnimation(brightness_sequence);

  delta = base::TimeDelta::FromSeconds(2);
  LayerAnimationSequence* bounds_sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta));
  animator->StartAnimation(bounds_sequence);

  base::TimeTicks start_time = animator->last_step_time();
  element->Step(start_time + base::TimeDelta::FromMilliseconds(1500));

  EXPECT_TRUE(observer_was_deleted);
}

TEST(LayerAnimatorTest, ObserverDeletesAnimatorAfterStoppingAnimating) {
  bool observer_was_deleted = false;
  DeletingObserver* observer = new DeletingObserver(&observer_was_deleted);
  observer->set_delete_on_animation_ended(true);
  observer->set_delete_on_animation_aborted(true);
  LayerAnimator* animator = observer->animator();
  animator->set_disable_timer_for_test(true);
  TestLayerAnimationDelegate delegate;
  animator->SetDelegate(&delegate);

  delegate.SetOpacityFromAnimation(0.0f);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect target_bounds(10, 10, 100, 100);

  delegate.SetBoundsFromAnimation(start_bounds);

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  LayerAnimationSequence* opacity_sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateOpacityElement(1.0f, delta));
  animator->StartAnimation(opacity_sequence);

  delta = base::TimeDelta::FromSeconds(2);
  LayerAnimationSequence* bounds_sequence = new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta));
  animator->StartAnimation(bounds_sequence);

  animator->StopAnimating();

  EXPECT_TRUE(observer_was_deleted);
}

TEST(LayerAnimatorTest, ObserverDeletesAnimatorAfterScheduling) {
  bool observer_was_deleted = false;
  TestLayerAnimationDelegate delegate;
  DeletingObserver* observer = new DeletingObserver(&observer_was_deleted);
  observer->set_delete_on_animation_scheduled(true);
  LayerAnimator* animator = observer->animator();
  animator->set_disable_timer_for_test(true);
  animator->SetDelegate(&delegate);

  delegate.SetOpacityFromAnimation(0.0f);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect target_bounds(10, 10, 100, 100);

  delegate.SetBoundsFromAnimation(start_bounds);

  std::vector<LayerAnimationSequence*> to_start;

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  to_start.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateOpacityElement(1.0f, delta)));

  delta = base::TimeDelta::FromSeconds(2);
  to_start.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta)));

  animator->ScheduleTogether(to_start);

  EXPECT_TRUE(observer_was_deleted);
}

TEST(LayerAnimatorTest, ObserverDeletesAnimatorAfterAborted) {
  bool observer_was_deleted = false;
  DeletingObserver* observer = new DeletingObserver(&observer_was_deleted);
  TestLayerAnimationDelegate delegate;
  observer->set_delete_on_animation_aborted(true);
  LayerAnimator* animator = observer->animator();
  animator->set_preemption_strategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
  animator->set_disable_timer_for_test(true);
  animator->SetDelegate(&delegate);

  delegate.SetOpacityFromAnimation(0.0f);

  gfx::Rect start_bounds(0, 0, 50, 50);
  gfx::Rect target_bounds(10, 10, 100, 100);

  delegate.SetBoundsFromAnimation(start_bounds);

  std::vector<LayerAnimationSequence*> to_start;

  base::TimeDelta delta = base::TimeDelta::FromSeconds(1);
  to_start.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateOpacityElement(1.0f, delta)));

  delta = base::TimeDelta::FromSeconds(2);
  to_start.push_back(new LayerAnimationSequence(
      LayerAnimationElement::CreateBoundsElement(target_bounds, delta)));

  animator->ScheduleTogether(to_start);

  EXPECT_FALSE(observer_was_deleted);

  animator->StartAnimation(new LayerAnimationSequence(
      LayerAnimationElement::CreateOpacityElement(1.0f, delta)));

  EXPECT_TRUE(observer_was_deleted);
}


TEST(LayerAnimatorTest, TestSetterRespectEnqueueStrategy) {
  TestLayerAnimationDelegate delegate;
  scoped_refptr<LayerAnimator> animator(LayerAnimator::CreateDefaultAnimator());
  animator->set_disable_timer_for_test(true);

  animator->SetDelegate(&delegate);

  float start_opacity = 0.0f;
  float target_opacity = 1.0f;
  float magic_opacity = 0.123f;

  delegate.SetOpacityFromAnimation(start_opacity);

  ScopedLayerAnimationSettings settings(animator);
  settings.SetPreemptionStrategy(
      LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
  settings.SetTransitionDuration(base::TimeDelta::FromSeconds(1));
  animator->SetOpacity(target_opacity);

  EXPECT_EQ(start_opacity, delegate.GetOpacityForAnimation());

  settings.SetPreemptionStrategy(
      LayerAnimator::ENQUEUE_NEW_ANIMATION);
  settings.SetTransitionDuration(base::TimeDelta());
  animator->SetOpacity(magic_opacity);

  EXPECT_EQ(start_opacity, delegate.GetOpacityForAnimation());
}

}  // namespace ui
