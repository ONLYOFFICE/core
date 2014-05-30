// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_windowing.h"

#include <gdk/gdkx.h>

#include "base/logging.h"
#include "ui/base/gtk/gtk_compat.h"
#include "ui/base/x/x11_util.h"

namespace ui {

void StackPopupWindow(GtkWidget* popup, GtkWidget* toplevel) {
  DCHECK(GTK_IS_WINDOW(popup) && gtk_widget_is_toplevel(popup) &&
         gtk_widget_get_realized(popup));
  DCHECK(GTK_IS_WINDOW(toplevel) && gtk_widget_is_toplevel(toplevel) &&
         gtk_widget_get_realized(toplevel));

  // Stack the |popup| window directly above the |toplevel| window.
  // The popup window is a direct child of the root window, so we need to
  // find a similar ancestor for the toplevel window (which might have been
  // reparented by a window manager).  We grab the server while we're doing
  // this -- otherwise, we'll get an error if the window manager reparents the
  // toplevel window right after we call GetHighestAncestorWindow().
  gdk_x11_display_grab(gtk_widget_get_display(toplevel));
  XID toplevel_window_base = ui::GetHighestAncestorWindow(
      ui::GetX11WindowFromGtkWidget(toplevel),
      ui::GetX11RootWindow());
  if (toplevel_window_base) {
    XID window_xid = ui::GetX11WindowFromGtkWidget(popup);
    XID window_parent = ui::GetParentWindow(window_xid);
    if (window_parent == ui::GetX11RootWindow()) {
      ui::RestackWindow(window_xid, toplevel_window_base, true);
    } else {
      // The window manager shouldn't reparent override-redirect windows.
      DLOG(ERROR) << "override-redirect window " << window_xid
                  << "'s parent is " << window_parent
                  << ", rather than root window "
                  << ui::GetX11RootWindow();
    }
  }
  gdk_x11_display_ungrab(gtk_widget_get_display(toplevel));
}

}  // namespace ui

