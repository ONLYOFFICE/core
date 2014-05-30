// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/label_button.h"

#include "base/logging.h"
#include "grit/ui_resources.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/focus_border.h"
#include "ui/views/window/dialog_delegate.h"

#if defined(OS_WIN)
#include "ui/gfx/color_utils.h"
#include "ui/native_theme/native_theme_win.h"
#endif

namespace {

// The spacing between the icon and text.
const int kSpacing = 5;

// The length of the hover fade animation.
const int kHoverAnimationDurationMs = 170;

}  // namespace

namespace views {

// static
const char LabelButton::kViewClassName[] = "views/LabelButton";

LabelButton::LabelButton(ButtonListener* listener, const string16& text)
    : CustomButton(listener),
      image_(new ImageView()),
      label_(new Label()),
      button_state_images_(),
      button_state_colors_(),
      explicitly_set_colors_(),
      is_default_(false),
      style_(STYLE_TEXTBUTTON) {
  SetAnimationDuration(kHoverAnimationDurationMs);
  SetText(text);

  AddChildView(image_);
  image_->set_interactive(false);

  AddChildView(label_);
  label_->SetAutoColorReadabilityEnabled(false);
  label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  // Initialize the colors, border, and layout.
  SetStyle(style_);

  SetAccessibleName(text);
}

LabelButton::~LabelButton() {}

const gfx::ImageSkia& LabelButton::GetImage(ButtonState for_state) {
  if (for_state != STATE_NORMAL && button_state_images_[for_state].isNull())
    return button_state_images_[STATE_NORMAL];
  return button_state_images_[for_state];
}

void LabelButton::SetImage(ButtonState for_state, const gfx::ImageSkia& image) {
  button_state_images_[for_state] = image;
  image_->SetImage(GetImage(state()));
}

const string16& LabelButton::GetText() const {
  return label_->text();
}

void LabelButton::SetText(const string16& text) {
  SetAccessibleName(text);
  label_->SetText(text);
  SetAccessibleName(text);
}

void LabelButton::SetTextColor(ButtonState for_state, SkColor color) {
  button_state_colors_[for_state] = color;
  if (for_state == STATE_DISABLED)
    label_->SetDisabledColor(color);
  else if (for_state == state())
    label_->SetEnabledColor(color);
  explicitly_set_colors_[for_state] = true;
}

bool LabelButton::GetTextMultiLine() const {
  return label_->is_multi_line();
}

void LabelButton::SetTextMultiLine(bool text_multi_line) {
  label_->SetMultiLine(text_multi_line);
}

const gfx::Font& LabelButton::GetFont() const {
  return label_->font();
}

void LabelButton::SetFont(const gfx::Font& font) {
  label_->SetFont(font);
}

gfx::HorizontalAlignment LabelButton::GetHorizontalAlignment() const {
  return label_->horizontal_alignment();
}

void LabelButton::SetHorizontalAlignment(gfx::HorizontalAlignment alignment) {
  label_->SetHorizontalAlignment(alignment);
  InvalidateLayout();
}

void LabelButton::SetIsDefault(bool is_default) {
  if (is_default == is_default_)
    return;
  is_default_ = is_default;
  ui::Accelerator accel(ui::VKEY_RETURN, ui::EF_NONE);
  is_default_ ? AddAccelerator(accel) : RemoveAccelerator(accel);

  // STYLE_BUTTON uses bold text to indicate default buttons.
  if (style_ == STYLE_BUTTON) {
    int style = label_->font().GetStyle();
    style = is_default ? style | gfx::Font::BOLD : style & !gfx::Font::BOLD;
    label_->SetFont(label_->font().DeriveFont(0, style));
  }
}

void LabelButton::SetStyle(ButtonStyle style) {
  // Use the new button style instead of the native button style.
  // TODO(msw): Officialy deprecate and remove STYLE_NATIVE_TEXTBUTTON.
  if (DialogDelegate::UseNewStyle() && style == STYLE_NATIVE_TEXTBUTTON)
    style = STYLE_BUTTON;

  style_ = style;
  set_border(new LabelButtonBorder(style));
  // Inset the button focus rect from the actual border; roughly match Windows.
  if (style == STYLE_TEXTBUTTON || style == STYLE_NATIVE_TEXTBUTTON)
    set_focus_border(FocusBorder::CreateDashedFocusBorder(3, 3, 3, 3));
  if (style == STYLE_BUTTON || style == STYLE_NATIVE_TEXTBUTTON) {
    label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
    set_focusable(true);
  }
  if (style == STYLE_BUTTON) {
    set_min_size(gfx::Size(70, 31));
    const SkColor color = GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_WindowBackground);
    label_->SetShadowColors(color, color);
    label_->SetShadowOffset(0, 1);
  }
  // Invalidate the layout to pickup the new insets from the border.
  InvalidateLayout();
  ResetColorsFromNativeTheme();
}

gfx::Size LabelButton::GetPreferredSize() {
  // Accommodate bold text in case this STYLE_BUTTON button is made default.
  const gfx::Font font = label_->font();
  if (style_ == STYLE_BUTTON)
    label_->SetFont(font.DeriveFont(0, font.GetStyle() | gfx::Font::BOLD));

  // Resize multi-line labels paired with images to use their available width.
  const gfx::Size image_size(image_->GetPreferredSize());
  if (GetTextMultiLine() && !image_size.IsEmpty() && !GetText().empty() &&
      GetHorizontalAlignment() == gfx::ALIGN_CENTER) {
    label_->SizeToFit(GetLocalBounds().width() - image_size.width() - kSpacing);
  }

  // Calculate the required size.
  gfx::Size size(label_->GetPreferredSize());
  if (image_size.width() > 0 && size.width() > 0)
    size.Enlarge(kSpacing, 0);
  size.set_height(std::max(size.height(), image_size.height()));
  size.Enlarge(image_size.width() + GetInsets().width(), GetInsets().height());

  // Restore the label's original font without the temporary bold style.
  if (style_ == STYLE_BUTTON)
    label_->SetFont(font);

  // Increase the minimum size monotonically with the preferred size.
  size.SetSize(std::max(min_size_.width(), size.width()),
               std::max(min_size_.height(), size.height()));
  min_size_ = size;

  // Return the largest known size clamped to the maximum size (if valid).
  if (max_size_.width() > 0)
    size.set_width(std::min(max_size_.width(), size.width()));
  if (max_size_.height() > 0)
    size.set_height(std::min(max_size_.height(), size.height()));
  return size;
}

std::string LabelButton::GetClassName() const {
  return kViewClassName;
}

void LabelButton::ResetColorsFromNativeTheme() {
  const ui::NativeTheme* theme = GetNativeTheme();
  SkColor colors[STATE_COUNT] = {
    theme->GetSystemColor(ui::NativeTheme::kColorId_TextButtonEnabledColor),
    theme->GetSystemColor(ui::NativeTheme::kColorId_TextButtonHoverColor),
    theme->GetSystemColor(ui::NativeTheme::kColorId_TextButtonHoverColor),
    theme->GetSystemColor(ui::NativeTheme::kColorId_TextButtonDisabledColor),
  };

  // Certain styles do not change text color when hovered or pressed.
  bool constant_text_color = style() == STYLE_BUTTON;
#if defined(OS_WIN)
  constant_text_color |= (style() == STYLE_NATIVE_TEXTBUTTON &&
                          theme == ui::NativeThemeWin::instance());
#endif
  if (constant_text_color)
    colors[STATE_HOVERED] = colors[STATE_PRESSED] = colors[STATE_NORMAL];

  for (size_t state = STATE_NORMAL; state < STATE_COUNT; ++state) {
    if (!explicitly_set_colors_[state]) {
      SetTextColor(static_cast<ButtonState>(state), colors[state]);
      explicitly_set_colors_[state] = false;
    }
  }
}

void LabelButton::StateChanged() {
  const gfx::Size previous_image_size(image_->GetPreferredSize());
  image_->SetImage(GetImage(state()));
  const SkColor color = button_state_colors_[state()];
  if (state() != STATE_DISABLED && label_->enabled_color() != color)
    label_->SetEnabledColor(color);
  label_->SetEnabled(state() != STATE_DISABLED);
  if (image_->GetPreferredSize() != previous_image_size)
    Layout();
}

void LabelButton::Layout() {
  gfx::Rect child_area(GetLocalBounds());
  child_area.Inset(GetInsets());

  gfx::Size image_size(image_->GetPreferredSize());
  image_size.set_width(std::min(image_size.width(), child_area.width()));
  image_size.set_height(std::min(image_size.height(), child_area.height()));

  // The label takes any remaining width after sizing the image, unless both
  // views are centered. In that case, using the tighter preferred label width
  // avoids wasted space within the label that would look like awkward padding.
  gfx::Size label_size(child_area.size());
  if (!image_size.IsEmpty() && !label_size.IsEmpty()) {
    label_size.set_width(
        std::max(child_area.width() - image_size.width() - kSpacing, 0));
    if (GetHorizontalAlignment() == gfx::ALIGN_CENTER) {
      // Ensure multi-line labels paired with images use their available width.
      if (GetTextMultiLine())
        label_->SizeToFit(label_size.width());
      label_size.set_width(
          std::min(label_size.width(), label_->GetPreferredSize().width()));
    }
  }

  gfx::Point image_origin(child_area.origin());
  image_origin.Offset(0, (child_area.height() - image_size.height()) / 2);
  if (GetHorizontalAlignment() == gfx::ALIGN_CENTER) {
    const int total_width = image_size.width() + label_size.width() +
        ((image_size.width() > 0 && label_size.width() > 0) ? kSpacing : 0);
    image_origin.Offset((child_area.width() - total_width) / 2, 0);
  } else if (GetHorizontalAlignment() == gfx::ALIGN_RIGHT) {
    image_origin.Offset(child_area.width() - image_size.width(), 0);
  }

  gfx::Point label_origin(child_area.origin());
  if (!image_size.IsEmpty() && GetHorizontalAlignment() != gfx::ALIGN_RIGHT)
    label_origin.set_x(image_origin.x() + image_size.width() + kSpacing);

  image_->SetBoundsRect(gfx::Rect(image_origin, image_size));
  label_->SetBoundsRect(gfx::Rect(label_origin, label_size));
}

void LabelButton::ChildPreferredSizeChanged(View* child) {
  PreferredSizeChanged();
}

void LabelButton::OnNativeThemeChanged(const ui::NativeTheme* theme) {
  ResetColorsFromNativeTheme();
}

ui::NativeTheme::Part LabelButton::GetThemePart() const {
  return ui::NativeTheme::kPushButton;
}

gfx::Rect LabelButton::GetThemePaintRect() const {
  return GetLocalBounds();
}

ui::NativeTheme::State LabelButton::GetThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  switch(state()) {
    case STATE_NORMAL:   return ui::NativeTheme::kNormal;
    case STATE_HOVERED:  return ui::NativeTheme::kHovered;
    case STATE_PRESSED:  return ui::NativeTheme::kPressed;
    case STATE_DISABLED: return ui::NativeTheme::kDisabled;
    case STATE_COUNT:    NOTREACHED() << "Unknown state: " << state();
  }
  return ui::NativeTheme::kNormal;
}

const ui::Animation* LabelButton::GetThemeAnimation() const {
#if defined(OS_WIN)
  if (style() == STYLE_NATIVE_TEXTBUTTON &&
      GetNativeTheme() == ui::NativeThemeWin::instance()) {
    return ui::NativeThemeWin::instance()->IsThemingActive() ?
        hover_animation_.get() : NULL;
  }
#endif
  return hover_animation_.get();
}

ui::NativeTheme::State LabelButton::GetBackgroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kNormal;
}

ui::NativeTheme::State LabelButton::GetForegroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kHovered;
}

void LabelButton::GetExtraParams(ui::NativeTheme::ExtraParams* params) const {
  params->button.checked = false;
  params->button.indeterminate = false;
  params->button.is_default = is_default_;
  params->button.is_focused = HasFocus() && IsAccessibilityFocusable();
  params->button.has_border = style() == STYLE_NATIVE_TEXTBUTTON;
  params->button.classic_state = 0;
  params->button.background_color = GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_TextButtonBackgroundColor);
}

}  // namespace views
