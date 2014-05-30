// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_DELEGATE_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_DELEGATE_H_

#include <vector>

#include "ui/base/events/event_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Path;
class Point;
class Size;
}

namespace ui {
class GestureEvent;
class KeyEvent;
class Layer;
class MouseEvent;
class TouchEvent;
class ScrollEvent;
}

namespace views {
class InputMethod;
class Widget;

namespace internal {

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetDelegate
//
//  An interface implemented by the object that handles events sent by a
//  NativeWidget implementation.
//
class VIEWS_EXPORT NativeWidgetDelegate {
 public:
  virtual ~NativeWidgetDelegate() {}

  // Returns true if the window is modal.
  virtual bool IsModal() const = 0;

  // Returns true if the window is a dialog box.
  virtual bool IsDialogBox() const = 0;

  // Returns true if the window can be activated.
  virtual bool CanActivate() const = 0;

  virtual bool IsInactiveRenderingDisabled() const = 0;
  virtual void EnableInactiveRendering() = 0;

  // Called when the activation state of a window has changed.
  virtual void OnNativeWidgetActivationChanged(bool active) = 0;

  // Called when native focus moves from one native view to another.
  virtual void OnNativeFocus(gfx::NativeView focused_view) = 0;
  virtual void OnNativeBlur(gfx::NativeView focused_view) = 0;

  // Called when the window is shown/hidden.
  virtual void OnNativeWidgetVisibilityChanged(bool visible) = 0;

  // Called when the native widget is created.
  virtual void OnNativeWidgetCreated() = 0;

  // Called just before the native widget is destroyed. This is the delegate's
  // last chance to do anything with the native widget handle.
  virtual void OnNativeWidgetDestroying() = 0;

  // Called just after the native widget is destroyed.
  virtual void OnNativeWidgetDestroyed() = 0;

  // Returns the smallest size the window can be resized to by the user.
  virtual gfx::Size GetMinimumSize() = 0;

  // Returns the largest size the window can be resized to by the user.
  virtual gfx::Size GetMaximumSize() = 0;

  // Called when the NativeWidget changed position.
  virtual void OnNativeWidgetMove() = 0;

  // Called when the NativeWidget changed size to |new_size|.
  virtual void OnNativeWidgetSizeChanged(const gfx::Size& new_size) = 0;

  // Called when the user begins/ends to change the bounds of the window.
  virtual void OnNativeWidgetBeginUserBoundsChange() = 0;
  virtual void OnNativeWidgetEndUserBoundsChange() = 0;

  // Returns true if the delegate has a FocusManager.
  virtual bool HasFocusManager() const = 0;

  // Paints the widget using acceleration. If the widget is not using
  // accelerated painting this returns false and does nothing.
  virtual bool OnNativeWidgetPaintAccelerated(
      const gfx::Rect& dirty_region) = 0;

  // Paints the rootview in the canvas. This will also refresh the compositor
  // tree if necessary when accelerated painting is enabled.
  virtual void OnNativeWidgetPaint(gfx::Canvas* canvas) = 0;

  // Returns the non-client component (see ui/base/hit_test.h) containing
  // |point|, in client coordinates.
  virtual int GetNonClientComponent(const gfx::Point& point) = 0;

  // Mouse and key event handlers.
  virtual void OnKeyEvent(ui::KeyEvent* event) = 0;
  virtual void OnMouseEvent(ui::MouseEvent* event) = 0;
  virtual void OnMouseCaptureLost() = 0;

  virtual void OnTouchEvent(ui::TouchEvent* event) = 0;
  virtual void OnScrollEvent(ui::ScrollEvent* event) = 0;
  virtual void OnGestureEvent(ui::GestureEvent* event) = 0;

  // Runs the specified native command. Returns true if the command is handled.
  virtual bool ExecuteCommand(int command_id) = 0;

  // Returns the input method of the widget this delegate is associated with.
  // Note that this does not use the top level widget, so may return NULL
  // if the widget doesn't have input method.
  virtual InputMethod* GetInputMethodDirect() = 0;

  // Returns the child Layers of the Widgets layer that were created by Views.
  virtual const std::vector<ui::Layer*>& GetRootLayers() = 0;

  // Returns true if window has a hit-test mask.
  virtual bool HasHitTestMask() const = 0;

  // Provides the hit-test mask if HasHitTestMask above returns true.
  virtual void GetHitTestMask(gfx::Path* mask) const = 0;

  //
  virtual Widget* AsWidget() = 0;
  virtual const Widget* AsWidget() const = 0;
};

}  // namespace internal
}  // namespace views

#endif  // UI_VIEWS_WIDGET_NATIVE_WIDGET_DELEGATE_H_
