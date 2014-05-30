// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/cursor/cursor_loader_x11.h"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "base/logging.h"
#include "grit/ui_resources.h"
#include "ui/base/cursor/cursor.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/skbitmap_operations.h"

namespace {

// Returns X font cursor shape from an Aura cursor.
int CursorShapeFromNative(const gfx::NativeCursor& native_cursor) {
  switch (native_cursor.native_type()) {
    case ui::kCursorMiddlePanning:
      return XC_fleur;
    case ui::kCursorEastPanning:
      return XC_sb_right_arrow;
    case ui::kCursorNorthPanning:
      return XC_sb_up_arrow;
    case ui::kCursorNorthEastPanning:
      return XC_top_right_corner;
    case ui::kCursorNorthWestPanning:
      return XC_top_left_corner;
    case ui::kCursorSouthPanning:
      return XC_sb_down_arrow;
    case ui::kCursorSouthEastPanning:
      return XC_bottom_right_corner;
    case ui::kCursorSouthWestPanning:
      return XC_bottom_left_corner;
    case ui::kCursorWestPanning:
      return XC_sb_left_arrow;
    case ui::kCursorNone:
    case ui::kCursorGrab:
    case ui::kCursorGrabbing:
      // TODO(jamescook): Need cursors for these.  crbug.com/111650
      return XC_left_ptr;

#if defined(OS_CHROMEOS)
    case ui::kCursorNull:
    case ui::kCursorPointer:
    case ui::kCursorNoDrop:
    case ui::kCursorNotAllowed:
    case ui::kCursorCopy:
    case ui::kCursorMove:
    case ui::kCursorEastResize:
    case ui::kCursorNorthResize:
    case ui::kCursorSouthResize:
    case ui::kCursorWestResize:
    case ui::kCursorNorthEastResize:
    case ui::kCursorNorthWestResize:
    case ui::kCursorSouthWestResize:
    case ui::kCursorSouthEastResize:
    case ui::kCursorIBeam:
    case ui::kCursorAlias:
    case ui::kCursorCell:
    case ui::kCursorContextMenu:
    case ui::kCursorCross:
    case ui::kCursorHelp:
    case ui::kCursorWait:
    case ui::kCursorNorthSouthResize:
    case ui::kCursorEastWestResize:
    case ui::kCursorNorthEastSouthWestResize:
    case ui::kCursorNorthWestSouthEastResize:
    case ui::kCursorProgress:
    case ui::kCursorColumnResize:
    case ui::kCursorRowResize:
    case ui::kCursorVerticalText:
    case ui::kCursorZoomIn:
    case ui::kCursorZoomOut:
    case ui::kCursorHand:
      // In some environments, the image assets are not set (e.g. in
      // content-browsertests, content-shell etc.).
      return XC_left_ptr;
#else  // defined(OS_CHROMEOS)
    case ui::kCursorNull:
      return XC_left_ptr;
    case ui::kCursorPointer:
      return XC_left_ptr;
    case ui::kCursorMove:
      return XC_fleur;
    case ui::kCursorCross:
      return XC_crosshair;
    case ui::kCursorHand:
      return XC_hand2;
    case ui::kCursorIBeam:
      return XC_xterm;
    case ui::kCursorProgress:
    case ui::kCursorWait:
      return XC_watch;
    case ui::kCursorHelp:
      return XC_question_arrow;
    case ui::kCursorEastResize:
      return XC_right_side;
    case ui::kCursorNorthResize:
      return XC_top_side;
    case ui::kCursorNorthEastResize:
      return XC_top_right_corner;
    case ui::kCursorNorthWestResize:
      return XC_top_left_corner;
    case ui::kCursorSouthResize:
      return XC_bottom_side;
    case ui::kCursorSouthEastResize:
      return XC_bottom_right_corner;
    case ui::kCursorSouthWestResize:
      return XC_bottom_left_corner;
    case ui::kCursorWestResize:
      return XC_left_side;
    case ui::kCursorNorthSouthResize:
      return XC_sb_v_double_arrow;
    case ui::kCursorEastWestResize:
      return XC_sb_h_double_arrow;
    case ui::kCursorColumnResize:
      return XC_sb_h_double_arrow;
    case ui::kCursorRowResize:
      return XC_sb_v_double_arrow;
#endif  // defined(OS_CHROMEOS)
    case ui::kCursorCustom:
      NOTREACHED();
      return XC_left_ptr;
  }
  NOTREACHED() << "Case not handled for " << native_cursor.native_type();
  return XC_left_ptr;
}

}  // namespace

namespace ui {

CursorLoader* CursorLoader::Create() {
  return new CursorLoaderX11;
}

CursorLoaderX11::CursorLoaderX11()
    : invisible_cursor_(CreateInvisibleCursor(), GetXDisplay()) {
}

CursorLoaderX11::~CursorLoaderX11() {
  UnloadAll();
}

void CursorLoaderX11::LoadImageCursor(int id,
                                      int resource_id,
                                      const gfx::Point& hot) {
  const gfx::ImageSkia* image =
      ResourceBundle::GetSharedInstance().GetImageSkiaNamed(resource_id);
  const gfx::ImageSkiaRep& image_rep = image->GetRepresentation(
      GetScaleFactorFromScale(display().device_scale_factor()));
  SkBitmap bitmap = image_rep.sk_bitmap();
  gfx::Point hotpoint = hot;
  switch (display().rotation()) {
    case gfx::Display::ROTATE_0:
      break;
    case gfx::Display::ROTATE_90:
      hotpoint.SetPoint(bitmap.height() - hot.y(), hot.x());
      bitmap = SkBitmapOperations::Rotate(
          bitmap, SkBitmapOperations::ROTATION_90_CW);
      break;
    case gfx::Display::ROTATE_180:
      hotpoint.SetPoint(bitmap.width() - hot.x(), bitmap.height() - hot.y());
      bitmap = SkBitmapOperations::Rotate(
          bitmap, SkBitmapOperations::ROTATION_180_CW);
      break;
    case gfx::Display::ROTATE_270:
      hotpoint.SetPoint(hot.y(), bitmap.width() - hot.x());
      bitmap = SkBitmapOperations::Rotate(
          bitmap, SkBitmapOperations::ROTATION_270_CW);
      break;
  }
  XcursorImage* x_image = SkBitmapToXcursorImage(&bitmap, hotpoint);
  cursors_[id] = CreateReffedCustomXCursor(x_image);
  // |image_rep| is owned by the resource bundle. So we do not need to free it.
}

void CursorLoaderX11::LoadAnimatedCursor(int id,
                                         int resource_id,
                                         const gfx::Point& hot,
                                         int frame_delay_ms) {
  const gfx::ImageSkia* image =
      ResourceBundle::GetSharedInstance().GetImageSkiaNamed(resource_id);
  const gfx::ImageSkiaRep& image_rep = image->GetRepresentation(
      GetScaleFactorFromScale(display().device_scale_factor()));
  const SkBitmap bitmap = image_rep.sk_bitmap();
  DCHECK_EQ(bitmap.config(), SkBitmap::kARGB_8888_Config);
  int frame_width = bitmap.height();
  int frame_height = frame_width;
  int total_width = bitmap.width();
  DCHECK_EQ(total_width % frame_width, 0);
  int frame_count = total_width / frame_width;
  DCHECK_GT(frame_count, 0);
  XcursorImages* x_images = XcursorImagesCreate(frame_count);
  x_images->nimage = frame_count;
  bitmap.lockPixels();
  unsigned int* pixels = bitmap.getAddr32(0, 0);
  // Create each frame.
  for (int frame = 0; frame < frame_count; ++frame) {
    XcursorImage* x_image = XcursorImageCreate(frame_width, frame_height);
    for (int row = 0; row < frame_height; ++row) {
      // Copy |row|'th row of |frame|'th frame.
      memcpy(x_image->pixels + row * frame_width,
             pixels + frame * frame_width + row * total_width,
             frame_width * 4);
    }
    x_image->xhot = hot.x();
    x_image->yhot = hot.y();
    x_image->delay = frame_delay_ms;
    x_images->images[frame] = x_image;
  }
  bitmap.unlockPixels();

  animated_cursors_[id] = std::make_pair(
      XcursorImagesLoadCursor(GetXDisplay(), x_images), x_images);
  // |bitmap| is owned by the resource bundle. So we do not need to free it.
}

void CursorLoaderX11::UnloadAll() {
  for (ImageCursorMap::const_iterator it = cursors_.begin();
       it != cursors_.end(); ++it)
    UnrefCustomXCursor(it->second);

  // Free animated cursors and images.
  for (AnimatedCursorMap::iterator it = animated_cursors_.begin();
       it != animated_cursors_.end(); ++it) {
    XcursorImagesDestroy(it->second.second);  // also frees individual frames.
    XFreeCursor(GetXDisplay(), it->second.first);
  }
}

void CursorLoaderX11::SetPlatformCursor(gfx::NativeCursor* cursor) {
  DCHECK(cursor);

  ::Cursor xcursor;
  if (IsImageCursor(*cursor))
    xcursor = ImageCursorFromNative(*cursor);
  else if (*cursor == kCursorNone)
    xcursor =  invisible_cursor_.get();
  else if (*cursor == kCursorCustom)
    xcursor = cursor->platform();
  else if (display().device_scale_factor() == 1.0f &&
           display().rotation() == gfx::Display::ROTATE_0) {
    xcursor = GetXCursor(CursorShapeFromNative(*cursor));
  } else {
    xcursor = ImageCursorFromNative(kCursorPointer);
  }

  cursor->SetPlatformCursor(xcursor);
}

bool CursorLoaderX11::IsImageCursor(gfx::NativeCursor native_cursor) {
  int type = native_cursor.native_type();
  return cursors_.count(type) || animated_cursors_.count(type);
}

::Cursor CursorLoaderX11::ImageCursorFromNative(
    gfx::NativeCursor native_cursor) {
  int type = native_cursor.native_type();
  if (animated_cursors_.count(type))
    return animated_cursors_[type].first;

  ImageCursorMap::iterator find = cursors_.find(type);
  if (find != cursors_.end())
    return cursors_[type];
  return GetXCursor(CursorShapeFromNative(native_cursor));
}

}  // namespace ui
