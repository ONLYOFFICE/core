// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/cached_label.h"

#include "base/utf_string_conversions.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"

namespace app_list {

CachedLabel::CachedLabel()
    : needs_repaint_(true) {
}

void CachedLabel::PaintToBackingImage() {
  if (image_.size() == size() && !needs_repaint_)
    return;
  gfx::Canvas canvas(size(), ui::SCALE_FACTOR_100P, false /* is_opaque */);
  canvas.FillRect(GetLocalBounds(), SkColorSetARGB(0, 0, 0, 0),
                  SkXfermode::kSrc_Mode);
  Label::OnPaint(&canvas);
  image_ = gfx::ImageSkia(canvas.ExtractImageRep());
  needs_repaint_ = false;
}

#if defined(OS_WIN)
void CachedLabel::OnPaint(gfx::Canvas* canvas) {
  PaintToBackingImage();
  canvas->DrawImageInt(image_, 0, 0);
}
#endif

}  // namespace app_list
