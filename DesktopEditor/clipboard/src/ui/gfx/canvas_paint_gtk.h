
// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_LINUX_H_
#define UI_GFX_CANVAS_PAINT_LINUX_H_

#include "base/logging.h"
#include "skia/ext/platform_canvas.h"
#include "ui/gfx/canvas.h"
#include <gdk/gdk.h>

namespace gfx {

// A class designed to translate skia painting into a region in a GdkWindow.
// On construction, it will set up a context for painting into, and on
// destruction, it will commit it to the GdkWindow.
// Note: The created context is always inialized to (0, 0, 0, 0).
class UI_EXPORT CanvasSkiaPaint : public Canvas {
 public:
  // This constructor assumes the result is opaque.
  explicit CanvasSkiaPaint(GdkEventExpose* event);
  CanvasSkiaPaint(GdkEventExpose* event, bool opaque);
  virtual ~CanvasSkiaPaint();

  // Sets whether the bitmap is composited in such a way that the alpha channel
  // is honored. This is only useful if you've enabled an RGBA colormap on the
  // widget. The default is false.
  void set_composite_alpha(bool composite_alpha) {
    composite_alpha_ = composite_alpha;
  }

  // Returns true if the invalid region is empty. The caller should call this
  // function to determine if anything needs painting.
  bool is_empty() const {
    return gdk_region_empty(region_);
  }

  GdkRectangle rectangle() const {
    GdkRectangle bounds;
    gdk_region_get_clipbox(region_, &bounds);
    return bounds;
  }

 private:
  void Init(bool opaque);

  cairo_t* context_;
  GdkWindow* window_;
  GdkRegion* region_;
  // See description above setter.
  bool composite_alpha_;

  // Disallow copy and assign.
  CanvasSkiaPaint(const CanvasSkiaPaint&);
  CanvasSkiaPaint& operator=(const CanvasSkiaPaint&);
};

}  // namespace gfx

#endif  // UI_GFX_CANVAS_PAINT_LINUX_H_
