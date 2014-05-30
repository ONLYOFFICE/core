// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/x11_desktop_window_move_client.h"

#include <X11/Xlib.h>
// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include "base/debug/stack_trace.h"
#include "base/message_loop.h"
#include "base/message_pump_aurax11.h"
#include "base/run_loop.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/base/events/event.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/screen.h"

namespace views {

X11DesktopWindowMoveClient::X11DesktopWindowMoveClient()
    : in_move_loop_(false),
      grab_input_window_(None),
      root_window_(NULL) {
}

X11DesktopWindowMoveClient::~X11DesktopWindowMoveClient() {}

////////////////////////////////////////////////////////////////////////////////
// DesktopRootWindowHostLinux, MessageLoop::Dispatcher implementation:

bool X11DesktopWindowMoveClient::Dispatch(const base::NativeEvent& event) {
  XEvent* xev = event;

  // Note: the escape key is handled in the tab drag controller, which has
  // keyboard focus even though we took pointer grab.
  switch (xev->type) {
    case MotionNotify: {
      gfx::Point cursor_point(xev->xmotion.x_root, xev->xmotion.y_root);
      gfx::Point system_loc = cursor_point - window_offset_;
      root_window_->SetHostBounds(gfx::Rect(
          system_loc, root_window_->GetHostSize()));
      break;
    }
    case ButtonPress:
    case ButtonRelease: {
      EndMoveLoop();
      break;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// DesktopRootWindowHostLinux, aura::client::WindowMoveClient implementation:

aura::client::WindowMoveResult X11DesktopWindowMoveClient::RunMoveLoop(
    aura::Window* source,
    const gfx::Vector2d& drag_offset,
    aura::client::WindowMoveSource move_source) {
  DCHECK(!in_move_loop_);  // Can only handle one nested loop at a time.
  in_move_loop_ = true;
  window_offset_ = drag_offset;

  root_window_ = source->GetRootWindow();

  Display* display = base::MessagePumpAuraX11::GetDefaultXDisplay();

  // Creates an invisible, InputOnly toplevel window. This window will receive
  // all mouse movement for drags. It turns out that normal windows doing a
  // grab doesn't redirect pointer motion events if the pointer isn't over the
  // grabbing window. But InputOnly windows are able to grab everything. This
  // is what GTK+ does, and I found a patch to KDE that did something similar.
  unsigned long attribute_mask = CWEventMask | CWOverrideRedirect;
  XSetWindowAttributes swa;
  memset(&swa, 0, sizeof(swa));
  swa.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                   StructureNotifyMask;
  swa.override_redirect = True;
  grab_input_window_ = XCreateWindow(
      display,
      DefaultRootWindow(display),
      -100, -100, 10, 10,
      0, 0, InputOnly, CopyFromParent,
      attribute_mask, &swa);
  base::MessagePumpAuraX11::Current()->AddDispatcherForWindow(
      this, grab_input_window_);

  // Wait for the window to be mapped. If we don't, XGrabPointer fails.
  XMapRaised(display, grab_input_window_);
  base::MessagePumpAuraX11::Current()->BlockUntilWindowMapped(
      grab_input_window_);

  XGrabServer(display);
  XUngrabPointer(display, CurrentTime);
  int ret = XGrabPointer(
      display,
      grab_input_window_,
      False,
      ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
      GrabModeAsync,
      GrabModeAsync,
      None,
      None,
      CurrentTime);
  XUngrabServer(display);
  if (ret != GrabSuccess) {
    DLOG(ERROR) << "Grabbing new tab for dragging failed: "
                << ui::GetX11ErrorString(display, ret);
    return aura::client::MOVE_CANCELED;
  }

  base::MessageLoopForUI* loop = base::MessageLoopForUI::current();
  base::MessageLoop::ScopedNestableTaskAllower allow_nested(loop);
  base::RunLoop run_loop(aura::Env::GetInstance()->GetDispatcher());
  quit_closure_ = run_loop.QuitClosure();
  run_loop.Run();
  return aura::client::MOVE_SUCCESSFUL;
}

void X11DesktopWindowMoveClient::EndMoveLoop() {
  if (!in_move_loop_)
    return;

  // TODO(erg): Is this ungrab the cause of having to click to give input focus
  // on drawn out windows? Not ungrabbing here screws the X server until I kill
  // the chrome process.

  // Ungrab before we let go of the window.
  Display* display = base::MessagePumpAuraX11::GetDefaultXDisplay();
  XUngrabPointer(display, CurrentTime);

  base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(
      grab_input_window_);
  root_window_ = NULL;
  XDestroyWindow(display, grab_input_window_);

  in_move_loop_ = false;
  quit_closure_.Run();
}

}  // namespace views
