// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "skia/ext/paint_simplifier.h"
#include "third_party/skia/include/core/SkPaint.h"

namespace skia {

PaintSimplifier::PaintSimplifier()
  : INHERITED() {

}

PaintSimplifier::~PaintSimplifier() {

}

bool PaintSimplifier::filter(SkPaint* paint, Type type) {

  // Preserve a modicum of text quality; black & white text is
  // just too blocky, even during a fling.
  if (type != kText_Type) {
    paint->setAntiAlias(false);
  }
  paint->setSubpixelText(false);
  paint->setLCDRenderText(false);

  paint->setFilterBitmap(false);
  paint->setMaskFilter(NULL);

  // Uncomment this line to shade simplified tiles pink during debugging.
  //paint->setColor(SkColorSetRGB(255, 127, 127));

  return true;
}


}  // namespace skia


