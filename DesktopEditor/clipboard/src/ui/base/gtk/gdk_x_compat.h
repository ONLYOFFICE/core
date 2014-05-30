// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GDK_X_COMPAT_H_
#define UI_BASE_GTK_GDK_X_COMPAT_H_

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

// Google Chrome must depend on GTK 2.18, at least until the next LTS drops
// (and we might have to extend which version of GTK we want to target due to
// RHEL). To make our porting job for GTK3 easier, we define all the methods
// that replace deprecated APIs in this file and then include it everywhere.
//
// This file is organized first by version, and then with each version,
// alphabetically by method.

#if !GTK_CHECK_VERSION(2, 24, 0)
inline GdkWindow* gdk_x11_window_lookup_for_display(GdkDisplay* display,
                                                    Window window) {
  return static_cast<GdkWindow*>(gdk_xid_table_lookup_for_display(display,
                                                                  window));
}
#endif

#endif  // UI_BASE_GTK_GDK_X_COMPAT_H_
