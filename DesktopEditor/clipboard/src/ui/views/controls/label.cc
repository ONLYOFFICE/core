// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/label.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/strings/string_split.h"
#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/text/text_elider.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/font.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/shadow_value.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/background.h"

namespace {

// The padding for the focus border when rendering focused text.
const int kFocusBorderPadding = 1;
const int kCachedSizeLimit = 10;

}  // namespace

namespace views {

// static
const char Label::kViewClassName[] = "views/Label";

Label::Label() {
  Init(string16(), GetDefaultFont());
}

Label::Label(const string16& text) {
  Init(text, GetDefaultFont());
}

Label::Label(const string16& text, const gfx::Font& font) {
  Init(text, font);
}

Label::~Label() {
}

void Label::SetFont(const gfx::Font& font) {
  font_ = font;
  ResetCachedSize();
  PreferredSizeChanged();
  SchedulePaint();
}

void Label::SetText(const string16& text) {
  if (text == text_)
    return;
  text_ = text;
  ResetCachedSize();
  PreferredSizeChanged();
  SchedulePaint();
}

void Label::SetAutoColorReadabilityEnabled(bool enabled) {
  auto_color_readability_ = enabled;
  RecalculateColors();
}

void Label::SetEnabledColor(SkColor color) {
  requested_enabled_color_ = color;
  enabled_color_set_ = true;
  RecalculateColors();
}

void Label::SetDisabledColor(SkColor color) {
  requested_disabled_color_ = color;
  disabled_color_set_ = true;
  RecalculateColors();
}

void Label::SetBackgroundColor(SkColor color) {
  background_color_ = color;
  background_color_set_ = true;
  RecalculateColors();
}

void Label::SetShadowColors(SkColor enabled_color, SkColor disabled_color) {
  enabled_shadow_color_ = enabled_color;
  disabled_shadow_color_ = disabled_color;
  has_shadow_ = true;
}

void Label::SetShadowOffset(int x, int y) {
  shadow_offset_.SetPoint(x, y);
}

void Label::ClearEmbellishing() {
  has_shadow_ = false;
}

void Label::SetHorizontalAlignment(gfx::HorizontalAlignment alignment) {
  // If the View's UI layout is right-to-left and directionality_mode_ is
  // USE_UI_DIRECTIONALITY, we need to flip the alignment so that the alignment
  // settings take into account the text directionality.
  if (base::i18n::IsRTL() && (directionality_mode_ == USE_UI_DIRECTIONALITY) &&
      (alignment != gfx::ALIGN_CENTER)) {
    alignment = (alignment == gfx::ALIGN_LEFT) ?
        gfx::ALIGN_RIGHT : gfx::ALIGN_LEFT;
  }
  if (horizontal_alignment_ != alignment) {
    horizontal_alignment_ = alignment;
    SchedulePaint();
  }
}

void Label::SetLineHeight(int height) {
  if (height != line_height_) {
    line_height_ = height;
    ResetCachedSize();
    PreferredSizeChanged();
    SchedulePaint();
  }
}

void Label::SetMultiLine(bool multi_line) {
  DCHECK(!multi_line || elide_behavior_ != ELIDE_IN_MIDDLE);
  if (multi_line != is_multi_line_) {
    is_multi_line_ = multi_line;
    ResetCachedSize();
    PreferredSizeChanged();
    SchedulePaint();
  }
}

void Label::SetAllowCharacterBreak(bool allow_character_break) {
  if (allow_character_break != allow_character_break_) {
    allow_character_break_ = allow_character_break;
    ResetCachedSize();
    PreferredSizeChanged();
    SchedulePaint();
  }
}

void Label::SetElideBehavior(ElideBehavior elide_behavior) {
  DCHECK(elide_behavior != ELIDE_IN_MIDDLE || !is_multi_line_);
  if (elide_behavior != elide_behavior_) {
    elide_behavior_ = elide_behavior;
    ResetCachedSize();
    PreferredSizeChanged();
    SchedulePaint();
  }
}

void Label::SetTooltipText(const string16& tooltip_text) {
  tooltip_text_ = tooltip_text;
}

void Label::SizeToFit(int max_width) {
  DCHECK(is_multi_line_);

  std::vector<string16> lines;
  base::SplitString(text_, '\n', &lines);

  int label_width = 0;
  for (std::vector<string16>::const_iterator iter = lines.begin();
       iter != lines.end(); ++iter) {
    label_width = std::max(label_width, font_.GetStringWidth(*iter));
  }

  label_width += GetInsets().width();

  if (max_width > 0)
    label_width = std::min(label_width, max_width);

  SetBounds(x(), y(), label_width, 0);
  SizeToPreferredSize();
}

void Label::SetHasFocusBorder(bool has_focus_border) {
  has_focus_border_ = has_focus_border;
  if (is_multi_line_) {
    ResetCachedSize();
    PreferredSizeChanged();
  }
}

gfx::Insets Label::GetInsets() const {
  gfx::Insets insets = View::GetInsets();
  if (focusable() || has_focus_border_) {
    insets += gfx::Insets(kFocusBorderPadding, kFocusBorderPadding,
                          kFocusBorderPadding, kFocusBorderPadding);
  }
  return insets;
}

int Label::GetBaseline() const {
  return GetInsets().top() + font_.GetBaseline();
}

gfx::Size Label::GetPreferredSize() {
  // Return a size of (0, 0) if the label is not visible and if the
  // collapse_when_hidden_ flag is set.
  // TODO(munjal): This logic probably belongs to the View class. But for now,
  // put it here since putting it in View class means all inheriting classes
  // need ot respect the collapse_when_hidden_ flag.
  if (!visible() && collapse_when_hidden_)
    return gfx::Size();

  gfx::Size prefsize(GetTextSize());
  gfx::Insets insets = GetInsets();
  prefsize.Enlarge(insets.width(), insets.height());
  return prefsize;
}

int Label::GetHeightForWidth(int w) {
  if (!is_multi_line_)
    return View::GetHeightForWidth(w);

  w = std::max(0, w - GetInsets().width());

  for (size_t i = 0; i < cached_heights_.size(); ++i) {
    const gfx::Size& s = cached_heights_[i];
    if (s.width() == w)
      return s.height() + GetInsets().height();
  }

  int cache_width = w;

  int h = font_.GetHeight();
  const int flags = ComputeDrawStringFlags();
  gfx::Canvas::SizeStringInt(text_, font_, &w, &h, line_height_, flags);
  cached_heights_[cached_heights_cursor_] = gfx::Size(cache_width, h);
  cached_heights_cursor_ = (cached_heights_cursor_ + 1) % kCachedSizeLimit;
  return h + GetInsets().height();
}

std::string Label::GetClassName() const {
  return kViewClassName;
}

View* Label::GetTooltipHandlerForPoint(const gfx::Point& point) {
  // Bail out if the label does not contain the point.
  // Note that HitTestPoint() cannot be used here as it uses
  // Label::HitTestRect() to determine if the point hits the label; and
  // Label::HitTestRect() always fails. Instead, default HitTestRect()
  // implementation should be used.
  if (!View::HitTestRect(gfx::Rect(point, gfx::Size(1, 1))))
    return NULL;

  if (tooltip_text_.empty() && !ShouldShowDefaultTooltip())
    return NULL;

  return this;
}

bool Label::HitTestRect(const gfx::Rect& rect) const {
  return false;
}

bool Label::GetTooltipText(const gfx::Point& p, string16* tooltip) const {
  DCHECK(tooltip);

  // If a tooltip has been explicitly set, use it.
  if (!tooltip_text_.empty()) {
    tooltip->assign(tooltip_text_);
    return true;
  }

  // Show the full text if the text does not fit.
  if (ShouldShowDefaultTooltip()) {
    *tooltip = text_;
    return true;
  }

  return false;
}

void Label::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_STATICTEXT;
  state->state = ui::AccessibilityTypes::STATE_READONLY;
  state->name = text_;
}

void Label::PaintText(gfx::Canvas* canvas,
                      const string16& text,
                      const gfx::Rect& text_bounds,
                      int flags) {
  gfx::ShadowValues shadows;
  if (has_shadow_)
    shadows.push_back(gfx::ShadowValue(shadow_offset_, 0,
        enabled() ? enabled_shadow_color_ : disabled_shadow_color_));
  canvas->DrawStringWithShadows(text, font_,
      enabled() ? actual_enabled_color_ : actual_disabled_color_,
      text_bounds, line_height_, flags, shadows);

  if (HasFocus()) {
    gfx::Rect focus_bounds = text_bounds;
    focus_bounds.Inset(-kFocusBorderPadding, -kFocusBorderPadding);
    canvas->DrawFocusRect(focus_bounds);
  }
}

gfx::Size Label::GetTextSize() const {
  if (!text_size_valid_) {
    // For single-line strings, we supply the largest possible width, because
    // while adding NO_ELLIPSIS to the flags works on Windows for forcing
    // SizeStringInt() to calculate the desired width, it doesn't seem to work
    // on Linux.
    int w = is_multi_line_ ?
        GetAvailableRect().width() : std::numeric_limits<int>::max();
    int h = font_.GetHeight();
    // For single-line strings, ignore the available width and calculate how
    // wide the text wants to be.
    int flags = ComputeDrawStringFlags();
    if (!is_multi_line_)
      flags |= gfx::Canvas::NO_ELLIPSIS;
    gfx::Canvas::SizeStringInt(text_, font_, &w, &h, line_height_, flags);
    text_size_.SetSize(w, h);
    text_size_valid_ = true;
  }

  return text_size_;
}

void Label::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  text_size_valid_ &= !is_multi_line_;
}

void Label::OnPaint(gfx::Canvas* canvas) {
  OnPaintBackground(canvas);
  // We skip painting the focus border because it is being handled seperately by
  // some subclasses of Label. We do not want View's focus border painting to
  // interfere with that.
  OnPaintBorder(canvas);

  string16 paint_text;
  gfx::Rect text_bounds;
  int flags = 0;
  CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  PaintText(canvas, paint_text, text_bounds, flags);
}

void Label::OnNativeThemeChanged(const ui::NativeTheme* theme) {
  UpdateColorsFromTheme(theme);
}

// static
gfx::Font Label::GetDefaultFont() {
  return ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::BaseFont);
}

void Label::Init(const string16& text, const gfx::Font& font) {
  font_ = font;
  enabled_color_set_ = disabled_color_set_ = background_color_set_ = false;
  auto_color_readability_ = true;
  UpdateColorsFromTheme(ui::NativeTheme::instance());
  horizontal_alignment_ = gfx::ALIGN_CENTER;
  line_height_ = 0;
  is_multi_line_ = false;
  allow_character_break_ = false;
  elide_behavior_ = NO_ELIDE;
  collapse_when_hidden_ = false;
  directionality_mode_ = USE_UI_DIRECTIONALITY;
  has_focus_border_ = false;
  enabled_shadow_color_ = 0;
  disabled_shadow_color_ = 0;
  shadow_offset_.SetPoint(1, 1);
  has_shadow_ = false;
  cached_heights_.resize(kCachedSizeLimit);
  ResetCachedSize();

  SetText(text);
}

void Label::RecalculateColors() {
  actual_enabled_color_ = auto_color_readability_ ?
      color_utils::GetReadableColor(requested_enabled_color_,
                                    background_color_) :
      requested_enabled_color_;
  actual_disabled_color_ = auto_color_readability_ ?
      color_utils::GetReadableColor(requested_disabled_color_,
                                    background_color_) :
      requested_disabled_color_;
}

gfx::Rect Label::GetTextBounds() const {
  gfx::Rect available_rect(GetAvailableRect());
  gfx::Size text_size(GetTextSize());
  text_size.set_width(std::min(available_rect.width(), text_size.width()));

  gfx::Insets insets = GetInsets();
  gfx::Point text_origin(insets.left(), insets.top());
  switch (horizontal_alignment_) {
    case gfx::ALIGN_LEFT:
      break;
    case gfx::ALIGN_CENTER:
      // We put any extra margin pixel on the left rather than the right.  We
      // used to do this because measurement on Windows used
      // GetTextExtentPoint32(), which could report a value one too large on the
      // right; we now use DrawText(), and who knows if it can also do this.
      text_origin.Offset((available_rect.width() + 1 - text_size.width()) / 2,
                         0);
      break;
    case gfx::ALIGN_RIGHT:
      text_origin.set_x(available_rect.right() - text_size.width());
      break;
    default:
      NOTREACHED();
      break;
  }
  text_origin.Offset(0,
      std::max(0, (available_rect.height() - text_size.height())) / 2);
  return gfx::Rect(text_origin, text_size);
}

int Label::ComputeDrawStringFlags() const {
  int flags = 0;

  // We can't use subpixel rendering if the background is non-opaque.
  if (SkColorGetA(background_color_) != 0xFF)
    flags |= gfx::Canvas::NO_SUBPIXEL_RENDERING;

  if (directionality_mode_ == AUTO_DETECT_DIRECTIONALITY) {
    base::i18n::TextDirection direction =
        base::i18n::GetFirstStrongCharacterDirection(text_);
    if (direction == base::i18n::RIGHT_TO_LEFT)
      flags |= gfx::Canvas::FORCE_RTL_DIRECTIONALITY;
    else
      flags |= gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
  }

  switch (horizontal_alignment_) {
    case gfx::ALIGN_LEFT:
      flags |= gfx::Canvas::TEXT_ALIGN_LEFT;
      break;
    case gfx::ALIGN_CENTER:
      flags |= gfx::Canvas::TEXT_ALIGN_CENTER;
      break;
    case gfx::ALIGN_RIGHT:
      flags |= gfx::Canvas::TEXT_ALIGN_RIGHT;
      break;
  }

  if (!is_multi_line_)
    return flags;

  flags |= gfx::Canvas::MULTI_LINE;
#if !defined(OS_WIN)
    // Don't elide multiline labels on Linux.
    // Todo(davemoore): Do we depend on eliding multiline text?
    // Pango insists on limiting the number of lines to one if text is
    // elided. You can get around this if you can pass a maximum height
    // but we don't currently have that data when we call the pango code.
    flags |= gfx::Canvas::NO_ELLIPSIS;
#endif
  if (allow_character_break_)
    flags |= gfx::Canvas::CHARACTER_BREAK;

  return flags;
}

gfx::Rect Label::GetAvailableRect() const {
  gfx::Rect bounds(size());
  bounds.Inset(GetInsets());
  return bounds;
}

void Label::CalculateDrawStringParams(string16* paint_text,
                                      gfx::Rect* text_bounds,
                                      int* flags) const {
  DCHECK(paint_text && text_bounds && flags);

  // TODO(msw): Use ElideRectangleText to support eliding multi-line text.
  if (elide_behavior_ == ELIDE_AS_EMAIL) {
    *paint_text = ui::ElideEmail(text_, font_, GetAvailableRect().width());
  } else if (elide_behavior_ == ELIDE_IN_MIDDLE) {
    *paint_text = ui::ElideText(text_, font_, GetAvailableRect().width(),
                                ui::ELIDE_IN_MIDDLE);
  } else if (elide_behavior_ == ELIDE_AT_END) {
    *paint_text = ui::ElideText(text_, font_, GetAvailableRect().width(),
                                ui::ELIDE_AT_END);
  } else {
    *paint_text = text_;
  }

  *text_bounds = GetTextBounds();
  *flags = ComputeDrawStringFlags();
}

void Label::UpdateColorsFromTheme(const ui::NativeTheme* theme) {
  if (!enabled_color_set_) {
    requested_enabled_color_ = theme->GetSystemColor(
        ui::NativeTheme::kColorId_LabelEnabledColor);
  }
  if (!disabled_color_set_) {
    requested_disabled_color_ = theme->GetSystemColor(
        ui::NativeTheme::kColorId_LabelDisabledColor);
  }
  if (!background_color_set_) {
    background_color_ = theme->GetSystemColor(
        ui::NativeTheme::kColorId_LabelBackgroundColor);
  }
  RecalculateColors();
}

void Label::ResetCachedSize() {
  text_size_valid_ = false;
  cached_heights_cursor_ = 0;
  for (int i = 0; i < kCachedSizeLimit; ++i)
    cached_heights_[i] = gfx::Size();
}

bool Label::ShouldShowDefaultTooltip() const {
  return !is_multi_line_ &&
      font_.GetStringWidth(text_) > GetAvailableRect().width();
}

}  // namespace views
