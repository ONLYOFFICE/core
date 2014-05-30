// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/event_synthesis_gtk.h"

#include "ui/base/keycodes/keyboard_code_conversion_gtk.h"

namespace ui {

GdkEvent* SynthesizeKeyEvent(GdkWindow* window,
                             bool press, guint gdk_key, guint state) {
  GdkEvent* event = gdk_event_new(press ? GDK_KEY_PRESS : GDK_KEY_RELEASE);

  event->key.type = press ? GDK_KEY_PRESS : GDK_KEY_RELEASE;
  event->key.window = window;
  if (window)
    g_object_ref(window);
  event->key.send_event = false;

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  event->key.time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

  event->key.state = state;
  event->key.keyval = gdk_key;

  GdkKeymapKey* keys;
  gint n_keys;
  if (event->key.keyval != 0 &&
      gdk_keymap_get_entries_for_keyval(gdk_keymap_get_default(),
                                        event->key.keyval, &keys, &n_keys)) {
    event->key.hardware_keycode = keys[0].keycode;
    event->key.group = keys[0].group;
    g_free(keys);
  }

  return event;
}

void SynthesizeKeyPressEvents(GdkWindow* window,
                              KeyboardCode key,
                              bool control, bool shift, bool alt,
                              std::vector<GdkEvent*>* events) {
  if (control)
    events->push_back(
        SynthesizeKeyEvent(window, true, GDK_Control_L, 0));

  if (shift) {
    events->push_back(SynthesizeKeyEvent(window, true, GDK_Shift_L,
                                         control ? GDK_CONTROL_MASK : 0));
  }

  if (alt) {
    guint state = (control ? GDK_CONTROL_MASK : 0) |
                  (shift ? GDK_SHIFT_MASK : 0);
    events->push_back(
        SynthesizeKeyEvent(window, true, GDK_Alt_L, state));
  }

  // TODO(estade): handle other state flags besides control, shift, alt?
  // For example caps lock.
  guint state = (control ? GDK_CONTROL_MASK : 0) |
                (shift ? GDK_SHIFT_MASK : 0) |
                (alt ? GDK_MOD1_MASK : 0);

  guint gdk_key = GdkKeyCodeForWindowsKeyCode(key, shift);
  events->push_back(SynthesizeKeyEvent(window, true, gdk_key, state));
  events->push_back(SynthesizeKeyEvent(window, false, gdk_key, state));

  if (alt) {
    guint state = (control ? GDK_CONTROL_MASK : 0) |
                  (shift ? GDK_SHIFT_MASK : 0) | GDK_MOD1_MASK;
    events->push_back(
        SynthesizeKeyEvent(window, false, GDK_Alt_L, state));
  }

  if (shift) {
    events->push_back(
        SynthesizeKeyEvent(window, false, GDK_Shift_L,
                           (control ? GDK_CONTROL_MASK : 0) | GDK_SHIFT_MASK));
  }

  if (control) {
    events->push_back(
        SynthesizeKeyEvent(window, false, GDK_Control_L, GDK_CONTROL_MASK));
  }
}

}  // namespace ui
