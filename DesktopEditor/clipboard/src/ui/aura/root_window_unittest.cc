// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/root_window.h"

#include <vector>

#include "testing/gtest/include/gtest/gtest.h"
#include "ui/aura/client/event_client.h"
#include "ui/aura/env.h"
#include "ui/aura/focus_manager.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/event_generator.h"
#include "ui/aura/test/test_event_handler.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/aura/test/test_windows.h"
#include "ui/aura/window_tracker.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_handler.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/gestures/gesture_configuration.h"
#include "ui/base/hit_test.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/transform.h"

namespace aura {
namespace {

// A delegate that always returns a non-client component for hit tests.
class NonClientDelegate : public test::TestWindowDelegate {
 public:
  NonClientDelegate()
      : non_client_count_(0),
        mouse_event_count_(0),
        mouse_event_flags_(0x0) {
  }
  virtual ~NonClientDelegate() {}

  int non_client_count() const { return non_client_count_; }
  gfx::Point non_client_location() const { return non_client_location_; }
  int mouse_event_count() const { return mouse_event_count_; }
  gfx::Point mouse_event_location() const { return mouse_event_location_; }
  int mouse_event_flags() const { return mouse_event_flags_; }

  virtual int GetNonClientComponent(const gfx::Point& location) const OVERRIDE {
    NonClientDelegate* self = const_cast<NonClientDelegate*>(this);
    self->non_client_count_++;
    self->non_client_location_ = location;
    return HTTOPLEFT;
  }
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    mouse_event_count_++;
    mouse_event_location_ = event->location();
    mouse_event_flags_ = event->flags();
    event->SetHandled();
  }

 private:
  int non_client_count_;
  gfx::Point non_client_location_;
  int mouse_event_count_;
  gfx::Point mouse_event_location_;
  int mouse_event_flags_;

  DISALLOW_COPY_AND_ASSIGN(NonClientDelegate);
};

// A simple event handler that consumes key events.
class ConsumeKeyHandler : public test::TestEventHandler {
 public:
  ConsumeKeyHandler() {}
  virtual ~ConsumeKeyHandler() {}

  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE {
    test::TestEventHandler::OnKeyEvent(event);
    event->StopPropagation();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ConsumeKeyHandler);
};

bool IsFocusedWindow(aura::Window* window) {
  return client::GetFocusClient(window)->GetFocusedWindow() == window;
}

}  // namespace

typedef test::AuraTestBase RootWindowTest;

TEST_F(RootWindowTest, OnHostMouseEvent) {
  // Create two non-overlapping windows so we don't have to worry about which
  // is on top.
  scoped_ptr<NonClientDelegate> delegate1(new NonClientDelegate());
  scoped_ptr<NonClientDelegate> delegate2(new NonClientDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  gfx::Rect bounds1(100, 200, kWindowWidth, kWindowHeight);
  gfx::Rect bounds2(300, 400, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window1(CreateTestWindowWithDelegate(
      delegate1.get(), -1234, bounds1, root_window()));
  scoped_ptr<aura::Window> window2(CreateTestWindowWithDelegate(
      delegate2.get(), -5678, bounds2, root_window()));

  // Send a mouse event to window1.
  gfx::Point point(101, 201);
  ui::MouseEvent event1(
      ui::ET_MOUSE_PRESSED, point, point, ui::EF_LEFT_MOUSE_BUTTON);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(&event1);

  // Event was tested for non-client area for the target window.
  EXPECT_EQ(1, delegate1->non_client_count());
  EXPECT_EQ(0, delegate2->non_client_count());
  // The non-client component test was in local coordinates.
  EXPECT_EQ(gfx::Point(1, 1), delegate1->non_client_location());
  // Mouse event was received by target window.
  EXPECT_EQ(1, delegate1->mouse_event_count());
  EXPECT_EQ(0, delegate2->mouse_event_count());
  // Event was in local coordinates.
  EXPECT_EQ(gfx::Point(1, 1), delegate1->mouse_event_location());
  // Non-client flag was set.
  EXPECT_TRUE(delegate1->mouse_event_flags() & ui::EF_IS_NON_CLIENT);
}

TEST_F(RootWindowTest, RepostEvent) {
  // Test RepostEvent in RootWindow. It only works for Mouse Press.
  EXPECT_FALSE(Env::GetInstance()->is_mouse_button_down());
  gfx::Point point(10, 10);
  ui::MouseEvent event(
      ui::ET_MOUSE_PRESSED, point, point, ui::EF_LEFT_MOUSE_BUTTON);
  root_window()->RepostEvent(event);
  RunAllPendingInMessageLoop();
  EXPECT_TRUE(Env::GetInstance()->is_mouse_button_down());
}

// Check that we correctly track the state of the mouse buttons in response to
// button press and release events.
TEST_F(RootWindowTest, MouseButtonState) {
  EXPECT_FALSE(Env::GetInstance()->is_mouse_button_down());

  gfx::Point location;
  scoped_ptr<ui::MouseEvent> event;

  // Press the left button.
  event.reset(new ui::MouseEvent(
      ui::ET_MOUSE_PRESSED,
      location,
      location,
      ui::EF_LEFT_MOUSE_BUTTON));
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(event.get());
  EXPECT_TRUE(Env::GetInstance()->is_mouse_button_down());

  // Additionally press the right.
  event.reset(new ui::MouseEvent(
      ui::ET_MOUSE_PRESSED,
      location,
      location,
      ui::EF_LEFT_MOUSE_BUTTON | ui::EF_RIGHT_MOUSE_BUTTON));
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(event.get());
  EXPECT_TRUE(Env::GetInstance()->is_mouse_button_down());

  // Release the left button.
  event.reset(new ui::MouseEvent(
      ui::ET_MOUSE_RELEASED,
      location,
      location,
      ui::EF_RIGHT_MOUSE_BUTTON));
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(event.get());
  EXPECT_TRUE(Env::GetInstance()->is_mouse_button_down());

  // Release the right button.  We should ignore the Shift-is-down flag.
  event.reset(new ui::MouseEvent(
      ui::ET_MOUSE_RELEASED,
      location,
      location,
      ui::EF_SHIFT_DOWN));
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(event.get());
  EXPECT_FALSE(Env::GetInstance()->is_mouse_button_down());

  // Press the middle button.
  event.reset(new ui::MouseEvent(
      ui::ET_MOUSE_PRESSED,
      location,
      location,
      ui::EF_MIDDLE_MOUSE_BUTTON));
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(event.get());
  EXPECT_TRUE(Env::GetInstance()->is_mouse_button_down());
}

TEST_F(RootWindowTest, TranslatedEvent) {
  scoped_ptr<Window> w1(test::CreateTestWindowWithDelegate(NULL, 1,
      gfx::Rect(50, 50, 100, 100), root_window()));

  gfx::Point origin(100, 100);
  ui::MouseEvent root(ui::ET_MOUSE_PRESSED, origin, origin, 0);

  EXPECT_EQ("100,100", root.location().ToString());
  EXPECT_EQ("100,100", root.root_location().ToString());

  ui::MouseEvent translated_event(
      root, static_cast<Window*>(root_window()), w1.get(),
      ui::ET_MOUSE_ENTERED, root.flags());
  EXPECT_EQ("50,50", translated_event.location().ToString());
  EXPECT_EQ("100,100", translated_event.root_location().ToString());
}

namespace {

class TestEventClient : public client::EventClient {
 public:
  static const int kNonLockWindowId = 100;
  static const int kLockWindowId = 200;

  explicit TestEventClient(RootWindow* root_window)
      : root_window_(root_window),
        lock_(false) {
    client::SetEventClient(root_window_, this);
    Window* lock_window =
        test::CreateTestWindowWithBounds(root_window_->bounds(), root_window_);
    lock_window->set_id(kLockWindowId);
    Window* non_lock_window =
        test::CreateTestWindowWithBounds(root_window_->bounds(), root_window_);
    non_lock_window->set_id(kNonLockWindowId);
  }
  virtual ~TestEventClient() {
    client::SetEventClient(root_window_, NULL);
  }

  // Starts/stops locking. Locking prevents windows other than those inside
  // the lock container from receiving events, getting focus etc.
  void Lock() {
    lock_ = true;
  }
  void Unlock() {
    lock_ = false;
  }

  Window* GetLockWindow() {
    return const_cast<Window*>(
        static_cast<const TestEventClient*>(this)->GetLockWindow());
  }
  const Window* GetLockWindow() const {
    return root_window_->GetChildById(kLockWindowId);
  }
  Window* GetNonLockWindow() {
    return root_window_->GetChildById(kNonLockWindowId);
  }

 private:
  // Overridden from client::EventClient:
  virtual bool CanProcessEventsWithinSubtree(
      const Window* window) const OVERRIDE {
    return lock_ ?
        window->Contains(GetLockWindow()) || GetLockWindow()->Contains(window) :
        true;
  }

  virtual ui::EventTarget* GetToplevelEventTarget() OVERRIDE {
    return NULL;
  }

  RootWindow* root_window_;
  bool lock_;

  DISALLOW_COPY_AND_ASSIGN(TestEventClient);
};

}  // namespace

TEST_F(RootWindowTest, CanProcessEventsWithinSubtree) {
  TestEventClient client(root_window());
  test::TestWindowDelegate d;

  test::TestEventHandler* nonlock_ef = new test::TestEventHandler;
  test::TestEventHandler* lock_ef = new test::TestEventHandler;
  client.GetNonLockWindow()->SetEventFilter(nonlock_ef);
  client.GetLockWindow()->SetEventFilter(lock_ef);

  Window* w1 = test::CreateTestWindowWithBounds(gfx::Rect(10, 10, 20, 20),
                                                client.GetNonLockWindow());
  w1->set_id(1);
  Window* w2 = test::CreateTestWindowWithBounds(gfx::Rect(30, 30, 20, 20),
                                                client.GetNonLockWindow());
  w2->set_id(2);
  scoped_ptr<Window> w3(
      test::CreateTestWindowWithDelegate(&d, 3, gfx::Rect(30, 30, 20, 20),
                                         client.GetLockWindow()));

  w1->Focus();
  EXPECT_TRUE(IsFocusedWindow(w1));

  client.Lock();

  // Since we're locked, the attempt to focus w2 will be ignored.
  w2->Focus();
  EXPECT_TRUE(IsFocusedWindow(w1));
  EXPECT_FALSE(IsFocusedWindow(w2));

  {
    // Attempting to send a key event to w1 (not in the lock container) should
    // cause focus to be reset.
    test::EventGenerator generator(root_window());
    generator.PressKey(ui::VKEY_SPACE, 0);
    EXPECT_EQ(NULL, client::GetFocusClient(w1)->GetFocusedWindow());
  }

  {
    // Events sent to a window not in the lock container will not be processed.
    // i.e. never sent to the non-lock container's event filter.
    test::EventGenerator generator(root_window(), w1);
    generator.PressLeftButton();
    EXPECT_EQ(0, nonlock_ef->num_mouse_events());

    // Events sent to a window in the lock container will be processed.
    test::EventGenerator generator3(root_window(), w3.get());
    generator3.PressLeftButton();
    EXPECT_EQ(1, lock_ef->num_mouse_events());
  }

  // Prevent w3 from being deleted by the hierarchy since its delegate is owned
  // by this scope.
  w3->parent()->RemoveChild(w3.get());
}

TEST_F(RootWindowTest, IgnoreUnknownKeys) {
  test::TestEventHandler* filter = new ConsumeKeyHandler;
  root_window()->SetEventFilter(filter);  // passes ownership

  ui::KeyEvent unknown_event(ui::ET_KEY_PRESSED, ui::VKEY_UNKNOWN, 0, false);
  EXPECT_FALSE(root_window()->AsRootWindowHostDelegate()->OnHostKeyEvent(
      &unknown_event));
  EXPECT_EQ(0, filter->num_key_events());

  ui::KeyEvent known_event(ui::ET_KEY_PRESSED, ui::VKEY_A, 0, false);
  EXPECT_TRUE(root_window()->AsRootWindowHostDelegate()->OnHostKeyEvent(
      &known_event));
  EXPECT_EQ(1, filter->num_key_events());
}

// Tests that touch-events that are beyond the bounds of the root-window do get
// propagated to the event filters correctly with the root as the target.
TEST_F(RootWindowTest, TouchEventsOutsideBounds) {
  test::TestEventHandler* filter = new test::TestEventHandler;
  root_window()->SetEventFilter(filter);  // passes ownership

  gfx::Point position = root_window()->bounds().origin();
  position.Offset(-10, -10);
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, position, 0, base::TimeDelta());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_EQ(1, filter->num_touch_events());

  position = root_window()->bounds().origin();
  position.Offset(root_window()->bounds().width() + 10,
                  root_window()->bounds().height() + 10);
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, position, 0, base::TimeDelta());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_EQ(2, filter->num_touch_events());
}

// Tests that scroll events are dispatched correctly.
TEST_F(RootWindowTest, ScrollEventDispatch) {
  base::TimeDelta now = ui::EventTimeForNow();
  test::TestEventHandler* filter = new test::TestEventHandler;
  root_window()->SetEventFilter(filter);

  test::TestWindowDelegate delegate;
  scoped_ptr<Window> w1(CreateNormalWindow(1, root_window(), &delegate));
  w1->SetBounds(gfx::Rect(20, 20, 40, 40));

  // A scroll event on the root-window itself is dispatched.
  ui::ScrollEvent scroll1(ui::ET_SCROLL,
                          gfx::Point(10, 10),
                          now,
                          0,
                          0, -10,
                          0, -10,
                          2);
  root_window()->AsRootWindowHostDelegate()->OnHostScrollEvent(&scroll1);
  EXPECT_EQ(1, filter->num_scroll_events());

  // Scroll event on a window should be dispatched properly.
  ui::ScrollEvent scroll2(ui::ET_SCROLL,
                          gfx::Point(25, 30),
                          now,
                          0,
                          -10, 0,
                          -10, 0,
                          2);
  root_window()->AsRootWindowHostDelegate()->OnHostScrollEvent(&scroll2);
  EXPECT_EQ(2, filter->num_scroll_events());
}

namespace {

// FilterFilter that tracks the types of events it's seen.
class EventFilterRecorder : public ui::EventHandler {
 public:
  typedef std::vector<ui::EventType> Events;

  EventFilterRecorder() {}

  Events& events() { return events_; }

  // ui::EventHandler overrides:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE {
    events_.push_back(event->type());
  }

  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    events_.push_back(event->type());
  }

  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE {
    events_.push_back(event->type());
  }

  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE {
    events_.push_back(event->type());
  }

  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    events_.push_back(event->type());
  }

 private:
  Events events_;

  DISALLOW_COPY_AND_ASSIGN(EventFilterRecorder);
};

// Converts an EventType to a string.
std::string EventTypeToString(ui::EventType type) {
  switch (type) {
    case ui::ET_TOUCH_RELEASED:
      return "TOUCH_RELEASED";

    case ui::ET_TOUCH_PRESSED:
      return "TOUCH_PRESSED";

    case ui::ET_TOUCH_MOVED:
      return "TOUCH_MOVED";

    case ui::ET_MOUSE_PRESSED:
      return "MOUSE_PRESSED";

    case ui::ET_MOUSE_DRAGGED:
      return "MOUSE_DRAGGED";

    case ui::ET_MOUSE_RELEASED:
      return "MOUSE_RELEASED";

    case ui::ET_MOUSE_MOVED:
      return "MOUSE_MOVED";

    case ui::ET_MOUSE_ENTERED:
      return "MOUSE_ENTERED";

    case ui::ET_MOUSE_EXITED:
      return "MOUSE_EXITED";

    case ui::ET_GESTURE_SCROLL_BEGIN:
      return "GESTURE_SCROLL_BEGIN";

    case ui::ET_GESTURE_SCROLL_END:
      return "GESTURE_SCROLL_END";

    case ui::ET_GESTURE_SCROLL_UPDATE:
      return "GESTURE_SCROLL_UPDATE";

    case ui::ET_GESTURE_TAP:
      return "GESTURE_TAP";

    case ui::ET_GESTURE_TAP_DOWN:
      return "GESTURE_TAP_DOWN";

    case ui::ET_GESTURE_BEGIN:
      return "GESTURE_BEGIN";

    case ui::ET_GESTURE_END:
      return "GESTURE_END";

    default:
      break;
  }
  return "";
}

std::string EventTypesToString(const EventFilterRecorder::Events& events) {
  std::string result;
  for (size_t i = 0; i < events.size(); ++i) {
    if (i != 0)
      result += " ";
    result += EventTypeToString(events[i]);
  }
  return result;
}

}  // namespace

TEST_F(RootWindowTest, HoldMouseMove) {
  EventFilterRecorder* filter = new EventFilterRecorder;
  root_window()->SetEventFilter(filter);  // passes ownership

  test::TestWindowDelegate delegate;
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      &delegate, 1, gfx::Rect(0, 0, 100, 100), root_window()));

  ui::MouseEvent mouse_move_event(ui::ET_MOUSE_MOVED, gfx::Point(0, 0),
                                  gfx::Point(0, 0), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_move_event);
  // Discard MOUSE_ENTER.
  filter->events().clear();

  root_window()->HoldMouseMoves();

  // Check that we don't immediately dispatch the MOUSE_DRAGGED event.
  ui::MouseEvent mouse_dragged_event(ui::ET_MOUSE_DRAGGED, gfx::Point(0, 0),
                                     gfx::Point(0, 0), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event);
  EXPECT_TRUE(filter->events().empty());

  // Check that we do dispatch the held MOUSE_DRAGGED event before another type
  // of event.
  ui::MouseEvent mouse_pressed_event(ui::ET_MOUSE_PRESSED, gfx::Point(0, 0),
                                     gfx::Point(0, 0), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_pressed_event);
  EXPECT_EQ("MOUSE_DRAGGED MOUSE_PRESSED",
            EventTypesToString(filter->events()));
  filter->events().clear();

  // Check that we coalesce held MOUSE_DRAGGED events.
  ui::MouseEvent mouse_dragged_event2(ui::ET_MOUSE_DRAGGED, gfx::Point(1, 1),
                                      gfx::Point(1, 1), 0);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event);
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event2);
  EXPECT_TRUE(filter->events().empty());
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_pressed_event);
  EXPECT_EQ("MOUSE_DRAGGED MOUSE_PRESSED",
            EventTypesToString(filter->events()));
  filter->events().clear();

  // Check that on ReleaseMouseMoves, held events are not dispatched
  // immediately, but posted instead.
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event);
  root_window()->ReleaseMouseMoves();
  EXPECT_TRUE(filter->events().empty());
  RunAllPendingInMessageLoop();
  EXPECT_EQ("MOUSE_DRAGGED", EventTypesToString(filter->events()));
  filter->events().clear();

  // However if another message comes in before the dispatch,
  // the Check that on ReleaseMouseMoves, held events are not dispatched
  // immediately, but posted instead.
  root_window()->HoldMouseMoves();
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event);
  root_window()->ReleaseMouseMoves();
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_pressed_event);
  EXPECT_EQ("MOUSE_DRAGGED MOUSE_PRESSED",
            EventTypesToString(filter->events()));
  filter->events().clear();
  RunAllPendingInMessageLoop();
  EXPECT_TRUE(filter->events().empty());

  // Check that if the other message is another MOUSE_DRAGGED, we still coalesce
  // them.
  root_window()->HoldMouseMoves();
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event);
  root_window()->ReleaseMouseMoves();
  root_window()->AsRootWindowHostDelegate()->OnHostMouseEvent(
      &mouse_dragged_event2);
  EXPECT_EQ("MOUSE_DRAGGED", EventTypesToString(filter->events()));
  filter->events().clear();
  RunAllPendingInMessageLoop();
  EXPECT_TRUE(filter->events().empty());
}

class DeletingEventFilter : public ui::EventHandler {
 public:
  DeletingEventFilter()
      : delete_during_pre_handle_(false) {}
  virtual ~DeletingEventFilter() {}

  void Reset(bool delete_during_pre_handle) {
    delete_during_pre_handle_ = delete_during_pre_handle;
  }

 private:
  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE {
    if (delete_during_pre_handle_)
      delete event->target();
  }

  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    if (delete_during_pre_handle_)
      delete event->target();
  }

  bool delete_during_pre_handle_;

  DISALLOW_COPY_AND_ASSIGN(DeletingEventFilter);
};

class DeletingWindowDelegate : public test::TestWindowDelegate {
 public:
  DeletingWindowDelegate()
      : window_(NULL),
        delete_during_handle_(false),
        got_event_(false) {}
  virtual ~DeletingWindowDelegate() {}

  void Reset(Window* window, bool delete_during_handle) {
    window_ = window;
    delete_during_handle_ = delete_during_handle;
    got_event_ = false;
  }
  bool got_event() const { return got_event_; }

 private:
  // Overridden from WindowDelegate:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE {
    if (delete_during_handle_)
      delete window_;
    got_event_ = true;
  }

  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    if (delete_during_handle_)
      delete window_;
    got_event_ = true;
  }

  Window* window_;
  bool delete_during_handle_;
  bool got_event_;

  DISALLOW_COPY_AND_ASSIGN(DeletingWindowDelegate);
};

TEST_F(RootWindowTest, DeleteWindowDuringDispatch) {
  // Verifies that we can delete a window during each phase of event handling.
  // Deleting the window should not cause a crash, only prevent further
  // processing from occurring.
  scoped_ptr<Window> w1(CreateNormalWindow(1, root_window(), NULL));
  DeletingWindowDelegate d11;
  Window* w11 = CreateNormalWindow(11, w1.get(), &d11);
  WindowTracker tracker;
  DeletingEventFilter* w1_filter = new DeletingEventFilter;
  w1->SetEventFilter(w1_filter);
  client::GetFocusClient(w1.get())->FocusWindow(w11);

  test::EventGenerator generator(root_window(), w11);

  // First up, no one deletes anything.
  tracker.Add(w11);
  d11.Reset(w11, false);

  generator.PressLeftButton();
  EXPECT_TRUE(tracker.Contains(w11));
  EXPECT_TRUE(d11.got_event());
  generator.ReleaseLeftButton();

  // Delegate deletes w11. This will prevent the post-handle step from applying.
  w1_filter->Reset(false);
  d11.Reset(w11, true);
  generator.PressKey(ui::VKEY_A, 0);
  EXPECT_FALSE(tracker.Contains(w11));
  EXPECT_TRUE(d11.got_event());

  // Pre-handle step deletes w11. This will prevent the delegate and the post-
  // handle steps from applying.
  w11 = CreateNormalWindow(11, w1.get(), &d11);
  w1_filter->Reset(true);
  d11.Reset(w11, false);
  generator.PressLeftButton();
  EXPECT_FALSE(tracker.Contains(w11));
  EXPECT_FALSE(d11.got_event());
}

namespace {

// A window delegate that detaches the parent of the target's parent window when
// it receives a tap event.
class DetachesParentOnTapDelegate : public test::TestWindowDelegate {
 public:
  DetachesParentOnTapDelegate() {}
  virtual ~DetachesParentOnTapDelegate() {}

 private:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    if (event->type() == ui::ET_GESTURE_TAP_DOWN) {
      event->SetHandled();
      return;
    }

    if (event->type() == ui::ET_GESTURE_TAP) {
      Window* parent = static_cast<Window*>(event->target())->parent();
      parent->parent()->RemoveChild(parent);
      event->SetHandled();
    }
  }

  DISALLOW_COPY_AND_ASSIGN(DetachesParentOnTapDelegate);
};

}  // namespace

// Tests that the gesture recognizer is reset for all child windows when a
// window hides. No expectations, just checks that the test does not crash.
TEST_F(RootWindowTest, GestureRecognizerResetsTargetWhenParentHides) {
  scoped_ptr<Window> w1(CreateNormalWindow(1, root_window(), NULL));
  DetachesParentOnTapDelegate delegate;
  scoped_ptr<Window> parent(CreateNormalWindow(22, w1.get(), NULL));
  Window* child = CreateNormalWindow(11, parent.get(), &delegate);
  test::EventGenerator generator(root_window(), child);
  generator.GestureTapAt(gfx::Point(40, 40));
}

namespace {

// A window delegate that processes nested gestures on tap.
class NestedGestureDelegate : public test::TestWindowDelegate {
 public:
  NestedGestureDelegate(test::EventGenerator* generator,
                        const gfx::Point tap_location)
      : generator_(generator),
        tap_location_(tap_location),
        gesture_end_count_(0) {}
  virtual ~NestedGestureDelegate() {}

  int gesture_end_count() const { return gesture_end_count_; }

 private:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    switch (event->type()) {
      case ui::ET_GESTURE_TAP_DOWN:
        event->SetHandled();
        break;
      case ui::ET_GESTURE_TAP:
        if (generator_)
          generator_->GestureTapAt(tap_location_);
        event->SetHandled();
        break;
      case ui::ET_GESTURE_END:
        ++gesture_end_count_;
        break;
      default:
        break;
    }
  }

  test::EventGenerator* generator_;
  const gfx::Point tap_location_;
  int gesture_end_count_;
  DISALLOW_COPY_AND_ASSIGN(NestedGestureDelegate);
};

}  // namespace

// Tests that gesture end is delivered after nested gesture processing.
TEST_F(RootWindowTest, GestureEndDeliveredAfterNestedGestures) {
  NestedGestureDelegate d1(NULL, gfx::Point());
  scoped_ptr<Window> w1(CreateNormalWindow(1, root_window(), &d1));
  w1->SetBounds(gfx::Rect(0, 0, 100, 100));

  test::EventGenerator nested_generator(root_window(), w1.get());
  NestedGestureDelegate d2(&nested_generator, w1->bounds().CenterPoint());
  scoped_ptr<Window> w2(CreateNormalWindow(1, root_window(), &d2));
  w2->SetBounds(gfx::Rect(100, 0, 100, 100));

  // Tap on w2 which triggers nested gestures for w1.
  test::EventGenerator generator(root_window(), w2.get());
  generator.GestureTapAt(w2->bounds().CenterPoint());

  // Both windows should get their gesture end events.
  EXPECT_EQ(1, d1.gesture_end_count());
  EXPECT_EQ(1, d2.gesture_end_count());
}

}  // namespace aura
