// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/accelerators/platform_accelerator_gtk.h"

#include "ui/base/events/event_conversion_gtk.h"
#include "ui/base/keycodes/keyboard_code_conversion_gtk.h"

namespace ui {

PlatformAcceleratorGtk::PlatformAcceleratorGtk()
    : gdk_key_code_(0),
      gdk_modifier_(static_cast<GdkModifierType>(0)) {
}

PlatformAcceleratorGtk::PlatformAcceleratorGtk(guint gdk_key_code,
                                               GdkModifierType gdk_modifier)
    : gdk_key_code_(gdk_key_code),
      gdk_modifier_(gdk_modifier) {
}

PlatformAcceleratorGtk::~PlatformAcceleratorGtk() {
}

scoped_ptr<PlatformAccelerator> PlatformAcceleratorGtk::CreateCopy() const {
  scoped_ptr<PlatformAcceleratorGtk> copy(new PlatformAcceleratorGtk);
  copy->gdk_key_code_ = gdk_key_code_;
  copy->gdk_modifier_ = gdk_modifier_;
  return scoped_ptr<PlatformAccelerator>(copy.release());
}

bool PlatformAcceleratorGtk::Equals(const PlatformAccelerator& rhs) const {
  const PlatformAcceleratorGtk& rhs_gtk =
      static_cast<const PlatformAcceleratorGtk&>(rhs);
  return gdk_key_code_ == rhs_gtk.gdk_key_code_ &&
         gdk_modifier_ == rhs_gtk.gdk_modifier_;
}

Accelerator AcceleratorForGdkKeyCodeAndModifier(guint gdk_key_code,
                                                GdkModifierType gdk_modifier) {
  ui::Accelerator accelerator(ui::WindowsKeyCodeForGdkKeyCode(gdk_key_code),
                              ui::GdkModifierToEventFlag(gdk_modifier));
  scoped_ptr<PlatformAccelerator> platform_accelerator(
      new PlatformAcceleratorGtk(gdk_key_code, gdk_modifier));
  accelerator.set_platform_accelerator(platform_accelerator.Pass());
  return accelerator;
}

guint GetGdkKeyCodeForAccelerator(const Accelerator& accelerator) {
  if (accelerator.platform_accelerator()) {
    return static_cast<const PlatformAcceleratorGtk*>(
        accelerator.platform_accelerator())->gdk_key_code();
  }
  // The second parameter is false because accelerator keys are expressed in
  // terms of the non-shift-modified key.
  return GdkKeyCodeForWindowsKeyCode(accelerator.key_code(), false);
}

GdkModifierType GetGdkModifierForAccelerator(const Accelerator& accelerator) {
  if (accelerator.platform_accelerator()) {
    return static_cast<const PlatformAcceleratorGtk*>(
        accelerator.platform_accelerator())->gdk_modifier();
  }
  return EventFlagToGdkModifier(accelerator.modifiers());
}

}  // namespace ui
