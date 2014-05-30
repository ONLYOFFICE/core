// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/canvas_skia_paint.h"
#include "ui/gfx/rect.h"

namespace {

#if !defined(USE_AURA)
class CanvasPaintWin : public gfx::CanvasPaint, public gfx::CanvasSkiaPaint {
 public:
  explicit CanvasPaintWin(gfx::NativeView view);
  virtual ~CanvasPaintWin();

  // Overridden from CanvasPaint:
  virtual bool IsValid() const OVERRIDE;
  virtual gfx::Rect GetInvalidRect() const OVERRIDE;
  virtual gfx::Canvas* AsCanvas() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(CanvasPaintWin);
};

CanvasPaintWin::CanvasPaintWin(gfx::NativeView view)
    : gfx::CanvasSkiaPaint(view) {}

CanvasPaintWin::~CanvasPaintWin() {}

bool CanvasPaintWin::IsValid() const {
  return isEmpty();
}

gfx::Rect CanvasPaintWin::GetInvalidRect() const {
  return gfx::Rect(paintStruct().rcPaint);
}

gfx::Canvas* CanvasPaintWin::AsCanvas() {
  return this;
}
#endif

}  // namespace

namespace gfx {

CanvasPaint* CanvasPaint::CreateCanvasPaint(gfx::NativeView view) {
#if !defined(USE_AURA)
  return new CanvasPaintWin(view);
#else
  return NULL;
#endif
}

}  // namespace gfx
