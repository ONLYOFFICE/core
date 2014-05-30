// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WINDOW_EVENT_FILTER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WINDOW_EVENT_FILTER_H_

#include <X11/Xlib.h>
// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include "base/compiler_specific.h"
#include "base/message_loop.h"
#include "ui/base/events/event_handler.h"
#include "ui/base/x/x11_atom_cache.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
class Window;
}

namespace gfx {
class Point;
}

namespace views {
class DesktopActivationClient;
class NativeWidgetAura;

// An EventFilter that sets properties on X11 windows.
class VIEWS_EXPORT X11WindowEventFilter : public ui::EventHandler {
 public:
  explicit X11WindowEventFilter(aura::RootWindow* root_window,
                                DesktopActivationClient* activation_client);
  virtual ~X11WindowEventFilter();

  // Changes whether borders are shown on this |root_window|.
  void SetUseHostWindowBorders(bool use_os_border);

  // Overridden from ui::EventHandler:
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

 private:
  // Dispatches a _NET_WM_MOVERESIZE message to the window manager to tell it
  // to act as if a border or titlebar drag occurred.
  bool DispatchHostWindowDragMovement(int hittest,
                                      const gfx::Point& screen_location);

  DesktopActivationClient* activation_client_;

  // The display and the native X window hosting the root window.
  Display* xdisplay_;
  ::Window xwindow_;

  // The native root window.
  ::Window x_root_window_;

  ui::X11AtomCache atom_cache_;

  // True if |xwindow_| is the current _NET_ACTIVE_WINDOW.
  bool is_active_;

  DISALLOW_COPY_AND_ASSIGN(X11WindowEventFilter);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_X11_WINDOW_EVENT_FILTER_H_
