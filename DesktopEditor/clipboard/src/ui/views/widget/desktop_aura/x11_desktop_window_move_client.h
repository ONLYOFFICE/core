// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_WINDOW_MOVE_CLIENT_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_WINDOW_MOVE_CLIENT_H_

#include <X11/Xlib.h>

// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop.h"
#include "ui/aura/client/window_move_client.h"
#include "ui/views/views_export.h"
#include "ui/gfx/point.h"

namespace aura {
class RootWindow;
}

namespace views {

// When we're dragging tabs, we need to manually position our window.
class VIEWS_EXPORT X11DesktopWindowMoveClient :
    public base::MessageLoop::Dispatcher,
    public aura::client::WindowMoveClient {
 public:
  X11DesktopWindowMoveClient();
  virtual ~X11DesktopWindowMoveClient();

  // Overridden from MessageLoop::Dispatcher:
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  // Overridden from aura::client::WindowMoveClient:
  virtual aura::client::WindowMoveResult RunMoveLoop(
      aura::Window* window,
      const gfx::Vector2d& drag_offset,
      aura::client::WindowMoveSource move_source) OVERRIDE;
  virtual void EndMoveLoop() OVERRIDE;

 private:
  // Are we running a nested message loop from RunMoveLoop()?
  bool in_move_loop_;

  // An invisible InputOnly window . We create this window so we can track the
  // cursor wherever it goes on screen during a drag, since normal windows
  // don't receive pointer motion events outside of their bounds.
  ::Window grab_input_window_;

  // We need to keep track of this so we can actually move it when reacting to
  // events from |grab_input_window_| during Dispatch().
  aura::RootWindow* root_window_;

  // Our cursor offset from the top left window origin when the drag
  // started. Used to calculate the window's new bounds relative to the current
  // location of the cursor.
  gfx::Vector2d window_offset_;

  base::Closure quit_closure_;
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_WINDOW_MOVE_CLIENT_H_
