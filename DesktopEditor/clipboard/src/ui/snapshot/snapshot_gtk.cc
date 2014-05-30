// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/snapshot/snapshot.h"

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "base/logging.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/rect.h"

namespace {

cairo_status_t SnapshotCallback(void* closure,
                                const unsigned char* data,
                                unsigned int length) {
  std::vector<unsigned char>* png_representation =
      static_cast<std::vector<unsigned char>*>(closure);

  size_t old_size = png_representation->size();
  png_representation->resize(old_size + length);
  memcpy(&(*png_representation)[old_size], data, length);
  return CAIRO_STATUS_SUCCESS;
}

}  // namespace

namespace ui {

bool GrabViewSnapshot(gfx::NativeView view_handle,
                        std::vector<unsigned char>* png_representation,
                        const gfx::Rect& snapshot_bounds) {
  GdkWindow* gdk_window = gtk_widget_get_window(view_handle);
  Display* display = GDK_WINDOW_XDISPLAY(gdk_window);
  XID win = GDK_WINDOW_XID(gdk_window);

  gfx::Rect window_bounds;
  if (ui::GetWindowRect(win, &window_bounds) == 0) {
    LOG(ERROR) << "Couldn't get window bounds";
    return false;
  }

  DCHECK_LE(snapshot_bounds.right(), window_bounds.width());
  DCHECK_LE(snapshot_bounds.bottom(), window_bounds.height());

  ui::XScopedImage image(XGetImage(
      display, win, snapshot_bounds.x(), snapshot_bounds.y(),
      snapshot_bounds.width(), snapshot_bounds.height(), AllPlanes, ZPixmap));
  if (!image.get()) {
    LOG(ERROR) << "Couldn't get image";
    return false;
  }
  if (image->depth != 24) {
    LOG(ERROR)<< "Unsupported image depth " << image->depth;
    return false;
  }
  cairo_surface_t* surface =
      cairo_image_surface_create_for_data(
          reinterpret_cast<unsigned char*>(image->data),
          CAIRO_FORMAT_RGB24,
          image->width,
          image->height,
          image->bytes_per_line);

  if (!surface) {
    LOG(ERROR) << "Unable to create Cairo surface from XImage data";
    return false;
  }
  cairo_surface_write_to_png_stream(
      surface, SnapshotCallback, png_representation);
  cairo_surface_destroy(surface);

  return true;
}

bool GrabWindowSnapshot(gfx::NativeWindow window_handle,
                        std::vector<unsigned char>* png_representation,
                        const gfx::Rect& snapshot_bounds) {
  return GrabViewSnapshot(GTK_WIDGET(window_handle), png_representation,
      snapshot_bounds);
}

}  // namespace ui
