// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_EVENT_GENERATOR_H_
#define UI_AURA_TEST_EVENT_GENERATOR_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/point.h"

namespace base {
class TimeDelta;
}

namespace ui {
class Event;
class KeyEvent;
class MouseEvent;
class ScrollEvent;
class TouchEvent;
}

namespace aura {
class RootWindow;
class Window;

namespace client {
class ScreenPositionClient;
}

namespace test {

typedef base::Callback<void(ui::EventType, const gfx::Vector2dF&)>
        ScrollStepCallback;

// A delegate interface for EventGenerator that provides a way to
// locate aura root window for given point.
class EventGeneratorDelegate {
 public:
  virtual ~EventGeneratorDelegate() {}

  // Returns a root window for given point.
  virtual RootWindow* GetRootWindowAt(const gfx::Point& point) const = 0;

  // Returns the screen position client that determines the
  // coordinates used in EventGenerator. EventGenerator uses
  // RootWindow's coordinate if this retruns NULL.
  virtual client::ScreenPositionClient* GetScreenPositionClient(
      const aura::Window* window) const = 0;
};

// EventGenerator is a tool that generates and dispatch events. The
// coordinates of the points in API is determined by the
// EventGeneratorDelegate.
class EventGenerator {
 public:
  // Creates an EventGenerator with the mouse/touch location (0,0),
  // which uses the |root_window|'s coordinates.
  explicit EventGenerator(RootWindow* root_window);

  // Create an EventGenerator with EventGeneratorDelegate,
  // which uses the coordinates used by |delegate|.
  explicit EventGenerator(EventGeneratorDelegate* delegate);

  // Creates an EventGenerator with the mouse/touch location
  // at |initial_location|, which uses the |root_window|'s coordinates.
  EventGenerator(RootWindow* root_window, const gfx::Point& initial_location);

  // Creates an EventGenerator with the mouse/touch location
  // centered over |window|, which uses the |root_window|'s coordinates.
  EventGenerator(RootWindow* root_window, Window* window);

  virtual ~EventGenerator();

  // Explicitly sets the location used by mouse/touch events. This is set by the
  // various methods that take a location but can be manipulated directly,
  // typically for touch.
  void set_current_location(const gfx::Point& location) {
    current_location_ = location;
  }
  const gfx::Point& current_location() const { return current_location_; }

  void set_async(bool async) { async_ = async; }
  bool async() const { return async_; }

  // Resets the event flags bitmask.
  void set_flags(int flags) { flags_ = flags; }

  // Generates a left button press event.
  void PressLeftButton();

  // Generates a left button release event.
  void ReleaseLeftButton();

  // Generates events to click (press, release) left button.
  void ClickLeftButton();

  // Generates a double click event using the left button.
  void DoubleClickLeftButton();

  // Generates a right button press event.
  void PressRightButton();

  // Generates a right button release event.
  void ReleaseRightButton();

  // Generates events to move mouse to be the given |point| in the
  // |current_root_window_|'s host window coordinates.
  void MoveMouseToInHost(const gfx::Point& point_in_host);
  void MoveMouseToInHost(int x, int y) {
    MoveMouseToInHost(gfx::Point(x, y));
  }

  // Generates events to move mouse to be the given |point| in screen
  // coordinates.
  void MoveMouseTo(const gfx::Point& point_in_screen, int count);
  void MoveMouseTo(const gfx::Point& point_in_screen) {
    MoveMouseTo(point_in_screen, 1);
  }
  void MoveMouseTo(int x, int y) {
    MoveMouseTo(gfx::Point(x, y));
  }

  // Generates events to move mouse to be the given |point| in |window|'s
  // coordinates.
  void MoveMouseRelativeTo(const Window* window, const gfx::Point& point);
  void MoveMouseRelativeTo(const Window* window, int x, int y) {
    MoveMouseRelativeTo(window, gfx::Point(x, y));
  }

  void MoveMouseBy(int x, int y) {
    MoveMouseTo(current_location_ + gfx::Vector2d(x, y));
  }

  // Generates events to drag mouse to given |point|.
  void DragMouseTo(const gfx::Point& point);

  void DragMouseTo(int x, int y) {
    DragMouseTo(gfx::Point(x, y));
  }

  void DragMouseBy(int dx, int dy) {
    DragMouseTo(current_location_ + gfx::Vector2d(dx, dy));
  }

  // Generates events to move the mouse to the center of the window.
  void MoveMouseToCenterOf(Window* window);

  // Generates a touch press event.
  void PressTouch();

  // Generates a ET_TOUCH_MOVED event to |point|.
  void MoveTouch(const gfx::Point& point);

  // Generates a touch release event.
  void ReleaseTouch();

  // Generates press, move and release event to move touch
  // to be the given |point|.
  void PressMoveAndReleaseTouchTo(const gfx::Point& point);

  void PressMoveAndReleaseTouchTo(int x, int y) {
    PressMoveAndReleaseTouchTo(gfx::Point(x, y));
  }

  void PressMoveAndReleaseTouchBy(int x, int y) {
    PressMoveAndReleaseTouchTo(current_location_ + gfx::Vector2d(x, y));
  }

  // Generates press, move and release events to move touch
  // to the center of the window.
  void PressMoveAndReleaseTouchToCenterOf(Window* window);

  // Generates and dispatches touch-events required to generate a TAP gesture.
  // Note that this can generate a number of other gesture events at the same
  // time (e.g. GESTURE_BEGIN, TAP_DOWN, END).
  void GestureTapAt(const gfx::Point& point);

  // Generates press and release touch-events to generate a TAP_DOWN event, but
  // without generating any scroll or tap events. This can also generate a few
  // other gesture events (e.g. GESTURE_BEGIN, END).
  void GestureTapDownAndUp(const gfx::Point& point);

  // Generates press, move, release touch-events to generate a sequence of
  // scroll events. |duration| and |steps| affect the velocity of the scroll,
  // and depending on these values, this may also generate FLING scroll
  // gestures. If velocity/fling is irrelevant for the test, then any non-zero
  // values for these should be sufficient.
  void GestureScrollSequence(const gfx::Point& start,
                             const gfx::Point& end,
                             const base::TimeDelta& duration,
                             int steps);

  // The same as GestureScrollSequence(), with the exception that |callback| is
  // called at each step of the scroll sequence. |callback| is called at the
  // start of the sequence with ET_GESTURE_SCROLL_BEGIN, followed by one or more
  // ET_GESTURE_SCROLL_UPDATE and ends with an ET_GESTURE_SCROLL_END.
  void GestureScrollSequenceWithCallback(const gfx::Point& start,
                                         const gfx::Point& end,
                                         const base::TimeDelta& duration,
                                         int steps,
                                         const ScrollStepCallback& callback);

  // Generates press, move, release touch-events to generate a sequence of
  // multi-finger scroll events. |count| specifies the number of touch-points
  // that should generate the scroll events. |start| are the starting positions
  // of all the touch points. |steps| and |event_separation_time_ms| are
  // relevant when testing velocity/fling/swipe, otherwise these can be any
  // non-zero value. |delta_x| and |delta_y| are the amount that each finger
  // should be moved.
  void GestureMultiFingerScroll(int count,
                                const gfx::Point* start,
                                int event_separation_time_ms,
                                int steps,
                                int move_x,
                                int move_y);

  // Generates scroll sequences of a FlingCancel, Scrolls, FlingStart, with
  // constant deltas to |x_offset| and |y_offset| in |steps|.
  void ScrollSequence(const gfx::Point& start,
                      const base::TimeDelta& step_delay,
                      float x_offset,
                      float y_offset,
                      int steps,
                      int num_fingers);

  // Generates scroll sequences of a FlingCancel, Scrolls, FlingStart, sending
  // scrolls of each of the values in |offsets|.
  void ScrollSequence(const gfx::Point& start,
                      const base::TimeDelta& step_delay,
                      const std::vector<gfx::Point>& offsets,
                      int num_fingers);

  // Generates a key press event. On platforms except Windows and X11, a key
  // event without native_event() is generated. Note that ui::EF_ flags should
  // be passed as |flags|, not the native ones like 'ShiftMask' in <X11/X.h>.
  // TODO(yusukes): Support native_event() on all platforms.
  void PressKey(ui::KeyboardCode key_code, int flags);

  // Generates a key release event. On platforms except Windows and X11, a key
  // event without native_event() is generated. Note that ui::EF_ flags should
  // be passed as |flags|, not the native ones like 'ShiftMask' in <X11/X.h>.
  // TODO(yusukes): Support native_event() on all platforms.
  void ReleaseKey(ui::KeyboardCode key_code, int flags);

  // Dispatch the event to the RootWindow.
  void Dispatch(ui::Event* event);

  void set_current_root_window(RootWindow* root_window) {
    current_root_window_ = root_window;
  }

 private:
  // Dispatch a key event to the RootWindow.
  void DispatchKeyEvent(bool is_press, ui::KeyboardCode key_code, int flags);

  void UpdateCurrentRootWindow(const gfx::Point& point);
  void PressButton(int flag);
  void ReleaseButton(int flag);

  // Convert a point between API's coordinates and
  // |target|'s coordinates.
  void ConvertPointFromTarget(const aura::Window* target,
                              gfx::Point* point) const;
  void ConvertPointToTarget(const aura::Window* target,
                            gfx::Point* point) const;

  gfx::Point GetLocationInCurrentRoot() const;
  gfx::Point CenterOfWindow(const Window* window) const;

  void DispatchNextPendingEvent();
  void DoDispatchEvent(ui::Event* event, bool async);

  scoped_ptr<EventGeneratorDelegate> delegate_;
  gfx::Point current_location_;
  RootWindow* current_root_window_;
  int flags_;
  bool grab_;
  std::list<ui::Event*> pending_events_;
  // Set to true to cause events to be posted asynchronously.
  bool async_;

  DISALLOW_COPY_AND_ASSIGN(EventGenerator);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_EVENT_GENERATOR_H_
