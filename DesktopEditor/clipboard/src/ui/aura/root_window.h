// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_H_
#define UI_AURA_ROOT_WINDOW_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/capture_delegate.h"
#include "ui/aura/root_window_host_delegate.h"
#include "ui/aura/window.h"
#include "ui/base/cursor/cursor.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_dispatcher.h"
#include "ui/base/gestures/gesture_recognizer.h"
#include "ui/base/gestures/gesture_types.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/transform.h"

class SkCanvas;

namespace gfx {
class Size;
class Transform;
}

namespace ui {
class GestureEvent;
class GestureRecognizer;
class KeyEvent;
class LayerAnimationSequence;
class MouseEvent;
class ScrollEvent;
class TouchEvent;
class ViewProp;
}

namespace aura {
class TestScreen;
class RootWindow;
class RootWindowHost;
class RootWindowObserver;
class RootWindowTransformer;

// RootWindow is responsible for hosting a set of windows.
class AURA_EXPORT RootWindow : public ui::CompositorDelegate,
                               public ui::CompositorObserver,
                               public Window,
                               public ui::EventDispatcherDelegate,
                               public ui::GestureEventHelper,
                               public ui::LayerAnimationObserver,
                               public aura::client::CaptureDelegate,
                               public aura::RootWindowHostDelegate {
 public:
  struct AURA_EXPORT CreateParams {
    // CreateParams with initial_bounds and default host in pixel.
    explicit CreateParams(const gfx::Rect& initial_bounds);
    ~CreateParams() {}

    gfx::Rect initial_bounds;

    // A host to use in place of the default one that RootWindow will create.
    // NULL by default.
    RootWindowHost* host;
  };

  explicit RootWindow(const CreateParams& params);
  virtual ~RootWindow();

  // Returns the RootWindowHost for the specified accelerated widget, or NULL
  // if there is none associated.
  static RootWindow* GetForAcceleratedWidget(gfx::AcceleratedWidget widget);

  ui::Compositor* compositor() { return compositor_.get(); }
  gfx::NativeCursor last_cursor() const { return last_cursor_; }
  Window* mouse_pressed_handler() { return mouse_pressed_handler_; }

  // Initializes the root window.
  void Init();

  // Shows the root window host.
  void ShowRootWindow();

  // Hides the root window host.
  void HideRootWindow();

  // Stop listening events in preparation for shutdown.
  void PrepareForShutdown();

  // Repost event for re-processing. Used when exiting context menus.
  void RepostEvent(const ui::LocatedEvent& event);

  RootWindowHostDelegate* AsRootWindowHostDelegate();

  // Gets/sets the size of the host window.
  void SetHostSize(const gfx::Size& size_in_pixel);
  gfx::Size GetHostSize() const;

  // Sets the bounds of the host window.
  void SetHostBounds(const gfx::Rect& size_in_pizel);

  // Returns where the RootWindow is on screen.
  gfx::Point GetHostOrigin() const;

  // Sets the currently-displayed cursor. If the cursor was previously hidden
  // via ShowCursor(false), it will remain hidden until ShowCursor(true) is
  // called, at which point the cursor that was last set via SetCursor() will be
  // used.
  void SetCursor(gfx::NativeCursor cursor);

  // Invoked when the cursor's visibility has changed.
  void OnCursorVisibilityChanged(bool visible);

  // Invoked when the mouse events get enabled or disabled.
  void OnMouseEventsEnableStateChanged(bool enabled);

  // Moves the cursor to the specified location relative to the root window.
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;

  // Moves the cursor to the |host_location| given in host coordinates.
  void MoveCursorToHostLocation(const gfx::Point& host_location);

  // Clips the cursor movement to the root_window.
  bool ConfineCursorToWindow();

  // Draws the necessary set of windows.
  void Draw();

  // Draw the whole screen.
  void ScheduleFullRedraw();

  // Draw the damage_rect.
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);

  // Returns a target window for the given gesture event.
  Window* GetGestureTarget(ui::GestureEvent* event);

  // Handles a gesture event. Returns true if handled. Unlike the other
  // event-dispatching function (e.g. for touch/mouse/keyboard events), gesture
  // events are dispatched from GestureRecognizer instead of RootWindowHost.
  bool DispatchGestureEvent(ui::GestureEvent* event);

  // Invoked when |window| is being destroyed.
  void OnWindowDestroying(Window* window);

  // Invoked when |window|'s bounds have changed. |contained_mouse| indicates if
  // the bounds before change contained the |last_moust_location()|.
  void OnWindowBoundsChanged(Window* window, bool contained_mouse);

  // Dispatches OnMouseExited to the |window| which is hiding if nessessary.
  void DispatchMouseExitToHidingWindow(Window* window);

  // Invoked when |window|'s visibility has changed.
  void OnWindowVisibilityChanged(Window* window, bool is_visible);

  // Invoked when |window|'s tranfrom has changed. |contained_mouse|
  // indicates if the bounds before change contained the
  // |last_moust_location()|.
  void OnWindowTransformed(Window* window, bool contained_mouse);

  // Invoked when the keyboard mapping (in X11 terms: the mapping between
  // keycodes and keysyms) has changed.
  void OnKeyboardMappingChanged();

  // The system windowing system has sent a request that we close our window.
  void OnRootWindowHostCloseRequested();

  // Add/remove observer. There is no need to remove the observer if
  // the root window is being deleted. In particular, you SHOULD NOT remove
  // in |WindowObserver::OnWindowDestroying| of the observer observing
  // the root window because it is too late to remove it.
  void AddRootWindowObserver(RootWindowObserver* observer);
  void RemoveRootWindowObserver(RootWindowObserver* observer);

  // Posts |native_event| to the platform's event queue.
  void PostNativeEvent(const base::NativeEvent& native_event);

  // Converts |point| from the root window's coordinate system to native
  // screen's.
  void ConvertPointToNativeScreen(gfx::Point* point) const;

  // Converts |point| from native screen coordinate system to the root window's.
  void ConvertPointFromNativeScreen(gfx::Point* point) const;

  // Converts |point| from the root window's coordinate system to the
  // host window's.
  void ConvertPointToHost(gfx::Point* point) const;

  // Converts |point| from the host window's coordinate system to the
  // root window's.
  void ConvertPointFromHost(gfx::Point* point) const;

  // Gesture Recognition -------------------------------------------------------

  // When a touch event is dispatched to a Window, it may want to process the
  // touch event asynchronously. In such cases, the window should consume the
  // event during the event dispatch. Once the event is properly processed, the
  // window should let the RootWindow know about the result of the event
  // processing, so that gesture events can be properly created and dispatched.
  void ProcessedTouchEvent(ui::TouchEvent* event,
                           Window* window,
                           ui::EventResult result);

  ui::GestureRecognizer* gesture_recognizer() const {
    return gesture_recognizer_.get();
  }

  // Provided only for testing:
  void SetGestureRecognizerForTesting(ui::GestureRecognizer* gr);

  // Returns the accelerated widget from the RootWindowHost.
  gfx::AcceleratedWidget GetAcceleratedWidget();

  // Toggles the host's full screen state.
  void ToggleFullScreen();

  // These methods are used to defer the processing of mouse events related
  // to resize. A client (typically a RenderWidgetHostViewAura) can call
  // HoldMouseMoves when an resize is initiated and then ReleaseMouseMoves
  // once the resize is completed.
  //
  // More than one hold can be invoked and each hold must be cancelled by a
  // release before we resume normal operation.
  void HoldMouseMoves();
  void ReleaseMouseMoves();

  // Sets if the window should be focused when shown.
  void SetFocusWhenShown(bool focus_when_shown);

  // Copies |source_bounds| from the root window (as displayed on the host
  // machine) to |canvas| at offset |dest_offset|.
  bool CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                          const gfx::Point& dest_offset,
                          SkCanvas* canvas);

  // Gets the last location seen in a mouse event in this root window's
  // coordinates. This may return a point outside the root window's bounds.
  gfx::Point GetLastMouseLocationInRoot() const;

  // Overridden from Window:
  virtual RootWindow* GetRootWindow() OVERRIDE;
  virtual const RootWindow* GetRootWindow() const OVERRIDE;
  virtual void SetTransform(const gfx::Transform& transform) OVERRIDE;

  // Overridden from ui::EventTarget:
  virtual ui::EventTarget* GetParentTarget() OVERRIDE;

  // Overridden from ui::CompositorDelegate:
  virtual void ScheduleDraw() OVERRIDE;

  // Overridden from ui::CompositorObserver:
  virtual void OnCompositingDidCommit(ui::Compositor*) OVERRIDE;
  virtual void OnCompositingStarted(ui::Compositor*,
                                    base::TimeTicks start_time) OVERRIDE;
  virtual void OnCompositingEnded(ui::Compositor*) OVERRIDE;
  virtual void OnCompositingAborted(ui::Compositor*) OVERRIDE;
  virtual void OnCompositingLockStateChanged(ui::Compositor*) OVERRIDE;
  virtual void OnUpdateVSyncParameters(ui::Compositor* compositor,
                                       base::TimeTicks timebase,
                                       base::TimeDelta interval) OVERRIDE;

  // Overridden from ui::LayerDelegate:
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;

  // Overridden from Window:
  virtual bool CanFocus() const OVERRIDE;
  virtual bool CanReceiveEvents() const OVERRIDE;

  // Overridden from aura::client::CaptureDelegate:
  virtual void UpdateCapture(Window* old_capture, Window* new_capture) OVERRIDE;
  virtual void SetNativeCapture() OVERRIDE;
  virtual void ReleaseNativeCapture() OVERRIDE;

  // Exposes RootWindowHost::QueryMouseLocation() for test purposes.
  bool QueryMouseLocationForTest(gfx::Point* point) const;

  // Clears internal mouse state (such as mouse ups should be sent to the same
  // window that ate mouse downs).
  void ClearMouseHandlers();

  void SetRootWindowTransformer(scoped_ptr<RootWindowTransformer> transformer);
  gfx::Transform GetRootTransform() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(RootWindowTest, KeepTranslatedEventInRoot);

  friend class Window;
  friend class TestScreen;

  // The parameter for OnWindowHidden() to specify why window is hidden.
  enum WindowHiddenReason {
    WINDOW_DESTROYED,  // Window is destroyed.
    WINDOW_HIDDEN,     // Window is hidden.
    WINDOW_MOVING,     // Window is temporarily marked as hidden due to move
                       // across root windows.
  };

  // Updates the event with the appropriate transform for the device scale
  // factor. The RootWindowHostDelegate dispatches events in the physical pixel
  // coordinate. But the event processing from RootWindow onwards happen in
  // device-independent pixel coordinate. So it is necessary to update the event
  // received from the host. When |keep_inside_root| is true and the event's
  // system location is inside host window's bounds, the location will be
  // kept inside the root window's bounds.
  void TransformEventForDeviceScaleFactor(bool keep_inside_root,
                                          ui::LocatedEvent* event);

  // Moves the cursor to the specified location. This method is internally used
  // by MoveCursorTo() and MoveCursorToHostLocation().
  void MoveCursorToInternal(const gfx::Point& root_location,
                            const gfx::Point& host_location);

  // Called whenever the mouse moves, tracks the current |mouse_moved_handler_|,
  // sending exited and entered events as its value changes.
  void HandleMouseMoved(const ui::MouseEvent& event, Window* target);

  // Dispatches the specified event type (intended for enter/exit) to the
  // |mouse_moved_handler_|.
  void DispatchMouseEnterOrExit(const ui::MouseEvent& event,
                                ui::EventType type);

  void ProcessEvent(Window* target, ui::Event* event);

  bool ProcessGestures(ui::GestureRecognizer::Gestures* gestures);

  // Called when a Window is attached or detached from the RootWindow.
  void OnWindowAddedToRootWindow(Window* window);
  void OnWindowRemovedFromRootWindow(Window* window, RootWindow* new_root);

  // Called when a window becomes invisible, either by being removed
  // from root window hierarchy, via SetVisible(false) or being destroyed.
  // |reason| specifies what triggered the hiding. |new_root| is the new root
  // window, and may be NULL.
  void OnWindowHidden(Window* invisible,
                      WindowHiddenReason reason,
                      RootWindow* new_root);

  // Cleans up the gesture recognizer for all windows in |window| (including
  // |window| itself).
  void CleanupGestureRecognizerState(Window* window);

  // Updates the root window's size using |host_size|, current
  // transform and insets.
  void UpdateWindowSize(const gfx::Size& host_size);

  // Overridden from ui::EventDispatcherDelegate.
  virtual bool CanDispatchToTarget(EventTarget* target) OVERRIDE;

  // Overridden from ui::GestureEventHelper.
  virtual bool DispatchLongPressGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool DispatchCancelTouchEvent(ui::TouchEvent* event) OVERRIDE;

  // Overridden from ui::LayerAnimationObserver:
  virtual void OnLayerAnimationEnded(
      ui::LayerAnimationSequence* animation) OVERRIDE;
  virtual void OnLayerAnimationScheduled(
      ui::LayerAnimationSequence* animation) OVERRIDE;
  virtual void OnLayerAnimationAborted(
      ui::LayerAnimationSequence* animation) OVERRIDE;

  // Overridden from aura::RootWindowHostDelegate:
  virtual bool OnHostKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual bool OnHostMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual bool OnHostScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual bool OnHostTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnHostCancelMode() OVERRIDE;
  virtual void OnHostActivated() OVERRIDE;
  virtual void OnHostLostWindowCapture() OVERRIDE;
  virtual void OnHostLostMouseGrab() OVERRIDE;
  virtual void OnHostPaint(const gfx::Rect& damage_rect) OVERRIDE;
  virtual void OnHostMoved(const gfx::Point& origin) OVERRIDE;
  virtual void OnHostResized(const gfx::Size& size) OVERRIDE;
  virtual float GetDeviceScaleFactor() OVERRIDE;
  virtual RootWindow* AsRootWindow() OVERRIDE;

  // We hold and aggregate mouse drags as a way of throttling resizes when
  // HoldMouseMoves() is called. The following methods are used to dispatch held
  // and newly incoming mouse events, typically when an event other than a mouse
  // drag needs dispatching or a matching ReleaseMouseMoves() is called.
  // NOTE: because these methods dispatch events from RootWindowHost the
  // coordinates are in terms of the root.
  bool DispatchMouseEventImpl(ui::MouseEvent* event);
  bool DispatchMouseEventRepost(ui::MouseEvent* event);
  bool DispatchMouseEventToTarget(ui::MouseEvent* event, Window* target);
  void DispatchHeldEvents();

  // Parses the switch describing the initial size for the host window and
  // returns bounds for the window.
  gfx::Rect GetInitialHostWindowBounds() const;

  // Posts a task to send synthesized mouse move event if there
  // is no a pending task.
  void PostMouseMoveEventAfterWindowChange();

  // Creates and dispatches synthesized mouse move event using the
  // current mouse location.
  void SynthesizeMouseMoveEvent();

  gfx::Transform GetInverseRootTransform() const;

  scoped_ptr<ui::Compositor> compositor_;

  scoped_ptr<RootWindowHost> host_;

  // Used to schedule painting.
  base::WeakPtrFactory<RootWindow> schedule_paint_factory_;

  // Use to post mouse move event.
  base::WeakPtrFactory<RootWindow> event_factory_;

  // ui::EventFlags containing the current state of the mouse buttons.
  int mouse_button_flags_;

  // Touch ids that are currently down.
  uint32 touch_ids_down_;

  // Last cursor set.  Used for testing.
  gfx::NativeCursor last_cursor_;

  ObserverList<RootWindowObserver> observers_;

  Window* mouse_pressed_handler_;
  Window* mouse_moved_handler_;
  Window* mouse_event_dispatch_target_;
  Window* event_dispatch_target_;

  // The gesture_recognizer_ for this.
  scoped_ptr<ui::GestureRecognizer> gesture_recognizer_;

  bool synthesize_mouse_move_;
  bool waiting_on_compositing_end_;
  bool draw_on_compositing_end_;

  bool defer_draw_scheduling_;

  // How many holds are outstanding. We try to defer dispatching mouse moves
  // while the count is > 0.
  int mouse_move_hold_count_;
  scoped_ptr<ui::MouseEvent> held_mouse_move_;
  // Used to schedule DispatchHeldEvents() when |mouse_move_hold_count_| goes
  // to 0.
  base::WeakPtrFactory<RootWindow> held_event_factory_;

  // Allowing for reposting of events. Used when exiting context menus.
  scoped_ptr<ui::LocatedEvent>  held_repostable_event_;
  base::WeakPtrFactory<RootWindow> repostable_event_factory_;

  scoped_ptr<ui::ViewProp> prop_;

  scoped_ptr<RootWindowTransformer> transformer_;

  DISALLOW_COPY_AND_ASSIGN(RootWindow);
};

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_H_
