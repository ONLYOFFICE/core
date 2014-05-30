// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_COMPOUND_EVENT_FILTER_H_
#define UI_VIEWS_COREWM_COMPOUND_EVENT_FILTER_H_

#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_handler.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace aura {
class CursorManager;
class RootWindow;
}

namespace ui {
class GestureEvent;
class KeyEvent;
class LocatedEvent;
class MouseEvent;
class TouchEvent;
}

namespace views {
namespace corewm {

// CompoundEventFilter gets all events first and can provide actions to those
// events. It implements global features such as click to activate a window and
// cursor change when moving mouse.
// Additional event filters can be added to CompoundEventFilter. Events will
// pass through those additional filters in their addition order and could be
// consumed by any of those filters. If an event is consumed by a filter, the
// rest of the filter(s) and CompoundEventFilter will not see the consumed
// event.
class VIEWS_EXPORT CompoundEventFilter : public ui::EventHandler {
 public:
  CompoundEventFilter();
  virtual ~CompoundEventFilter();

  // Returns the cursor for the specified component.
  static gfx::NativeCursor CursorForWindowComponent(int window_component);

  // Used to allow a mouse event to show the cursor even when
  // the cursor is hidden by |CursorClient::ShowCursor(false)|.
  void set_cursor_hidden_by_filter(bool cursor_hidden_by_filter) {
    cursor_hidden_by_filter_ = cursor_hidden_by_filter;
  }

  // Adds/removes additional event filters. This does not take ownership of
  // the EventHandler.
  // NOTE: These handlers are deprecated. Use env::AddPreTargetEventHandler etc.
  // instead.
  void AddHandler(ui::EventHandler* filter);
  void RemoveHandler(ui::EventHandler* filter);

 private:
  // Updates the cursor if the target provides a custom one, and provides
  // default resize cursors for window edges.
  void UpdateCursor(aura::Window* target, ui::MouseEvent* event);

  // Dispatches event to additional filters.
  void FilterKeyEvent(ui::KeyEvent* event);
  void FilterMouseEvent(ui::MouseEvent* event);
  void FilterTouchEvent(ui::TouchEvent* event);

  // Sets the visibility of the cursor if the event is not synthesized and
  // 1) it's hiding (show=false) when the cursor is currently shown, or
  // 2) it's showing (show=true) if the cursor is previously hidden
  //    by this event filter (see |cursor_hidden_by_filter_|),
  // so that it doesn't change the cursor visibility if the cursor was
  // intentionally hidden by other components.
  void SetCursorVisibilityOnEvent(aura::Window* target,
                                  ui::Event* event,
                                  bool show);

  // Enables or disables mouse events if the event is not synthesized.
  void SetMouseEventsEnableStateOnEvent(aura::Window* target,
                                        ui::Event* event,
                                        bool enable);

  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  // Additional pre-target event handlers.
  ObserverList<ui::EventHandler, true> handlers_;

  // True if the cursur was hidden by the filter.
  bool cursor_hidden_by_filter_;

  DISALLOW_COPY_AND_ASSIGN(CompoundEventFilter);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_COMPOUND_EVENT_FILTER_H_
