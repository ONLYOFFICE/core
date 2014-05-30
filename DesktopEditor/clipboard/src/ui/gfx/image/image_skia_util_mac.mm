// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_skia_util_mac.h"

#include <cmath>
#include <limits>

#import <AppKit/AppKit.h>

#include "base/mac/mac_util.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "skia/ext/skia_utils_mac.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image_skia.h"

namespace {

// Returns NSImageRep whose pixel size most closely matches |desired_size|.
NSImageRep* GetNSImageRepWithPixelSize(NSImage* image,
                                       NSSize desired_size) {
  float smallest_diff = std::numeric_limits<float>::max();
  NSImageRep* closest_match = nil;
  for (NSImageRep* image_rep in [image representations]) {
    float diff = std::abs(desired_size.width - [image_rep pixelsWide]) +
        std::abs(desired_size.height - [image_rep pixelsHigh]);
    if (diff < smallest_diff) {
      smallest_diff = diff;
      closest_match = image_rep;
    }
  }
  return closest_match;
}

// Returns true if NSImage has no representations
bool IsNSImageEmpty(NSImage* image) {
  return ([image representations].count == 0);
}

}  // namespace

namespace gfx {

gfx::ImageSkia ImageSkiaFromNSImage(NSImage* image) {
  return ImageSkiaFromResizedNSImage(image, [image size]);
}

gfx::ImageSkia ImageSkiaFromResizedNSImage(NSImage* image,
                                           NSSize desired_size) {
  // Resize and convert to ImageSkia simultaneously to save on computation.
  // TODO(pkotwicz): Separate resizing NSImage and converting to ImageSkia.
  // Convert to ImageSkia by finding the most appropriate NSImageRep for
  // each supported scale factor and resizing if necessary.

  if (IsNSImageEmpty(image))
    return gfx::ImageSkia();

  std::vector<ui::ScaleFactor> supported_scale_factors =
      ui::GetSupportedScaleFactors();

  gfx::ImageSkia image_skia;
  for (size_t i = 0; i < supported_scale_factors.size(); ++i) {
    float scale = ui::GetScaleFactorScale(supported_scale_factors[i]);
    NSSize desired_size_for_scale = NSMakeSize(desired_size.width * scale,
                                               desired_size.height * scale);
    NSImageRep* ns_image_rep = GetNSImageRepWithPixelSize(image,
        desired_size_for_scale);

    SkBitmap bitmap(gfx::NSImageRepToSkBitmap(ns_image_rep,
        desired_size_for_scale, false));
    if (bitmap.isNull())
      continue;

    image_skia.AddRepresentation(gfx::ImageSkiaRep(bitmap,
        supported_scale_factors[i]));
  }
  return image_skia;
}

gfx::ImageSkia ApplicationIconAtSize(int desired_size) {
  NSImage* image = [NSImage imageNamed:@"NSApplicationIcon"];
  return ImageSkiaFromResizedNSImage(image,
                                     NSMakeSize(desired_size, desired_size));
}

NSImage* NSImageFromImageSkia(const gfx::ImageSkia& image_skia) {
  if (image_skia.isNull())
    return nil;

  scoped_nsobject<NSImage> image([[NSImage alloc] init]);
  image_skia.EnsureRepsForSupportedScaleFactors();
  std::vector<gfx::ImageSkiaRep> image_reps = image_skia.image_reps();
  for (std::vector<gfx::ImageSkiaRep>::const_iterator it = image_reps.begin();
       it != image_reps.end(); ++it) {
    [image addRepresentation:
        gfx::SkBitmapToNSBitmapImageRep(it->sk_bitmap())];
  }

  [image setSize:NSMakeSize(image_skia.width(), image_skia.height())];
  return [image.release() autorelease];
}

NSImage* NSImageFromImageSkiaWithColorSpace(const gfx::ImageSkia& image_skia,
                                            CGColorSpaceRef color_space) {
  if (image_skia.isNull())
    return nil;

  scoped_nsobject<NSImage> image([[NSImage alloc] init]);
  image_skia.EnsureRepsForSupportedScaleFactors();
  std::vector<gfx::ImageSkiaRep> image_reps = image_skia.image_reps();
  for (std::vector<gfx::ImageSkiaRep>::const_iterator it = image_reps.begin();
       it != image_reps.end(); ++it) {
    [image addRepresentation:
        gfx::SkBitmapToNSBitmapImageRepWithColorSpace(it->sk_bitmap(),
                                                      color_space)];
  }

  [image setSize:NSMakeSize(image_skia.width(), image_skia.height())];
  return [image.release() autorelease];
}

}  // namespace gfx
