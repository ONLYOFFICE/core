// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_NATIVE_THEME_COMMON_THEME_H_
#define UI_NATIVE_THEME_COMMON_THEME_H_

#include "ui/native_theme/native_theme.h"

class SkCanvas;

namespace ui {

// Drawing code that is common for all platforms.

// Returns true and |color| if |color_id| is found, or false otherwise.
bool NATIVE_THEME_EXPORT CommonThemeGetSystemColor(
    NativeTheme::ColorId color_id,
    SkColor* color);

gfx::Size NATIVE_THEME_EXPORT CommonThemeGetPartSize(
    NativeTheme::Part part,
    NativeTheme::State state,
    const NativeTheme::ExtraParams& extra);

void NATIVE_THEME_EXPORT CommonThemePaintMenuSeparator(
    SkCanvas* canvas,
    const gfx::Rect& rect,
    const NativeTheme::MenuSeparatorExtraParams& extra);

void NATIVE_THEME_EXPORT CommonThemePaintMenuGutter(SkCanvas* canvas,
                                                    const gfx::Rect& rect);

void NATIVE_THEME_EXPORT CommonThemePaintMenuBackground(SkCanvas* canvas,
                                                        const gfx::Rect& rect);

void NATIVE_THEME_EXPORT CommonThemePaintMenuItemBackground(
    SkCanvas* canvas,
    NativeTheme::State state,
    const gfx::Rect& rect);

// Adjust some colors for menu Finch experiments.
// |color_id| id of a color to be adjusted.
// |color| color before adjustment.
// Returns adjusted color.
SkColor AdjustColorForMenuVariations(NativeTheme::ColorId color_id,
                                     SkColor color);

}  // namespace ui

#endif  // UI_NATIVE_THEME_COMMON_THEME_H_
