// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/focus_store_gtk.h"

#include <gtk/gtk.h>

namespace ui {

FocusStoreGtk::FocusStoreGtk()
    : widget_(NULL),
      destroy_handler_id_(0) {
}

FocusStoreGtk::~FocusStoreGtk() {
  DisconnectDestroyHandler();
}

void FocusStoreGtk::Store(GtkWidget* widget) {
  GtkWidget* focus_widget = NULL;
  if (widget) {
    // A detached widget won't have a toplevel window as an ancestor, so we
    // can't assume that the query for toplevel will return a window.
    GtkWidget* toplevel = gtk_widget_get_ancestor(widget, GTK_TYPE_WINDOW);
    GtkWindow* window = GTK_IS_WINDOW(toplevel) ? GTK_WINDOW(toplevel) : NULL;
    if (window)
      focus_widget = gtk_window_get_focus(window);
  }

  SetWidget(focus_widget);
}

void FocusStoreGtk::SetWidget(GtkWidget* widget) {
  DisconnectDestroyHandler();

  // We don't add a ref. The signal handler below effectively gives us a weak
  // reference.
  widget_ = widget;
  if (widget_) {
    // When invoked, |gtk_widget_destroyed| will set |widget_| to NULL.
    destroy_handler_id_ = g_signal_connect(widget_, "destroy",
                                           G_CALLBACK(gtk_widget_destroyed),
                                           &widget_);
  }
}

void FocusStoreGtk::DisconnectDestroyHandler() {
  if (widget_) {
    g_signal_handler_disconnect(widget_, destroy_handler_id_);
    widget_ = NULL;
  }
}

}  // namespace ui
