// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/scoped_vector.h"
#include "base/run_loop.h"
#include "base/string_number_conversions.h"
#include "base/timer.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/event_generator.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/aura/test/test_windows.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/gestures/gesture_configuration.h"
#include "ui/base/gestures/gesture_recognizer_impl.h"
#include "ui/base/gestures/gesture_sequence.h"
#include "ui/base/gestures/gesture_types.h"
#include "ui/base/hit_test.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

#include <queue>

namespace aura {
namespace test {

namespace {

std::string WindowIDAsString(ui::GestureConsumer* consumer) {
  return consumer && !consumer->ignores_events() ?
      base::IntToString(static_cast<Window*>(consumer)->id()) : "?";
}

#define EXPECT_0_EVENTS(events) \
    EXPECT_EQ(0u, events.size())

#define EXPECT_1_EVENT(events, e0) \
    EXPECT_EQ(1u, events.size()); \
    EXPECT_EQ(e0, events[0])

#define EXPECT_2_EVENTS(events, e0, e1) \
    EXPECT_EQ(2u, events.size()); \
    EXPECT_EQ(e0, events[0]); \
    EXPECT_EQ(e1, events[1])

#define EXPECT_3_EVENTS(events, e0, e1, e2) \
    EXPECT_EQ(3u, events.size()); \
    EXPECT_EQ(e0, events[0]); \
    EXPECT_EQ(e1, events[1]); \
    EXPECT_EQ(e2, events[2])

#define EXPECT_4_EVENTS(events, e0, e1, e2, e3) \
    EXPECT_EQ(4u, events.size()); \
    EXPECT_EQ(e0, events[0]); \
    EXPECT_EQ(e1, events[1]); \
    EXPECT_EQ(e2, events[2]); \
    EXPECT_EQ(e3, events[3])

// A delegate that keeps track of gesture events.
class GestureEventConsumeDelegate : public TestWindowDelegate {
 public:
  GestureEventConsumeDelegate()
      : tap_(false),
        tap_down_(false),
        tap_cancel_(false),
        begin_(false),
        end_(false),
        scroll_begin_(false),
        scroll_update_(false),
        scroll_end_(false),
        pinch_begin_(false),
        pinch_update_(false),
        pinch_end_(false),
        long_press_(false),
        fling_(false),
        two_finger_tap_(false),
        swipe_left_(false),
        swipe_right_(false),
        swipe_up_(false),
        swipe_down_(false),
        scroll_x_(0),
        scroll_y_(0),
        scroll_velocity_x_(0),
        scroll_velocity_y_(0),
        velocity_x_(0),
        velocity_y_(0),
        tap_count_(0),
        wait_until_event_(ui::ET_UNKNOWN) {
  }

  virtual ~GestureEventConsumeDelegate() {}

  void Reset() {
    events_.clear();
    tap_ = false;
    tap_down_ = false;
    tap_cancel_ = false;
    begin_ = false;
    end_ = false;
    scroll_begin_ = false;
    scroll_update_ = false;
    scroll_end_ = false;
    pinch_begin_ = false;
    pinch_update_ = false;
    pinch_end_ = false;
    long_press_ = false;
    fling_ = false;
    two_finger_tap_ = false;
    swipe_left_ = false;
    swipe_right_ = false;
    swipe_up_ = false;
    swipe_down_ = false;

    scroll_begin_position_.SetPoint(0, 0);
    tap_location_.SetPoint(0, 0);

    scroll_x_ = 0;
    scroll_y_ = 0;
    scroll_velocity_x_ = 0;
    scroll_velocity_y_ = 0;
    velocity_x_ = 0;
    velocity_y_ = 0;
    tap_count_ = 0;
  }

  const std::vector<ui::EventType>& events() const { return events_; };

  bool tap() const { return tap_; }
  bool tap_down() const { return tap_down_; }
  bool tap_cancel() const { return tap_cancel_; }
  bool begin() const { return begin_; }
  bool end() const { return end_; }
  bool scroll_begin() const { return scroll_begin_; }
  bool scroll_update() const { return scroll_update_; }
  bool scroll_end() const { return scroll_end_; }
  bool pinch_begin() const { return pinch_begin_; }
  bool pinch_update() const { return pinch_update_; }
  bool pinch_end() const { return pinch_end_; }
  bool long_press() const { return long_press_; }
  bool long_tap() const { return long_tap_; }
  bool fling() const { return fling_; }
  bool two_finger_tap() const { return two_finger_tap_; }
  bool swipe_left() const { return swipe_left_; }
  bool swipe_right() const { return swipe_right_; }
  bool swipe_up() const { return swipe_up_; }
  bool swipe_down() const { return swipe_down_; }

  const gfx::Point scroll_begin_position() const {
    return scroll_begin_position_;
  }

  const gfx::Point tap_location() const {
    return tap_location_;
  }

  float scroll_x() const { return scroll_x_; }
  float scroll_y() const { return scroll_y_; }
  float scroll_velocity_x() const { return scroll_velocity_x_; }
  float scroll_velocity_y() const { return scroll_velocity_y_; }
  int touch_id() const { return touch_id_; }
  float velocity_x() const { return velocity_x_; }
  float velocity_y() const { return velocity_y_; }
  const gfx::Rect& bounding_box() const { return bounding_box_; }
  int tap_count() const { return tap_count_; }

  void WaitUntilReceivedGesture(ui::EventType type) {
    wait_until_event_ = type;
    run_loop_.reset(new base::RunLoop(
        Env::GetInstance()->GetDispatcher()));
    run_loop_->Run();
  }

  virtual void OnGestureEvent(ui::GestureEvent* gesture) OVERRIDE {
    events_.push_back(gesture->type());
    bounding_box_ = gesture->details().bounding_box();
    switch (gesture->type()) {
      case ui::ET_GESTURE_TAP:
        tap_location_ = gesture->location();
        tap_count_ = gesture->details().tap_count();
        tap_ = true;
        break;
      case ui::ET_GESTURE_TAP_DOWN:
        tap_down_ = true;
        break;
      case ui::ET_GESTURE_TAP_CANCEL:
        tap_cancel_ = true;
        break;
      case ui::ET_GESTURE_BEGIN:
        begin_ = true;
        break;
      case ui::ET_GESTURE_END:
        end_ = true;
        break;
      case ui::ET_GESTURE_SCROLL_BEGIN:
        scroll_begin_ = true;
        scroll_begin_position_ = gesture->location();
        break;
      case ui::ET_GESTURE_SCROLL_UPDATE:
        scroll_update_ = true;
        scroll_x_ += gesture->details().scroll_x();
        scroll_y_ += gesture->details().scroll_y();
        scroll_velocity_x_ = gesture->details().velocity_x();
        scroll_velocity_y_ = gesture->details().velocity_y();
        break;
      case ui::ET_GESTURE_SCROLL_END:
        EXPECT_TRUE(velocity_x_ == 0 && velocity_y_ == 0);
        scroll_end_ = true;
        break;
      case ui::ET_GESTURE_PINCH_BEGIN:
        pinch_begin_ = true;
        break;
      case ui::ET_GESTURE_PINCH_UPDATE:
        pinch_update_ = true;
        break;
      case ui::ET_GESTURE_PINCH_END:
        pinch_end_ = true;
        break;
      case ui::ET_GESTURE_LONG_PRESS:
        long_press_ = true;
        touch_id_ = gesture->details().touch_id();
        break;
      case ui::ET_GESTURE_LONG_TAP:
        long_tap_ = true;
        break;
      case ui::ET_SCROLL_FLING_START:
        EXPECT_TRUE(gesture->details().velocity_x() != 0 ||
                    gesture->details().velocity_y() != 0);
        EXPECT_FALSE(scroll_end_);
        fling_ = true;
        velocity_x_ = gesture->details().velocity_x();
        velocity_y_ = gesture->details().velocity_y();
        break;
      case ui::ET_GESTURE_TWO_FINGER_TAP:
        two_finger_tap_ = true;
        break;
      case ui::ET_GESTURE_MULTIFINGER_SWIPE:
        swipe_left_ = gesture->details().swipe_left();
        swipe_right_ = gesture->details().swipe_right();
        swipe_up_ = gesture->details().swipe_up();
        swipe_down_ = gesture->details().swipe_down();
        break;
      default:
        NOTREACHED();
    }
    if (wait_until_event_ == gesture->type() && run_loop_) {
      run_loop_->Quit();
      wait_until_event_ = ui::ET_UNKNOWN;
    }
    gesture->StopPropagation();
  }

 private:
  scoped_ptr<base::RunLoop> run_loop_;
  std::vector<ui::EventType> events_;

  bool tap_;
  bool tap_down_;
  bool tap_cancel_;
  bool begin_;
  bool end_;
  bool scroll_begin_;
  bool scroll_update_;
  bool scroll_end_;
  bool pinch_begin_;
  bool pinch_update_;
  bool pinch_end_;
  bool long_press_;
  bool long_tap_;
  bool fling_;
  bool two_finger_tap_;
  bool swipe_left_;
  bool swipe_right_;
  bool swipe_up_;
  bool swipe_down_;

  gfx::Point scroll_begin_position_;
  gfx::Point tap_location_;

  float scroll_x_;
  float scroll_y_;
  float scroll_velocity_x_;
  float scroll_velocity_y_;
  float velocity_x_;
  float velocity_y_;
  int touch_id_;
  gfx::Rect bounding_box_;
  int tap_count_;

  ui::EventType wait_until_event_;

  DISALLOW_COPY_AND_ASSIGN(GestureEventConsumeDelegate);
};

class QueueTouchEventDelegate : public GestureEventConsumeDelegate {
 public:
  explicit QueueTouchEventDelegate(RootWindow* root_window)
      : window_(NULL),
        root_window_(root_window),
        queue_events_(true) {
  }
  virtual ~QueueTouchEventDelegate() {}

  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE {
    if (queue_events_) {
      queue_.push(new ui::TouchEvent(*event, window_, window_));
      event->StopPropagation();
    }
  }

  void ReceivedAck() {
    ReceivedAckImpl(false);
  }

  void ReceivedAckPreventDefaulted() {
    ReceivedAckImpl(true);
  }

  void set_window(Window* w) { window_ = w; }
  void set_queue_events(bool queue) { queue_events_ = queue; }

 private:
  void ReceivedAckImpl(bool prevent_defaulted) {
    scoped_ptr<ui::TouchEvent> event(queue_.front());
    root_window_->ProcessedTouchEvent(event.get(), window_,
        prevent_defaulted ? ui::ER_HANDLED : ui::ER_UNHANDLED);
    queue_.pop();
  }

  std::queue<ui::TouchEvent*> queue_;
  Window* window_;
  RootWindow* root_window_;
  bool queue_events_;

  DISALLOW_COPY_AND_ASSIGN(QueueTouchEventDelegate);
};

// A delegate that ignores gesture events but keeps track of [synthetic] mouse
// events.
class GestureEventSynthDelegate : public TestWindowDelegate {
 public:
  GestureEventSynthDelegate()
      : mouse_enter_(false),
        mouse_exit_(false),
        mouse_press_(false),
        mouse_release_(false),
        mouse_move_(false),
        double_click_(false) {
  }

  void Reset() {
    mouse_enter_ = false;
    mouse_exit_ = false;
    mouse_press_ = false;
    mouse_release_ = false;
    mouse_move_ = false;
    double_click_ = false;
  }

  bool mouse_enter() const { return mouse_enter_; }
  bool mouse_exit() const { return mouse_exit_; }
  bool mouse_press() const { return mouse_press_; }
  bool mouse_move() const { return mouse_move_; }
  bool mouse_release() const { return mouse_release_; }
  bool double_click() const { return double_click_; }

  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    switch (event->type()) {
      case ui::ET_MOUSE_PRESSED:
        double_click_ = event->flags() & ui::EF_IS_DOUBLE_CLICK;
        mouse_press_ = true;
        break;
      case ui::ET_MOUSE_RELEASED:
        mouse_release_ = true;
        break;
      case ui::ET_MOUSE_MOVED:
        mouse_move_ = true;
        break;
      case ui::ET_MOUSE_ENTERED:
        mouse_enter_ = true;
        break;
      case ui::ET_MOUSE_EXITED:
        mouse_exit_ = true;
        break;
      default:
        NOTREACHED();
    }
    event->SetHandled();
  }

 private:
  bool mouse_enter_;
  bool mouse_exit_;
  bool mouse_press_;
  bool mouse_release_;
  bool mouse_move_;
  bool double_click_;

  DISALLOW_COPY_AND_ASSIGN(GestureEventSynthDelegate);
};

class TestOneShotGestureSequenceTimer
    : public base::OneShotTimer<ui::GestureSequence> {
 public:
  TestOneShotGestureSequenceTimer() {}

  void ForceTimeout() {
    if (IsRunning()) {
      user_task().Run();
      Stop();
    }
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestOneShotGestureSequenceTimer);
};

class TimerTestGestureSequence : public ui::GestureSequence {
 public:
  explicit TimerTestGestureSequence(ui::GestureEventHelper* helper)
      : ui::GestureSequence(helper) {
  }

  void ForceTimeout() {
    static_cast<TestOneShotGestureSequenceTimer*>(
        GetLongPressTimer())->ForceTimeout();
  }

  bool IsTimerRunning() {
    return GetLongPressTimer()->IsRunning();
  }

  virtual base::OneShotTimer<ui::GestureSequence>* CreateTimer() OVERRIDE {
    return new TestOneShotGestureSequenceTimer();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TimerTestGestureSequence);
};

class TestGestureRecognizer : public ui::GestureRecognizerImpl {
 public:
  explicit TestGestureRecognizer(RootWindow* root_window)
      : GestureRecognizerImpl(root_window) {
  }

  ui::GestureSequence* GetGestureSequenceForTesting(Window* window) {
    return GetGestureSequenceForConsumer(window);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestGestureRecognizer);
};

class TimerTestGestureRecognizer : public TestGestureRecognizer {
 public:
  explicit TimerTestGestureRecognizer(RootWindow* root_window)
      : TestGestureRecognizer(root_window) {
  }

  virtual ui::GestureSequence* CreateSequence(
      ui::GestureEventHelper* helper) OVERRIDE {
    return new TimerTestGestureSequence(helper);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TimerTestGestureRecognizer);
};

base::TimeDelta GetTime() {
  return ui::EventTimeForNow();
}

class TimedEvents {
 private:
  int simulated_now_;

 public:
  TimedEvents() : simulated_now_(0) {
  }

  base::TimeDelta Now() {
    base::TimeDelta t = base::TimeDelta::FromMilliseconds(simulated_now_);
    simulated_now_++;
    return t;
  }

  base::TimeDelta LeapForward(int time_in_millis) {
    simulated_now_ += time_in_millis;
    return base::TimeDelta::FromMilliseconds(simulated_now_);
  }

  base::TimeDelta InFuture(int time_in_millis) {
    return base::TimeDelta::FromMilliseconds(simulated_now_ + time_in_millis);
  }

  void SendScrollEvents(RootWindow* root_window,
                        int x_start,
                        int y_start,
                        int dx,
                        int dy,
                        int touch_id,
                        int time_step,
                        int num_steps,
                        GestureEventConsumeDelegate* delegate) {
    int x = x_start;
    int y = y_start;

    for (int i = 0; i < num_steps; i++) {
      x += dx;
      y += dy;
      ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(x, y),
                          touch_id,
                          base::TimeDelta::FromMilliseconds(simulated_now_));
      root_window->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
      simulated_now_ += time_step;
    }
  }

  void SendScrollEvent(RootWindow* root_window,
                       int x,
                       int y,
                       int touch_id,
                       GestureEventConsumeDelegate* delegate) {
    delegate->Reset();
    ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(x, y),
                        touch_id,
                        base::TimeDelta::FromMilliseconds(simulated_now_));
    root_window->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
    simulated_now_++;
  }
};

}  // namespace

typedef AuraTestBase GestureRecognizerTest;

// Check that appropriate touch events generate tap gesture events.
TEST_F(GestureRecognizerTest, GestureEventTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());

  // Make sure there is enough delay before the touch is released so that it is
  // recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.LeapForward(50));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_TRUE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  EXPECT_EQ(1, delegate->tap_count());
}

// Check that appropriate touch events generate tap gesture events
// when information about the touch radii are provided.
TEST_F(GestureRecognizerTest, GestureEventTapRegion) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 800;
  const int kWindowHeight = 600;
  const int kTouchId = 2;
  gfx::Rect bounds(0, 0, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  // Test with no ET_TOUCH_MOVED events.
  {
     delegate->Reset();
     ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                          kTouchId, tes.Now());
     press.set_radius_x(5);
     press.set_radius_y(12);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
     EXPECT_FALSE(delegate->tap());
     EXPECT_TRUE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_TRUE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     // Make sure there is enough delay before the touch is released so that it
     // is recognized as a tap.
     delegate->Reset();
     ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                            kTouchId, tes.LeapForward(50));
     release.set_radius_x(5);
     release.set_radius_y(12);

     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
     EXPECT_TRUE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_TRUE(delegate->end());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());

     EXPECT_EQ(1, delegate->tap_count());
     gfx::Point actual_point(delegate->tap_location());
     EXPECT_EQ(24, delegate->bounding_box().width());
     EXPECT_EQ(24, delegate->bounding_box().height());
     EXPECT_EQ(101, actual_point.x());
     EXPECT_EQ(201, actual_point.y());
  }

  // Test with no ET_TOUCH_MOVED events but different touch points and radii.
  {
     delegate->Reset();
     ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(365, 290),
                          kTouchId, tes.Now());
     press.set_radius_x(8);
     press.set_radius_y(14);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
     EXPECT_FALSE(delegate->tap());
     EXPECT_TRUE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_TRUE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(367, 291),
                            kTouchId, tes.LeapForward(50));
     release.set_radius_x(20);
     release.set_radius_y(13);

     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
     EXPECT_TRUE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_TRUE(delegate->end());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());

     EXPECT_EQ(1, delegate->tap_count());
     gfx::Point actual_point(delegate->tap_location());
     EXPECT_EQ(40, delegate->bounding_box().width());
     EXPECT_EQ(40, delegate->bounding_box().height());
     EXPECT_EQ(367, actual_point.x());
     EXPECT_EQ(291, actual_point.y());
  }

  // Test with a single ET_TOUCH_MOVED event.
  {
     delegate->Reset();
     ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(46, 205),
                          kTouchId, tes.Now());
     press.set_radius_x(6);
     press.set_radius_y(10);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
     EXPECT_FALSE(delegate->tap());
     EXPECT_TRUE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_TRUE(delegate->begin());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(49, 204),
                         kTouchId, tes.LeapForward(50));
     move.set_radius_x(8);
     move.set_radius_y(12);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
     EXPECT_FALSE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(49, 204),
                            kTouchId, tes.LeapForward(50));
     release.set_radius_x(4);
     release.set_radius_y(8);

     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
     EXPECT_TRUE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_TRUE(delegate->end());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());

     EXPECT_EQ(1, delegate->tap_count());
     gfx::Point actual_point(delegate->tap_location());
     EXPECT_EQ(25, delegate->bounding_box().width());
     EXPECT_EQ(24, delegate->bounding_box().height());
     EXPECT_EQ(48, actual_point.x());
     EXPECT_EQ(204, actual_point.y());
  }

  // Test with a few ET_TOUCH_MOVED events.
  {
     delegate->Reset();
     ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(400, 150),
                          kTouchId, tes.Now());
     press.set_radius_x(7);
     press.set_radius_y(10);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
     EXPECT_FALSE(delegate->tap());
     EXPECT_TRUE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_TRUE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(397, 151),
                         kTouchId, tes.LeapForward(50));
     move.set_radius_x(13);
     move.set_radius_y(12);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
     EXPECT_FALSE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent move1(ui::ET_TOUCH_MOVED, gfx::Point(397, 149),
                          kTouchId, tes.LeapForward(50));
     move1.set_radius_x(16);
     move1.set_radius_y(16);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move1);
     EXPECT_FALSE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(400, 150),
                          kTouchId, tes.LeapForward(50));
     move2.set_radius_x(14);
     move2.set_radius_y(10);
     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
     EXPECT_FALSE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());
     EXPECT_FALSE(delegate->long_press());

     delegate->Reset();
     ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(401, 149),
                            kTouchId, tes.LeapForward(50));
     release.set_radius_x(8);
     release.set_radius_y(9);

     root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
     EXPECT_TRUE(delegate->tap());
     EXPECT_FALSE(delegate->tap_down());
     EXPECT_FALSE(delegate->tap_cancel());
     EXPECT_FALSE(delegate->begin());
     EXPECT_TRUE(delegate->end());
     EXPECT_FALSE(delegate->scroll_begin());
     EXPECT_FALSE(delegate->scroll_update());
     EXPECT_FALSE(delegate->scroll_end());

     EXPECT_EQ(1, delegate->tap_count());
     gfx::Point actual_point(delegate->tap_location());
     EXPECT_EQ(33, delegate->bounding_box().width());
     EXPECT_EQ(32, delegate->bounding_box().height());
     EXPECT_EQ(397, actual_point.x());
     EXPECT_EQ(149, actual_point.y());
  }
}

// Check that appropriate touch events generate scroll gesture events.
TEST_F(GestureRecognizerTest, GestureEventScroll) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 5;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_BEGIN,
                  ui::ET_GESTURE_TAP_DOWN);

  // Move the touch-point enough so that it is considered as a scroll. This
  // should generate both SCROLL_BEGIN and SCROLL_UPDATE gestures.
  // The first movement is diagonal, to ensure that we have a free scroll,
  // and not a rail scroll.
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId, delegate.get());
  EXPECT_3_EVENTS(delegate->events(),
                  ui::ET_GESTURE_TAP_CANCEL,
                  ui::ET_GESTURE_SCROLL_BEGIN,
                  ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(29, delegate->scroll_x());
  EXPECT_EQ(29, delegate->scroll_y());
  EXPECT_EQ(gfx::Point(1, 1).ToString(),
            delegate->scroll_begin_position().ToString());

  // When scrolling with a single finger, the bounding box of the gesture should
  // be empty, since it's a single point and the radius for testing is zero.
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());

  // Move some more to generate a few more scroll updates.
  tes.SendScrollEvent(root_window(), 110, 211, kTouchId, delegate.get());
  EXPECT_1_EVENT(delegate->events(), ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(-20, delegate->scroll_x());
  EXPECT_EQ(-19, delegate->scroll_y());
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());

  tes.SendScrollEvent(root_window(), 140, 215, kTouchId, delegate.get());
  EXPECT_1_EVENT(delegate->events(), ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(30, delegate->scroll_x());
  EXPECT_EQ(4, delegate->scroll_y());
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());

  // Release the touch. This should end the scroll.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId,
                         tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_SCROLL_FLING_START,
                  ui::ET_GESTURE_END);
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());
}

// Check that the bounding box during a scroll event is correct.
TEST_F(GestureRecognizerTest, GestureEventScrollBoundingBox) {
  TimedEvents tes;
  for (int radius = 1; radius <= 10; ++radius) {
    ui::GestureConfiguration::set_default_radius(radius);
    scoped_ptr<GestureEventConsumeDelegate> delegate(
        new GestureEventConsumeDelegate());
    const int kWindowWidth = 123;
    const int kWindowHeight = 45;
    const int kTouchId = 5;
    gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
    scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
        delegate.get(), -1234, bounds, root_window()));

    const int kPositionX = 101;
    const int kPositionY = 201;
    delegate->Reset();
    ui::TouchEvent press(ui::ET_TOUCH_PRESSED,
                         gfx::Point(kPositionX, kPositionY),
                         kTouchId,
                         tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
    EXPECT_EQ(gfx::Rect(kPositionX - radius,
                        kPositionY - radius,
                        radius * 2,
                        radius * 2).ToString(),
              delegate->bounding_box().ToString());

    const int kScrollAmount = 50;
    tes.SendScrollEvents(root_window(), kPositionX, kPositionY,
        1, 1, kTouchId, 1, kScrollAmount, delegate.get());
    EXPECT_EQ(gfx::Point(1, 1).ToString(),
              delegate->scroll_begin_position().ToString());
    EXPECT_EQ(gfx::Rect(kPositionX + kScrollAmount - radius,
                        kPositionY + kScrollAmount - radius,
                        radius * 2,
                        radius * 2).ToString(),
              delegate->bounding_box().ToString());

    // Release the touch. This should end the scroll.
    delegate->Reset();
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED,
                               gfx::Point(kPositionX + kScrollAmount,
                                          kPositionY + kScrollAmount),
                               kTouchId, press.time_stamp() +
                               base::TimeDelta::FromMilliseconds(50));
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    EXPECT_EQ(gfx::Rect(kPositionX + kScrollAmount - radius,
                        kPositionY + kScrollAmount - radius,
                        radius * 2,
                        radius * 2).ToString(),
              delegate->bounding_box().ToString());
  }
  ui::GestureConfiguration::set_default_radius(0);
}

// Check Scroll End Events report correct velocities
// if the user was on a horizontal rail
TEST_F(GestureRecognizerTest, GestureEventHorizontalRailFling) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kTouchId = 7;
  gfx::Rect bounds(0, 0, 1000, 1000);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 0),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);

  // Move the touch-point horizontally enough that it is considered a
  // horizontal scroll.
  tes.SendScrollEvent(root_window(), 20, 1, kTouchId, delegate.get());
  EXPECT_EQ(0, delegate->scroll_y());
  EXPECT_EQ(20, delegate->scroll_x());

  // Get a high x velocity, while still staying on the rail
  tes.SendScrollEvents(root_window(), 1, 1,
                   100, 10, kTouchId, 1,
                   ui::GestureConfiguration::points_buffered_for_velocity(),
                   delegate.get());
  // The y-velocity during the scroll should be 0 since this is in a horizontal
  // rail scroll.
  EXPECT_GT(delegate->scroll_velocity_x(), 0);
  EXPECT_EQ(0, delegate->scroll_velocity_y());

  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);

  EXPECT_TRUE(delegate->fling());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_GT(delegate->velocity_x(), 0);
  EXPECT_EQ(0, delegate->velocity_y());
}

// Check Scroll End Events report correct velocities
// if the user was on a vertical rail
TEST_F(GestureRecognizerTest, GestureEventVerticalRailFling) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kTouchId = 7;
  gfx::Rect bounds(0, 0, 1000, 1000);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 0),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);

  // Move the touch-point vertically enough that it is considered a
  // vertical scroll.
  tes.SendScrollEvent(root_window(), 1, 20, kTouchId, delegate.get());
  EXPECT_EQ(20, delegate->scroll_y());
  EXPECT_EQ(0, delegate->scroll_x());
  EXPECT_EQ(0, delegate->scroll_velocity_x());

  // Get a high y velocity, while still staying on the rail
  tes.SendScrollEvents(root_window(), 1, 1,
                       10, 100, kTouchId, 1,
                       ui::GestureConfiguration::points_buffered_for_velocity(),
                       delegate.get());
  EXPECT_EQ(0, delegate->scroll_velocity_x());
  EXPECT_GT(delegate->scroll_velocity_y(), 0);

  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);

  EXPECT_TRUE(delegate->fling());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(0, delegate->velocity_x());
  EXPECT_GT(delegate->velocity_y(), 0);
}

// Check Scroll End Events reports zero velocities
// if the user is not on a rail
TEST_F(GestureRecognizerTest, GestureEventNonRailFling) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kTouchId = 7;
  gfx::Rect bounds(0, 0, 1000, 1000);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 0),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);

  // Move the touch-point such that a non-rail scroll begins
  tes.SendScrollEvent(root_window(), 20, 20, kTouchId, delegate.get());
  EXPECT_EQ(20, delegate->scroll_y());
  EXPECT_EQ(20, delegate->scroll_x());

  tes.SendScrollEvents(root_window(), 1, 1,
                       10, 100, kTouchId, 1,
                       ui::GestureConfiguration::points_buffered_for_velocity(),
                       delegate.get());

  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);

  EXPECT_TRUE(delegate->fling());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_GT(delegate->velocity_x(), 0);
  EXPECT_GT(delegate->velocity_y(), 0);
}

// Check that appropriate touch events generate long press events
TEST_F(GestureRecognizerTest, GestureEventLongPress) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();

  TimerTestGestureRecognizer* gesture_recognizer =
      new TimerTestGestureRecognizer(root_window());

  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_TRUE(delegate->begin());
  EXPECT_FALSE(delegate->tap_cancel());

  // We haven't pressed long enough for a long press to occur
  EXPECT_FALSE(delegate->long_press());

  // Wait until the timer runs out
  delegate->WaitUntilReceivedGesture(ui::ET_GESTURE_LONG_PRESS);
  EXPECT_TRUE(delegate->long_press());
  EXPECT_EQ(0, delegate->touch_id());
  EXPECT_FALSE(delegate->tap_cancel());

  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->long_press());

  // Note the tap down isn't cancelled until the release
  EXPECT_TRUE(delegate->tap_cancel());
}

// Check that scrolling cancels a long press
TEST_F(GestureRecognizerTest, GestureEventLongPressCancelledByScroll) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  ui::GestureConfiguration::set_long_press_time_in_seconds(.01);
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 6;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();

  TimerTestGestureRecognizer* gesture_recognizer =
      new TimerTestGestureRecognizer(root_window());
  TimerTestGestureSequence* gesture_sequence =
      static_cast<TimerTestGestureSequence*>(
          gesture_recognizer->GetGestureSequenceForTesting(window.get()));

  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_TRUE(delegate->tap_down());

  // We haven't pressed long enough for a long press to occur
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->tap_cancel());

  // Scroll around, to cancel the long press
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId, delegate.get());
  // Wait until the timer runs out
  gesture_sequence->ForceTimeout();
  EXPECT_FALSE(delegate->long_press());
  EXPECT_TRUE(delegate->tap_cancel());

  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.LeapForward(10));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->tap_cancel());
}

// Check that appropriate touch events generate long tap events
TEST_F(GestureRecognizerTest, GestureEventLongTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();

  TimerTestGestureRecognizer* gesture_recognizer =
      new TimerTestGestureRecognizer(root_window());

  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_TRUE(delegate->begin());
  EXPECT_FALSE(delegate->tap_cancel());

  // We haven't pressed long enough for a long press to occur
  EXPECT_FALSE(delegate->long_press());

  // Wait until the timer runs out
  delegate->WaitUntilReceivedGesture(ui::ET_GESTURE_LONG_PRESS);
  EXPECT_TRUE(delegate->long_press());
  EXPECT_EQ(0, delegate->touch_id());
  EXPECT_FALSE(delegate->tap_cancel());

  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->long_press());
  EXPECT_TRUE(delegate->long_tap());

  // Note the tap down isn't cancelled until the release
  EXPECT_TRUE(delegate->tap_cancel());
}

// Check that second tap cancels a long press
TEST_F(GestureRecognizerTest, GestureEventLongPressCancelledBySecondTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  ui::GestureConfiguration::set_long_press_time_in_seconds(.01);
  const int kWindowWidth = 300;
  const int kWindowHeight = 400;
  const int kTouchId1 = 8;
  const int kTouchId2 = 2;
  gfx::Rect bounds(5, 5, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  TimerTestGestureRecognizer* gesture_recognizer =
      new TimerTestGestureRecognizer(root_window());
  TimerTestGestureSequence* gesture_sequence =
      static_cast<TimerTestGestureSequence*>(
          gesture_recognizer->GetGestureSequenceForTesting(window.get()));

  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_TRUE(delegate->begin());

  // We haven't pressed long enough for a long press to occur
  EXPECT_FALSE(delegate->long_press());

  // Second tap, to cancel the long press
  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_FALSE(delegate->tap_down());  // no touch down for second tap.
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());

  // Wait until the timer runs out
  gesture_sequence->ForceTimeout();

  // No long press occurred
  EXPECT_FALSE(delegate->long_press());

  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->long_press());
  EXPECT_TRUE(delegate->two_finger_tap());
  EXPECT_FALSE(delegate->tap_cancel());
}

// Check that horizontal scroll gestures cause scrolls on horizontal rails.
// Also tests that horizontal rails can be broken.
TEST_F(GestureRecognizerTest, GestureEventHorizontalRailScroll) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kTouchId = 7;
  gfx::Rect bounds(0, 0, 1000, 1000);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 0),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);

  // Move the touch-point horizontally enough that it is considered a
  // horizontal scroll.
  tes.SendScrollEvent(root_window(), 20, 1, kTouchId, delegate.get());
  EXPECT_EQ(0, delegate->scroll_y());
  EXPECT_EQ(20, delegate->scroll_x());

  tes.SendScrollEvent(root_window(), 25, 6, kTouchId, delegate.get());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(5, delegate->scroll_x());
  // y shouldn't change, as we're on a horizontal rail.
  EXPECT_EQ(0, delegate->scroll_y());

  // Send enough information that a velocity can be calculated for the gesture,
  // and we can break the rail
  tes.SendScrollEvents(root_window(), 1, 1,
                       1, 100, kTouchId, 1,
                       ui::GestureConfiguration::points_buffered_for_velocity(),
                       delegate.get());
  // Since the scroll is not longer railing, the velocity should be set for both
  // axis.
  EXPECT_GT(delegate->scroll_velocity_x(), 0);
  EXPECT_GT(delegate->scroll_velocity_y(), 0);

  tes.SendScrollEvent(root_window(), 0, 0, kTouchId, delegate.get());
  tes.SendScrollEvent(root_window(), 5, 5, kTouchId, delegate.get());

  // The rail should be broken
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(5, delegate->scroll_x());
  EXPECT_EQ(5, delegate->scroll_y());
}

// Check that vertical scroll gestures cause scrolls on vertical rails.
// Also tests that vertical rails can be broken.
TEST_F(GestureRecognizerTest, GestureEventVerticalRailScroll) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kTouchId = 7;
  gfx::Rect bounds(0, 0, 1000, 1000);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 0),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);

  // Move the touch-point vertically enough that it is considered a
  // vertical scroll.
  tes.SendScrollEvent(root_window(), 1, 20, kTouchId, delegate.get());
  EXPECT_EQ(0, delegate->scroll_x());
  EXPECT_EQ(20, delegate->scroll_y());

  tes.SendScrollEvent(root_window(), 6, 25, kTouchId, delegate.get());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(5, delegate->scroll_y());
  // x shouldn't change, as we're on a vertical rail.
  EXPECT_EQ(0, delegate->scroll_x());
  EXPECT_EQ(0, delegate->scroll_velocity_x());

  // Send enough information that a velocity can be calculated for the gesture,
  // and we can break the rail
  tes.SendScrollEvents(root_window(), 1, 1,
                       100, 1, kTouchId, 1,
                       ui::GestureConfiguration::points_buffered_for_velocity(),
                       delegate.get());
  EXPECT_GT(delegate->scroll_velocity_x(), 0);
  EXPECT_GT(delegate->scroll_velocity_y(), 0);

  tes.SendScrollEvent(root_window(), 0, 0, kTouchId, delegate.get());
  tes.SendScrollEvent(root_window(), 5, 5, kTouchId, delegate.get());

  // The rail should be broken
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(5, delegate->scroll_x());
  EXPECT_EQ(5, delegate->scroll_y());
}

TEST_F(GestureRecognizerTest, GestureTapFollowedByScroll) {
  // First, tap. Then, do a scroll using the same touch-id.
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 3;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Make sure there is enough delay before the touch is released so that it is
  // recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_TRUE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Now, do a scroll gesture. Delay it sufficiently so that it doesn't trigger
  // a double-tap.
  delegate->Reset();
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.LeapForward(1000));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Move the touch-point enough so that it is considered as a scroll. This
  // should generate both SCROLL_BEGIN and SCROLL_UPDATE gestures.
  // The first movement is diagonal, to ensure that we have a free scroll,
  // and not a rail scroll.
  delegate->Reset();
  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(130, 230),
                      kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(29, delegate->scroll_x());
  EXPECT_EQ(29, delegate->scroll_y());

  // Move some more to generate a few more scroll updates.
  delegate->Reset();
  ui::TouchEvent move1(ui::ET_TOUCH_MOVED, gfx::Point(110, 211),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(-20, delegate->scroll_x());
  EXPECT_EQ(-19, delegate->scroll_y());

  delegate->Reset();
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(140, 215),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(30, delegate->scroll_x());
  EXPECT_EQ(4, delegate->scroll_y());

  // Release the touch. This should end the scroll.
  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_TRUE(delegate->fling());
}

TEST_F(GestureRecognizerTest, AsynchronousGestureRecognition) {
  scoped_ptr<QueueTouchEventDelegate> queued_delegate(
      new QueueTouchEventDelegate(root_window()));
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 6;
  const int kTouchId2 = 4;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> queue(CreateTestWindowWithDelegate(
      queued_delegate.get(), -1234, bounds, root_window()));

  queued_delegate->set_window(queue.get());

  // Touch down on the window. This should not generate any gesture event.
  queued_delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                           kTouchId1, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  // Introduce some delay before the touch is released so that it is recognized
  // as a tap. However, this still should not create any gesture events.
  queued_delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                             kTouchId1, press.time_stamp() +
                             base::TimeDelta::FromMilliseconds(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  // Create another window, and place a touch-down on it. This should create a
  // tap-down gesture.
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -2345, gfx::Rect(0, 0, 50, 50), root_window()));
  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(10, 20),
                            kTouchId2, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(10, 20),
                              kTouchId2, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);

  // Process the first queued event.
  queued_delegate->Reset();
  queued_delegate->ReceivedAck();
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_TRUE(queued_delegate->tap_down());
  EXPECT_TRUE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  // Now, process the second queued event.
  queued_delegate->Reset();
  queued_delegate->ReceivedAck();
  EXPECT_TRUE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_TRUE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  // Start all over. Press on the first window, then press again on the second
  // window. The second press should still go to the first window.
  queued_delegate->Reset();
  ui::TouchEvent press3(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                            kTouchId1, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press3);
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  queued_delegate->Reset();
  delegate->Reset();
  ui::TouchEvent press4(ui::ET_TOUCH_PRESSED, gfx::Point(103, 203),
                            kTouchId2, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press4);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  // Move the second touch-point enough so that it is considered a pinch. This
  // should generate both SCROLL_BEGIN and PINCH_BEGIN gestures.
  queued_delegate->Reset();
  delegate->Reset();
  int x_move = ui::GestureConfiguration::max_touch_move_in_pixels_for_click();
  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(103 + x_move, 203),
                          kTouchId2, GetTime());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  queued_delegate->Reset();
  queued_delegate->ReceivedAck();
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_TRUE(queued_delegate->tap_down());
  EXPECT_TRUE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());

  queued_delegate->Reset();
  queued_delegate->ReceivedAck();
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());  // no touch down for second tap.
  EXPECT_TRUE(queued_delegate->tap_cancel());
  EXPECT_TRUE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_FALSE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());
  EXPECT_FALSE(queued_delegate->pinch_begin());
  EXPECT_FALSE(queued_delegate->pinch_update());
  EXPECT_FALSE(queued_delegate->pinch_end());

  queued_delegate->Reset();
  queued_delegate->ReceivedAck();
  EXPECT_FALSE(queued_delegate->tap());
  EXPECT_FALSE(queued_delegate->tap_down());
  EXPECT_FALSE(queued_delegate->tap_cancel());
  EXPECT_FALSE(queued_delegate->begin());
  EXPECT_FALSE(queued_delegate->end());
  EXPECT_TRUE(queued_delegate->scroll_begin());
  EXPECT_FALSE(queued_delegate->scroll_update());
  EXPECT_FALSE(queued_delegate->scroll_end());
  EXPECT_TRUE(queued_delegate->pinch_begin());
  EXPECT_FALSE(queued_delegate->pinch_update());
  EXPECT_FALSE(queued_delegate->pinch_end());
}

// Check that appropriate touch events generate pinch gesture events.
TEST_F(GestureRecognizerTest, GestureEventPinchFromScroll) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 300;
  const int kWindowHeight = 400;
  const int kTouchId1 = 5;
  const int kTouchId2 = 3;
  gfx::Rect bounds(5, 5, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  aura::RootWindow* root = root_window();

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_BEGIN,
                  ui::ET_GESTURE_TAP_DOWN);

  // Move the touch-point enough so that it is considered as a scroll. This
  // should generate both SCROLL_BEGIN and SCROLL_UPDATE gestures.
  delegate->Reset();
  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(130, 301),
                      kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
  EXPECT_3_EVENTS(delegate->events(),
                  ui::ET_GESTURE_TAP_CANCEL,
                  ui::ET_GESTURE_SCROLL_BEGIN,
                  ui::ET_GESTURE_SCROLL_UPDATE);

  // Press the second finger. It should cause pinch-begin. Note that we will not
  // transition to two finger tap here because the touch points are far enough.
  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_2_EVENTS(delegate->events(),
                 ui::ET_GESTURE_BEGIN,
                 ui::ET_GESTURE_PINCH_BEGIN);
  EXPECT_EQ(gfx::Rect(10, 10, 120, 291).ToString(),
            delegate->bounding_box().ToString());

  // Move the first finger.
  delegate->Reset();
  ui::TouchEvent move3(ui::ET_TOUCH_MOVED, gfx::Point(95, 201),
                       kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move3);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_PINCH_UPDATE,
                  ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(gfx::Rect(10, 10, 85, 191).ToString(),
            delegate->bounding_box().ToString());

  // Now move the second finger.
  delegate->Reset();
  ui::TouchEvent move4(ui::ET_TOUCH_MOVED, gfx::Point(55, 15),
                       kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move4);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_PINCH_UPDATE,
                  ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(gfx::Rect(55, 15, 40, 186).ToString(),
            delegate->bounding_box().ToString());

  // Release the first finger. This should end pinch.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_2_EVENTS(delegate->events(),
                 ui::ET_GESTURE_PINCH_END,
                 ui::ET_GESTURE_END);
  EXPECT_EQ(gfx::Rect(55, 15, 46, 186).ToString(),
            delegate->bounding_box().ToString());

  // Move the second finger. This should still generate a scroll.
  delegate->Reset();
  ui::TouchEvent move5(ui::ET_TOUCH_MOVED, gfx::Point(25, 10),
                       kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move5);
  EXPECT_1_EVENT(delegate->events(), ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());
}

TEST_F(GestureRecognizerTest, GestureEventPinchFromScrollFromPinch) {
scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 300;
  const int kWindowHeight = 400;
  const int kTouchId1 = 5;
  const int kTouchId2 = 3;
  gfx::Rect bounds(5, 5, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 301),
                       kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  // Since the touch points are far enough we will go to pinch rather than two
  // finger tap.
  EXPECT_TRUE(delegate->pinch_begin());

  tes.SendScrollEvent(root_window(), 130, 230, kTouchId1, delegate.get());
  EXPECT_TRUE(delegate->pinch_update());

  // Pinch has started, now release the second finger
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_TRUE(delegate->pinch_end());

  tes.SendScrollEvent(root_window(), 130, 230, kTouchId2, delegate.get());
  EXPECT_TRUE(delegate->scroll_update());

  // Pinch again
  delegate->Reset();
  ui::TouchEvent press3(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press3);
  // Now the touch points are close. So we will go into two finger tap.
  // Move the touch-point enough to break two-finger-tap and enter pinch.
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(101, 202),
                       kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  EXPECT_TRUE(delegate->pinch_begin());

  tes.SendScrollEvent(root_window(), 130, 230, kTouchId1, delegate.get());
  EXPECT_TRUE(delegate->pinch_update());
}

TEST_F(GestureRecognizerTest, GestureEventPinchFromTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;
  const int kWindowWidth = 300;
  const int kWindowHeight = 400;
  const int kTouchId1 = 3;
  const int kTouchId2 = 5;
  gfx::Rect bounds(5, 5, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  aura::RootWindow* root = root_window();

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 301),
                       kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_BEGIN,
                  ui::ET_GESTURE_TAP_DOWN);
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());

  // Press the second finger far enough to break two finger tap. It should
  // instead cause a scroll-begin and pinch-begin.
  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_4_EVENTS(delegate->events(),
                  ui::ET_GESTURE_TAP_CANCEL,
                  ui::ET_GESTURE_BEGIN,
                  ui::ET_GESTURE_PINCH_BEGIN,
                  ui::ET_GESTURE_SCROLL_BEGIN);
  EXPECT_EQ(gfx::Rect(10, 10, 91, 291).ToString(),
            delegate->bounding_box().ToString());

  // Move the first finger.
  delegate->Reset();
  ui::TouchEvent move3(ui::ET_TOUCH_MOVED, gfx::Point(65, 201),
                       kTouchId1, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move3);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_PINCH_UPDATE,
                  ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(gfx::Rect(10, 10, 55, 191).ToString(),
            delegate->bounding_box().ToString());

  // Now move the second finger.
  delegate->Reset();
  ui::TouchEvent move4(ui::ET_TOUCH_MOVED, gfx::Point(55, 15),
                       kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move4);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_PINCH_UPDATE,
                  ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_EQ(gfx::Rect(55, 15, 10, 186).ToString(),
            delegate->bounding_box().ToString());

  // Release the first finger. This should end pinch.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId1, tes.LeapForward(10));
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_2_EVENTS(delegate->events(),
                  ui::ET_GESTURE_PINCH_END,
                  ui::ET_GESTURE_END);
  EXPECT_EQ(gfx::Rect(55, 15, 46, 186).ToString(),
            delegate->bounding_box().ToString());

  // Move the second finger. This should still generate a scroll.
  delegate->Reset();
  ui::TouchEvent move5(ui::ET_TOUCH_MOVED, gfx::Point(25, 10),
                       kTouchId2, tes.Now());
  root->AsRootWindowHostDelegate()->OnHostTouchEvent(&move5);
  EXPECT_1_EVENT(delegate->events(), ui::ET_GESTURE_SCROLL_UPDATE);
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());
}

TEST_F(GestureRecognizerTest, GestureEventIgnoresDisconnectedEvents) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TimedEvents tes;

  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          6, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
}

// Check that a touch is locked to the window of the closest current touch
// within max_separation_for_gesture_touches_in_pixels
TEST_F(GestureRecognizerTest, GestureEventTouchLockSelectsCorrectWindow) {
  ui::GestureRecognizer* gesture_recognizer =
      new ui::GestureRecognizerImpl(root_window());
  TimedEvents tes;
  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  ui::GestureConsumer* target;
  const int kNumWindows = 4;

  scoped_ptr<GestureEventConsumeDelegate*[]> delegates(
      new GestureEventConsumeDelegate*[kNumWindows]);

  ui::GestureConfiguration::
      set_max_separation_for_gesture_touches_in_pixels(499);

  scoped_ptr<gfx::Rect[]> window_bounds(new gfx::Rect[kNumWindows]);
  window_bounds[0] = gfx::Rect(0, 0, 1, 1);
  window_bounds[1] = gfx::Rect(500, 0, 1, 1);
  window_bounds[2] = gfx::Rect(0, 500, 1, 1);
  window_bounds[3] = gfx::Rect(500, 500, 1, 1);

  scoped_ptr<aura::Window*[]> windows(new aura::Window*[kNumWindows]);

  // Instantiate windows with |window_bounds| and touch each window at
  // its origin.
  for (int i = 0; i < kNumWindows; ++i) {
    delegates[i] = new GestureEventConsumeDelegate();
    windows[i] = CreateTestWindowWithDelegate(
        delegates[i], i, window_bounds[i], root_window());
    windows[i]->set_id(i);
    ui::TouchEvent press(ui::ET_TOUCH_PRESSED, window_bounds[i].origin(),
                         i, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  }

  // Touches should now be associated with the closest touch within
  // ui::GestureConfiguration::max_separation_for_gesture_touches_in_pixels
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(11, 11));
  EXPECT_EQ("0", WindowIDAsString(target));
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(511, 11));
  EXPECT_EQ("1", WindowIDAsString(target));
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(11, 511));
  EXPECT_EQ("2", WindowIDAsString(target));
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(511, 511));
  EXPECT_EQ("3", WindowIDAsString(target));

  // Add a touch in the middle associated with windows[2]
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(0, 500),
                       kNumWindows, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(250, 250),
                      kNumWindows, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);

  target = gesture_recognizer->GetTargetForLocation(gfx::Point(250, 250));
  EXPECT_EQ("2", WindowIDAsString(target));

  // Make sure that ties are broken by distance to a current touch
  // Closer to the point in the bottom right.
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(380, 380));
  EXPECT_EQ("3", WindowIDAsString(target));

  // This touch is closer to the point in the middle
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(300, 300));
  EXPECT_EQ("2", WindowIDAsString(target));

  // A touch too far from other touches won't be locked to anything
  target = gesture_recognizer->GetTargetForLocation(gfx::Point(1000, 1000));
  EXPECT_TRUE(target == NULL);

  // Move a touch associated with windows[2] to 1000, 1000
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(1000, 1000),
                       kNumWindows, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);

  target = gesture_recognizer->GetTargetForLocation(gfx::Point(1000, 1000));
  EXPECT_EQ("2", WindowIDAsString(target));

  for (int i = 0; i < kNumWindows; ++i) {
    // Delete windows before deleting delegates.
    delete windows[i];
    delete delegates[i];
  }
}

// Check that touch events outside the root window are still handled
// by the root window's gesture sequence.
TEST_F(GestureRecognizerTest, GestureEventOutsideRootWindowTap) {
  TestGestureRecognizer* gesture_recognizer =
      new TestGestureRecognizer(root_window());
  TimedEvents tes;
  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  scoped_ptr<aura::Window> window(CreateTestWindowWithBounds(
      gfx::Rect(-100, -100, 2000, 2000), root_window()));

  ui::GestureSequence* window_gesture_sequence =
      gesture_recognizer->GetGestureSequenceForTesting(window.get());

  ui::GestureSequence* root_window_gesture_sequence =
      gesture_recognizer->GetGestureSequenceForTesting(root_window());

  gfx::Point pos1(-10, -10);
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, pos1, 0, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

  gfx::Point pos2(1000, 1000);
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, pos2, 1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

  // As these presses were outside the root window, they should be
  // associated with the root window.
  EXPECT_EQ(0, window_gesture_sequence->point_count());
  EXPECT_EQ(2, root_window_gesture_sequence->point_count());
}

TEST_F(GestureRecognizerTest, NoTapWithPreventDefaultedRelease) {
  scoped_ptr<QueueTouchEventDelegate> delegate(
      new QueueTouchEventDelegate(root_window()));
  TimedEvents tes;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, 100, 100);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  delegate->set_window(window.get());

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);

  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->tap_down());
  delegate->Reset();
  delegate->ReceivedAckPreventDefaulted();
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_cancel());
}

TEST_F(GestureRecognizerTest, PinchScrollWithPreventDefaultedRelease) {
  scoped_ptr<QueueTouchEventDelegate> delegate(
      new QueueTouchEventDelegate(root_window()));
  TimedEvents tes;
  const int kTouchId1 = 7;
  const int kTouchId2 = 5;
  gfx::Rect bounds(10, 20, 100, 100);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  delegate->set_window(window.get());

  {
    delegate->Reset();
    ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(15, 25), kTouchId1,
                         tes.Now());
    ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(20, 95), kTouchId1,
                        tes.LeapForward(200));
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(15, 25), kTouchId1,
                           tes.LeapForward(50));
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    delegate->Reset();

    // Ack the press event.
    delegate->ReceivedAck();
    EXPECT_TRUE(delegate->tap_down());
    delegate->Reset();

    // Ack the move event.
    delegate->ReceivedAck();
    EXPECT_TRUE(delegate->tap_cancel());
    EXPECT_TRUE(delegate->scroll_begin());
    delegate->Reset();

    // Ack the release event. Although the release event has been processed, it
    // should still generate a scroll-end event.
    delegate->ReceivedAckPreventDefaulted();
    EXPECT_TRUE(delegate->scroll_end());
  }

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(15, 25), kTouchId1,
                       tes.Now());
  ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::Point(20, 95), kTouchId1,
                      tes.LeapForward(200));
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(15, 25), kTouchId1,
                         tes.LeapForward(50));
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(55, 25), kTouchId2,
                        tes.Now());
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(45, 85), kTouchId2,
                       tes.LeapForward(1000));
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(45, 85), kTouchId2,
                          tes.LeapForward(14));

  // Do a pinch.
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);

  // Ack the press and move events.
  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->begin());
  EXPECT_TRUE(delegate->tap_down());

  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->scroll_begin());

  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->begin());
  EXPECT_TRUE(delegate->pinch_begin());

  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->pinch_update());

  // Ack the first release. Although the release is processed, it should still
  // generate a pinch-end event.
  delegate->Reset();
  delegate->ReceivedAckPreventDefaulted();
  EXPECT_TRUE(delegate->pinch_end());
  EXPECT_TRUE(delegate->end());

  delegate->Reset();
  delegate->ReceivedAckPreventDefaulted();
  EXPECT_TRUE(delegate->scroll_end());
  EXPECT_TRUE(delegate->end());
}

TEST_F(GestureRecognizerTest, CaptureSendsGestureEnd) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TestGestureRecognizer* gesture_recognizer =
      new TestGestureRecognizer(root_window());
  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, gfx::Rect(10, 10, 300, 300), root_window()));
  EventGenerator generator(root_window());

  generator.MoveMouseRelativeTo(window.get(), gfx::Point(10, 10));
  generator.PressTouch();
  RunAllPendingInMessageLoop();

  EXPECT_TRUE(delegate->tap_down());

  scoped_ptr<aura::Window> capture(CreateTestWindowWithBounds(
      gfx::Rect(10, 10, 200, 200), root_window()));
  capture->SetCapture();
  RunAllPendingInMessageLoop();

  EXPECT_TRUE(delegate->end());
  EXPECT_TRUE(delegate->tap_cancel());
}

TEST_F(GestureRecognizerTest, PressDoesNotCrash) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  TestGestureRecognizer* gesture_recognizer =
      new TestGestureRecognizer(root_window());
  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);
  TimedEvents tes;

  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, gfx::Rect(10, 10, 300, 300), root_window()));

  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(45, 45), 7, tes.Now());
  press.set_radius_x(40);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_EQ(gfx::Rect(5, 5, 80, 80).ToString(),
            delegate->bounding_box().ToString());
  delegate->Reset();

  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(55, 45), 7, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

  // This new press should not generate a tap-down.
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
}

TEST_F(GestureRecognizerTest, TwoFingerTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  delegate->Reset();
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->two_finger_tap());

  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());  // no touch down for second tap.
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->two_finger_tap());

  // Little bit of touch move should not affect our state.
  delegate->Reset();
  ui::TouchEvent move1(ui::ET_TOUCH_MOVED, gfx::Point(102, 202),
                       kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move1);
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(131, 202),
                       kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->two_finger_tap());

  // Make sure there is enough delay before the touch is released so that it is
  // recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId1, tes.LeapForward(50));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_TRUE(delegate->two_finger_tap());

  // Lift second finger.
  // Make sure there is enough delay before the touch is released so that it is
  // recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(130, 201),
                          kTouchId2, tes.LeapForward(50));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_TRUE(delegate->fling());
  EXPECT_FALSE(delegate->two_finger_tap());
}

TEST_F(GestureRecognizerTest, TwoFingerTapExpired) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  delegate->Reset();
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

  // Send release event after sufficient delay so that two finger time expires.
  delegate->Reset();
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId1, tes.LeapForward(1000));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  EXPECT_FALSE(delegate->two_finger_tap());

  // Lift second finger.
  // Make sure there is enough delay before the touch is released so that it is
  // recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(130, 201),
                          kTouchId2, tes.LeapForward(50));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);
  EXPECT_FALSE(delegate->two_finger_tap());
}

TEST_F(GestureRecognizerTest, TwoFingerTapChangesToPinch) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  TimedEvents tes;

  // Test moving first finger
  {
    gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
    scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
        delegate.get(), -1234, bounds, root_window()));

    delegate->Reset();
    ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                          kTouchId1, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

    delegate->Reset();
    ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                          kTouchId2, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

    tes.SendScrollEvent(root_window(), 130, 230, kTouchId1, delegate.get());
    EXPECT_FALSE(delegate->two_finger_tap());
    EXPECT_TRUE(delegate->pinch_begin());

    // Make sure there is enough delay before the touch is released so that it
    // is recognized as a tap.
    delegate->Reset();
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                           kTouchId2, tes.LeapForward(50));

    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    EXPECT_FALSE(delegate->two_finger_tap());
    EXPECT_TRUE(delegate->pinch_end());
  }

  // Test moving second finger
  {
    gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
    scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
        delegate.get(), -1234, bounds, root_window()));

    delegate->Reset();
    ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                          kTouchId1, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

    delegate->Reset();
    ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                          kTouchId2, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

    tes.SendScrollEvent(root_window(), 101, 230, kTouchId2, delegate.get());
    EXPECT_FALSE(delegate->two_finger_tap());
    EXPECT_TRUE(delegate->pinch_begin());

    // Make sure there is enough delay before the touch is released so that it
    // is recognized as a tap.
    delegate->Reset();
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                           kTouchId1, tes.LeapForward(50));

    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    EXPECT_FALSE(delegate->two_finger_tap());
    EXPECT_TRUE(delegate->pinch_end());
  }
}

TEST_F(GestureRecognizerTest, NoTwoFingerTapWhenFirstFingerHasScrolled) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  TimedEvents tes;

  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  delegate->Reset();
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId1, delegate.get());

  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

  EXPECT_TRUE(delegate->pinch_begin());

  // Make sure there is enough delay before the touch is released so that it
  // is recognized as a tap.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId2, tes.LeapForward(50));

  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_FALSE(delegate->two_finger_tap());
  EXPECT_TRUE(delegate->pinch_end());
}

TEST_F(GestureRecognizerTest, MultiFingerSwipe) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;

  gfx::Rect bounds(5, 10, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  const int kSteps = 15;
  const int kTouchPoints = 4;
  gfx::Point points[kTouchPoints] = {
    gfx::Point(10, 30),
    gfx::Point(30, 20),
    gfx::Point(50, 30),
    gfx::Point(80, 50)
  };

  aura::test::EventGenerator generator(root_window(), window.get());

  for (int count = 2; count <= kTouchPoints; ++count) {
    generator.GestureMultiFingerScroll(count, points, 15, kSteps, 0, -150);
    EXPECT_TRUE(delegate->swipe_up());
    delegate->Reset();

    generator.GestureMultiFingerScroll(count, points, 15, kSteps, 0, 150);
    EXPECT_TRUE(delegate->swipe_down());
    delegate->Reset();

    generator.GestureMultiFingerScroll(count, points, 15, kSteps, -150, 0);
    EXPECT_TRUE(delegate->swipe_left());
    delegate->Reset();

    generator.GestureMultiFingerScroll(count, points, 15, kSteps, 150, 0);
    EXPECT_TRUE(delegate->swipe_right());
    delegate->Reset();
  }
}

TEST_F(GestureRecognizerTest, TwoFingerTapCancelled) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  TimedEvents tes;

  // Test canceling first finger.
  {
    gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
    scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
        delegate.get(), -1234, bounds, root_window()));

    delegate->Reset();
    ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                          kTouchId1, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

    delegate->Reset();
    ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                          kTouchId2, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

    delegate->Reset();
    ui::TouchEvent cancel(ui::ET_TOUCH_CANCELLED, gfx::Point(130, 201),
                          kTouchId1, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&cancel);
    EXPECT_FALSE(delegate->two_finger_tap());

    // Make sure there is enough delay before the touch is released so that it
    // is recognized as a tap.
    delegate->Reset();
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                           kTouchId2, tes.LeapForward(50));

    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    EXPECT_FALSE(delegate->two_finger_tap());
  }

  // Test canceling second finger
  {
    gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
    scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
        delegate.get(), -1234, bounds, root_window()));

    delegate->Reset();
    ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                          kTouchId1, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);

    delegate->Reset();
    ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(130, 201),
                          kTouchId2, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);

    delegate->Reset();
    ui::TouchEvent cancel(ui::ET_TOUCH_CANCELLED, gfx::Point(130, 201),
                          kTouchId2, tes.Now());
    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&cancel);
    EXPECT_FALSE(delegate->two_finger_tap());

    // Make sure there is enough delay before the touch is released so that it
    // is recognized as a tap.
    delegate->Reset();
    ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                           kTouchId1, tes.LeapForward(50));

    root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
    EXPECT_FALSE(delegate->two_finger_tap());
  }
}

TEST_F(GestureRecognizerTest, VeryWideTwoFingerTouchDownShouldBeAPinch) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 523;
  const int kWindowHeight = 45;
  const int kTouchId1 = 2;
  const int kTouchId2 = 3;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  delegate->Reset();
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->two_finger_tap());

  delegate->Reset();
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(430, 201),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());  // no touch down for second tap.
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_FALSE(delegate->long_press());
  EXPECT_FALSE(delegate->two_finger_tap());
  EXPECT_TRUE(delegate->pinch_begin());
}

// Verifies if a window is the target of multiple touch-ids and we hide the
// window everything is cleaned up correctly.
TEST_F(GestureRecognizerTest, FlushAllOnHide) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  gfx::Rect bounds(0, 0, 200, 200);
  scoped_ptr<aura::Window> window(
      CreateTestWindowWithDelegate(delegate.get(), 0, bounds, root_window()));
  const int kTouchId1 = 8;
  const int kTouchId2 = 2;
  TimedEvents tes;

  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(10, 10),
                        kTouchId1, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(20, 20),
                        kTouchId2, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  window->Hide();
  EXPECT_EQ(NULL,
            root_window()->gesture_recognizer()->GetTouchLockedTarget(&press1));
  EXPECT_EQ(NULL,
            root_window()->gesture_recognizer()->GetTouchLockedTarget(&press2));
}

TEST_F(GestureRecognizerTest, LongPressTimerStopsOnPreventDefaultedTouchMoves) {
  scoped_ptr<QueueTouchEventDelegate> delegate(
      new QueueTouchEventDelegate(root_window()));
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, 100, 100);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  delegate->set_window(window.get());
  TimedEvents tes;

  TimerTestGestureRecognizer* gesture_recognizer =
      new TimerTestGestureRecognizer(root_window());
  TimerTestGestureSequence* gesture_sequence =
      static_cast<TimerTestGestureSequence*>(
          gesture_recognizer->GetGestureSequenceForTesting(window.get()));

  root_window()->SetGestureRecognizerForTesting(gesture_recognizer);

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  // Scroll around, to cancel the long press
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId, delegate.get());

  delegate->Reset();
  delegate->ReceivedAck();
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_TRUE(gesture_sequence->IsTimerRunning());

  delegate->Reset();
  delegate->ReceivedAckPreventDefaulted();
  EXPECT_FALSE(gesture_sequence->IsTimerRunning());
  gesture_sequence->ForceTimeout();
  EXPECT_FALSE(delegate->long_press());
}

// Same as GestureEventConsumeDelegate, but consumes all the touch-move events.
class ConsumesTouchMovesDelegate : public GestureEventConsumeDelegate {
 public:
  ConsumesTouchMovesDelegate() : consume_touch_move_(true) {}
  virtual ~ConsumesTouchMovesDelegate() {}

  void set_consume_touch_move(bool consume) { consume_touch_move_ = consume; }

 private:
  virtual void OnTouchEvent(ui::TouchEvent* touch) OVERRIDE {
    if (consume_touch_move_ && touch->type() == ui::ET_TOUCH_MOVED)
      touch->SetHandled();
    else
      GestureEventConsumeDelegate::OnTouchEvent(touch);
  }

  bool consume_touch_move_;

  DISALLOW_COPY_AND_ASSIGN(ConsumesTouchMovesDelegate);
};

// Same as GestureEventScroll, but tests that the behavior is the same
// even if all the touch-move events are consumed.
TEST_F(GestureRecognizerTest, GestureEventScrollTouchMoveConsumed) {
  scoped_ptr<ConsumesTouchMovesDelegate> delegate(
      new ConsumesTouchMovesDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 5;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Move the touch-point enough so that it would normally be considered a
  // scroll. But since the touch-moves will be consumed, the scroll should not
  // start.
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId, delegate.get());
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  // TODO(rbyers): Really we should get the TapCancel here instead of below,
  // but this is a symptom of a larger issue: crbug.com/146397.
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Release the touch back at the start point. This should end without causing
  // a tap.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(130, 230),
                         kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
}

// Like as GestureEventTouchMoveConsumed but tests the different behavior
// depending on whether the events were consumed before or after the scroll
// started.
TEST_F(GestureRecognizerTest, GestureEventScrollTouchMovePartialConsumed) {
  scoped_ptr<ConsumesTouchMovesDelegate> delegate(
      new ConsumesTouchMovesDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 5;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  delegate->Reset();
  ui::TouchEvent press(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                       kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press);
  EXPECT_FALSE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Move the touch-point enough so that it would normally be considered a
  // scroll. But since the touch-moves will be consumed, the scroll should not
  // start.
  tes.SendScrollEvent(root_window(), 130, 230, kTouchId, delegate.get());
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  // TODO(rbyers): Really we should get the TapCancel here instead of below,
  // but this is a symptom of a larger issue: crbug.com/146397.
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  // Now, stop consuming touch-move events, and move the touch-point again.
  delegate->set_consume_touch_move(false);
  tes.SendScrollEvent(root_window(), 159, 259, kTouchId, delegate.get());
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_TRUE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  // Consuming move events doesn't effect what the ultimate scroll position
  // will be if scrolling is later allowed to happen.
  EXPECT_EQ(58, delegate->scroll_x());
  EXPECT_EQ(58, delegate->scroll_y());
  EXPECT_EQ(gfx::Point(1, 1).ToString(),
            delegate->scroll_begin_position().ToString());

  // Start consuming touch-move events again. However, since gesture-scroll has
  // already started, the touch-move events should still result in scroll-update
  // gestures.
  delegate->set_consume_touch_move(true);

  // Move some more to generate a few more scroll updates.
  tes.SendScrollEvent(root_window(), 110, 211, kTouchId, delegate.get());
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(-49, delegate->scroll_x());
  EXPECT_EQ(-48, delegate->scroll_y());

  tes.SendScrollEvent(root_window(), 140, 215, kTouchId, delegate.get());
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  EXPECT_EQ(30, delegate->scroll_x());
  EXPECT_EQ(4, delegate->scroll_y());

  // Release the touch. This should end the scroll.
  delegate->Reset();
  ui::TouchEvent release(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                         kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());
  // Moves arrive without delays and hence have high velocity.
  EXPECT_TRUE(delegate->fling());
}

// Check that appropriate touch events generate double tap gesture events.
TEST_F(GestureRecognizerTest, GestureEventDoubleTap) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  // First tap (tested in GestureEventTap)
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(104, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(104, 201),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  delegate->Reset();

  // Second tap
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(101, 203),
                        kTouchId, tes.LeapForward(200));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(102, 206),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);

  EXPECT_TRUE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  EXPECT_EQ(2, delegate->tap_count());
}

// Check that we don't get a double tap when the two taps are far apart.
TEST_F(GestureRecognizerTest, TwoTapsFarApart) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  // First tap (tested in GestureEventTap)
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  delegate->Reset();

  // Second tap, close in time but far in distance
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(201, 201),
                        kTouchId, tes.LeapForward(200));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(201, 201),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);

  EXPECT_TRUE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  EXPECT_EQ(1, delegate->tap_count());
}

// Check that we don't get a double tap when the two taps have a long enough
// delay in between.
TEST_F(GestureRecognizerTest, TwoTapsWithDelayBetween) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 123;
  const int kWindowHeight = 45;
  const int kTouchId = 2;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  // First tap (tested in GestureEventTap)
  ui::TouchEvent press1(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  ui::TouchEvent release1(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release1);
  delegate->Reset();

  // Second tap, close in distance but after some delay
  ui::TouchEvent press2(ui::ET_TOUCH_PRESSED, gfx::Point(101, 201),
                        kTouchId, tes.LeapForward(2000));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  ui::TouchEvent release2(ui::ET_TOUCH_RELEASED, gfx::Point(101, 201),
                          kTouchId, tes.LeapForward(50));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&release2);

  EXPECT_TRUE(delegate->tap());
  EXPECT_TRUE(delegate->tap_down());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->begin());
  EXPECT_TRUE(delegate->end());
  EXPECT_FALSE(delegate->scroll_begin());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->scroll_end());

  EXPECT_EQ(1, delegate->tap_count());
}

// Checks that if the bounding-box of a gesture changes because of change in
// radius of a touch-point, and not because of change in position, then there
// are not gesture events from that.
TEST_F(GestureRecognizerTest, BoundingBoxRadiusChange) {
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 234;
  const int kWindowHeight = 345;
  const int kTouchId = 5, kTouchId2 = 7;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));
  TimedEvents tes;

  ui::TouchEvent press1(
      ui::ET_TOUCH_PRESSED, gfx::Point(101, 201), kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_TRUE(delegate->bounding_box().IsEmpty());

  delegate->Reset();

  ui::TouchEvent press2(
      ui::ET_TOUCH_PRESSED, gfx::Point(201, 201), kTouchId2,
      tes.LeapForward(400));
  press2.set_radius_x(5);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press2);
  EXPECT_FALSE(delegate->pinch_begin());
  EXPECT_EQ(gfx::Rect(101, 201, 100, 0).ToString(),
            delegate->bounding_box().ToString());

  delegate->Reset();

  ui::TouchEvent move1(ui::ET_TOUCH_MOVED, gfx::Point(141, 201), kTouchId,
                       tes.LeapForward(40));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move1);
  EXPECT_TRUE(delegate->pinch_begin());
  EXPECT_EQ(gfx::Rect(141, 201, 60, 0).ToString(),
            delegate->bounding_box().ToString());

  delegate->Reset();

  // The position doesn't move, but the radius changes.
  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(101, 201), kTouchId,
                       tes.LeapForward(40));
  move2.set_radius_x(50);
  move2.set_radius_y(60);
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  EXPECT_FALSE(delegate->tap());
  EXPECT_FALSE(delegate->tap_cancel());
  EXPECT_FALSE(delegate->scroll_update());
  EXPECT_FALSE(delegate->pinch_update());

  delegate->Reset();
}

// Checks that slow scrolls deliver the correct deltas.
// In particular, fix for http;//crbug.com/150573.
TEST_F(GestureRecognizerTest, NoDriftInScroll) {
  ui::GestureConfiguration::set_max_touch_move_in_pixels_for_click(3);
  ui::GestureConfiguration::set_min_scroll_delta_squared(9);
  scoped_ptr<GestureEventConsumeDelegate> delegate(
      new GestureEventConsumeDelegate());
  const int kWindowWidth = 234;
  const int kWindowHeight = 345;
  const int kTouchId = 5;
  TimedEvents tes;
  gfx::Rect bounds(100, 200, kWindowWidth, kWindowHeight);
  scoped_ptr<aura::Window> window(CreateTestWindowWithDelegate(
      delegate.get(), -1234, bounds, root_window()));

  ui::TouchEvent press1(
      ui::ET_TOUCH_PRESSED, gfx::Point(101, 208), kTouchId, tes.Now());
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&press1);
  EXPECT_TRUE(delegate->begin());

  delegate->Reset();

  ui::TouchEvent move1(ui::ET_TOUCH_MOVED, gfx::Point(101, 206), kTouchId,
                       tes.LeapForward(40));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move1);
  EXPECT_FALSE(delegate->scroll_begin());

  delegate->Reset();

  ui::TouchEvent move2(ui::ET_TOUCH_MOVED, gfx::Point(101, 204), kTouchId,
                       tes.LeapForward(40));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move2);
  EXPECT_TRUE(delegate->tap_cancel());
  EXPECT_TRUE(delegate->scroll_begin());
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(-4, delegate->scroll_y());

  delegate->Reset();

  ui::TouchEvent move3(ui::ET_TOUCH_MOVED, gfx::Point(101, 204), kTouchId,
                       tes.LeapForward(40));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move3);
  EXPECT_FALSE(delegate->scroll_update());

  delegate->Reset();

  ui::TouchEvent move4(ui::ET_TOUCH_MOVED, gfx::Point(101, 203), kTouchId,
                       tes.LeapForward(40));
  root_window()->AsRootWindowHostDelegate()->OnHostTouchEvent(&move4);
  EXPECT_TRUE(delegate->scroll_update());
  EXPECT_EQ(-1, delegate->scroll_y());

  delegate->Reset();
}

}  // namespace test
}  // namespace aura
