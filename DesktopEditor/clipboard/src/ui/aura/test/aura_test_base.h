// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_AURA_TEST_BASE_H_
#define UI_AURA_TEST_AURA_TEST_BASE_H_

#include "base/compiler_specific.h"
#include "base/basictypes.h"
#include "base/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/aura/test/aura_test_helper.h"

namespace aura {
class RootWindow;
class Window;
class WindowDelegate;
namespace test {

// A base class for aura unit tests.
// TODO(beng): Instances of this test will create and own a RootWindow.
class AuraTestBase : public testing::Test {
 public:
  AuraTestBase();
  virtual ~AuraTestBase();

  // testing::Test:
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  // Creates a normal window parented to |parent|.
  aura::Window* CreateNormalWindow(int id, Window* parent,
                                   aura::WindowDelegate* delegate);

  // Creates a transient window that is transient to |parent|.
  aura::Window* CreateTransientChild(int id, aura::Window* parent);

  // Attach |window| to the current shell's root window.
  void SetDefaultParentByPrimaryRootWindow(aura::Window* window);

 protected:
  void RunAllPendingInMessageLoop();

  RootWindow* root_window() { return helper_->root_window(); }

 private:
  bool setup_called_;
  bool teardown_called_;
  base::MessageLoopForUI message_loop_;
  scoped_ptr<AuraTestHelper> helper_;

  DISALLOW_COPY_AND_ASSIGN(AuraTestBase);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_AURA_TEST_BASE_H_
