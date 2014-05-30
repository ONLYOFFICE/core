// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/test_event_handler.h"

#include "ui/base/events/event.h"

namespace aura {
namespace test {

TestEventHandler::TestEventHandler()
    : num_key_events_(0),
      num_mouse_events_(0),
      num_scroll_events_(0),
      num_touch_events_(0),
      num_gesture_events_(0) {
}

TestEventHandler::~TestEventHandler() {}

void TestEventHandler::Reset() {
  num_key_events_ = 0;
  num_mouse_events_ = 0;
  num_scroll_events_ = 0;
  num_touch_events_ = 0;
  num_gesture_events_ = 0;
}

void TestEventHandler::OnKeyEvent(ui::KeyEvent* event) {
  num_key_events_++;
  event->SetHandled();
}

void TestEventHandler::OnMouseEvent(ui::MouseEvent* event) {
  num_mouse_events_++;
}

void TestEventHandler::OnScrollEvent(ui::ScrollEvent* event) {
  num_scroll_events_++;
}

void TestEventHandler::OnTouchEvent(ui::TouchEvent* event) {
  num_touch_events_++;
}

void TestEventHandler::OnGestureEvent(ui::GestureEvent* event) {
  num_gesture_events_++;
}

}  // namespace test
}  // namespace aura
