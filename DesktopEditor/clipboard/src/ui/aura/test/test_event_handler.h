// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_EVENT_HANDLER_H_
#define UI_AURA_TEST_TEST_EVENT_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/events/event_handler.h"

namespace aura {
namespace test {

// A simple EventHandler that keeps track of the number of key events that it's
// seen.
class TestEventHandler : public ui::EventHandler {
 public:
  TestEventHandler();
  virtual ~TestEventHandler();

  int num_key_events() const { return num_key_events_; }
  int num_mouse_events() const { return num_mouse_events_; }
  int num_scroll_events() const { return num_scroll_events_; }
  int num_touch_events() const { return num_touch_events_; }
  int num_gesture_events() const { return num_gesture_events_; }

  void Reset();

  // ui::EventHandler overrides:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  // How many events have been received of each type?
  int num_key_events_;
  int num_mouse_events_;
  int num_scroll_events_;
  int num_touch_events_;
  int num_gesture_events_;

  DISALLOW_COPY_AND_ASSIGN(TestEventHandler);
};

}  // namespace test
}  // namespace aura

#endif // UI_AURA_TEST_TEST_EVENT_HANDLER_H_
