// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/aura_test_base.h"

#include "ui/aura/test/aura_test_helper.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/aura/window.h"
#include "ui/base/gestures/gesture_configuration.h"
#include "ui/base/ime/text_input_test_support.h"

#if defined(OS_WIN)
#include "base/win/metro.h"
#include "ui/base/ime/win/tsf_bridge.h"
#endif

namespace aura {
namespace test {

AuraTestBase::AuraTestBase()
    : setup_called_(false),
      teardown_called_(false) {
}

AuraTestBase::~AuraTestBase() {
  CHECK(setup_called_)
      << "You have overridden SetUp but never called super class's SetUp";
  CHECK(teardown_called_)
      << "You have overridden TearDown but never called super class's TearDown";
}

void AuraTestBase::SetUp() {
  setup_called_ = true;
  testing::Test::SetUp();
  ui::TextInputTestSupport::Initialize();

  // Changing the parameters for gesture recognition shouldn't cause
  // tests to fail, so we use a separate set of parameters for unit
  // testing.
  ui::GestureConfiguration::set_long_press_time_in_seconds(1.0);
  ui::GestureConfiguration::set_semi_long_press_time_in_seconds(0.4);
  ui::GestureConfiguration::set_max_distance_for_two_finger_tap_in_pixels(300);
  ui::GestureConfiguration::set_max_seconds_between_double_click(0.7);
  ui::GestureConfiguration::
      set_max_separation_for_gesture_touches_in_pixels(150);
  ui::GestureConfiguration::
      set_max_touch_down_duration_in_seconds_for_click(0.8);
  ui::GestureConfiguration::set_max_touch_move_in_pixels_for_click(5);
  ui::GestureConfiguration::set_max_distance_between_taps_for_double_tap(20);
  ui::GestureConfiguration::set_min_distance_for_pinch_scroll_in_pixels(20);
  ui::GestureConfiguration::set_min_flick_speed_squared(550.f * 550.f);
  ui::GestureConfiguration::set_min_pinch_update_distance_in_pixels(5);
  ui::GestureConfiguration::set_min_rail_break_velocity(200);
  ui::GestureConfiguration::set_min_scroll_delta_squared(5 * 5);
  ui::GestureConfiguration::
      set_min_touch_down_duration_in_seconds_for_click(0.01);
  ui::GestureConfiguration::set_points_buffered_for_velocity(10);
  ui::GestureConfiguration::set_rail_break_proportion(15);
  ui::GestureConfiguration::set_rail_start_proportion(2);
  ui::GestureConfiguration::set_default_radius(0);
  ui::GestureConfiguration::set_fling_acceleration_curve_coefficients(
      0, 0.0166667f);
  ui::GestureConfiguration::set_fling_acceleration_curve_coefficients(
      1, -0.0238095f);
  ui::GestureConfiguration::set_fling_acceleration_curve_coefficients(
      2, 0.0452381f);
  ui::GestureConfiguration::set_fling_acceleration_curve_coefficients(
      3, 0.8f);
  ui::GestureConfiguration::set_fling_velocity_cap(15000.0f);

  helper_.reset(new AuraTestHelper(&message_loop_));
  helper_->SetUp();
#if defined(OS_WIN)
    if (base::win::IsTSFAwareRequired())
      ui::TSFBridge::Initialize();
#endif
}

void AuraTestBase::TearDown() {
  teardown_called_ = true;

  // Flush the message loop because we have pending release tasks
  // and these tasks if un-executed would upset Valgrind.
  RunAllPendingInMessageLoop();

  helper_->TearDown();
  ui::TextInputTestSupport::Shutdown();
  testing::Test::TearDown();
}

Window* AuraTestBase::CreateNormalWindow(int id, Window* parent,
                                         WindowDelegate* delegate) {
  Window* window = new Window(
      delegate ? delegate :
      test::TestWindowDelegate::CreateSelfDestroyingDelegate());
  window->set_id(id);
  window->Init(ui::LAYER_TEXTURED);
  parent->AddChild(window);
  window->SetBounds(gfx::Rect(0, 0, 100, 100));
  window->Show();
  return window;
}

Window* AuraTestBase::CreateTransientChild(int id, Window* parent) {
  Window* window = new Window(NULL);
  window->set_id(id);
  window->SetType(aura::client::WINDOW_TYPE_NORMAL);
  window->Init(ui::LAYER_TEXTURED);
  window->SetDefaultParentByRootWindow(root_window(), gfx::Rect());
  parent->AddTransientChild(window);
  return window;
}

void AuraTestBase::SetDefaultParentByPrimaryRootWindow(aura::Window* window) {
  window->SetDefaultParentByRootWindow(root_window(), gfx::Rect());
}

void AuraTestBase::RunAllPendingInMessageLoop() {
  helper_->RunAllPendingInMessageLoop();
}

}  // namespace test
}  // namespace aura
