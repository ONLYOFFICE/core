// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/window_animations.h"

#include <math.h>

#include <algorithm>
#include <vector>

#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/message_loop.h"
#include "base/stl_util.h"
#include "base/time.h"
#include "ui/aura/client/animation_host.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/window.h"
#include "ui/aura/window_delegate.h"
#include "ui/aura/window_observer.h"
#include "ui/aura/window_property.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/compositor/layer_animation_sequence.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/scoped_layer_animation_settings.h"
#include "ui/gfx/interpolated_transform.h"
#include "ui/gfx/rect_conversions.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/vector3d_f.h"
#include "ui/views/corewm/corewm_switches.h"
#include "ui/views/corewm/window_util.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"

DECLARE_WINDOW_PROPERTY_TYPE(int)
DECLARE_WINDOW_PROPERTY_TYPE(views::corewm::WindowVisibilityAnimationType)
DECLARE_WINDOW_PROPERTY_TYPE(views::corewm::WindowVisibilityAnimationTransition)
DECLARE_WINDOW_PROPERTY_TYPE(float)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(VIEWS_EXPORT, bool)

using aura::Window;
using base::TimeDelta;
using ui::Layer;

namespace views {
namespace corewm {
namespace {
const float kWindowAnimation_Vertical_TranslateY = 15.f;
}  // namespace

DEFINE_WINDOW_PROPERTY_KEY(int,
                           kWindowVisibilityAnimationTypeKey,
                           WINDOW_VISIBILITY_ANIMATION_TYPE_DEFAULT);
DEFINE_WINDOW_PROPERTY_KEY(int, kWindowVisibilityAnimationDurationKey, 0);
DEFINE_WINDOW_PROPERTY_KEY(WindowVisibilityAnimationTransition,
                           kWindowVisibilityAnimationTransitionKey,
                           ANIMATE_BOTH);
DEFINE_WINDOW_PROPERTY_KEY(float,
                           kWindowVisibilityAnimationVerticalPositionKey,
                           kWindowAnimation_Vertical_TranslateY);

namespace {

const int kDefaultAnimationDurationForMenuMS = 150;

const float kWindowAnimation_HideOpacity = 0.f;
const float kWindowAnimation_ShowOpacity = 1.f;
const float kWindowAnimation_TranslateFactor = 0.5f;
const float kWindowAnimation_ScaleFactor = .95f;

const int kWindowAnimation_Rotate_DurationMS = 180;
const int kWindowAnimation_Rotate_OpacityDurationPercent = 90;
const float kWindowAnimation_Rotate_TranslateY = -20.f;
const float kWindowAnimation_Rotate_PerspectiveDepth = 500.f;
const float kWindowAnimation_Rotate_DegreesX = 5.f;
const float kWindowAnimation_Rotate_ScaleFactor = .99f;

const float kWindowAnimation_Bounce_Scale = 1.02f;
const int kWindowAnimation_Bounce_DurationMS = 180;
const int kWindowAnimation_Bounce_GrowShrinkDurationPercent = 40;

base::TimeDelta GetWindowVisibilityAnimationDuration(aura::Window* window) {
  int duration =
      window->GetProperty(kWindowVisibilityAnimationDurationKey);
  if (duration == 0 && window->type() == aura::client::WINDOW_TYPE_MENU) {
    return base::TimeDelta::FromMilliseconds(
        kDefaultAnimationDurationForMenuMS);
  }
  return TimeDelta::FromInternalValue(duration);
}

// Gets/sets the WindowVisibilityAnimationType associated with a window.
// TODO(beng): redundant/fold into method on public api?
int GetWindowVisibilityAnimationType(aura::Window* window) {
  int type = window->GetProperty(kWindowVisibilityAnimationTypeKey);
  if (type == WINDOW_VISIBILITY_ANIMATION_TYPE_DEFAULT) {
    return (window->type() == aura::client::WINDOW_TYPE_MENU ||
            window->type() == aura::client::WINDOW_TYPE_TOOLTIP) ?
        WINDOW_VISIBILITY_ANIMATION_TYPE_FADE :
        WINDOW_VISIBILITY_ANIMATION_TYPE_DROP;
  }
  return type;
}

// Observes a hide animation.
// A window can be hidden for a variety of reasons. Sometimes, Hide() will be
// called and life is simple. Sometimes, the window is actually bound to a
// views::Widget and that Widget is closed, and life is a little more
// complicated. When a Widget is closed the aura::Window* is actually not
// destroyed immediately - it is actually just immediately hidden and then
// destroyed when the stack unwinds. To handle this case, we start the hide
// animation immediately when the window is hidden, then when the window is
// subsequently destroyed this object acquires ownership of the window's layer,
// so that it can continue animating it until the animation completes.
// Regardless of whether or not the window is destroyed, this object deletes
// itself when the animation completes.
class HidingWindowAnimationObserver : public ui::ImplicitAnimationObserver,
                                      public aura::WindowObserver {
 public:
  explicit HidingWindowAnimationObserver(aura::Window* window)
      : window_(window) {
    window_->AddObserver(this);
  }
  virtual ~HidingWindowAnimationObserver() {
    STLDeleteElements(&layers_);
  }

 private:
  // Overridden from ui::ImplicitAnimationObserver:
  virtual void OnImplicitAnimationsCompleted() OVERRIDE {
    // Window may have been destroyed by this point.
    if (window_) {
      aura::client::AnimationHost* animation_host =
          aura::client::GetAnimationHost(window_);
      if (animation_host)
        animation_host->OnWindowHidingAnimationCompleted();
      window_->RemoveObserver(this);
    }
    delete this;
  }

  // Overridden from aura::WindowObserver:
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE {
    DCHECK_EQ(window, window_);
    DCHECK(layers_.empty());
    AcquireAllLayers(window_);

    // If the Widget has views with layers, then it is necessary to take
    // ownership of those layers too.
    views::Widget* widget = views::Widget::GetWidgetForNativeWindow(window_);
    const views::Widget* const_widget = widget;
    if (widget && const_widget->GetRootView() && widget->GetContentsView())
      AcquireAllViewLayers(widget->GetContentsView());
    window_->RemoveObserver(this);
    window_ = NULL;
  }

  void AcquireAllLayers(aura::Window* window) {
    ui::Layer* layer = window->AcquireLayer();
    DCHECK(layer);
    layers_.push_back(layer);
    for (aura::Window::Windows::const_iterator it = window->children().begin();
         it != window->children().end();
         ++it)
      AcquireAllLayers(*it);
  }

  void AcquireAllViewLayers(views::View* view) {
    for (int i = 0; i < view->child_count(); ++i)
      AcquireAllViewLayers(view->child_at(i));
    if (view->layer()) {
      ui::Layer* layer = view->RecreateLayer();
      if (layer) {
        layer->SuppressPaint();
        layers_.push_back(layer);
      }
    }
  }

  aura::Window* window_;
  std::vector<ui::Layer*> layers_;

  DISALLOW_COPY_AND_ASSIGN(HidingWindowAnimationObserver);
};

void GetTransformRelativeToRoot(ui::Layer* layer, gfx::Transform* transform) {
  const Layer* root = layer;
  while (root->parent())
    root = root->parent();
  layer->GetTargetTransformRelativeTo(root, transform);
}

gfx::Rect GetLayerWorldBoundsAfterTransform(ui::Layer* layer,
                                            const gfx::Transform& transform) {
  gfx::Transform in_world = transform;
  GetTransformRelativeToRoot(layer, &in_world);

  gfx::RectF transformed = layer->bounds();
  in_world.TransformRect(&transformed);

  return gfx::ToEnclosingRect(transformed);
}

// Augment the host window so that the enclosing bounds of the full
// animation will fit inside of it.
void AugmentWindowSize(aura::Window* window,
                       const gfx::Transform& start_transform,
                       const gfx::Transform& end_transform) {
  aura::client::AnimationHost* animation_host =
      aura::client::GetAnimationHost(window);
  if (!animation_host)
    return;

  gfx::Rect world_at_start =
      GetLayerWorldBoundsAfterTransform(window->layer(), start_transform);
  gfx::Rect world_at_end =
      GetLayerWorldBoundsAfterTransform(window->layer(), end_transform);
  gfx::Rect union_in_window_space =
      gfx::UnionRects(world_at_start, world_at_end);
  gfx::Rect current_bounds = window->bounds();
  gfx::Rect result(union_in_window_space.x() - current_bounds.x(),
                   union_in_window_space.y() - current_bounds.y(),
                   union_in_window_space.width() - current_bounds.width(),
                   union_in_window_space.height() - current_bounds.height());
  animation_host->SetHostTransitionBounds(result);
}

// Shows a window using an animation, animating its opacity from 0.f to 1.f,
// its visibility to true, and its transform from |start_transform| to
// |end_transform|.
void AnimateShowWindowCommon(aura::Window* window,
                             const gfx::Transform& start_transform,
                             const gfx::Transform& end_transform) {
  window->layer()->set_delegate(window);

  AugmentWindowSize(window, start_transform, end_transform);

  window->layer()->SetOpacity(kWindowAnimation_HideOpacity);
  window->layer()->SetTransform(start_transform);
  window->layer()->SetVisible(true);

  {
    // Property sets within this scope will be implicitly animated.
    ui::ScopedLayerAnimationSettings settings(window->layer()->GetAnimator());
    base::TimeDelta duration = GetWindowVisibilityAnimationDuration(window);
    if (duration.ToInternalValue() > 0)
      settings.SetTransitionDuration(duration);

    window->layer()->SetTransform(end_transform);
    window->layer()->SetOpacity(kWindowAnimation_ShowOpacity);
  }
}

// Hides a window using an animation, animating its opacity from 1.f to 0.f,
// its visibility to false, and its transform to |end_transform|.
void AnimateHideWindowCommon(aura::Window* window,
                             const gfx::Transform& end_transform) {
  AugmentWindowSize(window, gfx::Transform(), end_transform);
  window->layer()->set_delegate(NULL);

  // Property sets within this scope will be implicitly animated.
  ui::ScopedLayerAnimationSettings settings(window->layer()->GetAnimator());
  settings.AddObserver(new HidingWindowAnimationObserver(window));

  base::TimeDelta duration = GetWindowVisibilityAnimationDuration(window);
  if (duration.ToInternalValue() > 0)
    settings.SetTransitionDuration(duration);

  window->layer()->SetOpacity(kWindowAnimation_HideOpacity);
  window->layer()->SetTransform(end_transform);
  window->layer()->SetVisible(false);
}

static gfx::Transform GetScaleForWindow(aura::Window* window) {
  gfx::Rect bounds = window->bounds();
  gfx::Transform scale = gfx::GetScaleTransform(
      gfx::Point(kWindowAnimation_TranslateFactor * bounds.width(),
                 kWindowAnimation_TranslateFactor * bounds.height()),
      kWindowAnimation_ScaleFactor);
  return scale;
}

// Show/Hide windows using a shrink animation.
void AnimateShowWindow_Drop(aura::Window* window) {
  AnimateShowWindowCommon(window, GetScaleForWindow(window), gfx::Transform());
}

void AnimateHideWindow_Drop(aura::Window* window) {
  AnimateHideWindowCommon(window, GetScaleForWindow(window));
}

// Show/Hide windows using a vertical Glenimation.
void AnimateShowWindow_Vertical(aura::Window* window) {
  gfx::Transform transform;
  transform.Translate(0, window->GetProperty(
      kWindowVisibilityAnimationVerticalPositionKey));
  AnimateShowWindowCommon(window, transform, gfx::Transform());
}

void AnimateHideWindow_Vertical(aura::Window* window) {
  gfx::Transform transform;
  transform.Translate(0, window->GetProperty(
      kWindowVisibilityAnimationVerticalPositionKey));
  AnimateHideWindowCommon(window, transform);
}

// Show/Hide windows using a fade.
void AnimateShowWindow_Fade(aura::Window* window) {
  AnimateShowWindowCommon(window, gfx::Transform(), gfx::Transform());
}

void AnimateHideWindow_Fade(aura::Window* window) {
  AnimateHideWindowCommon(window, gfx::Transform());
}

ui::LayerAnimationElement* CreateGrowShrinkElement(
    aura::Window* window, bool grow) {
  scoped_ptr<ui::InterpolatedTransform> scale(new ui::InterpolatedScale(
      gfx::Point3F(kWindowAnimation_Bounce_Scale,
                   kWindowAnimation_Bounce_Scale,
                   1),
      gfx::Point3F(1, 1, 1)));
  scoped_ptr<ui::InterpolatedTransform> scale_about_pivot(
      new ui::InterpolatedTransformAboutPivot(
          gfx::Point(window->bounds().width() * 0.5,
                     window->bounds().height() * 0.5),
          scale.release()));
  scale_about_pivot->SetReversed(grow);
  scoped_ptr<ui::LayerAnimationElement> transition(
      ui::LayerAnimationElement::CreateInterpolatedTransformElement(
          scale_about_pivot.release(),
          base::TimeDelta::FromMilliseconds(
              kWindowAnimation_Bounce_DurationMS *
                  kWindowAnimation_Bounce_GrowShrinkDurationPercent / 100)));
  transition->set_tween_type(grow ? ui::Tween::EASE_OUT : ui::Tween::EASE_IN);
  return transition.release();
}

void AnimateBounce(aura::Window* window) {
  ui::ScopedLayerAnimationSettings scoped_settings(
      window->layer()->GetAnimator());
  scoped_settings.SetPreemptionStrategy(
      ui::LayerAnimator::REPLACE_QUEUED_ANIMATIONS);
  window->layer()->set_delegate(window);
  scoped_ptr<ui::LayerAnimationSequence> sequence(
      new ui::LayerAnimationSequence);
  sequence->AddElement(CreateGrowShrinkElement(window, true));
  ui::LayerAnimationElement::AnimatableProperties paused_properties;
  paused_properties.insert(ui::LayerAnimationElement::BOUNDS);
  sequence->AddElement(ui::LayerAnimationElement::CreatePauseElement(
      paused_properties,
      base::TimeDelta::FromMilliseconds(
        kWindowAnimation_Bounce_DurationMS *
            (100 - 2 * kWindowAnimation_Bounce_GrowShrinkDurationPercent) /
            100)));
  sequence->AddElement(CreateGrowShrinkElement(window, false));
  window->layer()->GetAnimator()->StartAnimation(sequence.release());
}

void AddLayerAnimationsForRotate(aura::Window* window, bool show) {
  window->layer()->set_delegate(window);
  if (show)
    window->layer()->SetOpacity(kWindowAnimation_HideOpacity);

  base::TimeDelta duration = base::TimeDelta::FromMilliseconds(
      kWindowAnimation_Rotate_DurationMS);

  if (!show) {
    new HidingWindowAnimationObserver(window);
    window->layer()->GetAnimator()->SchedulePauseForProperties(
        duration * (100 - kWindowAnimation_Rotate_OpacityDurationPercent) / 100,
        ui::LayerAnimationElement::OPACITY,
        -1);
  }
  scoped_ptr<ui::LayerAnimationElement> opacity(
      ui::LayerAnimationElement::CreateOpacityElement(
          show ? kWindowAnimation_ShowOpacity : kWindowAnimation_HideOpacity,
          duration * kWindowAnimation_Rotate_OpacityDurationPercent / 100));
  opacity->set_tween_type(ui::Tween::EASE_IN_OUT);
  window->layer()->GetAnimator()->ScheduleAnimation(
      new ui::LayerAnimationSequence(opacity.release()));

  float xcenter = window->bounds().width() * 0.5;

  gfx::Transform transform;
  transform.Translate(xcenter, 0);
  transform.ApplyPerspectiveDepth(kWindowAnimation_Rotate_PerspectiveDepth);
  transform.Translate(-xcenter, 0);
  scoped_ptr<ui::InterpolatedTransform> perspective(
      new ui::InterpolatedConstantTransform(transform));

  scoped_ptr<ui::InterpolatedTransform> scale(
      new ui::InterpolatedScale(1, kWindowAnimation_Rotate_ScaleFactor));
  scoped_ptr<ui::InterpolatedTransform> scale_about_pivot(
      new ui::InterpolatedTransformAboutPivot(
          gfx::Point(xcenter, kWindowAnimation_Rotate_TranslateY),
          scale.release()));

  scoped_ptr<ui::InterpolatedTransform> translation(
      new ui::InterpolatedTranslation(gfx::Point(), gfx::Point(
          0, kWindowAnimation_Rotate_TranslateY)));

  scoped_ptr<ui::InterpolatedTransform> rotation(
      new ui::InterpolatedAxisAngleRotation(
          gfx::Vector3dF(1, 0, 0), 0, kWindowAnimation_Rotate_DegreesX));

  scale_about_pivot->SetChild(perspective.release());
  translation->SetChild(scale_about_pivot.release());
  rotation->SetChild(translation.release());
  rotation->SetReversed(show);

  scoped_ptr<ui::LayerAnimationElement> transition(
      ui::LayerAnimationElement::CreateInterpolatedTransformElement(
          rotation.release(), duration));

  window->layer()->GetAnimator()->ScheduleAnimation(
      new ui::LayerAnimationSequence(transition.release()));
}

void AnimateShowWindow_Rotate(aura::Window* window) {
  AddLayerAnimationsForRotate(window, true);
}

void AnimateHideWindow_Rotate(aura::Window* window) {
  AddLayerAnimationsForRotate(window, false);
}

bool AnimateShowWindow(aura::Window* window) {
  if (!HasWindowVisibilityAnimationTransition(window, ANIMATE_SHOW))
    return false;

  switch (GetWindowVisibilityAnimationType(window)) {
    case WINDOW_VISIBILITY_ANIMATION_TYPE_DROP:
      AnimateShowWindow_Drop(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_VERTICAL:
      AnimateShowWindow_Vertical(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_FADE:
      AnimateShowWindow_Fade(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_ROTATE:
      AnimateShowWindow_Rotate(window);
      return true;
    default:
      return false;
  }
}

bool AnimateHideWindow(aura::Window* window) {
  if (!HasWindowVisibilityAnimationTransition(window, ANIMATE_HIDE))
    return false;

  switch (GetWindowVisibilityAnimationType(window)) {
    case WINDOW_VISIBILITY_ANIMATION_TYPE_DROP:
      AnimateHideWindow_Drop(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_VERTICAL:
      AnimateHideWindow_Vertical(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_FADE:
      AnimateHideWindow_Fade(window);
      return true;
    case WINDOW_VISIBILITY_ANIMATION_TYPE_ROTATE:
      AnimateHideWindow_Rotate(window);
      return true;
    default:
      return false;
  }
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// External interface

void SetWindowVisibilityAnimationType(aura::Window* window, int type) {
  window->SetProperty(kWindowVisibilityAnimationTypeKey, type);
}

int GetWindowVisibilityAnimationType(aura::Window* window) {
  return window->GetProperty(kWindowVisibilityAnimationTypeKey);
}

void SetWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition) {
  window->SetProperty(kWindowVisibilityAnimationTransitionKey, transition);
}

bool HasWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition) {
  WindowVisibilityAnimationTransition prop = window->GetProperty(
      kWindowVisibilityAnimationTransitionKey);
  return (prop & transition) != 0;
}

void SetWindowVisibilityAnimationDuration(aura::Window* window,
                                          const TimeDelta& duration) {
  window->SetProperty(kWindowVisibilityAnimationDurationKey,
                      static_cast<int>(duration.ToInternalValue()));
}

void SetWindowVisibilityAnimationVerticalPosition(aura::Window* window,
                                                  float position) {
  window->SetProperty(kWindowVisibilityAnimationVerticalPositionKey, position);
}

ui::ImplicitAnimationObserver* CreateHidingWindowAnimationObserver(
    aura::Window* window) {
  return new HidingWindowAnimationObserver(window);
}

bool AnimateOnChildWindowVisibilityChanged(aura::Window* window, bool visible) {
  if (WindowAnimationsDisabled(window))
    return false;
  if (visible)
    return AnimateShowWindow(window);
  // Don't start hiding the window again if it's already being hidden.
  return window->layer()->GetTargetOpacity() != 0.0f &&
      AnimateHideWindow(window);
}

bool AnimateWindow(aura::Window* window, WindowAnimationType type) {
  switch (type) {
  case WINDOW_ANIMATION_TYPE_BOUNCE:
    AnimateBounce(window);
    return true;
  default:
    NOTREACHED();
    return false;
  }
}

bool WindowAnimationsDisabled(aura::Window* window) {
  return (window &&
          window->GetProperty(aura::client::kAnimationsDisabledKey)) ||
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kWindowAnimationsDisabled);
}

}  // namespace corewm
}  // namespace views
