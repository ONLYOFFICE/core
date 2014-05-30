// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "base/memory/scoped_nsobject.h"
#import "testing/gtest_mac.h"
#include "ui/app_list/app_list_view_delegate.h"
#import "ui/app_list/cocoa/app_list_view_controller.h"
#import "ui/app_list/cocoa/app_list_window_controller.h"
#include "ui/app_list/test/app_list_test_view_delegate.h"
#import "ui/base/test/ui_cocoa_test_helper.h"

namespace {

class AppListWindowControllerTest : public ui::CocoaTest {
 public:
  AppListWindowControllerTest();

 protected:
  virtual void TearDown() OVERRIDE;

  scoped_nsobject<AppListWindowController> controller_;

  app_list::test::AppListTestViewDelegate* delegate() {
    return static_cast<app_list::test::AppListTestViewDelegate*>(
        [[controller_ appListViewController] delegate]);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(AppListWindowControllerTest);
};

AppListWindowControllerTest::AppListWindowControllerTest() {
  Init();
  scoped_ptr<app_list::AppListViewDelegate> delegate(
      new app_list::test::AppListTestViewDelegate);
  controller_.reset([[AppListWindowController alloc] init]);
  [[controller_ appListViewController] setDelegate:delegate.Pass()];
}

void AppListWindowControllerTest::TearDown() {
  EXPECT_TRUE(controller_.get());
  [[controller_ window] close];
  controller_.reset();
  ui::CocoaTest::TearDown();
}

}  // namespace

// Test showing, hiding and closing the app list window.
TEST_F(AppListWindowControllerTest, ShowHideCloseRelease) {
  EXPECT_TRUE([controller_ window]);
  [[controller_ window] makeKeyAndOrderFront:nil];
  EXPECT_TRUE([[controller_ window] isVisible]);
  EXPECT_TRUE([[[controller_ window] contentView] superview]);
  [[controller_ window] close];  // Hide.
  EXPECT_FALSE([[controller_ window] isVisible]);
  [[controller_ window] makeKeyAndOrderFront:nil];
}

// Test that the key bound to cancel (usually Escape) asks the controller to
// dismiss the window.
TEST_F(AppListWindowControllerTest, DismissWithEscape) {
  [[controller_ window] makeKeyAndOrderFront:nil];
  EXPECT_EQ(0, delegate()->dismiss_count());
  [[controller_ window] cancelOperation:controller_];
  EXPECT_EQ(1, delegate()->dismiss_count());
}
