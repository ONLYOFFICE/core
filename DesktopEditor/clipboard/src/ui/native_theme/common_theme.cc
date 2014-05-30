// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/common_theme.h"

#include "base/logging.h"
#include "grit/ui_resources.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/sys_color_change_listener.h"
#include "ui/views/controls/menu/menu_config.h"

namespace {

// Theme colors returned by GetSystemColor().

// MenuItem:
const SkColor kMenuBackgroundColor = SK_ColorWHITE;
const SkColor kMenuHighlightBackgroundColor = SkColorSetA(SK_ColorBLACK, 15);
const SkColor kMenuHighlightBackgroundColor2 = SkColorSetRGB(0x42, 0x81, 0xF4);
const SkColor kMenuInvertedSchemeHighlightBackgroundColor =
    SkColorSetRGB(48, 48, 48);
const SkColor kMenuBorderColor = SkColorSetRGB(0xBA, 0xBA, 0xBA);
const SkColor kEnabledMenuButtonBorderColor = SkColorSetARGB(36, 0, 0, 0);
const SkColor kFocusedMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);
const SkColor kHoverMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);
const SkColor kMenuSeparatorColor = SkColorSetRGB(0xE9, 0xE9, 0xE9);
const SkColor kEnabledMenuItemForegroundColor = SK_ColorBLACK;
const SkColor kDisabledMenuItemForegroundColor = SkColorSetRGB(161, 161, 146);
const SkColor kHoverMenuItemBackgroundColor =
    SkColorSetARGB(204, 255, 255, 255);

}  // namespace

namespace ui {

bool CommonThemeGetSystemColor(NativeTheme::ColorId color_id, SkColor* color) {
  switch (color_id) {
    // MenuItem
    case NativeTheme::kColorId_MenuBorderColor:
      *color = kMenuBorderColor;
      break;
    case NativeTheme::kColorId_EnabledMenuButtonBorderColor:
      *color = kEnabledMenuButtonBorderColor;
      break;
    case NativeTheme::kColorId_FocusedMenuButtonBorderColor:
      *color = kFocusedMenuButtonBorderColor;
      break;
    case NativeTheme::kColorId_HoverMenuButtonBorderColor:
      *color = kHoverMenuButtonBorderColor;
      break;
    case NativeTheme::kColorId_MenuSeparatorColor:
      *color = kMenuSeparatorColor;
      break;
    case NativeTheme::kColorId_MenuBackgroundColor:
      *color = kMenuBackgroundColor;
      break;
    case NativeTheme::kColorId_FocusedMenuItemBackgroundColor:
      *color = kMenuHighlightBackgroundColor;
      break;
    case NativeTheme::kColorId_HoverMenuItemBackgroundColor:
      *color = kHoverMenuItemBackgroundColor;
      break;
    case NativeTheme::kColorId_EnabledMenuItemForegroundColor:
      *color = kEnabledMenuItemForegroundColor;
      break;
    case NativeTheme::kColorId_DisabledMenuItemForegroundColor:
      *color = kDisabledMenuItemForegroundColor;
      break;
    case NativeTheme::kColorId_SelectedMenuItemForegroundColor:
      *color = kEnabledMenuItemForegroundColor;
      break;
    case NativeTheme::kColorId_TextButtonDisabledColor:
      *color = kDisabledMenuItemForegroundColor;
      break;
    default:
      return false;
  }

  *color = AdjustColorForMenuVariations(color_id, *color);

  if (gfx::IsInvertedColorScheme()) {
    switch (color_id) {
      case NativeTheme::kColorId_FocusedMenuItemBackgroundColor:
        *color = kMenuInvertedSchemeHighlightBackgroundColor;
        break;
      default:
        *color = color_utils::InvertColor(*color);
    }
  }
  return true;
}

gfx::Size CommonThemeGetPartSize(NativeTheme::Part part,
                                 NativeTheme::State state,
                                 const NativeTheme::ExtraParams& extra) {
  gfx::Size size;
  switch (part) {
    case NativeTheme::kMenuCheck: {
      const gfx::ImageSkia* check =
          ui::ResourceBundle::GetSharedInstance().GetImageNamed(
              IDR_MENU_CHECK_CHECKED).ToImageSkia();
      size.SetSize(check->width(), check->height());
      break;
    }
    default:
      break;
  }

  return size;
}

void CommonThemePaintMenuSeparator(
    SkCanvas* canvas,
    const gfx::Rect& rect,
    const NativeTheme::MenuSeparatorExtraParams& extra) {
  SkColor color;
  CommonThemeGetSystemColor(NativeTheme::kColorId_MenuSeparatorColor, &color);
  SkPaint paint;
  paint.setColor(kMenuSeparatorColor);
  int position_y = rect.y() + rect.height() / 2;
  canvas->drawLine(rect.x(), position_y, rect.right(), position_y, paint);
}

void CommonThemePaintMenuGutter(SkCanvas* canvas, const gfx::Rect& rect) {
  SkColor color;
  CommonThemeGetSystemColor(NativeTheme::kColorId_MenuSeparatorColor, &color);
  SkPaint paint;
  paint.setColor(kMenuSeparatorColor);
  int position_x = rect.x() + rect.width() / 2;
  canvas->drawLine(position_x, rect.y(), position_x, rect.bottom(), paint);
}

void CommonThemePaintMenuBackground(SkCanvas* canvas, const gfx::Rect& rect) {
  SkColor color;
  CommonThemeGetSystemColor(NativeTheme::kColorId_MenuBackgroundColor, &color);
  SkPaint paint;
  paint.setColor(color);
  canvas->drawRect(gfx::RectToSkRect(rect), paint);
}

void CommonThemePaintMenuItemBackground(SkCanvas* canvas,
                                        NativeTheme::State state,
                                        const gfx::Rect& rect) {
  SkColor color;
  SkPaint paint;
  switch (state) {
    case NativeTheme::kNormal:
    case NativeTheme::kDisabled:
      CommonThemeGetSystemColor(NativeTheme::kColorId_MenuBackgroundColor,
                                &color);
      paint.setColor(color);
      break;
    case NativeTheme::kHovered:
      CommonThemeGetSystemColor(
          NativeTheme::kColorId_FocusedMenuItemBackgroundColor, &color);
      paint.setColor(color);
      break;
    default:
      NOTREACHED() << "Invalid state " << state;
      break;
  }
  canvas->drawRect(gfx::RectToSkRect(rect), paint);
}

SkColor AdjustColorForMenuVariations(NativeTheme::ColorId color_id,
                                     SkColor color) {
  if (NativeTheme::GetMenuVariation() != NativeTheme::MENU_VARIATION_CONTRAST)
    return color;

  switch(color_id) {
    case NativeTheme::kColorId_FocusedMenuItemBackgroundColor:
      return kMenuHighlightBackgroundColor2;
    case NativeTheme::kColorId_SelectedMenuItemForegroundColor:
      return SK_ColorWHITE;
    default:
      return color;
  }
}

}  // namespace ui
