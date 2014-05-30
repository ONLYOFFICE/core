// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/aura_test_helper.h"

#include "base/message_loop.h"
#include "base/run_loop.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/client/default_capture_client.h"
#include "ui/aura/client/focus_client.h"
#include "ui/aura/env.h"
#include "ui/aura/focus_manager.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/test_activation_client.h"
#include "ui/aura/test/test_screen.h"
#include "ui/aura/test/test_stacking_client.h"
#include "ui/base/test/dummy_input_method.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/scoped_animation_duration_scale_mode.h"
#include "ui/gfx/screen.h"

#if defined(USE_X11)
#include "ui/base/x/x11_util.h"
#endif

namespace aura {
namespace test {

AuraTestHelper::AuraTestHelper(base::MessageLoopForUI* message_loop)
    : setup_called_(false),
      teardown_called_(false),
      owns_root_window_(false) {
  DCHECK(message_loop);
  message_loop_ = message_loop;
  // Disable animations during tests.
  zero_duration_mode_.reset(new ui::ScopedAnimationDurationScaleMode(
      ui::ScopedAnimationDurationScaleMode::ZERO_DURATION));
}

AuraTestHelper::~AuraTestHelper() {
  CHECK(setup_called_)
      << "AuraTestHelper::SetUp() never called.";
  CHECK(teardown_called_)
      << "AuraTestHelper::TearDown() never called.";
}

void AuraTestHelper::SetUp() {
  setup_called_ = true;
  Env::GetInstance();
  test_screen_.reset(TestScreen::Create());
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, test_screen_.get());
  root_window_.reset(test_screen_->CreateRootWindowForPrimaryDisplay());

  focus_client_.reset(new FocusManager);
  client::SetFocusClient(root_window_.get(), focus_client_.get());
  stacking_client_.reset(new TestStackingClient(root_window_.get()));
  test_activation_client_.reset(
      new test::TestActivationClient(root_window_.get()));
  capture_client_.reset(new client::DefaultCaptureClient(root_window_.get()));
  test_input_method_.reset(new ui::test::DummyInputMethod);
  root_window_->SetProperty(
      client::kRootWindowInputMethodKey,
      test_input_method_.get());

  root_window_->Show();
  // Ensure width != height so tests won't confuse them.
  root_window_->SetHostSize(gfx::Size(800, 600));
}

void AuraTestHelper::TearDown() {
  teardown_called_ = true;
  test_input_method_.reset();
  stacking_client_.reset();
  test_activation_client_.reset();
  capture_client_.reset();
  focus_client_.reset();
  root_window_.reset();
  test_screen_.reset();
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, NULL);

#if defined(USE_X11)
  ui::ResetXCursorCache();
#endif

  Env::DeleteInstance();
}

void AuraTestHelper::RunAllPendingInMessageLoop() {
#if !defined(OS_MACOSX)
  // TODO(jbates) crbug.com/134753 Find quitters of this RunLoop and have them
  //              use run_loop.QuitClosure().
  base::RunLoop run_loop(Env::GetInstance()->GetDispatcher());
  run_loop.RunUntilIdle();
#endif
}

}  // namespace test
}  // namespace aura
