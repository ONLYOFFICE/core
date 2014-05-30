// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ANIMATION_BOUNDS_ANIMATOR_H_
#define UI_VIEWS_ANIMATION_BOUNDS_ANIMATOR_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "ui/base/animation/animation_container_observer.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/tween.h"
#include "ui/gfx/rect.h"
#include "ui/views/views_export.h"

namespace ui {
class SlideAnimation;
}

namespace views {

class BoundsAnimatorObserver;
class View;

// Bounds animator is responsible for animating the bounds of a view from the
// the views current location and size to a target position and size. To use
// BoundsAnimator invoke AnimateViewTo for the set of views you want to
// animate.
//
// BoundsAnimator internally creates an animation for each view. If you need
// a specific animation invoke SetAnimationForView after invoking AnimateViewTo.
// You can attach an AnimationDelegate to the individual animation for a view
// by way of SetAnimationDelegate. Additionally you can attach an observer to
// the BoundsAnimator that is notified when all animations are complete.
class VIEWS_EXPORT BoundsAnimator : public ui::AnimationDelegate,
                                    public ui::AnimationContainerObserver {
 public:
  // If |delete_when_done| is set to true in |SetAnimationDelegate| the
  // |AnimationDelegate| must subclass this class.
  class OwnedAnimationDelegate : public ui::AnimationDelegate {
   public:
    virtual ~OwnedAnimationDelegate() {}
  };

  explicit BoundsAnimator(View* view);
  virtual ~BoundsAnimator();

  // Starts animating |view| from its current bounds to |target|. If there is
  // already an animation running for the view it's stopped and a new one
  // started. If an AnimationDelegate has been set for |view| it is removed
  // (after being notified that the animation was canceled).
  void AnimateViewTo(View* view, const gfx::Rect& target);

  // Similar to |AnimateViewTo|, but does not reset the animation, only the
  // target bounds. If |view| is not being animated this is the same as
  // invoking |AnimateViewTo|.
  void SetTargetBounds(View* view, const gfx::Rect& target);

  // Returns the target bounds for the specified view. If |view| is not
  // animating its current bounds is returned.
  gfx::Rect GetTargetBounds(View* view);

  // Sets the animation for the specified view. BoundsAnimator takes ownership
  // of the specified animation.
  void SetAnimationForView(View* view, ui::SlideAnimation* animation);

  // Returns the animation for the specified view. BoundsAnimator owns the
  // returned Animation.
  const ui::SlideAnimation* GetAnimationForView(View* view);

  // Stops animating the specified view.
  void StopAnimatingView(View* view);

  // Sets the delegate for the animation created for the specified view. If
  // |delete_when_done| is true the |delegate| is deleted when done and
  // |delegate| must subclass OwnedAnimationDelegate.
  void SetAnimationDelegate(View* view,
                            ui::AnimationDelegate* delegate,
                            bool delete_when_done);

  // Returns true if BoundsAnimator is animating the bounds of |view|.
  bool IsAnimating(View* view) const;

  // Returns true if BoundsAnimator is animating any view.
  bool IsAnimating() const;

  // Cancels all animations, leaving the views at their current location and
  // size. Any views marked for deletion are deleted.
  void Cancel();

  // Overrides default animation duration. |duration_ms| is the new duration in
  // milliseconds.
  void SetAnimationDuration(int duration_ms);

  // Sets the tween type for new animations. Default is EASE_OUT.
  void set_tween_type(ui::Tween::Type type) { tween_type_ = type; }

  void AddObserver(BoundsAnimatorObserver* observer);
  void RemoveObserver(BoundsAnimatorObserver* observer);

 protected:
  // Creates the animation to use for animating views.
  virtual ui::SlideAnimation* CreateAnimation();

 private:
  // Tracks data about the view being animated.
  struct Data {
    Data()
        : delete_delegate_when_done(false),
          animation(NULL),
          delegate(NULL) {}

    // If true the delegate is deleted when done.
    bool delete_delegate_when_done;

    // The initial bounds.
    gfx::Rect start_bounds;

    // Target bounds.
    gfx::Rect target_bounds;

    // The animation. We own this.
    ui::SlideAnimation* animation;

    // Additional delegate for the animation, may be null.
    ui::AnimationDelegate* delegate;
  };

  // Used by AnimationEndedOrCanceled.
  enum AnimationEndType {
    ANIMATION_ENDED,
    ANIMATION_CANCELED
  };

  typedef std::map<View*, Data> ViewToDataMap;

  typedef std::map<const ui::Animation*, View*> AnimationToViewMap;

  // Removes references to |view| and its animation. This does NOT delete the
  // animation or delegate.
  void RemoveFromMaps(View* view);

  // Does the necessary cleanup for |data|. If |send_cancel| is true and a
  // delegate has been installed on |data| AnimationCanceled is invoked on it.
  void CleanupData(bool send_cancel, Data* data, View* view);

  // Used when changing the animation for a view. This resets the maps for
  // the animation used by view and returns the current animation. Ownership
  // of the returned animation passes to the caller.
  ui::Animation* ResetAnimationForView(View* view);

  // Invoked from AnimationEnded and AnimationCanceled.
  void AnimationEndedOrCanceled(const ui::Animation* animation,
                                AnimationEndType type);

  // ui::AnimationDelegate overrides.
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const ui::Animation* animation) OVERRIDE;

  // ui::AnimationContainerObserver overrides.
  virtual void AnimationContainerProgressed(
      ui::AnimationContainer* container) OVERRIDE;
  virtual void AnimationContainerEmpty(
      ui::AnimationContainer* container) OVERRIDE;

  // Parent of all views being animated.
  View* parent_;

  ObserverList<BoundsAnimatorObserver> observers_;

  // All animations we create up with the same container.
  scoped_refptr<ui::AnimationContainer> container_;

  // Maps from view being animated to info about the view.
  ViewToDataMap data_;

  // Maps from animation to view.
  AnimationToViewMap animation_to_view_;

  // As the animations we create update (AnimationProgressed is invoked) this
  // is updated. When all the animations have completed for a given tick of
  // the timer (AnimationContainerProgressed is invoked) the parent_ is asked
  // to repaint these bounds.
  gfx::Rect repaint_bounds_;

  int animation_duration_ms_;

  ui::Tween::Type tween_type_;

  DISALLOW_COPY_AND_ASSIGN(BoundsAnimator);
};

}  // namespace views

#endif  // UI_VIEWS_ANIMATION_BOUNDS_ANIMATOR_H_
