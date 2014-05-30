// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_WIN_H_
#define UI_GFX_CANVAS_PAINT_WIN_H_

#include "skia/ext/platform_canvas.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/canvas_paint.h"
#include "ui/gfx/size.h"

namespace gfx {

// A class designed to help with WM_PAINT operations on Windows. It will
// do BeginPaint/EndPaint on init/destruction, and will create the bitmap and
// canvas with the correct size and transform for the dirty rect. The bitmap
// will be automatically painted to the screen on destruction.
//
// You MUST call isEmpty before painting to determine if anything needs
// painting. Sometimes the dirty rect can actually be empty, and this makes
// the bitmap functions we call unhappy. The caller should not paint in this
// case.
//
// Therefore, all you need to do is:
//   case WM_PAINT: {
//     gfx::PlatformCanvasPaint canvas(hwnd);
//     if (!canvas.isEmpty()) {
//       ... paint to the canvas ...
//     }
//     return 0;
//   }
// Note: The created context is always inialized to (0, 0, 0, 0).
class UI_EXPORT CanvasSkiaPaint : public Canvas {
 public:
  // This constructor assumes the canvas is opaque.
  explicit CanvasSkiaPaint(HWND hwnd) : hwnd_(hwnd), paint_dc_(NULL),
    for_paint_(true) {
    memset(&ps_, 0, sizeof(ps_));
    initPaint(true);
  }

  CanvasSkiaPaint(HWND hwnd, bool opaque) : hwnd_(hwnd), paint_dc_(NULL),
      for_paint_(true) {
    memset(&ps_, 0, sizeof(ps_));
    initPaint(opaque);
  }

  // Creates a CanvasSkiaPaint for the specified region that paints to the
  // specified dc. This does NOT do BeginPaint/EndPaint.
  CanvasSkiaPaint(HDC dc, bool opaque, int x, int y, int w, int h)
      : hwnd_(NULL),
        paint_dc_(dc),
        for_paint_(false) {
    memset(&ps_, 0, sizeof(ps_));
    ps_.rcPaint.left = x;
    ps_.rcPaint.right = x + w;
    ps_.rcPaint.top = y;
    ps_.rcPaint.bottom = y + h;
    init(opaque);
  }

  virtual ~CanvasSkiaPaint() {
    if (!isEmpty()) {
      skia::PlatformCanvas* canvas = platform_canvas();
      canvas->restoreToCount(1);
      // Commit the drawing to the screen
      skia::DrawToNativeContext(canvas, paint_dc_, ps_.rcPaint.left,
                                ps_.rcPaint.top, NULL);
    }
    if (for_paint_)
      EndPaint(hwnd_, &ps_);
  }

  // Returns true if the invalid region is empty. The caller should call this
  // function to determine if anything needs painting.
  bool isEmpty() const {
    return ps_.rcPaint.right - ps_.rcPaint.left == 0 ||
           ps_.rcPaint.bottom - ps_.rcPaint.top == 0;
  }

  // Use to access the Windows painting parameters, especially useful for
  // getting the bounding rect for painting: paintstruct().rcPaint
  const PAINTSTRUCT& paintStruct() const {
    return ps_;
  }

  // Returns the DC that will be painted to
  HDC paintDC() const {
    return paint_dc_;
  }

 protected:
  HWND hwnd_;
  HDC paint_dc_;
  PAINTSTRUCT ps_;

 private:
  void initPaint(bool opaque) {
    paint_dc_ = BeginPaint(hwnd_, &ps_);

    init(opaque);
  }

  void init(bool opaque) {
    // FIXME(brettw) for ClearType, we probably want to expand the bounds of
    // painting by one pixel so that the boundaries will be correct (ClearType
    // text can depend on the adjacent pixel). Then we would paint just the
    // inset pixels to the screen.
    const int width = ps_.rcPaint.right - ps_.rcPaint.left;
    const int height = ps_.rcPaint.bottom - ps_.rcPaint.top;

    RecreateBackingCanvas(gfx::Size(width, height), ui::SCALE_FACTOR_100P,
                          opaque);
    skia::PlatformCanvas* canvas = platform_canvas();

    canvas->clear(SkColorSetARGB(0, 0, 0, 0));

    // This will bring the canvas into the screen coordinate system for the
    // dirty rect
    canvas->translate(SkIntToScalar(-ps_.rcPaint.left),
                      SkIntToScalar(-ps_.rcPaint.top));
  }

  // If true, this canvas was created for a BeginPaint.
  const bool for_paint_;

  // Disallow copy and assign.
  CanvasSkiaPaint(const CanvasSkiaPaint&);
  CanvasSkiaPaint& operator=(const CanvasSkiaPaint&);
};

}  // namespace gfx

#endif  // UI_GFX_CANVAS_PAINT_WIN_H_
