// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/visibility_controller.h"

#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/aura/test/test_windows.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/scoped_animation_duration_scale_mode.h"

namespace views {
namespace corewm {

typedef aura::test::AuraTestBase VisibilityControllerTest;

// Hiding a window in an animatable container should not hide the window's layer
// immediately.
TEST_F(VisibilityControllerTest, AnimateHideDoesntHideWindowLayer) {
  // We cannot disable animations for this test.
  ui::ScopedAnimationDurationScaleMode normal_duration_mode(
      ui::ScopedAnimationDurationScaleMode::NORMAL_DURATION);

  VisibilityController controller;
  aura::client::SetVisibilityClient(root_window(), &controller);

  SetChildWindowVisibilityChangesAnimated(root_window());

  aura::test::TestWindowDelegate d;
  scoped_ptr<aura::Window> animatable(
      aura::test::CreateTestWindowWithDelegate(
          &d, -2, gfx::Rect(0, 0, 50, 50), root_window()));
  scoped_ptr<aura::Window> non_animatable(
      aura::test::CreateTestWindowWithDelegateAndType(
          &d, aura::client::WINDOW_TYPE_CONTROL, -3, gfx::Rect(51, 51, 50, 50),
          root_window()));
  EXPECT_TRUE(animatable->IsVisible());
  EXPECT_TRUE(animatable->layer()->visible());
  animatable->Hide();
  EXPECT_FALSE(animatable->IsVisible());
  EXPECT_TRUE(animatable->layer()->visible());

  EXPECT_TRUE(non_animatable->IsVisible());
  EXPECT_TRUE(non_animatable->layer()->visible());
  non_animatable->Hide();
  EXPECT_FALSE(non_animatable->IsVisible());
  EXPECT_FALSE(non_animatable->layer()->visible());

  aura::client::SetVisibilityClient(root_window(), NULL);
}

}  // namespace corewm
}  // namespace views
