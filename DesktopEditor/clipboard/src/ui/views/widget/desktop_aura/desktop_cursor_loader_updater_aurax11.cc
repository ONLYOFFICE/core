// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_cursor_loader_updater_aurax11.h"

#include "ui/aura/root_window.h"
#include "ui/base/cursor/cursor_loader.h"
#include "ui/base/cursor/cursors_aura.h"
#include "ui/gfx/display.h"

namespace views {
namespace {

// Cursors that we need to supply our own image resources for. This was
// generated from webcursor_gtk.cc; any cursor that either was NOTIMPLEMENTED()
// or already was implemented with a pixmap is on this list.
const int kImageCursorIds[] = {
  ui::kCursorNorthEastSouthWestResize,
  ui::kCursorNorthWestSouthEastResize,
  ui::kCursorVerticalText,
  ui::kCursorCell,
  ui::kCursorContextMenu,
  ui::kCursorAlias,
  ui::kCursorNoDrop,
  ui::kCursorCopy,
  ui::kCursorNotAllowed,
  ui::kCursorZoomIn,
  ui::kCursorZoomOut,
  ui::kCursorGrab,
  ui::kCursorGrabbing,
};

void LoadImageCursors(float device_scale_factor, ui::CursorLoader* loader) {
  int resource_id;
  gfx::Point point;
  for (size_t i = 0; i < arraysize(kImageCursorIds); ++i) {
    bool success = ui::GetCursorDataFor(kImageCursorIds[i], device_scale_factor,
                                        &resource_id, &point);
    DCHECK(success);
    loader->LoadImageCursor(kImageCursorIds[i], resource_id, point);
  }
}

}  // namespace

DesktopCursorLoaderUpdaterAuraX11::DesktopCursorLoaderUpdaterAuraX11() {}

DesktopCursorLoaderUpdaterAuraX11::~DesktopCursorLoaderUpdaterAuraX11() {}

void DesktopCursorLoaderUpdaterAuraX11::OnCreate(
    aura::RootWindow* window,
    ui::CursorLoader* loader) {
  LoadImageCursors(window->compositor()->device_scale_factor(), loader);
}

void DesktopCursorLoaderUpdaterAuraX11::OnDisplayUpdated(
    const gfx::Display& display,
    ui::CursorLoader* loader) {
  LoadImageCursors(display.device_scale_factor(), loader);
}

}  // namespace views
