// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/canvas.h"

#include "base/logging.h"
#include "ui/gfx/font.h"

namespace gfx {

// static
void Canvas::SizeStringInt(const string16& text,
                           const gfx::Font& font,
                           int* width,
                           int* height,
                           int line_height,
                           int flags) {
  NOTIMPLEMENTED();
}

void Canvas::DrawStringWithShadows(const string16& text,
                                   const gfx::Font& font,
                                   SkColor color,
                                   const gfx::Rect& text_bounds,
                                   int line_height,
                                   int flags,
                                   const ShadowValues& shadows) {
  NOTIMPLEMENTED();
}

}  // namespace gfx
