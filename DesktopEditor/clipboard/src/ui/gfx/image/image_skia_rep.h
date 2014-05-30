// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_SKIA_REP_H_
#define UI_GFX_IMAGE_IMAGE_SKIA_REP_H_

#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/layout.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/size.h"

namespace gfx {

// An ImageSkiaRep represents a bitmap and the scale factor it is intended for.
class UI_EXPORT ImageSkiaRep {
 public:
  // Create null bitmap.
  ImageSkiaRep();
  ~ImageSkiaRep();

  // Creates a bitmap with kARGB_8888_Config config with given |size| in DIP.
  // This allocates pixels in the bitmap.
  ImageSkiaRep(const gfx::Size& size, ui::ScaleFactor scale_factor);

  // Creates a bitmap with given scale factor.
  // Adds ref to |src|.
  ImageSkiaRep(const SkBitmap& src, ui::ScaleFactor scale_factor);

  // Returns true if the backing bitmap is null.
  bool is_null() const { return bitmap_.isNull(); }

  // Get width and height of bitmap in DIP.
  int GetWidth() const;
  int GetHeight() const;

  // Get width and height of bitmap in pixels.
  int pixel_width() const { return bitmap_.width(); }
  int pixel_height() const { return bitmap_.height(); }
  Size pixel_size() const {
    return Size(pixel_width(), pixel_height());
  }

  // Retrieves the scale that the bitmap will be painted at.
  float GetScale() const;
  ui::ScaleFactor scale_factor() const { return scale_factor_; }

  // Returns backing bitmap.
  const SkBitmap& sk_bitmap() const { return bitmap_; }

 private:
  friend class ImageSkia;
  SkBitmap& mutable_sk_bitmap() { return bitmap_; }

  SkBitmap bitmap_;
  ui::ScaleFactor scale_factor_;
};

}  // namespace gfx

#endif  // UI_GFX_IMAGE_IMAGE_SKIA_REP_H_
