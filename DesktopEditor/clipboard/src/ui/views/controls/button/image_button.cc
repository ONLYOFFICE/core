// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/image_button.h"

#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image_skia_operations.h"
#include "ui/views/widget/widget.h"

namespace views {

static const int kDefaultWidth = 16;   // Default button width if no theme.
static const int kDefaultHeight = 14;  // Default button height if no theme.

////////////////////////////////////////////////////////////////////////////////
// ImageButton, public:

ImageButton::ImageButton(ButtonListener* listener)
    : CustomButton(listener),
      h_alignment_(ALIGN_LEFT),
      v_alignment_(ALIGN_TOP),
      preferred_size_(kDefaultWidth, kDefaultHeight) {
  // By default, we request that the gfx::Canvas passed to our View::OnPaint()
  // implementation is flipped horizontally so that the button's images are
  // mirrored when the UI directionality is right-to-left.
  EnableCanvasFlippingForRTLUI(true);
}

ImageButton::~ImageButton() {
}

const gfx::ImageSkia& ImageButton::GetImage(ButtonState state) const {
  return images_[state];
}

void ImageButton::SetImage(ButtonState state, const gfx::ImageSkia* image) {
  images_[state] = image ? *image : gfx::ImageSkia();
  PreferredSizeChanged();
}

void ImageButton::SetBackground(SkColor color,
                                const gfx::ImageSkia* image,
                                const gfx::ImageSkia* mask) {
  if (image == NULL || mask == NULL) {
    background_image_ = gfx::ImageSkia();
    return;
  }

  background_image_ = gfx::ImageSkiaOperations::CreateButtonBackground(color,
     *image, *mask);
}

void ImageButton::SetOverlayImage(const gfx::ImageSkia* image) {
  if (!image) {
    overlay_image_ = gfx::ImageSkia();
    return;
  }
  overlay_image_ = *image;
}

void ImageButton::SetImageAlignment(HorizontalAlignment h_align,
                                    VerticalAlignment v_align) {
  h_alignment_ = h_align;
  v_alignment_ = v_align;
  SchedulePaint();
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton, View overrides:

gfx::Size ImageButton::GetPreferredSize() {
  gfx::Size size = preferred_size_;
  if (!images_[STATE_NORMAL].isNull()) {
    size = gfx::Size(images_[STATE_NORMAL].width(),
                     images_[STATE_NORMAL].height());
  }

  gfx::Insets insets = GetInsets();
  size.Enlarge(insets.width(), insets.height());
  return size;
}

void ImageButton::OnPaint(gfx::Canvas* canvas) {
  // Call the base class first to paint any background/borders.
  View::OnPaint(canvas);

  gfx::ImageSkia img = GetImageToPaint();

  if (!img.isNull()) {
    gfx::Point position = ComputeImagePaintPosition(img);
    if (!background_image_.isNull())
      canvas->DrawImageInt(background_image_, position.x(), position.y());

    canvas->DrawImageInt(img, position.x(), position.y());

    if (!overlay_image_.isNull())
      canvas->DrawImageInt(overlay_image_, position.x(), position.y());
  }
  OnPaintFocusBorder(canvas);
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton, protected:

gfx::ImageSkia ImageButton::GetImageToPaint() {
  gfx::ImageSkia img;

  if (!images_[STATE_HOVERED].isNull() && hover_animation_->is_animating()) {
    img = gfx::ImageSkiaOperations::CreateBlendedImage(images_[STATE_NORMAL],
        images_[STATE_HOVERED], hover_animation_->GetCurrentValue());
  } else {
    img = images_[state_];
  }

  return !img.isNull() ? img : images_[STATE_NORMAL];
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton, private:

gfx::Point ImageButton::ComputeImagePaintPosition(const gfx::ImageSkia& image) {
  int x = 0, y = 0;
  gfx::Rect rect = GetContentsBounds();

  if (h_alignment_ == ALIGN_CENTER)
    x = (rect.width() - image.width()) / 2;
  else if (h_alignment_ == ALIGN_RIGHT)
    x = rect.width() - image.width();

  if (v_alignment_ == ALIGN_MIDDLE)
    y = (rect.height() - image.height()) / 2;
  else if (v_alignment_ == ALIGN_BOTTOM)
    y = rect.height() - image.height();

  x += rect.x();
  y += rect.y();

  return gfx::Point(x, y);
}

////////////////////////////////////////////////////////////////////////////////
// ToggleImageButton, public:

ToggleImageButton::ToggleImageButton(ButtonListener* listener)
    : ImageButton(listener),
      toggled_(false) {
}

ToggleImageButton::~ToggleImageButton() {
}

void ToggleImageButton::SetToggled(bool toggled) {
  if (toggled == toggled_)
    return;

  for (int i = 0; i < STATE_COUNT; ++i) {
    gfx::ImageSkia temp = images_[i];
    images_[i] = alternate_images_[i];
    alternate_images_[i] = temp;
  }
  toggled_ = toggled;
  SchedulePaint();

  NotifyAccessibilityEvent(ui::AccessibilityTypes::EVENT_VALUE_CHANGED, true);
}

void ToggleImageButton::SetToggledImage(ButtonState state,
                                        const gfx::ImageSkia* image) {
  if (toggled_) {
    images_[state] = image ? *image : gfx::ImageSkia();
    if (state_ == state)
      SchedulePaint();
  } else {
    alternate_images_[state] = image ? *image : gfx::ImageSkia();
  }
}

void ToggleImageButton::SetToggledTooltipText(const string16& tooltip) {
  toggled_tooltip_text_ = tooltip;
}

////////////////////////////////////////////////////////////////////////////////
// ToggleImageButton, ImageButton overrides:

const gfx::ImageSkia& ToggleImageButton::GetImage(ButtonState state) const {
  if (toggled_)
    return alternate_images_[state];
  return images_[state];
}

void ToggleImageButton::SetImage(ButtonState state,
                                 const gfx::ImageSkia* image) {
  if (toggled_) {
    alternate_images_[state] = image ? *image : gfx::ImageSkia();
  } else {
    images_[state] = image ? *image : gfx::ImageSkia();
    if (state_ == state)
      SchedulePaint();
  }
  PreferredSizeChanged();
}

////////////////////////////////////////////////////////////////////////////////
// ToggleImageButton, View overrides:

bool ToggleImageButton::GetTooltipText(const gfx::Point& p,
                                       string16* tooltip) const {
  if (!toggled_ || toggled_tooltip_text_.empty())
    return Button::GetTooltipText(p, tooltip);

  *tooltip = toggled_tooltip_text_;
  return true;
}

void ToggleImageButton::GetAccessibleState(ui::AccessibleViewState* state) {
  ImageButton::GetAccessibleState(state);
  GetTooltipText(gfx::Point(), &state->name);
}

}  // namespace views
