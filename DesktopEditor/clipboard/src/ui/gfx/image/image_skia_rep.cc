// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_skia_rep.h"

namespace gfx {

ImageSkiaRep::ImageSkiaRep()
    : scale_factor_(ui::SCALE_FACTOR_NONE) {
}

ImageSkiaRep::~ImageSkiaRep() {
}

ImageSkiaRep::ImageSkiaRep(const gfx::Size& size,
                           ui::ScaleFactor scale_factor)
    : scale_factor_(scale_factor) {
  float scale = ui::GetScaleFactorScale(scale_factor);
  bitmap_.setConfig(SkBitmap::kARGB_8888_Config,
                    static_cast<int>(size.width() * scale),
                    static_cast<int>(size.height() * scale));
  bitmap_.allocPixels();
}

ImageSkiaRep::ImageSkiaRep(const SkBitmap& src,
                           ui::ScaleFactor scale_factor)
    : bitmap_(src),
      scale_factor_(scale_factor) {
}

int ImageSkiaRep::GetWidth() const {
  return static_cast<int>(bitmap_.width() /
      ui::GetScaleFactorScale(scale_factor_));
}

int ImageSkiaRep::GetHeight() const {
  return static_cast<int>(bitmap_.height() /
      ui::GetScaleFactorScale(scale_factor_));
}

float ImageSkiaRep::GetScale() const {
  return ui::GetScaleFactorScale(scale_factor_);
}

}  // namespace gfx
