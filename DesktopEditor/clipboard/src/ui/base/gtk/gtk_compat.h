// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_COMPAT_H_
#define UI_BASE_GTK_GTK_COMPAT_H_

#include <gtk/gtk.h>

// Google Chrome must depend on GTK 2.18, at least until the next LTS drops
// (and we might have to extend which version of GTK we want to target due to
// RHEL). To make our porting job for GTK3 easier, we define all the methods
// that replace deprecated APIs in this file and then include it everywhere.
//
// This file is organized first by version, and then with each version,
// alphabetically by method.
//
// For Google Chrome builds, we want to support RHEL 6, which uses GTK 2.18,
// but the official builder is Ubuntu Lucid with GTK 2.20. Thus for Google
// Chrome builds, we define the GTK 2.20.0 compatibility functions even though
// the system GTK provides the functions.

#if !GTK_CHECK_VERSION(2, 20, 0) || defined(GOOGLE_CHROME_BUILD)
inline gboolean gtk_widget_get_mapped(GtkWidget* widget) {
  return GTK_WIDGET_MAPPED(widget);
}

inline gboolean gtk_widget_get_realized(GtkWidget* widget) {
  return GTK_WIDGET_REALIZED(widget);
}

inline gboolean gtk_widget_is_toplevel(GtkWidget* widget) {
  return GTK_WIDGET_TOPLEVEL(widget);
}

inline void gtk_widget_set_mapped(GtkWidget* widget,
                                  gboolean mapped) {
  if (mapped)
    GTK_WIDGET_SET_FLAGS(widget, GTK_MAPPED);
  else
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_MAPPED);
}

inline void gtk_widget_set_realized(GtkWidget* widget,
                                    gboolean realized) {
  if (realized)
    GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
  else
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_REALIZED);
}

inline void gtk_widget_style_attach(GtkWidget* widget) {
  widget->style = gtk_style_attach(widget->style, widget->window);
}
#endif  // !GTK_CHECK_VERSION(2, 20, 0) || defined(GOOGLE_CHROME_BUILD)

#if !GTK_CHECK_VERSION(2, 22, 0)
inline GdkWindow* gdk_drag_context_get_source_window(GdkDragContext *context) {
  return context->source_window;
}

inline gint gdk_visual_get_depth(GdkVisual* visual) {
  return visual->depth;
}

inline GdkWindow* gtk_button_get_event_window(GtkButton* button) {
  return button->event_window;
}
#endif  // !GTK_CHECK_VERSION(2, 22, 0)

#if !GTK_CHECK_VERSION(2, 24, 0)
inline void gdk_pixmap_get_size(GdkPixmap* pixmap, gint* width, gint* height) {
  gdk_drawable_get_size(GDK_DRAWABLE(pixmap), width, height);
}

inline GdkDisplay* gdk_window_get_display(GdkWindow* window) {
  return gdk_drawable_get_display(GDK_DRAWABLE(window));
}

inline int gdk_window_get_height(GdkWindow* window) {
  int height;
  gdk_drawable_get_size(GDK_DRAWABLE(window), NULL, &height);
  return height;
}

inline GdkScreen* gdk_window_get_screen(GdkWindow* window) {
  return gdk_drawable_get_screen(GDK_DRAWABLE(window));
}

inline int gdk_window_get_width(GdkWindow* window) {
  int width;
  gdk_drawable_get_size(GDK_DRAWABLE(window), &width, NULL);
  return width;
}
#endif  // !GTK_CHECK_VERSION(2, 24, 0)

#endif  // UI_BASE_GTK_GTK_COMPAT_H_
