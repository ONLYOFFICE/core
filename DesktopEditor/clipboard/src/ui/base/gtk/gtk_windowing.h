// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_WINDOWING_H_
#define UI_BASE_GTK_GTK_WINDOWING_H_

#include <gtk/gtk.h>

#include "ui/base/ui_export.h"

namespace ui {

// Stacks a |popup| window directly on top of a |toplevel| window.
UI_EXPORT void StackPopupWindow(GtkWidget* popup, GtkWidget* toplevel);

}  // namespace ui

#endif  // UI_BASE_GTK_GTK_WINDOWING_H_
