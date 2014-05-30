// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_capture_client.h"

#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/test_screen.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/base/events/event.h"

namespace views {

class DesktopCaptureClientTest : public aura::test::AuraTestBase {
 public:
  virtual void SetUp() OVERRIDE {
    AuraTestBase::SetUp();
    desktop_capture_client_.reset(new DesktopCaptureClient(root_window()));

    second_root_.reset(new aura::RootWindow(
        aura::RootWindow::CreateParams(gfx::Rect(0, 0, 800, 600))));
    second_root_->Init();
    second_root_->Show();
    second_root_->SetHostSize(gfx::Size(800, 600));
    second_desktop_capture_client_.reset(
      new DesktopCaptureClient(second_root_.get()));
  }

  virtual void TearDown() OVERRIDE {
    RunAllPendingInMessageLoop();
    desktop_capture_client_.reset();

    AuraTestBase::TearDown();
  }

  scoped_ptr<DesktopCaptureClient> desktop_capture_client_;
  scoped_ptr<aura::RootWindow> second_root_;
  scoped_ptr<DesktopCaptureClient> second_desktop_capture_client_;
};

// Makes sure that internal details that are set on mouse down (such as
// mouse_pressed_handler()) are cleared when another root window takes capture.
TEST_F(DesktopCaptureClientTest, ResetMouseEventHandlerOnCapture) {
  // Create a window inside the RootWindow.
  scoped_ptr<aura::Window> w1(CreateNormalWindow(1, root_window(), NULL));

  // Make a synthesized mouse down event. Ensure that the RootWindow will
  // dispatch further mouse events to |w1|.
  ui::MouseEvent mouse_pressed_event(ui::ET_MOUSE_PRESSED, gfx::Point(5, 5),
                                     gfx::Point(5, 5), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_pressed_event);
  EXPECT_EQ(w1.get(), root_window()->mouse_pressed_handler());

  // Build a window in the second RootWindow.
  scoped_ptr<aura::Window> w2(CreateNormalWindow(2, second_root_.get(), NULL));

  // The act of having the second window take capture should clear out mouse
  // pressed handler in the first RootWindow.
  w2->SetCapture();
  EXPECT_EQ(NULL, root_window()->mouse_pressed_handler());
}

// Makes sure that when one window gets capture, it forces the release on the
// other. This is needed has to be handled explicitly on Linux, and is a sanity
// check on Windows.
TEST_F(DesktopCaptureClientTest, ResetOtherWindowCaptureOnCapture) {
  // Create a window inside the RootWindow.
  scoped_ptr<aura::Window> w1(CreateNormalWindow(1, root_window(), NULL));
  w1->SetCapture();
  EXPECT_EQ(w1.get(), desktop_capture_client_->GetCaptureWindow());

  // Build a window in the second RootWindow and give it capture. The other
  // capture should be cleared.
  scoped_ptr<aura::Window> w2(CreateNormalWindow(2, second_root_.get(), NULL));
  w2->SetCapture();
  EXPECT_EQ(NULL, desktop_capture_client_->GetCaptureWindow());
  EXPECT_EQ(w2.get(), second_desktop_capture_client_->GetCaptureWindow());
}

}  // namespace views
