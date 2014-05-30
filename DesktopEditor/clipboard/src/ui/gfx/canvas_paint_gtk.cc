// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/canvas_skia_paint.h"
#include "ui/gfx/rect.h"

namespace gfx {

CanvasSkiaPaint::CanvasSkiaPaint(GdkEventExpose* event)
    : context_(NULL),
      window_(event->window),
      region_(gdk_region_copy(event->region)),
      composite_alpha_(false) {
  Init(true);
}

CanvasSkiaPaint::CanvasSkiaPaint(GdkEventExpose* event, bool opaque)
    : context_(NULL),
      window_(event->window),
      region_(gdk_region_copy(event->region)),
      composite_alpha_(false) {
  Init(opaque);
}

CanvasSkiaPaint::~CanvasSkiaPaint() {
  if (!is_empty()) {
    platform_canvas()->restoreToCount(1);

    // Blit the dirty rect to the window.
    CHECK(window_);
    cairo_t* cr = gdk_cairo_create(window_);
    CHECK(cr);
    if (composite_alpha_)
      cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_surface_t* source_surface = cairo_get_target(context_);
    CHECK(source_surface);
    // Flush cairo's cache of the surface.
    cairo_surface_mark_dirty(source_surface);
    GdkRectangle bounds = rectangle();
    cairo_set_source_surface(cr, source_surface, bounds.x, bounds.y);
    gdk_cairo_region(cr, region_);
    cairo_fill(cr);
    cairo_destroy(cr);
  }

  gdk_region_destroy(region_);
}

void CanvasSkiaPaint::Init(bool opaque) {
  GdkRectangle bounds = rectangle();
  RecreateBackingCanvas(gfx::Size(bounds.width, bounds.height),
                        ui::SCALE_FACTOR_100P, opaque);

  skia::PlatformCanvas* canvas = platform_canvas();

  // Need to translate so that the dirty region appears at the origin of the
  // surface.
  canvas->translate(-SkIntToScalar(bounds.x), -SkIntToScalar(bounds.y));

  context_ = skia::BeginPlatformPaint(canvas);
}

} // namespace gfx

 
