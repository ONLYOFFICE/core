// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_CAIRO_CACHED_SURFACE_H_
#define UI_GFX_IMAGE_CAIRO_CACHED_SURFACE_H_

#include <vector>

#include "ui/base/ui_export.h"

typedef struct _GdkDisplay GdkDisplay;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkWidget GtkWidget;
typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

namespace gfx {

// A helper class that takes a GdkPixbuf* and renders it to the screen. Unlike
// gdk_cairo_set_source_pixbuf(), CairoCachedSurface assumes that the pixbuf is
// immutable after UsePixbuf() is called and can be sent to the display server
// once. From then on, that cached version is used so we don't upload the same
// image each and every time we expose.
//
// Most cached surfaces are owned by the GtkThemeService, which associates
// them with a certain XDisplay. Some users of surfaces (CustomDrawButtonBase,
// for example) own their surfaces instead since they interact with the
// ResourceBundle instead of the GtkThemeService.
class UI_EXPORT CairoCachedSurface {
 public:
  CairoCachedSurface();
  ~CairoCachedSurface();

  // Whether this CairoCachedSurface owns a GdkPixbuf.
  bool valid() const {
    return pixbuf_;
  }

  // Delete all our data.
  void Reset();

  // The dimensions of the underlying pixbuf/surface. (or -1 if invalid.)
  int Width() const;
  int Height() const;

  // Sets the pixbuf that we pass to cairo. Calling UsePixbuf() only derefs the
  // current pixbuf and surface (if they exist). Actually transfering data to
  // the X server occurs at SetSource() time. Calling UsePixbuf() should only
  // be done once as it clears cached data from the X server.
  void UsePixbuf(GdkPixbuf* pixbuf);

  // Sets our pixbuf as the active surface starting at (x, y), uploading it in
  // case we don't have an X backed surface cached.
  void SetSource(cairo_t* cr, GtkWidget* widget, int x, int y) const;
  void SetSource(cairo_t* cr, GdkDisplay* display, int x, int y) const;

  // Performs a mask operation, using this surface as the alpha channel.
  void MaskSource(cairo_t* cr, GtkWidget* widget, int x, int y) const;
  void MaskSource(cairo_t* cr, GdkDisplay* display, int x, int y) const;

  // Raw access to the pixbuf. May be NULL. Used for a few gdk operations
  // regarding window shaping.
  GdkPixbuf* pixbuf() { return pixbuf_; }

 private:
  typedef std::vector<std::pair<GdkDisplay*, cairo_surface_t*> > SurfaceVector;

  // Returns a surface . Caches results so only one copy of the image data
  // lives on the display server.
  cairo_surface_t* GetSurfaceFor(cairo_t* cr, GdkDisplay* display) const;

  // The source pixbuf.
  GdkPixbuf* pixbuf_;

  // Our list of cached surfaces. 99% of the time, this will only contain a
  // single entry. At most two. We need to get this right for multiple displays
  // to work correct, since each GdkDisplay is a different display server.
  mutable SurfaceVector surface_map_;
};

}  // namespace gfx

#endif  // UI_GFX_IMAGE_CAIRO_CACHED_SURFACE_H_
