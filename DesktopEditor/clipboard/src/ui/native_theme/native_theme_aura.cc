// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme_aura.h"

#include "base/logging.h"
#include "grit/ui_resources.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/path.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gfx/skbitmap_operations.h"
#include "ui/native_theme/common_theme.h"

namespace {

const SkColor kMenuBackgroundColor = SK_ColorWHITE;

// Theme colors returned by GetSystemColor().
const SkColor kInvalidColorIdColor = SkColorSetRGB(255, 0, 128);
// Windows:
const SkColor kWindowBackgroundColor = SK_ColorWHITE;
// Dialogs:
const SkColor kDialogBackgroundColor = SkColorSetRGB(251, 251, 251);
// FocusableBorder:
const SkColor kFocusedBorderColor = SkColorSetRGB(0x4D, 0x90, 0xFE);
const SkColor kUnfocusedBorderColor = SkColorSetRGB(0xD9, 0xD9, 0xD9);
// TextButton:
const SkColor kTextButtonBackgroundColor = SkColorSetRGB(0xDE, 0xDE, 0xDE);
const SkColor kTextButtonEnabledColor = SkColorSetRGB(0x22, 0x22, 0x22);
const SkColor kTextButtonDisabledColor = SkColorSetRGB(0x99, 0x99, 0x99);
const SkColor kTextButtonHighlightColor = SkColorSetRGB(0, 0, 0);
const SkColor kTextButtonHoverColor = kTextButtonEnabledColor;
// MenuItem:
const SkColor kEnabledMenuItemForegroundColor = kTextButtonEnabledColor;
const SkColor kDisabledMenuItemForegroundColor = kTextButtonDisabledColor;
const SkColor kFocusedMenuItemBackgroundColor = SkColorSetRGB(0xF1, 0xF1, 0xF1);
const SkColor kHoverMenuItemBackgroundColor =
    SkColorSetARGB(204, 255, 255, 255);
const SkColor kMenuSeparatorColor = SkColorSetRGB(0xED, 0xED, 0xED);
const SkColor kEnabledMenuButtonBorderColor = SkColorSetARGB(36, 0, 0, 0);
const SkColor kFocusedMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);
const SkColor kHoverMenuButtonBorderColor = SkColorSetARGB(72, 0, 0, 0);
// Label:
const SkColor kLabelEnabledColor = kTextButtonEnabledColor;
const SkColor kLabelDisabledColor = kTextButtonDisabledColor;
const SkColor kLabelBackgroundColor = SK_ColorWHITE;
// Textfield:
const SkColor kTextfieldDefaultColor = SK_ColorBLACK;
const SkColor kTextfieldDefaultBackground = SK_ColorWHITE;
const SkColor kTextfieldReadOnlyColor = SK_ColorDKGRAY;
const SkColor kTextfieldReadOnlyBackground = SK_ColorWHITE;
const SkColor kTextfieldSelectionBackgroundFocused =
    SkColorSetARGB(0x54, 0x60, 0xA8, 0xEB);
const SkColor kTextfieldSelectionBackgroundUnfocused = SK_ColorLTGRAY;
const SkColor kTextfieldSelectionColor =
    color_utils::AlphaBlend(SK_ColorBLACK,
        kTextfieldSelectionBackgroundFocused, 0xdd);
// Tree
const SkColor kTreeBackground = SK_ColorWHITE;
const SkColor kTreeTextColor = SK_ColorBLACK;
const SkColor kTreeSelectedTextColor = SK_ColorBLACK;
const SkColor kTreeSelectionBackgroundColor = SkColorSetRGB(0xEE, 0xEE, 0xEE);
const SkColor kTreeArrowColor = SkColorSetRGB(0x7A, 0x7A, 0x7A);
// Table
const SkColor kTableBackground = SK_ColorWHITE;
const SkColor kTableTextColor = SK_ColorBLACK;
const SkColor kTableSelectedTextColor = SK_ColorBLACK;
const SkColor kTableSelectionBackgroundColor = SkColorSetRGB(0xEE, 0xEE, 0xEE);
const SkColor kTableGroupingIndicatorColor = SkColorSetRGB(0xCC, 0xCC, 0xCC);

}  // namespace

namespace ui {

// static
NativeTheme* NativeTheme::instance() {
  return NativeThemeAura::instance();
}

// static
NativeThemeAura* NativeThemeAura::instance() {
  CR_DEFINE_STATIC_LOCAL(NativeThemeAura, s_native_theme, ());
  return &s_native_theme;
}

NativeThemeAura::NativeThemeAura() {
  // We don't draw scrollbar buttons.
  set_scrollbar_button_length(0);
}

NativeThemeAura::~NativeThemeAura() {
}

SkColor NativeThemeAura::GetSystemColor(ColorId color_id) const {
  // This implementation returns hardcoded colors.
  SkColor color;
  if (CommonThemeGetSystemColor(color_id, &color))
    return color;

  switch (color_id) {
    // Windows
    case kColorId_WindowBackground:
      return kWindowBackgroundColor;

    // Dialogs
    case kColorId_DialogBackground:
      return kDialogBackgroundColor;

    // FocusableBorder
    case kColorId_FocusedBorderColor:
      return kFocusedBorderColor;
    case kColorId_UnfocusedBorderColor:
      return kUnfocusedBorderColor;

    // TextButton
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

    // MenuItem
    case kColorId_EnabledMenuItemForegroundColor:
      return kEnabledMenuItemForegroundColor;
    case kColorId_DisabledMenuItemForegroundColor:
      return kDisabledMenuItemForegroundColor;
    case kColorId_SelectedMenuItemForegroundColor:
      return kEnabledMenuItemForegroundColor;
    case kColorId_FocusedMenuItemBackgroundColor:
      return kFocusedMenuItemBackgroundColor;
    case kColorId_HoverMenuItemBackgroundColor:
      return kHoverMenuItemBackgroundColor;
    case kColorId_MenuSeparatorColor:
      return kMenuSeparatorColor;
    case kColorId_EnabledMenuButtonBorderColor:
      return kEnabledMenuButtonBorderColor;
    case kColorId_FocusedMenuButtonBorderColor:
      return kFocusedMenuButtonBorderColor;
    case kColorId_HoverMenuButtonBorderColor:
      return kHoverMenuButtonBorderColor;

    // Label
    case kColorId_LabelEnabledColor:
      return kLabelEnabledColor;
    case kColorId_LabelDisabledColor:
      return kLabelDisabledColor;
    case kColorId_LabelBackgroundColor:
      return kLabelBackgroundColor;

    // Textfield
    case kColorId_TextfieldDefaultColor:
      return kTextfieldDefaultColor;
    case kColorId_TextfieldDefaultBackground:
      return kTextfieldDefaultBackground;
    case kColorId_TextfieldReadOnlyColor:
      return kTextfieldReadOnlyColor;
    case kColorId_TextfieldReadOnlyBackground:
      return kTextfieldReadOnlyBackground;
    case kColorId_TextfieldSelectionColor:
      return kTextfieldSelectionColor;
    case kColorId_TextfieldSelectionBackgroundFocused:
      return kTextfieldSelectionBackgroundFocused;
    case kColorId_TextfieldSelectionBackgroundUnfocused:
      return kTextfieldSelectionBackgroundUnfocused;

    // Tree
    case kColorId_TreeBackground:
      return kTreeBackground;
    case kColorId_TreeText:
      return kTreeTextColor;
    case kColorId_TreeSelectedText:
    case kColorId_TreeSelectedTextUnfocused:
      return kTreeSelectedTextColor;
    case kColorId_TreeSelectionBackgroundFocused:
    case kColorId_TreeSelectionBackgroundUnfocused:
      return kTreeSelectionBackgroundColor;
    case kColorId_TreeArrow:
      return kTreeArrowColor;

    // Table
    case kColorId_TableBackground:
      return kTableBackground;
    case kColorId_TableText:
      return kTableTextColor;
    case kColorId_TableSelectedText:
    case kColorId_TableSelectedTextUnfocused:
      return kTableSelectedTextColor;
    case kColorId_TableSelectionBackgroundFocused:
    case kColorId_TableSelectionBackgroundUnfocused:
      return kTableSelectionBackgroundColor;
    case kColorId_TableGroupingIndicatorColor:
      return kTableGroupingIndicatorColor;

    case kColorId_MenuBackgroundColor:
      return kMenuBackgroundColor;
    case kColorId_MenuBorderColor:
      NOTREACHED();
      break;
  }

  return kInvalidColorIdColor;
}

void NativeThemeAura::PaintMenuPopupBackground(
    SkCanvas* canvas,
    const gfx::Size& size,
    const MenuBackgroundExtraParams& menu_background) const {
  if (menu_background.corner_radius > 0) {
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setFlags(SkPaint::kAntiAlias_Flag);
    paint.setColor(kMenuBackgroundColor);

    gfx::Path path;
    SkRect rect = SkRect::MakeWH(SkIntToScalar(size.width()),
                                 SkIntToScalar(size.height()));
    SkScalar radius = SkIntToScalar(menu_background.corner_radius);
    SkScalar radii[8] = {radius, radius, radius, radius,
                         radius, radius, radius, radius};
    path.addRoundRect(rect, radii);

    canvas->drawPath(path, paint);
  } else {
    canvas->drawColor(kMenuBackgroundColor, SkXfermode::kSrc_Mode);
  }
}

void NativeThemeAura::PaintScrollbarTrack(
    SkCanvas* canvas,
    Part part,
    State state,
    const ScrollbarTrackExtraParams& extra_params,
    const gfx::Rect& rect) const {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  if (part == kScrollbarVerticalTrack) {
    int center_offset = 0;
    int center_height = rect.height();

    if (rect.y() == extra_params.track_y) {
      // TODO(derat): Honor |state| instead of only using the highlighted images
      // after updating WebKit so we can draw the entire track in one go instead
      // of as two separate pieces: otherwise, only the portion of the scrollbar
      // that the mouse is over gets the highlighted state.
      gfx::ImageSkia* top = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_VERTICAL_TOP_H);
      DrawTiledImage(canvas, *top,
                     0, 0, 1.0, 1.0,
                     rect.x(), rect.y(), top->width(), top->height());
      center_offset += top->height();
      center_height -= top->height();
    }

    if (rect.y() + rect.height() ==
        extra_params.track_y + extra_params.track_height) {
      gfx::ImageSkia* bottom = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_VERTICAL_BOTTOM_H);
      DrawTiledImage(canvas, *bottom,
                     0, 0, 1.0, 1.0,
                     rect.x(), rect.y() + rect.height() - bottom->height(),
                     bottom->width(), bottom->height());
      center_height -= bottom->height();
    }

    if (center_height > 0) {
      gfx::ImageSkia* center = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_VERTICAL_CENTER_H);
      DrawTiledImage(canvas, *center,
                     0, 0, 1.0, 1.0,
                     rect.x(), rect.y() + center_offset,
                     center->width(), center_height);
    }
  } else {
    int center_offset = 0;
    int center_width = rect.width();

    if (rect.x() == extra_params.track_x) {
      gfx::ImageSkia* left = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_HORIZONTAL_LEFT_H);
      DrawTiledImage(canvas, *left,
                     0, 0, 1.0, 1.0,
                     rect.x(), rect.y(), left->width(), left->height());
      center_offset += left->width();
      center_width -= left->width();
    }

    if (rect.x() + rect.width() ==
        extra_params.track_x + extra_params.track_width) {
      gfx::ImageSkia* right = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_HORIZONTAL_RIGHT_H);
      DrawTiledImage(canvas, *right,
                     0, 0, 1.0, 1.0,
                     rect.x() + rect.width() - right->width(), rect.y(),
                     right->width(), right->height());
      center_width -= right->width();
    }

    if (center_width > 0) {
      gfx::ImageSkia* center = rb.GetImageSkiaNamed(
          IDR_SCROLL_BASE_HORIZONTAL_CENTER_H);
      DrawTiledImage(canvas, *center,
                     0, 0, 1.0, 1.0,
                     rect.x() + center_offset, rect.y(),
                     center_width, center->height());
    }
  }
}

void NativeThemeAura::PaintScrollbarThumb(SkCanvas* canvas,
                                          Part part,
                                          State state,
                                          const gfx::Rect& rect) const {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  if (part == kScrollbarVerticalThumb) {
    int top_resource_id =
        state == kHovered ? IDR_SCROLL_THUMB_VERTICAL_TOP_H :
        state == kPressed ? IDR_SCROLL_THUMB_VERTICAL_TOP_P :
        IDR_SCROLL_THUMB_VERTICAL_TOP;
    gfx::ImageSkia* top = rb.GetImageSkiaNamed(top_resource_id);
    DrawTiledImage(canvas, *top,
                   0, 0, 1.0, 1.0,
                   rect.x(), rect.y(), top->width(), top->height());

    int bottom_resource_id =
        state == kHovered ? IDR_SCROLL_THUMB_VERTICAL_BOTTOM_H :
        state == kPressed ? IDR_SCROLL_THUMB_VERTICAL_BOTTOM_P :
        IDR_SCROLL_THUMB_VERTICAL_BOTTOM;
    gfx::ImageSkia* bottom = rb.GetImageSkiaNamed(bottom_resource_id);
    DrawTiledImage(canvas, *bottom,
                   0, 0, 1.0, 1.0,
                   rect.x(), rect.y() + rect.height() - bottom->height(),
                   bottom->width(), bottom->height());

    if (rect.height() > top->height() + bottom->height()) {
      int center_resource_id =
          state == kHovered ? IDR_SCROLL_THUMB_VERTICAL_CENTER_H :
          state == kPressed ? IDR_SCROLL_THUMB_VERTICAL_CENTER_P :
          IDR_SCROLL_THUMB_VERTICAL_CENTER;
      gfx::ImageSkia* center = rb.GetImageSkiaNamed(center_resource_id);
      DrawTiledImage(canvas, *center,
                     0, 0, 1.0, 1.0,
                     rect.x(), rect.y() + top->height(),
                     center->width(),
                     rect.height() - top->height() - bottom->height());
    }
  } else {
    int left_resource_id =
        state == kHovered ? IDR_SCROLL_THUMB_HORIZONTAL_LEFT_H :
        state == kPressed ? IDR_SCROLL_THUMB_HORIZONTAL_LEFT_P :
        IDR_SCROLL_THUMB_HORIZONTAL_LEFT;
    gfx::ImageSkia* left = rb.GetImageSkiaNamed(left_resource_id);
    DrawTiledImage(canvas, *left,
                   0, 0, 1.0, 1.0,
                   rect.x(), rect.y(), left->width(), left->height());

    int right_resource_id =
        state == kHovered ? IDR_SCROLL_THUMB_HORIZONTAL_RIGHT_H :
        state == kPressed ? IDR_SCROLL_THUMB_HORIZONTAL_RIGHT_P :
        IDR_SCROLL_THUMB_HORIZONTAL_RIGHT;
    gfx::ImageSkia* right = rb.GetImageSkiaNamed(right_resource_id);
    DrawTiledImage(canvas, *right,
                   0, 0, 1.0, 1.0,
                   rect.x() + rect.width() - right->width(), rect.y(),
                   right->width(), right->height());

    if (rect.width() > left->width() + right->width()) {
      int center_resource_id =
          state == kHovered ? IDR_SCROLL_THUMB_HORIZONTAL_CENTER_H :
          state == kPressed ? IDR_SCROLL_THUMB_HORIZONTAL_CENTER_P :
          IDR_SCROLL_THUMB_HORIZONTAL_CENTER;
      gfx::ImageSkia* center = rb.GetImageSkiaNamed(center_resource_id);
      DrawTiledImage(canvas, *center,
                     0, 0, 1.0, 1.0,
                     rect.x() + left->width(), rect.y(),
                     rect.width() - left->width() - right->width(),
                     center->height());
    }
  }
}

}  // namespace ui
