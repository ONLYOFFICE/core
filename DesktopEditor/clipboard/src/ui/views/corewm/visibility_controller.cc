// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/visibility_controller.h"

#include "ui/aura/window.h"
#include "ui/aura/window_property.h"
#include "ui/compositor/layer.h"
#include "ui/views/corewm/window_animations.h"

namespace views {
namespace corewm {

namespace {

// Property set on all windows whose child windows' visibility changes are
// animated.
DEFINE_WINDOW_PROPERTY_KEY(
    bool, kChildWindowVisibilityChangesAnimatedKey, false);

bool ShouldAnimateWindow(aura::Window* window) {
  return window->parent() && window->parent()->GetProperty(
      kChildWindowVisibilityChangesAnimatedKey);
}

}  // namespace

VisibilityController::VisibilityController() {
}

VisibilityController::~VisibilityController() {
}

bool VisibilityController::CallAnimateOnChildWindowVisibilityChanged(
    aura::Window* window,
    bool visible) {
  return AnimateOnChildWindowVisibilityChanged(window, visible);
}

void VisibilityController::UpdateLayerVisibility(aura::Window* window,
                                                 bool visible) {
  bool animated = window->type() != aura::client::WINDOW_TYPE_CONTROL &&
                  window->type() != aura::client::WINDOW_TYPE_UNKNOWN &&
                  ShouldAnimateWindow(window);
  animated = animated &&
      CallAnimateOnChildWindowVisibilityChanged(window, visible);

  if (!visible) {
    // For window hiding animation, we want to check if the window is already
    // animating, and not do SetVisible(false) if it is.
    // TODO(vollick): remove this.
    animated = animated || (window->layer()->GetAnimator()->
        IsAnimatingProperty(ui::LayerAnimationElement::OPACITY) &&
        window->layer()->GetTargetOpacity() == 0.0f);
  }

  // When a window is made visible, we always make its layer visible
  // immediately. When a window is hidden, the layer must be left visible and
  // only made not visible once the animation is complete.
  if (!animated || visible)
    window->layer()->SetVisible(visible);
}

SuspendChildWindowVisibilityAnimations::SuspendChildWindowVisibilityAnimations(
    aura::Window* window)
    : window_(window),
      original_enabled_(window->GetProperty(
          kChildWindowVisibilityChangesAnimatedKey)) {
  window_->ClearProperty(kChildWindowVisibilityChangesAnimatedKey);
}

SuspendChildWindowVisibilityAnimations::
    ~SuspendChildWindowVisibilityAnimations() {
  if (original_enabled_)
    window_->SetProperty(kChildWindowVisibilityChangesAnimatedKey, true);
  else
    window_->ClearProperty(kChildWindowVisibilityChangesAnimatedKey);
}

void SetChildWindowVisibilityChangesAnimated(aura::Window* window) {
  window->SetProperty(kChildWindowVisibilityChangesAnimatedKey, true);
}

}  // namespace corewm
}  // namespace views

