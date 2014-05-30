// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A list of constants that are used in setting up GTK widgets.
//
// This contains named color constants, along with spacing constants from the
// GNOME Human Interface Guide.

#ifndef UI_BASE_GTK_GTK_HIG_CONSTANTS_H_
#define UI_BASE_GTK_GTK_HIG_CONSTANTS_H_

#include "ui/base/ui_export.h"

typedef struct _GdkColor GdkColor;

// Define a macro for creating GdkColors from RGB values.  This is a macro to
// allow static construction of literals, etc.  Use this like:
//   GdkColor white = GDK_COLOR_RGB(0xff, 0xff, 0xff);
#define GDK_COLOR_RGB(r, g, b) {0, r * ::ui::kSkiaToGDKMultiplier,  \
        g * ::ui::kSkiaToGDKMultiplier, b * ::ui::kSkiaToGDKMultiplier}

namespace ui {

// Multiply uint8 color components by this.
const int kSkiaToGDKMultiplier = 257;

// Common color constants.
const GdkColor kGdkWhite = GDK_COLOR_RGB(0xFF, 0xFF, 0xFF);
const GdkColor kGdkBlack = GDK_COLOR_RGB(0x00, 0x00, 0x00);
const GdkColor kGdkGray = GDK_COLOR_RGB(0x7F, 0x7F, 0x7F);

// Constants relating to the layout of dialog windows:
// (See http://library.gnome.org/devel/hig-book/stable/design-window.html.en)

// Spacing between controls of the same group.
const int kControlSpacing = 6;

// Horizontal spacing between a label and its control.
const int kLabelSpacing = 12;

// Indent of the controls within each group.
const int kGroupIndent = 12;

// Space around the outside of a dialog's contents.
const int kContentAreaBorder = 12;

// Spacing between groups of controls.
const int kContentAreaSpacing = 18;

// Horizontal Spacing between controls in a form.
const int kFormControlSpacing = 10;

}  // namespace ui

#endif  // UI_BASE_GTK_GTK_HIG_CONSTANTS_H_
