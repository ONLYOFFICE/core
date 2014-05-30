// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme_gtk.h"

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "ui/gfx/skia_utils_gtk.h"

namespace {

const SkColor kInvalidColorIdColor = SkColorSetRGB(255, 0, 128);

// Theme colors returned by GetSystemColor().

// FocusableBorder:
const SkColor kFocusedBorderColor = SkColorSetRGB(0x4D, 0x90, 0xFE);
const SkColor kUnfocusedBorderColor = SkColorSetRGB(0xD9, 0xD9, 0xD9);

// MenuItem
const SkColor kFocusedMenuItemBackgroundColor = SkColorSetARGB(13, 0, 0, 0);
const SkColor kHoverMenuItemBackgroundColor = SkColorSetRGB(204, 204, 204);

// MenuButton
const SkColor kEnabledMenuButtonBorderColor = SkColorSetARGB(36, 0, 0, 0);
const SkColor kFocusedMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);
const SkColor kHoverMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);

// TextButton:
const SkColor kTextButtonBackgroundColor = SkColorSetRGB(0xde, 0xde, 0xde);
const SkColor kTextButtonEnabledColor = SkColorSetRGB(6, 45, 117);
const SkColor kTextButtonDisabledColor = SkColorSetRGB(161, 161, 146);
const SkColor kTextButtonHighlightColor = SkColorSetARGB(200, 255, 255, 255);
const SkColor kTextButtonHoverColor = kTextButtonEnabledColor;

}  // namespace

namespace ui {

// static
NativeTheme* NativeTheme::instance() {
  return NativeThemeGtk::instance();
}

// static
NativeThemeGtk* NativeThemeGtk::instance() {
  CR_DEFINE_STATIC_LOCAL(NativeThemeGtk, s_native_theme, ());
  return &s_native_theme;
}

SkColor NativeThemeGtk::GetSystemColor(ColorId color_id) const {
  switch (color_id) {
    case kColorId_DialogBackground:
      // TODO(benrg): This code used to call gtk_widget_get_style() on the
      // widget being styled. After refactoring, that widget is not available
      // and we have to call gtk_widget_get_default_style(). Unfortunately,
      // it turns out that this breaks everything (chromium bug 105609,
      // chromium-os bug 23461). Need to figure out the right thing and do it.
      return gfx::GdkColorToSkColor(
                     gtk_widget_get_default_style()->bg[GTK_STATE_NORMAL]);

    // FocusableBorder:
    case kColorId_FocusedBorderColor:
      return kFocusedBorderColor;
    case kColorId_UnfocusedBorderColor:
      return kUnfocusedBorderColor;

    // MenuItem
    case kColorId_FocusedMenuItemBackgroundColor:
      return kFocusedMenuItemBackgroundColor;
    case kColorId_HoverMenuItemBackgroundColor:
      return kHoverMenuItemBackgroundColor;
    case kColorId_EnabledMenuButtonBorderColor:
      return kEnabledMenuButtonBorderColor;
    case kColorId_FocusedMenuButtonBorderColor:
      return kFocusedMenuButtonBorderColor;
    case kColorId_HoverMenuButtonBorderColor:
      return kHoverMenuButtonBorderColor;

    // TextButton:
    case kColorId_TextButtonBackgroundColor:
      return kTextButtonBackgroundColor;
    case kColorId_TextButtonEnabledColor:
      return kTextButtonEnabledColor;
    case kColorId_TextButtonDisabledColor:
      return kTextButtonDisabledColor;
    case kColorId_TextButtonHighlightColor:
      return kTextButtonHighlightColor;
    case kColorId_TextButtonHoverColor:
      return kTextButtonHoverColor;

    default:
      NOTREACHED() << "Invalid color_id: " << color_id;
      break;
  }
  return kInvalidColorIdColor;
}

NativeThemeGtk::NativeThemeGtk() {
}

NativeThemeGtk::~NativeThemeGtk() {
}

}  // namespace ui
