// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file declares routines for creating fake GDK events (at the moment,
// only keyboard events). This is useful for a variety of testing purposes.
// NOTE: This should not be used outside of testing.

#ifndef UI_BASE_GTK_EVENT_SYNTHESIS_GTK_
#define UI_BASE_GTK_EVENT_SYNTHESIS_GTK_

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <vector>

#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/ui_export.h"

namespace ui {

// Creates and returns a key event. Passes ownership to the caller.
UI_EXPORT GdkEvent* SynthesizeKeyEvent(GdkWindow* event_window,
                                       bool press,
                                       guint gdk_key,
                                       guint state);

// Creates the proper sequence of key events for a key press + release.
// Ownership of the events in the vector is passed to the caller.
UI_EXPORT void SynthesizeKeyPressEvents(
    GdkWindow* window,
    KeyboardCode key,
    bool control, bool shift, bool alt,
    std::vector<GdkEvent*>* events);

}  // namespace ui

#endif  // UI_BASE_GTK_EVENT_SYNTHESIS_GTK_
