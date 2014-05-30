// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_CACHED_LABEL_H_
#define UI_APP_LIST_VIEWS_CACHED_LABEL_H_

#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/label.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace app_list {

// Subclass of views::Label that caches the rendered text in an ImageSkia.
class CachedLabel : public views::Label {
 public:
  CachedLabel();

  // Have the next call to OnPaint() update the backing image.
  void Invalidate() { needs_repaint_ = true; }

  // Calls the base label's OnPaint() to paint into a backing image.
  void PaintToBackingImage();

#if defined(OS_WIN)
  // views::View overrides.
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
#endif

 private:
  bool needs_repaint_;
  gfx::ImageSkia image_;
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_CACHED_LABEL_H_
