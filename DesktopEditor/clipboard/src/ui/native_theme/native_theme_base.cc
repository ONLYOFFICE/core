// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme_base.h"

#include <limits>

#include "base/command_line.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "grit/ui_resources.h"
#include "third_party/skia/include/effects/SkGradientShader.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gfx/skia_util.h"

namespace {

// These are the default dimensions of radio buttons and checkboxes.
const int kCheckboxAndRadioWidth = 13;
const int kCheckboxAndRadioHeight = 13;

// These sizes match the sizes in Chromium Win.
const int kSliderThumbWidth = 11;
const int kSliderThumbHeight = 21;

const SkColor kSliderTrackBackgroundColor =
    SkColorSetRGB(0xe3, 0xdd, 0xd8);
const SkColor kSliderThumbLightGrey = SkColorSetRGB(0xf4, 0xf2, 0xef);
const SkColor kSliderThumbDarkGrey = SkColorSetRGB(0xea, 0xe5, 0xe0);
const SkColor kSliderThumbBorderDarkGrey =
    SkColorSetRGB(0x9d, 0x96, 0x8e);

const SkColor kMenuPopupBackgroundColor = SkColorSetRGB(210, 225, 246);

const unsigned int kDefaultScrollbarWidth = 15;
const unsigned int kDefaultScrollbarButtonLength = 14;

const SkColor kCheckboxTinyColor = SK_ColorGRAY;
const SkColor kCheckboxShadowColor = SkColorSetARGB(0x15, 0, 0, 0);
const SkColor kCheckboxShadowHoveredColor = SkColorSetARGB(0x1F, 0, 0, 0);
const SkColor kCheckboxShadowDisabledColor = SkColorSetARGB(0, 0, 0, 0);
const SkColor kCheckboxGradientColors[] = {
    SkColorSetRGB(0xed, 0xed, 0xed),
    SkColorSetRGB(0xde, 0xde, 0xde) };
const SkColor kCheckboxGradientPressedColors[] = {
    SkColorSetRGB(0xe7, 0xe7, 0xe7),
    SkColorSetRGB(0xd7, 0xd7, 0xd7) };
const SkColor kCheckboxGradientHoveredColors[] = {
    SkColorSetRGB(0xf0, 0xf0, 0xf0),
    SkColorSetRGB(0xe0, 0xe0, 0xe0) };
const SkColor kCheckboxGradientDisabledColors[] = {
    SkColorSetARGB(0x80, 0xed, 0xed, 0xed),
    SkColorSetARGB(0x80, 0xde, 0xde, 0xde) };
const SkColor kCheckboxBorderColor = SkColorSetARGB(0x40, 0, 0, 0);
const SkColor kCheckboxBorderHoveredColor = SkColorSetARGB(0x4D, 0, 0, 0);
const SkColor kCheckboxBorderDisabledColor = SkColorSetARGB(0x20, 0, 0, 0);
const SkColor kCheckboxStrokeColor = SkColorSetARGB(0xB3, 0, 0, 0);
const SkColor kCheckboxStrokeDisabledColor = SkColorSetARGB(0x59, 0, 0, 0);
const SkColor kRadioDotColor = SkColorSetRGB(0x66, 0x66, 0x66);
const SkColor kRadioDotDisabledColor = SkColorSetARGB(0x80, 0x66, 0x66, 0x66);

// Get lightness adjusted color.
SkColor BrightenColor(const color_utils::HSL& hsl, SkAlpha alpha,
    double lightness_amount) {
  color_utils::HSL adjusted = hsl;
  adjusted.l += lightness_amount;
  if (adjusted.l > 1.0)
    adjusted.l = 1.0;
  if (adjusted.l < 0.0)
    adjusted.l = 0.0;

  return color_utils::HSLToSkColor(adjusted, alpha);
}

}  // namespace

namespace ui {

gfx::Size NativeThemeBase::GetPartSize(Part part,
                                       State state,
                                       const ExtraParams& extra) const {
  switch (part) {
    // Please keep these in the order of NativeTheme::Part.
    case kCheckbox:
      return gfx::Size(kCheckboxAndRadioWidth, kCheckboxAndRadioHeight);
    case kInnerSpinButton:
      return gfx::Size(scrollbar_width_, 0);
    case kMenuList:
      return gfx::Size();  // No default size.
    case kMenuCheck:
    case kMenuCheckBackground:
    case kMenuPopupArrow:
      NOTIMPLEMENTED();
      break;
    case kMenuPopupBackground:
      return gfx::Size();  // No default size.
    case kMenuPopupGutter:
    case kMenuPopupSeparator:
      NOTIMPLEMENTED();
      break;
    case kMenuItemBackground:
    case kProgressBar:
    case kPushButton:
      return gfx::Size();  // No default size.
    case kRadio:
      return gfx::Size(kCheckboxAndRadioWidth, kCheckboxAndRadioHeight);
    case kScrollbarDownArrow:
    case kScrollbarUpArrow:
      return gfx::Size(scrollbar_width_, scrollbar_button_length_);
    case kScrollbarLeftArrow:
    case kScrollbarRightArrow:
      return gfx::Size(scrollbar_button_length_, scrollbar_width_);
    case kScrollbarHorizontalThumb:
      // This matches Firefox on Linux.
      return gfx::Size(2 * scrollbar_width_, scrollbar_width_);
    case kScrollbarVerticalThumb:
      // This matches Firefox on Linux.
      return gfx::Size(scrollbar_width_, 2 * scrollbar_width_);
    case kScrollbarHorizontalTrack:
      return gfx::Size(0, scrollbar_width_);
    case kScrollbarVerticalTrack:
      return gfx::Size(scrollbar_width_, 0);
    case kScrollbarHorizontalGripper:
    case kScrollbarVerticalGripper:
      NOTIMPLEMENTED();
      break;
    case kSliderTrack:
      return gfx::Size();  // No default size.
    case kSliderThumb:
      // These sizes match the sizes in Chromium Win.
      return gfx::Size(kSliderThumbWidth, kSliderThumbHeight);
    case kTabPanelBackground:
      NOTIMPLEMENTED();
      break;
    case kTextField:
      return gfx::Size();  // No default size.
    case kTrackbarThumb:
    case kTrackbarTrack:
    case kWindowResizeGripper:
      NOTIMPLEMENTED();
      break;
    default:
      NOTREACHED() << "Unknown theme part: " << part;
      break;
  }
  return gfx::Size();
}

void NativeThemeBase::Paint(SkCanvas* canvas,
                            Part part,
                            State state,
                            const gfx::Rect& rect,
                            const ExtraParams& extra) const {
  if (rect.IsEmpty())
    return;

  switch (part) {
    // Please keep these in the order of NativeTheme::Part.
    case kCheckbox:
      PaintCheckbox(canvas, state, rect, extra.button);
      break;
    case kInnerSpinButton:
      PaintInnerSpinButton(canvas, state, rect, extra.inner_spin);
      break;
    case kMenuList:
      PaintMenuList(canvas, state, rect, extra.menu_list);
      break;
    case kMenuCheck:
    case kMenuCheckBackground:
    case kMenuPopupArrow:
      NOTIMPLEMENTED();
      break;
    case kMenuPopupBackground:
      PaintMenuPopupBackground(canvas, rect.size(), extra.menu_background);
      break;
    case kMenuPopupGutter:
    case kMenuPopupSeparator:
      NOTIMPLEMENTED();
      break;
    case kMenuItemBackground:
      PaintMenuItemBackground(canvas, state, rect, extra.menu_list);
      break;
    case kProgressBar:
      PaintProgressBar(canvas, state, rect, extra.progress_bar);
      break;
    case kPushButton:
      PaintButton(canvas, state, rect, extra.button);
      break;
    case kRadio:
      PaintRadio(canvas, state, rect, extra.button);
      break;
    case kScrollbarDownArrow:
    case kScrollbarUpArrow:
    case kScrollbarLeftArrow:
    case kScrollbarRightArrow:
      PaintArrowButton(canvas, rect, part, state);
      break;
    case kScrollbarHorizontalThumb:
    case kScrollbarVerticalThumb:
      PaintScrollbarThumb(canvas, part, state, rect);
      break;
    case kScrollbarHorizontalTrack:
    case kScrollbarVerticalTrack:
      PaintScrollbarTrack(canvas, part, state, extra.scrollbar_track, rect);
      break;
    case kScrollbarHorizontalGripper:
    case kScrollbarVerticalGripper:
      // Invoked by views scrollbar code, don't care about for non-win
      // implementations, so no NOTIMPLEMENTED.
      break;
    case kSliderTrack:
      PaintSliderTrack(canvas, state, rect, extra.slider);
      break;
    case kSliderThumb:
      PaintSliderThumb(canvas, state, rect, extra.slider);
      break;
    case kTabPanelBackground:
      NOTIMPLEMENTED();
      break;
    case kTextField:
      PaintTextField(canvas, state, rect, extra.text_field);
      break;
    case kTrackbarThumb:
    case kTrackbarTrack:
    case kWindowResizeGripper:
      NOTIMPLEMENTED();
      break;
    default:
      NOTREACHED() << "Unknown theme part: " << part;
      break;
  }
}

NativeThemeBase::NativeThemeBase()
    : scrollbar_width_(kDefaultScrollbarWidth),
      scrollbar_button_length_(kDefaultScrollbarButtonLength) {
}

NativeThemeBase::~NativeThemeBase() {
}

void NativeThemeBase::PaintArrowButton(
    SkCanvas* canvas,
    const gfx::Rect& rect, Part direction, State state) const {
  int widthMiddle, lengthMiddle;
  SkPaint paint;
  if (direction == kScrollbarUpArrow || direction == kScrollbarDownArrow) {
    widthMiddle = rect.width() / 2 + 1;
    lengthMiddle = rect.height() / 2 + 1;
  } else {
    lengthMiddle = rect.width() / 2 + 1;
    widthMiddle = rect.height() / 2 + 1;
  }

  // Calculate button color.
  SkScalar trackHSV[3];
  SkColorToHSV(track_color_, trackHSV);
  SkColor buttonColor = SaturateAndBrighten(trackHSV, 0, 0.2f);
  SkColor backgroundColor = buttonColor;
  if (state == kPressed) {
    SkScalar buttonHSV[3];
    SkColorToHSV(buttonColor, buttonHSV);
    buttonColor = SaturateAndBrighten(buttonHSV, 0, -0.1f);
  } else if (state == kHovered) {
    SkScalar buttonHSV[3];
    SkColorToHSV(buttonColor, buttonHSV);
    buttonColor = SaturateAndBrighten(buttonHSV, 0, 0.05f);
  }

  SkIRect skrect;
  skrect.set(rect.x(), rect.y(), rect.x() + rect.width(), rect.y()
      + rect.height());
  // Paint the background (the area visible behind the rounded corners).
  paint.setColor(backgroundColor);
  canvas->drawIRect(skrect, paint);

  // Paint the button's outline and fill the middle
  SkPath outline;
  switch (direction) {
    case kScrollbarUpArrow:
      outline.moveTo(rect.x() + 0.5, rect.y() + rect.height() + 0.5);
      outline.rLineTo(0, -(rect.height() - 2));
      outline.rLineTo(2, -2);
      outline.rLineTo(rect.width() - 5, 0);
      outline.rLineTo(2, 2);
      outline.rLineTo(0, rect.height() - 2);
      break;
    case kScrollbarDownArrow:
      outline.moveTo(rect.x() + 0.5, rect.y() - 0.5);
      outline.rLineTo(0, rect.height() - 2);
      outline.rLineTo(2, 2);
      outline.rLineTo(rect.width() - 5, 0);
      outline.rLineTo(2, -2);
      outline.rLineTo(0, -(rect.height() - 2));
      break;
    case kScrollbarRightArrow:
      outline.moveTo(rect.x() - 0.5, rect.y() + 0.5);
      outline.rLineTo(rect.width() - 2, 0);
      outline.rLineTo(2, 2);
      outline.rLineTo(0, rect.height() - 5);
      outline.rLineTo(-2, 2);
      outline.rLineTo(-(rect.width() - 2), 0);
      break;
    case kScrollbarLeftArrow:
      outline.moveTo(rect.x() + rect.width() + 0.5, rect.y() + 0.5);
      outline.rLineTo(-(rect.width() - 2), 0);
      outline.rLineTo(-2, 2);
      outline.rLineTo(0, rect.height() - 5);
      outline.rLineTo(2, 2);
      outline.rLineTo(rect.width() - 2, 0);
      break;
    default:
      break;
  }
  outline.close();

  paint.setStyle(SkPaint::kFill_Style);
  paint.setColor(buttonColor);
  canvas->drawPath(outline, paint);

  paint.setAntiAlias(true);
  paint.setStyle(SkPaint::kStroke_Style);
  SkScalar thumbHSV[3];
  SkColorToHSV(thumb_inactive_color_, thumbHSV);
  paint.setColor(OutlineColor(trackHSV, thumbHSV));
  canvas->drawPath(outline, paint);

  // If the button is disabled or read-only, the arrow is drawn with the
  // outline color.
  if (state != kDisabled)
    paint.setColor(SK_ColorBLACK);

  paint.setAntiAlias(false);
  paint.setStyle(SkPaint::kFill_Style);

  SkPath path;
  // The constants in this block of code are hand-tailored to produce good
  // looking arrows without anti-aliasing.
  switch (direction) {
    case kScrollbarUpArrow:
      path.moveTo(rect.x() + widthMiddle - 4, rect.y() + lengthMiddle + 2);
      path.rLineTo(7, 0);
      path.rLineTo(-4, -4);
      break;
    case kScrollbarDownArrow:
      path.moveTo(rect.x() + widthMiddle - 4, rect.y() + lengthMiddle - 3);
      path.rLineTo(7, 0);
      path.rLineTo(-4, 4);
      break;
    case kScrollbarRightArrow:
      path.moveTo(rect.x() + lengthMiddle - 3, rect.y() + widthMiddle - 4);
      path.rLineTo(0, 7);
      path.rLineTo(4, -4);
      break;
    case kScrollbarLeftArrow:
      path.moveTo(rect.x() + lengthMiddle + 1, rect.y() + widthMiddle - 5);
      path.rLineTo(0, 9);
      path.rLineTo(-4, -4);
      break;
    default:
      break;
  }
  path.close();

  canvas->drawPath(path, paint);
}

void NativeThemeBase::PaintScrollbarTrack(SkCanvas* canvas,
    Part part,
    State state,
    const ScrollbarTrackExtraParams& extra_params,
    const gfx::Rect& rect) const {
  SkPaint paint;
  SkIRect skrect;

  skrect.set(rect.x(), rect.y(), rect.right(), rect.bottom());
  SkScalar track_hsv[3];
  SkColorToHSV(track_color_, track_hsv);
  paint.setColor(SaturateAndBrighten(track_hsv, 0, 0));
  canvas->drawIRect(skrect, paint);

  SkScalar thumb_hsv[3];
  SkColorToHSV(thumb_inactive_color_, thumb_hsv);

  paint.setColor(OutlineColor(track_hsv, thumb_hsv));
  DrawBox(canvas, rect, paint);
}

void NativeThemeBase::PaintScrollbarThumb(SkCanvas* canvas,
                                           Part part,
                                           State state,
                                           const gfx::Rect& rect) const {
  const bool hovered = state == kHovered;
  const int midx = rect.x() + rect.width() / 2;
  const int midy = rect.y() + rect.height() / 2;
  const bool vertical = part == kScrollbarVerticalThumb;

  SkScalar thumb[3];
  SkColorToHSV(hovered ? thumb_active_color_ : thumb_inactive_color_, thumb);

  SkPaint paint;
  paint.setColor(SaturateAndBrighten(thumb, 0, 0.02f));

  SkIRect skrect;
  if (vertical)
    skrect.set(rect.x(), rect.y(), midx + 1, rect.y() + rect.height());
  else
    skrect.set(rect.x(), rect.y(), rect.x() + rect.width(), midy + 1);

  canvas->drawIRect(skrect, paint);

  paint.setColor(SaturateAndBrighten(thumb, 0, -0.02f));

  if (vertical) {
    skrect.set(
        midx + 1, rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
  } else {
    skrect.set(
        rect.x(), midy + 1, rect.x() + rect.width(), rect.y() + rect.height());
  }

  canvas->drawIRect(skrect, paint);

  SkScalar track[3];
  SkColorToHSV(track_color_, track);
  paint.setColor(OutlineColor(track, thumb));
  DrawBox(canvas, rect, paint);

  if (rect.height() > 10 && rect.width() > 10) {
    const int grippy_half_width = 2;
    const int inter_grippy_offset = 3;
    if (vertical) {
      DrawHorizLine(canvas,
                    midx - grippy_half_width,
                    midx + grippy_half_width,
                    midy - inter_grippy_offset,
                    paint);
      DrawHorizLine(canvas,
                    midx - grippy_half_width,
                    midx + grippy_half_width,
                    midy,
                    paint);
      DrawHorizLine(canvas,
                    midx - grippy_half_width,
                    midx + grippy_half_width,
                    midy + inter_grippy_offset,
                    paint);
    } else {
      DrawVertLine(canvas,
                   midx - inter_grippy_offset,
                   midy - grippy_half_width,
                   midy + grippy_half_width,
                   paint);
      DrawVertLine(canvas,
                   midx,
                   midy - grippy_half_width,
                   midy + grippy_half_width,
                   paint);
      DrawVertLine(canvas,
                   midx + inter_grippy_offset,
                   midy - grippy_half_width,
                   midy + grippy_half_width,
                   paint);
    }
  }
}

void NativeThemeBase::PaintCheckbox(SkCanvas* canvas,
                                    State state,
                                    const gfx::Rect& rect,
                                    const ButtonExtraParams& button) const {
  SkRect skrect = PaintCheckboxRadioCommon(canvas, state, rect,
                                           SkIntToScalar(2));
  if (!skrect.isEmpty()) {
    // Draw the checkmark / dash.
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    if (state == kDisabled)
      paint.setColor(kCheckboxStrokeDisabledColor);
    else
      paint.setColor(kCheckboxStrokeColor);
    if (button.indeterminate) {
      SkPath dash;
      dash.moveTo(skrect.x() + skrect.width() * 0.16,
                  (skrect.y() + skrect.bottom()) / 2);
      dash.rLineTo(skrect.width() * 0.68, 0);
      paint.setStrokeWidth(SkFloatToScalar(skrect.height() * 0.2));
      canvas->drawPath(dash, paint);
    } else if (button.checked) {
      SkPath check;
      check.moveTo(skrect.x() + skrect.width() * 0.2,
                   skrect.y() + skrect.height() * 0.5);
      check.rLineTo(skrect.width() * 0.2, skrect.height() * 0.2);
      paint.setStrokeWidth(SkFloatToScalar(skrect.height() * 0.23));
      check.lineTo(skrect.right() - skrect.width() * 0.2,
                   skrect.y() + skrect.height() * 0.2);
      canvas->drawPath(check, paint);
    }
  }
}

// Draws the common elements of checkboxes and radio buttons.
// Returns the rectangle within which any additional decorations should be
// drawn, or empty if none.
SkRect NativeThemeBase::PaintCheckboxRadioCommon(
    SkCanvas* canvas,
    State state,
    const gfx::Rect& rect,
    const SkScalar borderRadius) const {

  SkRect skrect = gfx::RectToSkRect(rect);

  // Use the largest square that fits inside the provided rectangle.
  // No other browser seems to support non-square widget, so accidentally
  // having non-square sizes is common (eg. amazon and webkit dev tools).
  if (skrect.width() != skrect.height()) {
    SkScalar size = SkMinScalar(skrect.width(), skrect.height());
    skrect.inset((skrect.width() - size) / 2, (skrect.height() - size) / 2);
  }

  // If the rectangle is too small then paint only a rectangle.  We don't want
  // to have to worry about '- 1' and '+ 1' calculations below having overflow
  // or underflow.
  if (skrect.width() <= 2) {
    SkPaint paint;
    paint.setColor(kCheckboxTinyColor);
    paint.setStyle(SkPaint::kFill_Style);
    canvas->drawRect(skrect, paint);
    // Too small to draw anything more.
    return SkRect::MakeEmpty();
  }

  // Make room for the drop shadow.
  skrect.iset(skrect.x(), skrect.y(), skrect.right() - 1, skrect.bottom() - 1);

  // Draw the drop shadow below the widget.
  if (state != kPressed) {
    SkPaint paint;
    paint.setAntiAlias(true);
    SkRect shadowRect = skrect;
    shadowRect.offset(0, 1);
    if (state == kDisabled)
     paint.setColor(kCheckboxShadowDisabledColor);
    else if (state == kHovered)
      paint.setColor(kCheckboxShadowHoveredColor);
    else
      paint.setColor(kCheckboxShadowColor);
    paint.setStyle(SkPaint::kFill_Style);
    canvas->drawRoundRect(shadowRect, borderRadius, borderRadius, paint);
  }

  // Draw the gradient-filled rectangle
  SkPoint gradient_bounds[3];
  gradient_bounds[0].set(skrect.x(), skrect.y());
  gradient_bounds[1].set(skrect.x(), skrect.y() + skrect.height() * 0.38);
  gradient_bounds[2].set(skrect.x(), skrect.bottom());
  const SkColor* startEndColors;
  if (state == kPressed)
    startEndColors = kCheckboxGradientPressedColors;
  else if (state == kHovered)
    startEndColors = kCheckboxGradientHoveredColors;
  else if (state == kDisabled)
    startEndColors = kCheckboxGradientDisabledColors;
  else /* kNormal */
    startEndColors = kCheckboxGradientColors;
  SkColor colors[3] = {startEndColors[0], startEndColors[0], startEndColors[1]};
  skia::RefPtr<SkShader> shader = skia::AdoptRef(
      SkGradientShader::CreateLinear(
          gradient_bounds, colors, NULL, 3, SkShader::kClamp_TileMode, NULL));
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setShader(shader.get());
  paint.setStyle(SkPaint::kFill_Style);
  canvas->drawRoundRect(skrect, borderRadius, borderRadius, paint);
  paint.setShader(NULL);

  // Draw the border.
  if (state == kHovered)
    paint.setColor(kCheckboxBorderHoveredColor);
  else if (state == kDisabled)
    paint.setColor(kCheckboxBorderDisabledColor);
  else
    paint.setColor(kCheckboxBorderColor);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(SkIntToScalar(1));
  skrect.inset(SkFloatToScalar(.5f), SkFloatToScalar(.5f));
  canvas->drawRoundRect(skrect, borderRadius, borderRadius, paint);

  // Return the rectangle excluding the drop shadow for drawing any additional
  // decorations.
  return skrect;
}

void NativeThemeBase::PaintRadio(SkCanvas* canvas,
                                  State state,
                                  const gfx::Rect& rect,
                                  const ButtonExtraParams& button) const {

  // Most of a radio button is the same as a checkbox, except the the rounded
  // square is a circle (i.e. border radius >= 100%).
  const SkScalar radius = SkFloatToScalar(
      static_cast<float>(std::max(rect.width(), rect.height())) / 2);
  SkRect skrect = PaintCheckboxRadioCommon(canvas, state, rect, radius);
  if (!skrect.isEmpty() && button.checked) {
    // Draw the dot.
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    if (state == kDisabled)
      paint.setColor(kRadioDotDisabledColor);
    else
      paint.setColor(kRadioDotColor);
    skrect.inset(skrect.width() * 0.25, skrect.height() * 0.25);
    // Use drawRoundedRect instead of drawOval to be completely consistent
    // with the border in PaintCheckboxRadioNewCommon.
    canvas->drawRoundRect(skrect, radius, radius, paint);
  }
}

void NativeThemeBase::PaintButton(SkCanvas* canvas,
                                  State state,
                                  const gfx::Rect& rect,
                                  const ButtonExtraParams& button) const {
  SkPaint paint;
  const int kRight = rect.right();
  const int kBottom = rect.bottom();
  SkRect skrect = SkRect::MakeLTRB(rect.x(), rect.y(), kRight, kBottom);
  SkColor base_color = button.background_color;

  color_utils::HSL base_hsl;
  color_utils::SkColorToHSL(base_color, &base_hsl);

  // Our standard gradient is from 0xdd to 0xf8. This is the amount of
  // increased luminance between those values.
  SkColor light_color(BrightenColor(base_hsl, SkColorGetA(base_color), 0.105));

  // If the button is too small, fallback to drawing a single, solid color
  if (rect.width() < 5 || rect.height() < 5) {
    paint.setColor(base_color);
    canvas->drawRect(skrect, paint);
    return;
  }

  paint.setColor(SK_ColorBLACK);
  const int kLightEnd = state == kPressed ? 1 : 0;
  const int kDarkEnd = !kLightEnd;
  SkPoint gradient_bounds[2];
  gradient_bounds[kLightEnd].iset(rect.x(), rect.y());
  gradient_bounds[kDarkEnd].iset(rect.x(), kBottom - 1);
  SkColor colors[2];
  colors[0] = light_color;
  colors[1] = base_color;

  skia::RefPtr<SkShader> shader = skia::AdoptRef(
      SkGradientShader::CreateLinear(
          gradient_bounds, colors, NULL, 2, SkShader::kClamp_TileMode, NULL));
  paint.setStyle(SkPaint::kFill_Style);
  paint.setAntiAlias(true);
  paint.setShader(shader.get());

  canvas->drawRoundRect(skrect, SkIntToScalar(1), SkIntToScalar(1), paint);
  paint.setShader(NULL);

  if (button.has_border) {
    int border_alpha = state == kHovered ? 0x80 : 0x55;
    if (button.is_focused) {
      border_alpha = 0xff;
      paint.setColor(GetSystemColor(kColorId_FocusedBorderColor));
    }
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(SkIntToScalar(1));
    paint.setAlpha(border_alpha);
    skrect.inset(SkFloatToScalar(.5f), SkFloatToScalar(.5f));
    canvas->drawRoundRect(skrect, SkIntToScalar(1), SkIntToScalar(1), paint);
  }
}

void NativeThemeBase::PaintTextField(SkCanvas* canvas,
                                     State state,
                                     const gfx::Rect& rect,
                                     const TextFieldExtraParams& text) const {
  // The following drawing code simulates the user-agent css border for
  // text area and text input so that we do not break layout tests. Once we
  // have decided the desired looks, we should update the code here and
  // the layout test expectations.
  SkRect bounds;
  bounds.set(rect.x(), rect.y(), rect.right() - 1, rect.bottom() - 1);

  SkPaint fill_paint;
  fill_paint.setStyle(SkPaint::kFill_Style);
  fill_paint.setColor(text.background_color);
  canvas->drawRect(bounds, fill_paint);

  if (text.is_text_area) {
    // Draw text area border: 1px solid black
    SkPaint stroke_paint;
    fill_paint.setStyle(SkPaint::kStroke_Style);
    fill_paint.setColor(SK_ColorBLACK);
    canvas->drawRect(bounds, fill_paint);
  } else {
    // Draw text input and listbox inset border
    //   Text Input: 2px inset #eee
    //   Listbox: 1px inset #808080
    const SkColor kLightColor = text.is_listbox ?
        SkColorSetRGB(0x80, 0x80, 0x80) : SkColorSetRGB(0xee, 0xee, 0xee);
    const SkColor kDarkColor = text.is_listbox ?
        SkColorSetRGB(0x2c, 0x2c, 0x2c) : SkColorSetRGB(0x9a, 0x9a, 0x9a);
    const int kBorderWidth = text.is_listbox ? 1 : 2;

    SkPaint dark_paint;
    dark_paint.setAntiAlias(true);
    dark_paint.setStyle(SkPaint::kFill_Style);
    dark_paint.setColor(kDarkColor);

    SkPaint light_paint;
    light_paint.setAntiAlias(true);
    light_paint.setStyle(SkPaint::kFill_Style);
    light_paint.setColor(kLightColor);

    int left = rect.x();
    int top = rect.y();
    int right = rect.right();
    int bottom = rect.bottom();

    SkPath path;
    path.incReserve(4);

    // Top
    path.moveTo(SkIntToScalar(left), SkIntToScalar(top));
    path.lineTo(SkIntToScalar(left + kBorderWidth),
                SkIntToScalar(top + kBorderWidth));
    path.lineTo(SkIntToScalar(right - kBorderWidth),
                SkIntToScalar(top + kBorderWidth));
    path.lineTo(SkIntToScalar(right), SkIntToScalar(top));
    canvas->drawPath(path, dark_paint);

    // Bottom
    path.reset();
    path.moveTo(SkIntToScalar(left + kBorderWidth),
                SkIntToScalar(bottom - kBorderWidth));
    path.lineTo(SkIntToScalar(left), SkIntToScalar(bottom));
    path.lineTo(SkIntToScalar(right), SkIntToScalar(bottom));
    path.lineTo(SkIntToScalar(right - kBorderWidth),
                SkIntToScalar(bottom - kBorderWidth));
    canvas->drawPath(path, light_paint);

    // Left
    path.reset();
    path.moveTo(SkIntToScalar(left), SkIntToScalar(top));
    path.lineTo(SkIntToScalar(left), SkIntToScalar(bottom));
    path.lineTo(SkIntToScalar(left + kBorderWidth),
                SkIntToScalar(bottom - kBorderWidth));
    path.lineTo(SkIntToScalar(left + kBorderWidth),
                SkIntToScalar(top + kBorderWidth));
    canvas->drawPath(path, dark_paint);

    // Right
    path.reset();
    path.moveTo(SkIntToScalar(right - kBorderWidth),
                SkIntToScalar(top + kBorderWidth));
    path.lineTo(SkIntToScalar(right - kBorderWidth), SkIntToScalar(bottom));
    path.lineTo(SkIntToScalar(right), SkIntToScalar(bottom));
    path.lineTo(SkIntToScalar(right), SkIntToScalar(top));
    canvas->drawPath(path, light_paint);
  }
}

void NativeThemeBase::PaintMenuList(
    SkCanvas* canvas,
    State state,
    const gfx::Rect& rect,
    const MenuListExtraParams& menu_list) const {
  // If a border radius is specified, we let the WebCore paint the background
  // and the border of the control.
  if (!menu_list.has_border_radius) {
    ButtonExtraParams button = { 0 };
    button.background_color = menu_list.background_color;
    button.has_border = menu_list.has_border;
    PaintButton(canvas, state, rect, button);
  }

  SkPaint paint;
  paint.setColor(SK_ColorBLACK);
  paint.setAntiAlias(true);
  paint.setStyle(SkPaint::kFill_Style);

  SkPath path;
  path.moveTo(menu_list.arrow_x, menu_list.arrow_y - 3);
  path.rLineTo(6, 0);
  path.rLineTo(-3, 6);
  path.close();
  canvas->drawPath(path, paint);
}

void NativeThemeBase::PaintMenuPopupBackground(
    SkCanvas* canvas,
    const gfx::Size& size,
    const MenuBackgroundExtraParams& menu_background) const {
  canvas->drawColor(kMenuPopupBackgroundColor, SkXfermode::kSrc_Mode);
}

void NativeThemeBase::PaintMenuItemBackground(
    SkCanvas* canvas,
    State state,
    const gfx::Rect& rect,
    const MenuListExtraParams& menu_list) const {
  // By default don't draw anything over the normal background.
}

void NativeThemeBase::PaintSliderTrack(SkCanvas* canvas,
                                       State state,
                                       const gfx::Rect& rect,
                                       const SliderExtraParams& slider) const {
  const int kMidX = rect.x() + rect.width() / 2;
  const int kMidY = rect.y() + rect.height() / 2;

  SkPaint paint;
  paint.setColor(kSliderTrackBackgroundColor);

  SkRect skrect;
  if (slider.vertical) {
    skrect.set(std::max(rect.x(), kMidX - 2),
               rect.y(),
               std::min(rect.right(), kMidX + 2),
               rect.bottom());
  } else {
    skrect.set(rect.x(),
               std::max(rect.y(), kMidY - 2),
               rect.right(),
               std::min(rect.bottom(), kMidY + 2));
  }
  canvas->drawRect(skrect, paint);
}

void NativeThemeBase::PaintSliderThumb(SkCanvas* canvas,
                                       State state,
                                       const gfx::Rect& rect,
                                       const SliderExtraParams& slider) const {
  const bool hovered = (state == kHovered) || slider.in_drag;
  const int kMidX = rect.x() + rect.width() / 2;
  const int kMidY = rect.y() + rect.height() / 2;

  SkPaint paint;
  paint.setColor(hovered ? SK_ColorWHITE : kSliderThumbLightGrey);

  SkIRect skrect;
  if (slider.vertical)
    skrect.set(rect.x(), rect.y(), kMidX + 1, rect.bottom());
  else
    skrect.set(rect.x(), rect.y(), rect.right(), kMidY + 1);

  canvas->drawIRect(skrect, paint);

  paint.setColor(hovered ? kSliderThumbLightGrey : kSliderThumbDarkGrey);

  if (slider.vertical)
    skrect.set(kMidX + 1, rect.y(), rect.right(), rect.bottom());
  else
    skrect.set(rect.x(), kMidY + 1, rect.right(), rect.bottom());

  canvas->drawIRect(skrect, paint);

  paint.setColor(kSliderThumbBorderDarkGrey);
  DrawBox(canvas, rect, paint);

  if (rect.height() > 10 && rect.width() > 10) {
    DrawHorizLine(canvas, kMidX - 2, kMidX + 2, kMidY, paint);
    DrawHorizLine(canvas, kMidX - 2, kMidX + 2, kMidY - 3, paint);
    DrawHorizLine(canvas, kMidX - 2, kMidX + 2, kMidY + 3, paint);
  }
}

void NativeThemeBase::PaintInnerSpinButton(SkCanvas* canvas,
    State state,
    const gfx::Rect& rect,
    const InnerSpinButtonExtraParams& spin_button) const {
  if (spin_button.read_only)
    state = kDisabled;

  State north_state = state;
  State south_state = state;
  if (spin_button.spin_up)
    south_state = south_state != kDisabled ? kNormal : kDisabled;
  else
    north_state = north_state != kDisabled ? kNormal : kDisabled;

  gfx::Rect half = rect;
  half.set_height(rect.height() / 2);
  PaintArrowButton(canvas, half, kScrollbarUpArrow, north_state);

  half.set_y(rect.y() + rect.height() / 2);
  PaintArrowButton(canvas, half, kScrollbarDownArrow, south_state);
}

void NativeThemeBase::PaintProgressBar(SkCanvas* canvas,
    State state,
    const gfx::Rect& rect,
    const ProgressBarExtraParams& progress_bar) const {
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  gfx::ImageSkia* bar_image = rb.GetImageSkiaNamed(IDR_PROGRESS_BAR);
  gfx::ImageSkia* left_border_image = rb.GetImageSkiaNamed(
      IDR_PROGRESS_BORDER_LEFT);
  gfx::ImageSkia* right_border_image = rb.GetImageSkiaNamed(
      IDR_PROGRESS_BORDER_RIGHT);

  float tile_scale = static_cast<float>(rect.height()) /
      bar_image->height();

  int new_tile_width = static_cast<int>(bar_image->width() * tile_scale);
  float tile_scale_x = static_cast<float>(new_tile_width) /
      bar_image->width();

  DrawTiledImage(canvas, *bar_image, 0, 0, tile_scale_x, tile_scale,
      rect.x(), rect.y(), rect.width(), rect.height());

  if (progress_bar.value_rect_width) {
    gfx::ImageSkia* value_image = rb.GetImageSkiaNamed(IDR_PROGRESS_VALUE);

    new_tile_width = static_cast<int>(value_image->width() * tile_scale);
    tile_scale_x = static_cast<float>(new_tile_width) /
        value_image->width();

    DrawTiledImage(canvas, *value_image, 0, 0, tile_scale_x, tile_scale,
        progress_bar.value_rect_x,
        progress_bar.value_rect_y,
        progress_bar.value_rect_width,
        progress_bar.value_rect_height);
  }

  int dest_left_border_width = static_cast<int>(left_border_image->width() *
      tile_scale);
  DrawImageInt(canvas, *left_border_image, 0, 0, left_border_image->width(),
      left_border_image->height(), rect.x(), rect.y(), dest_left_border_width,
      rect.height());

  int dest_right_border_width = static_cast<int>(right_border_image->width() *
      tile_scale);
  int dest_x = rect.right() - dest_right_border_width;
  DrawImageInt(canvas, *right_border_image, 0, 0, right_border_image->width(),
               right_border_image->height(), dest_x, rect.y(),
               dest_right_border_width, rect.height());
}

bool NativeThemeBase::IntersectsClipRectInt(SkCanvas* canvas,
                                            int x, int y, int w, int h) const {
  SkRect clip;
  return canvas->getClipBounds(&clip) &&
      clip.intersect(SkIntToScalar(x), SkIntToScalar(y), SkIntToScalar(x + w),
                     SkIntToScalar(y + h));
}

void NativeThemeBase::DrawImageInt(
    SkCanvas* sk_canvas, const gfx::ImageSkia& image,
    int src_x, int src_y, int src_w, int src_h,
    int dest_x, int dest_y, int dest_w, int dest_h) const {
  // TODO(pkotwicz): Do something better and don't infer device
  // scale factor from canvas scale.
  SkMatrix m = sk_canvas->getTotalMatrix();
  ui::ScaleFactor device_scale_factor = ui::GetScaleFactorFromScale(
      SkScalarAbs(m.getScaleX()));
  scoped_ptr<gfx::Canvas> canvas(gfx::Canvas::CreateCanvasWithoutScaling(
      sk_canvas, device_scale_factor));
  canvas->DrawImageInt(image, src_x, src_y, src_w, src_h,
      dest_x, dest_y, dest_w, dest_h, true);
}

void NativeThemeBase::DrawTiledImage(SkCanvas* sk_canvas,
    const gfx::ImageSkia& image,
    int src_x, int src_y, float tile_scale_x, float tile_scale_y,
    int dest_x, int dest_y, int w, int h) const {
  // TODO(pkotwicz): Do something better and don't infer device
  // scale factor from canvas scale.
  SkMatrix m = sk_canvas->getTotalMatrix();
  ui::ScaleFactor device_scale_factor = ui::GetScaleFactorFromScale(
      SkScalarAbs(m.getScaleX()));
  scoped_ptr<gfx::Canvas> canvas(gfx::Canvas::CreateCanvasWithoutScaling(
      sk_canvas, device_scale_factor));
  canvas->TileImageInt(image, src_x, src_y, tile_scale_x,
      tile_scale_y, dest_x, dest_y, w, h);
}

SkColor NativeThemeBase::SaturateAndBrighten(SkScalar* hsv,
                                             SkScalar saturate_amount,
                                             SkScalar brighten_amount) const {
  SkScalar color[3];
  color[0] = hsv[0];
  color[1] = Clamp(hsv[1] + saturate_amount, 0.0, 1.0);
  color[2] = Clamp(hsv[2] + brighten_amount, 0.0, 1.0);
  return SkHSVToColor(color);
}

void NativeThemeBase::DrawVertLine(SkCanvas* canvas,
                                   int x,
                                   int y1,
                                   int y2,
                                   const SkPaint& paint) const {
  SkIRect skrect;
  skrect.set(x, y1, x + 1, y2 + 1);
  canvas->drawIRect(skrect, paint);
}

void NativeThemeBase::DrawHorizLine(SkCanvas* canvas,
                                    int x1,
                                    int x2,
                                    int y,
                                    const SkPaint& paint) const {
  SkIRect skrect;
  skrect.set(x1, y, x2 + 1, y + 1);
  canvas->drawIRect(skrect, paint);
}

void NativeThemeBase::DrawBox(SkCanvas* canvas,
                              const gfx::Rect& rect,
                              const SkPaint& paint) const {
  const int right = rect.x() + rect.width() - 1;
  const int bottom = rect.y() + rect.height() - 1;
  DrawHorizLine(canvas, rect.x(), right, rect.y(), paint);
  DrawVertLine(canvas, right, rect.y(), bottom, paint);
  DrawHorizLine(canvas, rect.x(), right, bottom, paint);
  DrawVertLine(canvas, rect.x(), rect.y(), bottom, paint);
}

SkScalar NativeThemeBase::Clamp(SkScalar value,
                                SkScalar min,
                                SkScalar max) const {
  return std::min(std::max(value, min), max);
}

SkColor NativeThemeBase::OutlineColor(SkScalar* hsv1, SkScalar* hsv2) const {
  // GTK Theme engines have way too much control over the layout of
  // the scrollbar. We might be able to more closely approximate its
  // look-and-feel, if we sent whole images instead of just colors
  // from the browser to the renderer. But even then, some themes
  // would just break.
  //
  // So, instead, we don't even try to 100% replicate the look of
  // the native scrollbar. We render our own version, but we make
  // sure to pick colors that blend in nicely with the system GTK
  // theme. In most cases, we can just sample a couple of pixels
  // from the system scrollbar and use those colors to draw our
  // scrollbar.
  //
  // This works fine for the track color and the overall thumb
  // color. But it fails spectacularly for the outline color used
  // around the thumb piece.  Not all themes have a clearly defined
  // outline. For some of them it is partially transparent, and for
  // others the thickness is very unpredictable.
  //
  // So, instead of trying to approximate the system theme, we
  // instead try to compute a reasonable looking choice based on the
  // known color of the track and the thumb piece. This is difficult
  // when trying to deal both with high- and low-contrast themes,
  // and both with positive and inverted themes.
  //
  // The following code has been tested to look OK with all of the
  // default GTK themes.
  SkScalar min_diff = Clamp((hsv1[1] + hsv2[1]) * 1.2f, 0.28f, 0.5f);
  SkScalar diff = Clamp(fabs(hsv1[2] - hsv2[2]) / 2, min_diff, 0.5f);

  if (hsv1[2] + hsv2[2] > 1.0)
    diff = -diff;

  return SaturateAndBrighten(hsv2, -0.2f, diff);
}

}  // namespace ui
