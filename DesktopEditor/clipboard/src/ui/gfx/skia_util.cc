// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/skia_util.h"

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkColorFilter.h"
#include "third_party/skia/include/core/SkColorPriv.h"
#include "third_party/skia/include/core/SkUnPreMultiply.h"
#include "third_party/skia/include/effects/SkBlurMaskFilter.h"
#include "third_party/skia/include/effects/SkGradientShader.h"
#include "third_party/skia/include/effects/SkLayerDrawLooper.h"
#include "ui/gfx/image/image_skia_rep.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/shadow_value.h"

namespace gfx {

SkRect RectToSkRect(const Rect& rect) {
  SkRect r;
  r.iset(rect.x(), rect.y(), rect.right(), rect.bottom());
  return r;
}

SkIRect RectToSkIRect(const Rect& rect) {
  return SkIRect::MakeXYWH(rect.x(), rect.y(), rect.width(), rect.height());
}

Rect SkIRectToRect(const SkIRect& rect) {
  return Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

SkRect RectFToSkRect(const RectF& rect) {
  return SkRect::MakeXYWH(SkFloatToScalar(rect.x()),
                          SkFloatToScalar(rect.y()),
                          SkFloatToScalar(rect.width()),
                          SkFloatToScalar(rect.height()));
}

RectF SkRectToRectF(const SkRect& rect) {
  return RectF(SkScalarToFloat(rect.x()),
               SkScalarToFloat(rect.y()),
               SkScalarToFloat(rect.width()),
               SkScalarToFloat(rect.height()));
}


skia::RefPtr<SkShader> CreateImageRepShader(const gfx::ImageSkiaRep& image_rep,
                                            SkShader::TileMode tile_mode,
                                            const SkMatrix& local_matrix) {
  skia::RefPtr<SkShader> shader = skia::AdoptRef(SkShader::CreateBitmapShader(
      image_rep.sk_bitmap(), tile_mode, tile_mode));
  SkScalar scale_x = local_matrix.getScaleX();
  SkScalar scale_y = local_matrix.getScaleY();
  SkScalar bitmap_scale = SkFloatToScalar(image_rep.GetScale());

  // Unscale matrix by |bitmap_scale| such that the bitmap is drawn at the
  // correct density.
  // Convert skew and translation to pixel coordinates.
  // Thus, for |bitmap_scale| = 2:
  //   x scale = 2, x translation = 1 DIP,
  // should be converted to
  //   x scale = 1, x translation = 2 pixels.
  SkMatrix shader_scale = local_matrix;
  shader_scale.preScale(bitmap_scale, bitmap_scale);
  shader_scale.setScaleX(SkScalarDiv(scale_x, bitmap_scale));
  shader_scale.setScaleY(SkScalarDiv(scale_y, bitmap_scale));

  shader->setLocalMatrix(shader_scale);
  return shader;
}

skia::RefPtr<SkShader> CreateGradientShader(int start_point,
                                            int end_point,
                                            SkColor start_color,
                                            SkColor end_color) {
  SkColor grad_colors[2] = { start_color, end_color};
  SkPoint grad_points[2];
  grad_points[0].iset(0, start_point);
  grad_points[1].iset(0, end_point);

  return skia::AdoptRef(SkGradientShader::CreateLinear(
      grad_points, grad_colors, NULL, 2, SkShader::kRepeat_TileMode));
}

skia::RefPtr<SkDrawLooper> CreateShadowDrawLooper(
    const std::vector<ShadowValue>& shadows) {
  if (shadows.empty())
    return skia::RefPtr<SkDrawLooper>();

  skia::RefPtr<SkLayerDrawLooper> looper =
      skia::AdoptRef(new SkLayerDrawLooper);

  looper->addLayer();  // top layer of the original.

  SkLayerDrawLooper::LayerInfo layer_info;
  layer_info.fPaintBits |= SkLayerDrawLooper::kMaskFilter_Bit;
  layer_info.fPaintBits |= SkLayerDrawLooper::kColorFilter_Bit;
  layer_info.fColorMode = SkXfermode::kSrc_Mode;

  for (size_t i = 0; i < shadows.size(); ++i) {
    const ShadowValue& shadow = shadows[i];

    layer_info.fOffset.set(SkIntToScalar(shadow.x()),
                           SkIntToScalar(shadow.y()));

    // SkBlurMaskFilter's blur radius defines the range to extend the blur from
    // original mask, which is half of blur amount as defined in ShadowValue.
    skia::RefPtr<SkMaskFilter> blur_mask = skia::AdoptRef(
        SkBlurMaskFilter::Create(SkDoubleToScalar(shadow.blur() / 2),
                                 SkBlurMaskFilter::kNormal_BlurStyle,
                                 SkBlurMaskFilter::kHighQuality_BlurFlag));
    skia::RefPtr<SkColorFilter> color_filter = skia::AdoptRef(
        SkColorFilter::CreateModeFilter(shadow.color(),
                                        SkXfermode::kSrcIn_Mode));

    SkPaint* paint = looper->addLayer(layer_info);
    paint->setMaskFilter(blur_mask.get());
    paint->setColorFilter(color_filter.get());
  }

  return looper;
}

bool BitmapsAreEqual(const SkBitmap& bitmap1, const SkBitmap& bitmap2) {
  void* addr1 = NULL;
  void* addr2 = NULL;
  size_t size1 = 0;
  size_t size2 = 0;

  bitmap1.lockPixels();
  addr1 = bitmap1.getAddr32(0, 0);
  size1 = bitmap1.getSize();
  bitmap1.unlockPixels();

  bitmap2.lockPixels();
  addr2 = bitmap2.getAddr32(0, 0);
  size2 = bitmap2.getSize();
  bitmap2.unlockPixels();

  return (size1 == size2) && (0 == memcmp(addr1, addr2, bitmap1.getSize()));
}

void ConvertSkiaToRGBA(const unsigned char* skia,
                       int pixel_width,
                       unsigned char* rgba) {
  int total_length = pixel_width * 4;
  for (int i = 0; i < total_length; i += 4) {
    const uint32_t pixel_in = *reinterpret_cast<const uint32_t*>(&skia[i]);

    // Pack the components here.
    int alpha = SkGetPackedA32(pixel_in);
    if (alpha != 0 && alpha != 255) {
      SkColor unmultiplied = SkUnPreMultiply::PMColorToColor(pixel_in);
      rgba[i + 0] = SkColorGetR(unmultiplied);
      rgba[i + 1] = SkColorGetG(unmultiplied);
      rgba[i + 2] = SkColorGetB(unmultiplied);
      rgba[i + 3] = alpha;
    } else {
      rgba[i + 0] = SkGetPackedR32(pixel_in);
      rgba[i + 1] = SkGetPackedG32(pixel_in);
      rgba[i + 2] = SkGetPackedB32(pixel_in);
      rgba[i + 3] = alpha;
    }
  }
}

}  // namespace gfx
