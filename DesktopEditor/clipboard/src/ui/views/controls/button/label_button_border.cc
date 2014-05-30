// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/label_button_border.h"

#include "base/logging.h"
#include "grit/ui_resources.h"
#include "ui/base/animation/animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/rect.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/native_theme_delegate.h"

namespace views {

namespace {

// The button image IDs and focused images IDs used in lieu of a focus border.
const int kNormalImages[] = IMAGE_GRID(IDR_BUTTON_NORMAL);
const int kHoveredImages[] = IMAGE_GRID(IDR_BUTTON_HOVER);
const int kPressedImages[] = IMAGE_GRID(IDR_BUTTON_PRESSED);
const int kFocusedNormalImages[] = IMAGE_GRID(IDR_BUTTON_FOCUSED_NORMAL);
const int kFocusedHoveredImages[] = IMAGE_GRID(IDR_BUTTON_FOCUSED_HOVER);
const int kFocusedPressedImages[] = IMAGE_GRID(IDR_BUTTON_FOCUSED_PRESSED);

// The text-button hot and pushed image IDs; normal is unadorned by default.
const int kTextHoveredImages[] = IMAGE_GRID(IDR_TEXTBUTTON_HOVER);
const int kTextPressedImages[] = IMAGE_GRID(IDR_TEXTBUTTON_PRESSED);

Button::ButtonState GetButtonState(ui::NativeTheme::State state) {
  switch(state) {
    case ui::NativeTheme::kDisabled: return Button::STATE_DISABLED;
    case ui::NativeTheme::kHovered:  return Button::STATE_HOVERED;
    case ui::NativeTheme::kNormal:   return Button::STATE_NORMAL;
    case ui::NativeTheme::kPressed:  return Button::STATE_PRESSED;
    case ui::NativeTheme::kMaxState: NOTREACHED() << "Unknown state: " << state;
  }
  return Button::STATE_NORMAL;
}

// A helper function to paint the native theme or images as appropriate.
void PaintHelper(LabelButtonBorder* border,
                 gfx::Canvas* canvas,
                 const ui::NativeTheme* theme,
                 ui::NativeTheme::Part part,
                 ui::NativeTheme::State state,
                 const gfx::Rect& rect,
                 const ui::NativeTheme::ExtraParams& extra) {
  if (border->style() == Button::STYLE_NATIVE_TEXTBUTTON) {
    theme->Paint(canvas->sk_canvas(), part, state, rect, extra);
  } else {
    Painter* painter =
        border->GetPainter(extra.button.is_focused, GetButtonState(state));
    // Paint any corresponding unfocused painter if there is no focused painter.
    if (!painter && extra.button.is_focused)
      painter = border->GetPainter(false, GetButtonState(state));
    if (painter)
      painter->Paint(canvas, rect.size());
  }
}

}  // namespace

LabelButtonBorder::LabelButtonBorder(Button::ButtonStyle style)
    : style_(style) {
  if (style == Button::STYLE_BUTTON) {
    SetPainter(false, Button::STATE_NORMAL,
               Painter::CreateImageGridPainter(kNormalImages));
    SetPainter(false, Button::STATE_HOVERED,
               Painter::CreateImageGridPainter(kHoveredImages));
    SetPainter(false, Button::STATE_PRESSED,
               Painter::CreateImageGridPainter(kPressedImages));
    SetPainter(false, Button::STATE_DISABLED,
               Painter::CreateImageGridPainter(kNormalImages));
    SetPainter(true, Button::STATE_NORMAL,
               Painter::CreateImageGridPainter(kFocusedNormalImages));
    SetPainter(true, Button::STATE_HOVERED,
               Painter::CreateImageGridPainter(kFocusedHoveredImages));
    SetPainter(true, Button::STATE_PRESSED,
               Painter::CreateImageGridPainter(kFocusedPressedImages));
    SetPainter(true, Button::STATE_DISABLED,
               Painter::CreateImageGridPainter(kNormalImages));
  } else if (style == Button::STYLE_TEXTBUTTON) {
    SetPainter(false, Button::STATE_HOVERED,
               Painter::CreateImageGridPainter(kTextHoveredImages));
    SetPainter(false, Button::STATE_PRESSED,
               Painter::CreateImageGridPainter(kTextPressedImages));
  }
}

LabelButtonBorder::~LabelButtonBorder() {}

void LabelButtonBorder::Paint(const View& view, gfx::Canvas* canvas) {
  DCHECK(view.GetClassName() == LabelButton::kViewClassName);
  const NativeThemeDelegate* native_theme_delegate =
      static_cast<const LabelButton*>(&view);
  ui::NativeTheme::Part part = native_theme_delegate->GetThemePart();
  gfx::Rect rect(native_theme_delegate->GetThemePaintRect());
  ui::NativeTheme::State state;
  ui::NativeTheme::ExtraParams extra;
  const ui::NativeTheme* theme = view.GetNativeTheme();
  const ui::Animation* animation = native_theme_delegate->GetThemeAnimation();
  state = native_theme_delegate->GetThemeState(&extra);
  int alpha = 0xff;

  if (animation && animation->is_animating()) {
    // Paint the background state.
    state = native_theme_delegate->GetBackgroundThemeState(&extra);
    PaintHelper(this, canvas, theme, part, state, rect, extra);
    // Composite the foreground state above the background state.
    alpha = animation->CurrentValueBetween(0, alpha);
    state = native_theme_delegate->GetForegroundThemeState(&extra);
  }

  if (state == ui::NativeTheme::kDisabled && style() == Button::STYLE_BUTTON)
    alpha /= 2;

  if (alpha != 0xff)
    canvas->SaveLayerAlpha(static_cast<uint8>(alpha));
  PaintHelper(this, canvas, theme, part, state, rect, extra);
  if (alpha != 0xff)
    canvas->Restore();

  // Draw the Views focus border for the native theme style.
  if (style() == Button::STYLE_NATIVE_TEXTBUTTON &&
      view.focus_border() && extra.button.is_focused)
    view.focus_border()->Paint(view, canvas);
}

gfx::Insets LabelButtonBorder::GetInsets() const {
  // Return the style-specific insets between button contents and edges.
  if (style() == Button::STYLE_BUTTON)
    return gfx::Insets(9, 13, 9, 13);
  if (style() == Button::STYLE_TEXTBUTTON)
    return gfx::Insets(5, 6, 5, 6);
  if (style() == Button::STYLE_NATIVE_TEXTBUTTON)
    return gfx::Insets(5, 12, 5, 12);
  NOTREACHED();
  return gfx::Insets();
}

Painter* LabelButtonBorder::GetPainter(bool focused,
                                       Button::ButtonState state) {
  return painters_[focused ? 1 : 0][state].get();
}

void LabelButtonBorder::SetPainter(bool focused,
                                   Button::ButtonState state,
                                   Painter* painter) {
  painters_[focused ? 1 : 0][state].reset(painter);
}

}  // namespace views
