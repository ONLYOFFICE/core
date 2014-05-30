// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_EVENTS_EVENT_DISPATCHER_H_
#define UI_BASE_EVENTS_EVENT_DISPATCHER_H_

#include "base/auto_reset.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_target.h"
#include "ui/base/ui_export.h"

namespace ui {

class EventDispatcher;

class UI_EXPORT EventDispatcherDelegate {
 public:
  EventDispatcherDelegate();
  virtual ~EventDispatcherDelegate();

  // Returns whether an event can still be dispatched to a target. (e.g. during
  // event dispatch, one of the handlers may have destroyed the target, in which
  // case the event can no longer be dispatched to the target).
  virtual bool CanDispatchToTarget(EventTarget* target) = 0;

  // Returns the event being dispatched (or NULL if no event is being
  // dispatched).
  Event* current_event();

 protected:
  // Dispatches the event to the target. Returns true if the delegate is still
  // alive after dispatching event, and false if the delegate was destroyed
  // during the event dispatch.
  bool DispatchEvent(EventTarget* target, Event* event);

 private:
  EventDispatcher* dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(EventDispatcherDelegate);
};

// Dispatches events to appropriate targets.
class UI_EXPORT EventDispatcher {
 public:
  explicit EventDispatcher(EventDispatcherDelegate* delegate);
  virtual ~EventDispatcher();

  void ProcessEvent(EventTarget* target, Event* event);

  const Event* current_event() const { return current_event_; }
  Event* current_event() { return current_event_; }

  bool delegate_destroyed() const { return !delegate_; }

  void OnHandlerDestroyed(EventHandler* handler);
  void OnDispatcherDelegateDestroyed();

 private:
  void DispatchEventToEventHandlers(EventHandlerList& list,
                                    Event* event);

  // Dispatches an event, and makes sure it sets ER_CONSUMED on the
  // event-handling result if the dispatcher itself has been destroyed during
  // dispatching the event to the event handler.
  void DispatchEvent(EventHandler* handler, Event* event);

  EventDispatcherDelegate* delegate_;

  Event* current_event_;

  EventHandlerList handler_list_;

  DISALLOW_COPY_AND_ASSIGN(EventDispatcher);
};

}  // namespace ui

#endif  // UI_BASE_EVENTS_EVENT_DISPATCHER_H_
