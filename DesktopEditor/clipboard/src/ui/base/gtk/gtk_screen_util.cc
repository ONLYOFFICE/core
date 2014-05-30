// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_screen_util.h"

#include "base/logging.h"

namespace ui {

bool IsScreenComposited() {
  GdkScreen* screen = gdk_screen_get_default();
  return gdk_screen_is_composited(screen) == TRUE;
}

gfx::Point ScreenPoint(GtkWidget* widget) {
  int x, y;
  gdk_display_get_pointer(gtk_widget_get_display(widget), NULL, &x, &y,
                          NULL);
  return gfx::Point(x, y);
}

gfx::Point ClientPoint(GtkWidget* widget) {
  int x, y;
  gtk_widget_get_pointer(widget, &x, &y);
  return gfx::Point(x, y);
}

gfx::Vector2d GetWidgetScreenOffset(GtkWidget* widget) {
  GdkWindow* window = gtk_widget_get_window(widget);

  if (!window) {
    NOTREACHED() << "Must only be called on realized widgets.";
    return gfx::Vector2d(0, 0);
  }

  gint x, y;
  gdk_window_get_origin(window, &x, &y);

  if (!gtk_widget_get_has_window(widget)) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    x += allocation.x;
    y += allocation.y;
  }

  return gfx::Vector2d(x, y);
}

gfx::Rect GetWidgetScreenBounds(GtkWidget* widget) {
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);

  return gfx::Rect(PointAtOffsetFromOrigin(GetWidgetScreenOffset(widget)),
                   gfx::Size(allocation.width, allocation.height));
}

}  // namespace ui
