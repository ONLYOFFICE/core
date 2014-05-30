// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/painter.h"

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/effects/SkGradientShader.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_operations.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace views {

namespace {

class GradientPainter : public Painter {
 public:
  GradientPainter(bool horizontal,
                  SkColor* colors,
                  SkScalar* pos,
                  size_t count)
      : horizontal_(horizontal),
        count_(count) {
    pos_.reset(new SkScalar[count_]);
    colors_.reset(new SkColor[count_]);

    for (size_t i = 0; i < count_; ++i) {
      pos_[i] = pos[i];
      colors_[i] = colors[i];
    }
  }

  virtual ~GradientPainter() {}

  // Overridden from Painter:
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) OVERRIDE {
    SkPaint paint;
    SkPoint p[2];
    p[0].iset(0, 0);
    if (horizontal_)
      p[1].iset(size.width(), 0);
    else
      p[1].iset(0, size.height());

    skia::RefPtr<SkShader> s = skia::AdoptRef(SkGradientShader::CreateLinear(
        p, colors_.get(), pos_.get(), count_, SkShader::kClamp_TileMode, NULL));
    paint.setStyle(SkPaint::kFill_Style);
    paint.setShader(s.get());
    // Need to unref shader, otherwise never deleted.

    canvas->sk_canvas()->drawRectCoords(SkIntToScalar(0), SkIntToScalar(0),
                                        SkIntToScalar(size.width()),
                                        SkIntToScalar(size.height()), paint);
  }

 private:
  // If |horizontal_| is true then the gradiant is painted horizontally.
  bool horizontal_;
  // The gradient colors.
  scoped_ptr<SkColor[]> colors_;
  // The relative positions of the corresponding gradient colors.
  scoped_ptr<SkScalar[]> pos_;
  // The number of elements in |colors_| and |pos_|.
  size_t count_;

  DISALLOW_COPY_AND_ASSIGN(GradientPainter);
};

// A helper fuction to stretch the given image over the specified canvas area.
void Fill(gfx::Canvas* c, const gfx::ImageSkia& i, int x, int y, int w, int h) {
  c->DrawImageInt(i, 0, 0, i.width(), i.height(), x, y, w, h, false);
}

// ImagePainter stores and paints nine images as a scalable grid.
class VIEWS_EXPORT ImagePainter : public Painter {
 public:
  // Construct an ImagePainter with the specified image resource ids.
  // See CreateImageGridPainter()'s comment regarding image ID count and order.
  explicit ImagePainter(const int image_ids[]);
  // Construct an ImagePainter with the specified image and insets.
  ImagePainter(const gfx::ImageSkia& image, const gfx::Insets& insets);

  virtual ~ImagePainter();

  // Returns true if the images are empty.
  bool IsEmpty() const;

  // Overridden from Painter:
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) OVERRIDE;

 private:
  // Images must share widths by column and heights by row as depicted below.
  // Coordinates along the X and Y axes are used for construction and painting.
  //     x0     x1     x2     x3
  // y0__|______|______|______|
  // y1__|__i0__|__i1__|__i2__|
  // y2__|__i3__|__i4__|__i5__|
  // y3__|__i6__|__i7__|__i8__|
  gfx::ImageSkia images_[9];

  DISALLOW_COPY_AND_ASSIGN(ImagePainter);
};

ImagePainter::ImagePainter(const int image_ids[]) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  for (size_t i = 0; i < 9; ++i)
    if (image_ids[i] != 0)
      images_[i] = *rb.GetImageSkiaNamed(image_ids[i]);
}

ImagePainter::ImagePainter(const gfx::ImageSkia& image,
                           const gfx::Insets& insets) {
  DCHECK_GE(image.width(), insets.width());
  DCHECK_GE(image.height(), insets.height());

  // Extract subsets of the original image to match the |images_| format.
  const int x[] =
      { 0, insets.left(), image.width() - insets.right(), image.width() };
  const int y[] =
      { 0, insets.top(), image.height() - insets.bottom(), image.height() };

  for (size_t j = 0; j < 3; ++j) {
    for (size_t i = 0; i < 3; ++i) {
      images_[i + j * 3] = gfx::ImageSkiaOperations::ExtractSubset(image,
          gfx::Rect(x[i], y[j], x[i + 1] - x[i], y[j + 1] - y[j]));
    }
  }
}

ImagePainter::~ImagePainter() {
}

bool ImagePainter::IsEmpty() const {
  return images_[0].isNull();
}

void ImagePainter::Paint(gfx::Canvas* canvas, const gfx::Size& size) {
  if (IsEmpty())
    return;

  // Paint image subsets in accordance with the |images_| format.
  const gfx::Rect rect(size);
  const int x[] = { rect.x(), rect.x() + images_[0].width(),
                    rect.right() - images_[2].width(), rect.right() };
  const int y[] = { rect.y(), rect.y() + images_[0].height(),
                    rect.bottom() - images_[6].height(), rect.bottom() };

  canvas->DrawImageInt(images_[0], x[0], y[0]);
  Fill(canvas, images_[1], x[1], y[0], x[2] - x[1], y[1] - y[0]);
  canvas->DrawImageInt(images_[2], x[2], y[0]);
  Fill(canvas, images_[3], x[0], y[1], x[1] - x[0], y[2] - y[1]);
  if (!images_[4].isNull())
    Fill(canvas, images_[4], x[1], y[1], x[2] - x[1], y[2] - y[1]);
  Fill(canvas, images_[5], x[2], y[1], x[3] - x[2], y[2] - y[1]);
  canvas->DrawImageInt(images_[6], 0, y[2]);
  Fill(canvas, images_[7], x[1], y[2], x[2] - x[1], y[3] - y[2]);
  canvas->DrawImageInt(images_[8], x[2], y[2]);
}

}  // namespace

// static
void Painter::PaintPainterAt(gfx::Canvas* canvas,
                             Painter* painter,
                             const gfx::Rect& rect) {
  DCHECK(canvas && painter);
  canvas->Save();
  canvas->Translate(rect.OffsetFromOrigin());
  painter->Paint(canvas, rect.size());
  canvas->Restore();
}

// static
Painter* Painter::CreateHorizontalGradient(SkColor c1, SkColor c2) {
  SkColor colors[2];
  colors[0] = c1;
  colors[1] = c2;
  SkScalar pos[] = {0, 1};
  return new GradientPainter(true, colors, pos, 2);
}

// static
Painter* Painter::CreateVerticalGradient(SkColor c1, SkColor c2) {
  SkColor colors[2];
  colors[0] = c1;
  colors[1] = c2;
  SkScalar pos[] = {0, 1};
  return new GradientPainter(false, colors, pos, 2);
}

// static
Painter* Painter::CreateVerticalMultiColorGradient(SkColor* colors,
                                                   SkScalar* pos,
                                                   size_t count) {
  return new GradientPainter(false, colors, pos, count);
}

// static
Painter* Painter::CreateImagePainter(const gfx::ImageSkia& image,
                                     const gfx::Insets& insets) {
  return new ImagePainter(image, insets);
}

// static
Painter* Painter::CreateImageGridPainter(const int image_ids[]) {
  return new ImagePainter(image_ids);
}

HorizontalPainter::HorizontalPainter(const int image_resource_names[]) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  for (int i = 0; i < 3; ++i)
    images_[i] = rb.GetImageNamed(image_resource_names[i]).ToImageSkia();
  height_ = images_[LEFT]->height();
  DCHECK(images_[LEFT]->height() == images_[RIGHT]->height() &&
         images_[LEFT]->height() == images_[CENTER]->height());
}

void HorizontalPainter::Paint(gfx::Canvas* canvas, const gfx::Size& size) {
  if (size.width() < (images_[LEFT]->width() + images_[CENTER]->width() +
      images_[RIGHT]->width())) {
    // No room to paint.
    return;
  }
  canvas->DrawImageInt(*images_[LEFT], 0, 0);
  canvas->DrawImageInt(*images_[RIGHT],
                       size.width() - images_[RIGHT]->width(), 0);
  canvas->TileImageInt(*images_[CENTER], images_[LEFT]->width(), 0,
      size.width() - images_[LEFT]->width() - images_[RIGHT]->width(), height_);
}

}  // namespace views
