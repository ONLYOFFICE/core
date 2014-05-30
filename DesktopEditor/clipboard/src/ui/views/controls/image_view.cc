// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/image_view.h"

#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/insets.h"

namespace views {

ImageView::ImageView()
    : image_size_set_(false),
      horiz_alignment_(CENTER),
      vert_alignment_(CENTER),
      interactive_(true) {
}

ImageView::~ImageView() {
}

void ImageView::SetImage(const gfx::ImageSkia& img) {
  gfx::Size pref_size(GetPreferredSize());
  image_ = img;
  if (pref_size != GetPreferredSize())
    PreferredSizeChanged();
  SchedulePaint();
}

void ImageView::SetImage(const gfx::ImageSkia* image_skia) {
  if (image_skia) {
    SetImage(*image_skia);
  } else {
    gfx::ImageSkia t;
    SetImage(t);
  }
}

const gfx::ImageSkia& ImageView::GetImage() {
  return image_;
}

void ImageView::SetImageSize(const gfx::Size& image_size) {
  image_size_set_ = true;
  image_size_ = image_size;
  PreferredSizeChanged();
}

bool ImageView::GetImageSize(gfx::Size* image_size) {
  DCHECK(image_size);
  if (image_size_set_)
    *image_size = image_size_;
  return image_size_set_;
}

gfx::Rect ImageView::GetImageBounds() const {
  gfx::Size image_size(image_size_set_ ?
    image_size_ : gfx::Size(image_.width(), image_.height()));
  return gfx::Rect(ComputeImageOrigin(image_size), image_size);
}

void ImageView::ResetImageSize() {
  image_size_set_ = false;
}

gfx::Size ImageView::GetPreferredSize() {
  gfx::Insets insets = GetInsets();
  if (image_size_set_) {
    gfx::Size image_size;
    GetImageSize(&image_size);
    image_size.Enlarge(insets.width(), insets.height());
    return image_size;
  }
  return gfx::Size(image_.width() + insets.width(),
                   image_.height() + insets.height());
}

gfx::Point ImageView::ComputeImageOrigin(const gfx::Size& image_size) const {
  gfx::Insets insets = GetInsets();

  int x;
  // In order to properly handle alignment of images in RTL locales, we need
  // to flip the meaning of trailing and leading. For example, if the
  // horizontal alignment is set to trailing, then we'll use left alignment for
  // the image instead of right alignment if the UI layout is RTL.
  Alignment actual_horiz_alignment = horiz_alignment_;
  if (base::i18n::IsRTL() && (horiz_alignment_ != CENTER))
    actual_horiz_alignment = (horiz_alignment_ == LEADING) ? TRAILING : LEADING;
  switch (actual_horiz_alignment) {
    case LEADING:  x = insets.left();                                 break;
    case TRAILING: x = width() - insets.right() - image_size.width(); break;
    case CENTER:   x = (width() - image_size.width()) / 2;            break;
    default:       NOTREACHED(); x = 0;                               break;
  }

  int y;
  switch (vert_alignment_) {
    case LEADING:  y = insets.top();                                     break;
    case TRAILING: y = height() - insets.bottom() - image_size.height(); break;
    case CENTER:   y = (height() - image_size.height()) / 2;             break;
    default:       NOTREACHED(); y = 0;                                  break;
  }

  return gfx::Point(x, y);
}

void ImageView::OnPaint(gfx::Canvas* canvas) {
  View::OnPaint(canvas);

  if (image_.isNull())
    return;

  gfx::Rect image_bounds(GetImageBounds());
  if (image_bounds.IsEmpty())
    return;

  if (image_bounds.size() != gfx::Size(image_.width(), image_.height())) {
    // Resize case
    SkPaint paint;
    paint.setFilterBitmap(true);
    canvas->DrawImageInt(image_, 0, 0, image_.width(), image_.height(),
        image_bounds.x(), image_bounds.y(), image_bounds.width(),
        image_bounds.height(), true, paint);
  } else {
    canvas->DrawImageInt(image_, image_bounds.x(), image_bounds.y());
  }
}

void ImageView::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_GRAPHIC;
  state->name = tooltip_text_;
}

void ImageView::SetHorizontalAlignment(Alignment ha) {
  if (ha != horiz_alignment_) {
    horiz_alignment_ = ha;
    SchedulePaint();
  }
}

ImageView::Alignment ImageView::GetHorizontalAlignment() const {
  return horiz_alignment_;
}

void ImageView::SetVerticalAlignment(Alignment va) {
  if (va != vert_alignment_) {
    vert_alignment_ = va;
    SchedulePaint();
  }
}

ImageView::Alignment ImageView::GetVerticalAlignment() const {
  return vert_alignment_;
}

void ImageView::SetTooltipText(const string16& tooltip) {
  tooltip_text_ = tooltip;
}

string16 ImageView::GetTooltipText() const {
  return tooltip_text_;
}

bool ImageView::GetTooltipText(const gfx::Point& p, string16* tooltip) const {
  if (tooltip_text_.empty())
    return false;

  *tooltip = GetTooltipText();
  return true;
}

bool ImageView::HitTestRect(const gfx::Rect& rect) const {
  return interactive_ ? View::HitTestRect(rect) : false;
}

}  // namespace views
