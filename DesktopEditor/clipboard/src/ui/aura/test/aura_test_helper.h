// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_AURA_TEST_HELPER_H_
#define UI_AURA_TEST_AURA_TEST_HELPER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class MessageLoopForUI;
}

namespace ui {
class InputMethod;
class ScopedAnimationDurationScaleMode;
}

namespace aura {
class RootWindow;
class TestScreen;
namespace client {
class DefaultCaptureClient;
class FocusClient;
}
namespace test {
class TestActivationClient;
class TestStackingClient;

// A helper class owned by tests that does common initialization required for
// Aura use. This class creates a root window with clients and other objects
// that are necessary to run test on Aura.
class AuraTestHelper {
 public:
  explicit AuraTestHelper(base::MessageLoopForUI* message_loop);
  ~AuraTestHelper();

  // Creates and initializes (shows and sizes) the RootWindow for use in tests.
  void SetUp();

  // Clean up objects that are created for tests. This also deletes the Env
  // object.
  void TearDown();

  // Flushes message loop.
  void RunAllPendingInMessageLoop();

  RootWindow* root_window() { return root_window_.get(); }

  TestScreen* test_screen() { return test_screen_.get(); }

 private:
  base::MessageLoopForUI* message_loop_;
  bool setup_called_;
  bool teardown_called_;
  bool owns_root_window_;
  scoped_ptr<RootWindow> root_window_;
  scoped_ptr<TestStackingClient> stacking_client_;
  scoped_ptr<TestActivationClient> test_activation_client_;
  scoped_ptr<client::DefaultCaptureClient> capture_client_;
  scoped_ptr<ui::InputMethod> test_input_method_;
  scoped_ptr<client::FocusClient> focus_client_;
  scoped_ptr<TestScreen> test_screen_;
  scoped_ptr<ui::ScopedAnimationDurationScaleMode> zero_duration_mode_;

  DISALLOW_COPY_AND_ASSIGN(AuraTestHelper);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_AURA_TEST_HELPER_H_
