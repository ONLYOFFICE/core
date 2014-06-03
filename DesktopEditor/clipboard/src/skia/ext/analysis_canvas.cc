// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/debug/trace_event.h"
#include "skia/ext/analysis_canvas.h"
#include "third_party/skia/include/core/SkDevice.h"
#include "third_party/skia/include/core/SkDraw.h"
#include "third_party/skia/include/core/SkRRect.h"
#include "third_party/skia/include/core/SkShader.h"
#include "third_party/skia/src/core/SkRasterClip.h"
#include "ui/gfx/rect_conversions.h"

namespace {

bool isSolidColorPaint(const SkPaint& paint) {
  SkXfermode::Mode xferMode;

  // getXfermode can return a NULL, but that is handled
  // gracefully by AsMode (NULL turns into kSrcOver mode).
  SkXfermode::AsMode(paint.getXfermode(), &xferMode);

  // Paint is solid color if the following holds:
  // - Alpha is 1.0, style is fill, and there are no special effects
  // - Xfer mode is either kSrc or kSrcOver (kSrcOver is equivalent
  //   to kSrc if source alpha is 1.0, which is already checked).
  return (paint.getAlpha() == 255 &&
          !paint.getShader() &&
          !paint.getLooper() &&
          !paint.getMaskFilter() &&
          !paint.getColorFilter() &&
          paint.getStyle() == SkPaint::kFill_Style &&
          (xferMode == SkXfermode::kSrc_Mode ||
           xferMode == SkXfermode::kSrcOver_Mode));
}

bool isFullQuad(const SkDraw& draw,
                const SkRect& canvasRect,
                const SkRect& drawnRect) {

  // If the transform results in a non-axis aligned
  // rect, then be conservative and return false.
  if (!draw.fMatrix->rectStaysRect())
    return false;

  SkRect drawBitmapRect;
  draw.fBitmap->getBounds(&drawBitmapRect);
  SkRect clipRect = SkRect::Make(draw.fRC->getBounds());
  SkRect deviceRect;
  draw.fMatrix->mapRect(&deviceRect, drawnRect);

  // The drawn rect covers the full canvas, if the following conditions hold:
  // - Clip rect is an actual rectangle.
  // - The rect we're drawing (post-transform) contains the clip rect.
  //   That is, all of clip rect will be colored by the rect.
  // - Clip rect contains the canvas rect.
  //   That is, we're not clipping to a portion of this canvas.
  // - The bitmap into which the draw call happens is at least as
  //   big as the canvas rect
  return draw.fRC->isRect() &&
         deviceRect.contains(clipRect) &&
         clipRect.contains(canvasRect) &&
         drawBitmapRect.contains(canvasRect);
}

} // namespace

namespace skia {

AnalysisDevice::AnalysisDevice(const SkBitmap& bm)
  : INHERITED(bm)
  , isForcedNotSolid_(false)
  , isForcedNotTransparent_(false)
  , isSolidColor_(false)
  , isTransparent_(false)
  , hasText_(false) {
}

AnalysisDevice::~AnalysisDevice() {

}

bool AnalysisDevice::getColorIfSolid(SkColor* color) const {
  if (isTransparent_) {
    *color = SK_ColorTRANSPARENT;
    return true;
  }
  if (isSolidColor_) {
    *color = color_;
    return true;
  }
  return false;
}

bool AnalysisDevice::hasText() const {
  return hasText_;
}

void AnalysisDevice::setForceNotSolid(bool flag) {
  isForcedNotSolid_ = flag;
  if (isForcedNotSolid_)
    isSolidColor_ = false;
}

void AnalysisDevice::setForceNotTransparent(bool flag) {
  isForcedNotTransparent_ = flag;
  if (isForcedNotTransparent_)
    isTransparent_ = false;
}

void AnalysisDevice::clear(SkColor color) {
  isTransparent_ = (!isForcedNotTransparent_ && SkColorGetA(color) == 0);
  hasText_ = false;

  if (!isForcedNotSolid_ && SkColorGetA(color) == 255) {
    isSolidColor_ = true;
    color_ = color;
  }
  else {
    isSolidColor_ = false;
  }
}

void AnalysisDevice::drawPaint(const SkDraw&, const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawPoints(const SkDraw&, SkCanvas::PointMode mode,
                          size_t count, const SkPoint[],
                          const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawRect(const SkDraw& draw, const SkRect& rect,
                        const SkPaint& paint) {
  bool doesCoverCanvas = isFullQuad(draw,
                                    SkRect::MakeWH(width(), height()),
                                    rect);

  SkXfermode::Mode xferMode;
  SkXfermode::AsMode(paint.getXfermode(), &xferMode);

  // This canvas will become transparent if the following holds:
  // - The quad is a full tile quad
  // - We're not in "forced not transparent" mode
  // - Transfer mode is clear (0 color, 0 alpha)
  //
  // If the paint alpha is not 0, or if the transfrer mode is
  // not src, then this canvas will not be transparent.
  //
  // In all other cases, we keep the current transparent value
  if (doesCoverCanvas &&
      !isForcedNotTransparent_ &&
      xferMode == SkXfermode::kClear_Mode) {
    isTransparent_ = true;
    hasText_ = false;
  }
  else if (paint.getAlpha() != 0 ||
           xferMode != SkXfermode::kSrc_Mode) {
    isTransparent_ = false;
  }

  // This bitmap is solid if and only if the following holds.
  // Note that this might be overly conservative:
  // - We're not in "forced not solid" mode
  // - Paint is solid color
  // - The quad is a full tile quad
  if (!isForcedNotSolid_ &&
      isSolidColorPaint(paint) &&
      doesCoverCanvas) {
    isSolidColor_ = true;
    color_ = paint.getColor();
    hasText_ = false;
  }
  else {
    isSolidColor_ = false;
  }
}

void AnalysisDevice::drawOval(const SkDraw&, const SkRect& oval,
                        const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawPath(const SkDraw&, const SkPath& path,
                        const SkPaint& paint,
                        const SkMatrix* prePathMatrix,
                        bool pathIsMutable ) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawBitmap(const SkDraw&, const SkBitmap& bitmap,
                          const SkIRect* srcRectOrNull,
                          const SkMatrix& matrix, const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawSprite(const SkDraw&, const SkBitmap& bitmap,
                          int x, int y, const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawBitmapRect(const SkDraw& draw, const SkBitmap& bitmap,
                              const SkRect* srcOrNull, const SkRect& dst,
                              const SkPaint& paint) {
  // Call drawRect to determine transparency,
  // but reset solid color to false.
  drawRect(draw, dst, paint);
  isSolidColor_ = false;
}


void AnalysisDevice::drawText(const SkDraw&, const void* text, size_t len,
                        SkScalar x, SkScalar y, const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
  hasText_ = true;
}

void AnalysisDevice::drawPosText(const SkDraw& draw, const void* text,
                           size_t len,
                           const SkScalar pos[], SkScalar constY,
                           int scalarsPerPos, const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
  hasText_ = true;
}

void AnalysisDevice::drawTextOnPath(const SkDraw&, const void* text,
                              size_t len,
                              const SkPath& path, const SkMatrix* matrix,
                              const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
  hasText_ = true;
}

#ifdef SK_BUILD_FOR_ANDROID
void AnalysisDevice::drawPosTextOnPath(const SkDraw& draw, const void* text,
                                 size_t len,
                                 const SkPoint pos[], const SkPaint& paint,
                                 const SkPath& path, const SkMatrix* matrix) {
  isSolidColor_ = false;
  isTransparent_ = false;
  hasText_ = true;
}
#endif

void AnalysisDevice::drawVertices(const SkDraw&, SkCanvas::VertexMode,
                            int vertexCount,
                            const SkPoint verts[], const SkPoint texs[],
                            const SkColor colors[], SkXfermode* xmode,
                            const uint16_t indices[], int indexCount,
                            const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}

void AnalysisDevice::drawDevice(const SkDraw&, SkDevice*, int x, int y,
                          const SkPaint& paint) {
  isSolidColor_ = false;
  isTransparent_ = false;
}


const int AnalysisCanvas::kNoLayer = -1;

AnalysisCanvas::AnalysisCanvas(AnalysisDevice* device)
  : INHERITED(device)
  , savedStackSize_(0)
  , forceNotSolidStackLevel_(kNoLayer)
  , forceNotTransparentStackLevel_(kNoLayer) {
}

AnalysisCanvas::~AnalysisCanvas() {
}


bool AnalysisCanvas::getColorIfSolid(SkColor* color) const {
  return (static_cast<AnalysisDevice*>(getDevice()))->getColorIfSolid(color);
}

bool AnalysisCanvas::hasText() const {
  return (static_cast<AnalysisDevice*>(getDevice()))->hasText();
}

bool AnalysisCanvas::clipRect(const SkRect& rect, SkRegion::Op op,
                        bool doAA) {
  return INHERITED::clipRect(rect, op, doAA);
}

bool AnalysisCanvas::clipPath(const SkPath& path, SkRegion::Op op,
                        bool doAA) {
  // clipPaths can make our calls to isFullQuad invalid (ie have false
  // positives). As a precaution, force the setting to be non-solid
  // and non-transparent until we pop this 
  if (forceNotSolidStackLevel_ == kNoLayer) {
    forceNotSolidStackLevel_ = savedStackSize_;
    (static_cast<AnalysisDevice*>(getDevice()))->setForceNotSolid(true);
  }
  if (forceNotTransparentStackLevel_ == kNoLayer) {
    forceNotTransparentStackLevel_ = savedStackSize_;
    (static_cast<AnalysisDevice*>(getDevice()))->setForceNotTransparent(true);
  }

  return INHERITED::clipRect(path.getBounds(), op, doAA);
}

bool AnalysisCanvas::clipRRect(const SkRRect& rrect, SkRegion::Op op,
                         bool doAA) {
  // clipRRect can make our calls to isFullQuad invalid (ie have false
  // positives). As a precaution, force the setting to be non-solid
  // and non-transparent until we pop this 
  if (forceNotSolidStackLevel_ == kNoLayer) {
    forceNotSolidStackLevel_ = savedStackSize_;
    (static_cast<AnalysisDevice*>(getDevice()))->setForceNotSolid(true);
  }
  if (forceNotTransparentStackLevel_ == kNoLayer) {
    forceNotTransparentStackLevel_ = savedStackSize_;
    (static_cast<AnalysisDevice*>(getDevice()))->setForceNotTransparent(true);
  }

  return INHERITED::clipRect(rrect.getBounds(), op, doAA);
}

int AnalysisCanvas::save(SkCanvas::SaveFlags flags) {
  ++savedStackSize_;
  return INHERITED::save(flags);
}

int AnalysisCanvas::saveLayer(const SkRect* bounds, const SkPaint* paint, 
                              SkCanvas::SaveFlags flags) {
  ++savedStackSize_;

  // If after we draw to the saved layer, we have to blend with the current
  // layer, then we can conservatively say that the canvas will not be of
  // solid color.
  if ((paint && !isSolidColorPaint(*paint)) ||
      (bounds && !bounds->contains(
          SkRect::MakeWH(getDevice()->width(), getDevice()->height())))) {
    if (forceNotSolidStackLevel_ == kNoLayer) {
      forceNotSolidStackLevel_ = savedStackSize_;
      (static_cast<AnalysisDevice*>(getDevice()))->setForceNotSolid(true);
    }
  }

  // If after we draw to the save layer, we have to blend with the current
  // layer using any part of the current layer's alpha, then we can
  // conservatively say that the canvas will not be transparent.
  SkXfermode::Mode xferMode = SkXfermode::kSrc_Mode;
  if (paint)
    SkXfermode::AsMode(paint->getXfermode(), &xferMode);
  if (xferMode != SkXfermode::kSrc_Mode) {
    if (forceNotTransparentStackLevel_ == kNoLayer) {
      forceNotTransparentStackLevel_ = savedStackSize_;
      (static_cast<AnalysisDevice*>(getDevice()))->setForceNotTransparent(true);
    }
  }

  // Actually saving a layer here could cause a new bitmap to be created
  // and real rendering to occur.
  int count = INHERITED::save(flags);
  if (bounds) {
    INHERITED::clipRectBounds(bounds, flags, NULL);
  }
  return count;
}

void AnalysisCanvas::restore() {
  INHERITED::restore();

  DCHECK(savedStackSize_);
  if (savedStackSize_) {
    --savedStackSize_;
    if (savedStackSize_ < forceNotSolidStackLevel_) {
      (static_cast<AnalysisDevice*>(getDevice()))->setForceNotSolid(false);
      forceNotSolidStackLevel_ = kNoLayer;
    }
    if (savedStackSize_ < forceNotTransparentStackLevel_) {
      (static_cast<AnalysisDevice*>(getDevice()))->setForceNotTransparent(false);
      forceNotTransparentStackLevel_ = kNoLayer;
    }
  }
}

}  // namespace skia


