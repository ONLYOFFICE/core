// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/event_conversion_gtk.h"

#include "ui/base/events/event_constants.h"

namespace ui {

int GdkModifierToEventFlag(GdkModifierType gdk_modifier) {
  int event_flags = 0;
  if (gdk_modifier & GDK_SHIFT_MASK)
    event_flags |= EF_SHIFT_DOWN;
  if (gdk_modifier & GDK_CONTROL_MASK)
    event_flags |= EF_CONTROL_DOWN;
  if (gdk_modifier & GDK_MOD1_MASK)
    event_flags |= EF_ALT_DOWN;
  return event_flags;
}

GdkModifierType EventFlagToGdkModifier(int event_flag) {
  int modifier = 0;
  if (event_flag & EF_SHIFT_DOWN)
    modifier |= GDK_SHIFT_MASK;
  if (event_flag & EF_CONTROL_DOWN)
    modifier |= GDK_CONTROL_MASK;
  if (event_flag & EF_ALT_DOWN)
    modifier |= GDK_MOD1_MASK;
  return static_cast<GdkModifierType>(modifier);
}

}  // namespace ui
