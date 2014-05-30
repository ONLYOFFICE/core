// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/x11_desktop_handler.h"

#include "base/message_loop.h"
#include "ui/aura/env.h"
#include "ui/aura/focus_manager.h"
#include "ui/aura/root_window.h"
#include "ui/base/x/x11_util.h"
#include "ui/views/widget/desktop_aura/desktop_activation_client.h"

#if !defined(OS_CHROMEOS)
#include "ui/views/ime/input_method.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host_x11.h"
#endif

namespace {

const char* kAtomsToCache[] = {
  "_NET_ACTIVE_WINDOW",
  NULL
};

// Our global instance. Deleted when our Env() is deleted.
views::X11DesktopHandler* g_handler = NULL;

}  // namespace

namespace views {

// static
X11DesktopHandler* X11DesktopHandler::get() {
  if (!g_handler)
    g_handler = new X11DesktopHandler;

  return g_handler;
}

X11DesktopHandler::X11DesktopHandler()
    : xdisplay_(base::MessagePumpAuraX11::GetDefaultXDisplay()),
      x_root_window_(DefaultRootWindow(xdisplay_)),
      current_window_(None),
      atom_cache_(xdisplay_, kAtomsToCache) {
  base::MessagePumpAuraX11::Current()->AddDispatcherForRootWindow(this);
  aura::Env::GetInstance()->AddObserver(this);

  XWindowAttributes attr;
  XGetWindowAttributes(xdisplay_, x_root_window_, &attr);
  XSelectInput(xdisplay_, x_root_window_,
               attr.your_event_mask | PropertyChangeMask |
               StructureNotifyMask | SubstructureNotifyMask);
}

X11DesktopHandler::~X11DesktopHandler() {
  aura::Env::GetInstance()->RemoveObserver(this);
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForRootWindow(this);
}

void X11DesktopHandler::ActivateWindow(::Window window) {
  DCHECK_EQ(base::MessagePumpAuraX11::GetDefaultXDisplay(), xdisplay_);

  XEvent xclient;
  memset(&xclient, 0, sizeof(xclient));
  xclient.type = ClientMessage;
  xclient.xclient.window = window;
  xclient.xclient.message_type = atom_cache_.GetAtom("_NET_ACTIVE_WINDOW");
  xclient.xclient.format = 32;
  xclient.xclient.data.l[0] = 1;  // Specified we are an app.
  xclient.xclient.data.l[1] = CurrentTime;
  xclient.xclient.data.l[2] = None;
  xclient.xclient.data.l[3] = 0;
  xclient.xclient.data.l[4] = 0;

  XSendEvent(xdisplay_, x_root_window_, False,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xclient);
}

bool X11DesktopHandler::IsActiveWindow(::Window window) const {
  return window == current_window_;
}

bool X11DesktopHandler::Dispatch(const base::NativeEvent& event) {
  // Check for a change to the active window.
  switch (event->type) {
    case PropertyNotify: {
      ::Atom active_window = atom_cache_.GetAtom("_NET_ACTIVE_WINDOW");

      if (event->xproperty.window == x_root_window_ &&
          event->xproperty.atom == active_window) {
        int window;
        if (ui::GetIntProperty(x_root_window_, "_NET_ACTIVE_WINDOW", &window) &&
            window) {
          OnActiveWindowChanged(static_cast< ::Window>(window));
        }
      }
      break;
    }
  }

  return true;
}

void X11DesktopHandler::OnWindowInitialized(aura::Window* window) {
}

void X11DesktopHandler::OnWillDestroyEnv() {
  g_handler = NULL;
  delete this;
}

void X11DesktopHandler::OnActiveWindowChanged(::Window xid) {
  DesktopRootWindowHostX11* old_host =
      views::DesktopRootWindowHostX11::GetHostForXID(current_window_);
  if (old_host)
    old_host->HandleNativeWidgetActivationChanged(false);

  DesktopRootWindowHostX11* new_host =
      views::DesktopRootWindowHostX11::GetHostForXID(xid);
  if (new_host)
    new_host->HandleNativeWidgetActivationChanged(true);

  current_window_ = xid;
}

}  // namespace views
