// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_skia_util_ios.h"

#include <UIKit/UIKit.h>

#include "base/logging.h"
#include "base/mac/scoped_cftyperef.h"
#include "skia/ext/skia_utils_ios.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image_skia.h"

namespace gfx {

gfx::ImageSkia ImageSkiaFromUIImage(UIImage* image) {
  gfx::ImageSkia image_skia;
  gfx::ImageSkiaRep image_skia_rep = ImageSkiaRepOfScaleFactorFromUIImage(
      image, ui::GetMaxScaleFactor());
  if (!image_skia_rep.is_null())
    image_skia.AddRepresentation(image_skia_rep);
  return image_skia;
}

gfx::ImageSkiaRep ImageSkiaRepOfScaleFactorFromUIImage(
    UIImage* image,
    ui::ScaleFactor scale_factor) {
  if (!image)
    return gfx::ImageSkiaRep();

  float scale = ui::GetScaleFactorScale(scale_factor);
  CGSize size = image.size;
  CGSize desired_size_for_scale =
      CGSizeMake(size.width * scale, size.height * scale);
  SkBitmap bitmap(gfx::CGImageToSkBitmap(image.CGImage,
                                         desired_size_for_scale,
                                         false));
  return gfx::ImageSkiaRep(bitmap, scale_factor);
}

UIImage* UIImageFromImageSkia(const gfx::ImageSkia& image_skia) {
  return UIImageFromImageSkiaRep(image_skia.GetRepresentation(
      ui::GetMaxScaleFactor()));
}

UIImage* UIImageFromImageSkiaRep(const gfx::ImageSkiaRep& image_skia_rep) {
  if (image_skia_rep.is_null())
    return nil;

  float scale = ui::GetScaleFactorScale(image_skia_rep.scale_factor());
  base::mac::ScopedCFTypeRef<CGColorSpaceRef> color_space(
      CGColorSpaceCreateDeviceRGB());
  return gfx::SkBitmapToUIImageWithColorSpace(image_skia_rep.sk_bitmap(), scale,
                                              color_space);
}

}  // namespace gfx
