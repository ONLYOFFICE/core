// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/compound_event_filter.h"

#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/event_generator.h"
#include "ui/aura/test/test_activation_client.h"
#include "ui/aura/test/test_windows.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_utils.h"

namespace {

base::TimeDelta GetTime() {
  return ui::EventTimeForNow();
}

class TestCursorClient : public aura::client::CursorClient {
 public:
  TestCursorClient() : visible_(true), mouse_events_enabled_(true) {}
  virtual ~TestCursorClient() {}

  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE {
  }

  virtual void ShowCursor() OVERRIDE {
    visible_ = true;
  }

  virtual void HideCursor() OVERRIDE {
    visible_ = false;
  }

  virtual bool IsCursorVisible() const OVERRIDE {
    return visible_;
  }

  virtual void EnableMouseEvents() OVERRIDE {
    mouse_events_enabled_ = true;
  }

  virtual void DisableMouseEvents() OVERRIDE {
    mouse_events_enabled_ = false;
  }

  virtual bool IsMouseEventsEnabled() const OVERRIDE {
    return mouse_events_enabled_;
  }

  virtual void SetDisplay(const gfx::Display& display) OVERRIDE {
  }

  virtual void LockCursor() OVERRIDE {
  }

  virtual void UnlockCursor() OVERRIDE {
  }

  virtual void SetCursorResourceModule(const string16& module_name) OVERRIDE {
  }

  virtual void AddObserver(
      aura::client::CursorClientObserver* observer) OVERRIDE {
  }

  virtual void RemoveObserver(
      aura::client::CursorClientObserver* observer) OVERRIDE {
  }

 private:
  bool visible_;
  bool mouse_events_enabled_;
};

}

namespace views {
namespace corewm {

namespace {

// An event filter that consumes all gesture events.
class ConsumeGestureEventFilter : public ui::EventHandler {
 public:
  ConsumeGestureEventFilter() {}
  virtual ~ConsumeGestureEventFilter() {}

 private:
  // Overridden from ui::EventHandler:
  virtual void OnGestureEvent(ui::GestureEvent* e) OVERRIDE {
    e->StopPropagation();
  }

  DISALLOW_COPY_AND_ASSIGN(ConsumeGestureEventFilter);
};

}  // namespace

typedef aura::test::AuraTestBase CompoundEventFilterTest;

TEST_F(CompoundEventFilterTest, TouchHidesCursor) {
  scoped_ptr<CompoundEventFilter> compound_filter(new CompoundEventFilter);
  aura::Env::GetInstance()->AddPreTargetHandler(compound_filter.get());
  aura::test::TestWindowDelegate delegate;
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(&delegate, 1234,
      gfx::Rect(5, 5, 100, 100), root_window()));
  window->Show();
  window->SetCapture();

  TestCursorClient cursor_client;
  aura::client::SetCursorClient(root_window(), &cursor_client);

  ui::MouseEvent mouse0(ui::ET_MOUSE_MOVED, gfx::Point(10, 10),
                        gfx::Point(10, 10), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(&mouse0);
  EXPECT_TRUE(cursor_client.IsMouseEventsEnabled());

  // This press is required for the GestureRecognizer to associate a target
  // with kTouchId
  ui::TouchEvent press0(
      ui::ET_TOUCH_PRESSED, gfx::Point(90, 90), 1, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press0);
  EXPECT_FALSE(cursor_client.IsMouseEventsEnabled());

  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(10, 10), 1, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
  EXPECT_FALSE(cursor_client.IsMouseEventsEnabled());

  ui::TouchEvent release(
      ui::ET_TOUCH_RELEASED, gfx::Point(10, 10), 1, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_FALSE(cursor_client.IsMouseEventsEnabled());

  ui::MouseEvent mouse1(ui::ET_MOUSE_MOVED, gfx::Point(10, 10),
                        gfx::Point(10, 10), 0);
  // Move the cursor again. The cursor should be visible.
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(&mouse1);
  EXPECT_TRUE(cursor_client.IsMouseEventsEnabled());

  // Now activate the window and press on it again.
  ui::TouchEvent press1(
      ui::ET_TOUCH_PRESSED, gfx::Point(90, 90), 1, GetTime());
  aura::client::GetActivationClient(
      root_window())->ActivateWindow(window.get());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_FALSE(cursor_client.IsMouseEventsEnabled());
  aura::Env::GetInstance()->RemovePreTargetHandler(compound_filter.get());
}

// Tests that if an event filter consumes a gesture, then it doesn't focus the
// window.
TEST_F(CompoundEventFilterTest, FilterConsumedGesture) {
  scoped_ptr<CompoundEventFilter> compound_filter(new CompoundEventFilter);
  scoped_ptr<ui::EventHandler> gesure_handler(new ConsumeGestureEventFilter);
  compound_filter->AddHandler(gesure_handler.get());
  aura::Env::GetInstance()->AddPreTargetHandler(compound_filter.get());
  aura::test::TestWindowDelegate delegate;
  DCHECK(root_window());
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(&delegate, 1234,
      gfx::Rect(5, 5, 100, 100), root_window()));
  window->Show();

  EXPECT_TRUE(window->CanFocus());
  EXPECT_FALSE(window->HasFocus());

  // Tap on the window should not focus it since the filter will be consuming
  // the gestures.
  aura::test::EventGenerator generator(root_window(), gfx::Point(50, 50));
  generator.PressTouch();
  EXPECT_FALSE(window->HasFocus());

  compound_filter->RemoveHandler(gesure_handler.get());
  aura::Env::GetInstance()->AddPreTargetHandler(compound_filter.get());
}

// Verifies we don't attempt to hide the mouse when the mouse is down and a
// touch event comes in.
TEST_F(CompoundEventFilterTest, DontHideWhenMouseDown) {
  aura::test::EventGenerator event_generator(root_window());

  scoped_ptr<CompoundEventFilter> compound_filter(new CompoundEventFilter);
  aura::Env::GetInstance()->AddPreTargetHandler(compound_filter.get());
  aura::test::TestWindowDelegate delegate;
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(&delegate, 1234,
      gfx::Rect(5, 5, 100, 100), root_window()));
  window->Show();

  TestCursorClient cursor_client;
  aura::client::SetCursorClient(root_window(), &cursor_client);

  // Move and press the mouse over the window.
  event_generator.MoveMouseTo(10, 10);
  EXPECT_TRUE(cursor_client.IsMouseEventsEnabled());
  event_generator.PressLeftButton();
  EXPECT_TRUE(cursor_client.IsMouseEventsEnabled());
  EXPECT_TRUE(aura::Env::GetInstance()->is_mouse_button_down());

  // Do a touch event. As the mouse button is down this should not disable mouse
  // events.
  event_generator.PressTouch();
  EXPECT_TRUE(cursor_client.IsMouseEventsEnabled());
  aura::Env::GetInstance()->RemovePreTargetHandler(compound_filter.get());
}

}  // namespace corewm
}  // namespace views
