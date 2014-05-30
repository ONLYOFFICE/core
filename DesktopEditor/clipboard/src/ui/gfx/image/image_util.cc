// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_util.h"

#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/codec/jpeg_codec.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"

namespace gfx {

// The iOS implementations of the JPEG functions are in image_util_ios.mm.
#if !defined(OS_IOS)
Image ImageFrom1xJPEGEncodedData(const unsigned char* input,
                                 size_t input_size) {
  scoped_ptr<SkBitmap> bitmap(gfx::JPEGCodec::Decode(input, input_size));
  if (bitmap.get())
    return Image::CreateFrom1xBitmap(*bitmap);

  return Image();
}

bool JPEG1xEncodedDataFromImage(const Image& image, int quality,
                              std::vector<unsigned char>* dst) {
  const gfx::ImageSkiaRep& image_skia_rep =
      image.AsImageSkia().GetRepresentation(ui::SCALE_FACTOR_100P);
  if (image_skia_rep.scale_factor() != ui::SCALE_FACTOR_100P)
    return false;

  const SkBitmap& bitmap = image_skia_rep.sk_bitmap();
  SkAutoLockPixels bitmap_lock(bitmap);

  if (!bitmap.readyToDraw())
    return false;

  return gfx::JPEGCodec::Encode(
          reinterpret_cast<unsigned char*>(bitmap.getAddr32(0, 0)),
          gfx::JPEGCodec::FORMAT_SkBitmap, bitmap.width(),
          bitmap.height(),
          static_cast<int>(bitmap.rowBytes()), quality,
          dst);
}
#endif  // !defined(OS_IOS)

}
