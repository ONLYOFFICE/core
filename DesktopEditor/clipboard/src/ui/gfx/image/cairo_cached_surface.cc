// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/cairo_cached_surface.h"

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/logging.h"

namespace gfx {

CairoCachedSurface::CairoCachedSurface() : pixbuf_(NULL) {
}

CairoCachedSurface::~CairoCachedSurface() {
  Reset();
}

void CairoCachedSurface::Reset() {
  for (SurfaceVector::iterator it = surface_map_.begin();
       it != surface_map_.end(); ++it) {
    cairo_surface_destroy(it->second);
  }
  surface_map_.clear();

  if (pixbuf_) {
    g_object_unref(pixbuf_);
    pixbuf_ = NULL;
  }
}

int CairoCachedSurface::Width() const {
  return pixbuf_ ? gdk_pixbuf_get_width(pixbuf_) : -1;
}

int CairoCachedSurface::Height() const {
  return pixbuf_ ? gdk_pixbuf_get_height(pixbuf_) : -1;
}

void CairoCachedSurface::UsePixbuf(GdkPixbuf* pixbuf) {
  if (pixbuf)
    g_object_ref(pixbuf);

  Reset();

  pixbuf_ = pixbuf;
}

void CairoCachedSurface::SetSource(cairo_t* cr, GtkWidget* widget,
                                   int x, int y) const {
  SetSource(cr, gtk_widget_get_display(widget), x, y);
}

void CairoCachedSurface::SetSource(cairo_t* cr, GdkDisplay* display,
                                   int x, int y) const {
  DCHECK(pixbuf_);
  DCHECK(cr);
  DCHECK(display);

  cairo_surface_t* surface = GetSurfaceFor(cr, display);
  cairo_set_source_surface(cr, surface, x, y);
}

void CairoCachedSurface::MaskSource(cairo_t* cr, GtkWidget* widget,
                                    int x, int y) const {
  MaskSource(cr, gtk_widget_get_display(widget), x, y);
}

void CairoCachedSurface::MaskSource(cairo_t* cr, GdkDisplay* display,
                                    int x, int y) const {
  DCHECK(pixbuf_);
  DCHECK(cr);
  DCHECK(display);

  cairo_surface_t* surface = GetSurfaceFor(cr, display);
  cairo_mask_surface(cr, surface, x, y);
}

cairo_surface_t* CairoCachedSurface::GetSurfaceFor(cairo_t* cr,
                                                   GdkDisplay* display) const {
  for (SurfaceVector::const_iterator it = surface_map_.begin();
       it != surface_map_.end(); ++it) {
    if (display == it->first) {
      return it->second;
    }
  }

  // First time here since last UsePixbuf call. Generate the surface.
  cairo_surface_t* target = cairo_get_target(cr);
  cairo_surface_t* out = cairo_surface_create_similar(
      target,
      CAIRO_CONTENT_COLOR_ALPHA,
      gdk_pixbuf_get_width(pixbuf_),
      gdk_pixbuf_get_height(pixbuf_));

  DCHECK(out);

  cairo_t* copy_cr = cairo_create(out);
  gdk_cairo_set_source_pixbuf(copy_cr, pixbuf_, 0, 0);
  cairo_paint(copy_cr);
  cairo_destroy(copy_cr);

  surface_map_.push_back(std::make_pair(display, out));
  return out;
}

}  // namespace gfx
