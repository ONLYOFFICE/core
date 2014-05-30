// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/mouse_watcher.h"

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/event_types.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_utils.h"
#include "ui/gfx/screen.h"

namespace views {

// Amount of time between when the mouse moves outside the Host's zone and when
// the listener is notified.
const int kNotifyListenerTimeMs = 300;

class MouseWatcher::Observer : public base::MessageLoopForUI::Observer {
 public:
  explicit Observer(MouseWatcher* mouse_watcher)
      : mouse_watcher_(mouse_watcher),
        notify_listener_factory_(this) {
    base::MessageLoopForUI::current()->AddObserver(this);
  }

  virtual ~Observer() {
    base::MessageLoopForUI::current()->RemoveObserver(this);
  }

  // MessageLoop::Observer implementation:
#if defined(OS_WIN)
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE {
    return base::EVENT_CONTINUE;
  }

  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE {
    // We spy on three different Windows messages here to see if the mouse has
    // moved out of the bounds of the current view. The messages are:
    //
    // WM_MOUSEMOVE:
    //   For when the mouse moves from the view into the rest of the browser UI,
    //   i.e. within the bounds of the same windows HWND.
    // WM_MOUSELEAVE:
    //   For when the mouse moves out of the bounds of the view's HWND.
    // WM_NCMOUSELEAVE:
    //   For notification when the mouse leaves the _non-client_ area.
    //
    switch (event.message) {
      case WM_MOUSEMOVE:
        HandleGlobalMouseMoveEvent(MouseWatcherHost::MOUSE_MOVE);
        break;
      case WM_MOUSELEAVE:
      case WM_NCMOUSELEAVE:
        HandleGlobalMouseMoveEvent(MouseWatcherHost::MOUSE_EXIT);
        break;
    }
  }
#elif defined(USE_AURA)
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE {
    return base::EVENT_CONTINUE;
  }
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE {
    switch (ui::EventTypeFromNative(event)) {
      case ui::ET_MOUSE_MOVED:
      case ui::ET_MOUSE_DRAGGED:
        // DRAGGED is a special case of MOVED. See events_win.cc/events_x.cc.
        HandleGlobalMouseMoveEvent(MouseWatcherHost::MOUSE_MOVE);
        break;
      case ui::ET_MOUSE_EXITED:
        HandleGlobalMouseMoveEvent(MouseWatcherHost::MOUSE_EXIT);
        break;
      default:
        break;
    }
  }
#endif

 private:
  MouseWatcherHost* host() const { return mouse_watcher_->host_.get(); }

  // Called from the message loop observer when a mouse movement has occurred.
  void HandleGlobalMouseMoveEvent(MouseWatcherHost::MouseEventType event_type) {
    bool contained = host()->Contains(
        // TODO(scottmg): Native is wrong http://crbug.com/133312
        gfx::Screen::GetNativeScreen()->GetCursorScreenPoint(),
        event_type);
    if (!contained) {
      // Mouse moved outside the host's zone, start a timer to notify the
      // listener.
      if (!notify_listener_factory_.HasWeakPtrs()) {
        base::MessageLoop::current()->PostDelayedTask(
            FROM_HERE,
            base::Bind(&Observer::NotifyListener,
                       notify_listener_factory_.GetWeakPtr()),
            event_type == MouseWatcherHost::MOUSE_MOVE
                ? base::TimeDelta::FromMilliseconds(kNotifyListenerTimeMs)
                : mouse_watcher_->notify_on_exit_time_);
      }
    } else {
      // Mouse moved quickly out of the host and then into it again, so cancel
      // the timer.
      notify_listener_factory_.InvalidateWeakPtrs();
    }
  }

  void NotifyListener() {
    mouse_watcher_->NotifyListener();
    // WARNING: we've been deleted.
  }

 private:
  MouseWatcher* mouse_watcher_;

  // A factory that is used to construct a delayed callback to the listener.
  base::WeakPtrFactory<Observer> notify_listener_factory_;

  DISALLOW_COPY_AND_ASSIGN(Observer);
};

MouseWatcherListener::~MouseWatcherListener() {
}

MouseWatcherHost::~MouseWatcherHost() {
}

MouseWatcher::MouseWatcher(MouseWatcherHost* host,
                           MouseWatcherListener* listener)
    : host_(host),
      listener_(listener),
      notify_on_exit_time_(base::TimeDelta::FromMilliseconds(
          kNotifyListenerTimeMs)) {
}

MouseWatcher::~MouseWatcher() {
}

void MouseWatcher::Start() {
  if (!is_observing())
    observer_.reset(new Observer(this));
}

void MouseWatcher::Stop() {
  observer_.reset(NULL);
}

void MouseWatcher::NotifyListener() {
  observer_.reset(NULL);
  listener_->MouseMovedOutOfHost();
}

}  // namespace views
