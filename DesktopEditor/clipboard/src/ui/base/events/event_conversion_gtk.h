// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_EVENTS_EVENT_CONVERSION_GTK_H_
#define UI_BASE_EVENTS_EVENT_CONVERSION_GTK_H_

#include <gtk/gtk.h>

#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT int GdkModifierToEventFlag(GdkModifierType gdk_modifier);

UI_EXPORT GdkModifierType EventFlagToGdkModifier(int event_flag);

}  // namespace ui

#endif  // UI_BASE_EVENTS_EVENT_CONVERSION_GTK_H_
