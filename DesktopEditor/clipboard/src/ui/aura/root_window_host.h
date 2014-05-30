// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_HOST_H_
#define UI_AURA_ROOT_WINDOW_HOST_H_

#include <vector>

#include "base/message_loop.h"
#include "ui/aura/aura_export.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/native_widget_types.h"

class SkCanvas;

namespace gfx {
class Insets;
class Point;
class Rect;
class Size;
}

namespace aura {

class RootWindow;
class RootWindowHostDelegate;

// RootWindowHost bridges between a native window and the embedded RootWindow.
// It provides the accelerated widget and maps events from the native os to
// aura.
class AURA_EXPORT RootWindowHost {
 public:
  virtual ~RootWindowHost() {}

  // Creates a new RootWindowHost. The caller owns the returned value.
  static RootWindowHost* Create(const gfx::Rect& bounds);

  // Returns the actual size of the screen.
  // (gfx::Screen only reports on the virtual desktop exposed by Aura.)
  static gfx::Size GetNativeScreenSize();

  // Sets the delegate, which is normally done by the root window.
  virtual void SetDelegate(RootWindowHostDelegate* delegate) = 0;

  virtual RootWindow* GetRootWindow() = 0;

  // Returns the accelerated widget.
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;

  // Shows the RootWindowHost.
  virtual void Show() = 0;

  // Hides the RootWindowHost.
  virtual void Hide() = 0;

  // Toggles the host's full screen state.
  virtual void ToggleFullScreen() = 0;

  // Gets/Sets the size of the RootWindowHost.
  virtual gfx::Rect GetBounds() const = 0;
  virtual void SetBounds(const gfx::Rect& bounds) = 0;

  // Sets/Gets the insets that specifies the effective root window area
  // in the host window.
  virtual gfx::Insets GetInsets() const = 0;
  virtual void SetInsets(const gfx::Insets& insets) = 0;

  // Returns the location of the RootWindow on native screen.
  virtual gfx::Point GetLocationOnNativeScreen() const = 0;

  // Sets the OS capture to the root window.
  virtual void SetCapture() = 0;

  // Releases OS capture of the root window.
  virtual void ReleaseCapture() = 0;

  // Sets the currently displayed cursor.
  virtual void SetCursor(gfx::NativeCursor cursor) = 0;

  // Queries the mouse's current position relative to the host window and sets
  // it in |location_return|. Returns true if the cursor is within the host
  // window. The position set to |location_return| is constrained within the
  // host window. If the cursor is disabled, returns false and (0, 0) is set to
  // |location_return|.
  // This method is expensive, instead use gfx::Screen::GetCursorScreenPoint().
  virtual bool QueryMouseLocation(gfx::Point* location_return) = 0;

  // Clips the cursor to the bounds of the root window until UnConfineCursor().
  virtual bool ConfineCursorToRootWindow() = 0;
  virtual void UnConfineCursor() = 0;

  // Called when the cursor visibility has changed.
  virtual void OnCursorVisibilityChanged(bool show) = 0;

  // Moves the cursor to the specified location relative to the root window.
  virtual void MoveCursorTo(const gfx::Point& location) = 0;

  // Sets if the window should be focused when shown.
  virtual void SetFocusWhenShown(bool focus_when_shown) = 0;

  // Copies |source_bounds| from the root window (as displayed on the host
  // machine) to |canvas| at offset |dest_offset|.  The bounds need to be in
  // physical pixels.
  virtual bool CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                  const gfx::Point& dest_offset,
                                  SkCanvas* canvas) = 0;

  // Posts |native_event| to the platform's event queue.
#if !defined(OS_MACOSX)
  virtual void PostNativeEvent(const base::NativeEvent& native_event) = 0;
#endif

  // Called when the device scale factor of the root window has chagned.
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) = 0;

  // Stop listening events in preparation for shutdown.
  virtual void PrepareForShutdown() = 0;
};

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_HOST_H_
