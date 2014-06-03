// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "skia/ext/platform_canvas.h"

#include "skia/ext/bitmap_platform_device.h"
#include "third_party/skia/include/core/SkTypes.h"

namespace skia {

SkDevice* GetTopDevice(const SkCanvas& canvas) {
  return canvas.getTopDevice(true);
}

bool SupportsPlatformPaint(const SkCanvas* canvas) {
  PlatformDevice* platform_device = GetPlatformDevice(GetTopDevice(*canvas));
  return platform_device && platform_device->SupportsPlatformPaint();
}

PlatformSurface BeginPlatformPaint(SkCanvas* canvas) {
  PlatformDevice* platform_device = GetPlatformDevice(GetTopDevice(*canvas));
  if (platform_device)
    return platform_device->BeginPlatformPaint();

  return 0;
}

void EndPlatformPaint(SkCanvas* canvas) {
  PlatformDevice* platform_device = GetPlatformDevice(GetTopDevice(*canvas));
  if (platform_device)
    platform_device->EndPlatformPaint();
}

void DrawToNativeContext(SkCanvas* canvas, PlatformSurface context, int x,
                         int y, const PlatformRect* src_rect) {
  PlatformDevice* platform_device = GetPlatformDevice(GetTopDevice(*canvas));
  if (platform_device)
    platform_device->DrawToNativeContext(context, x, y, src_rect);
}

static SkPMColor MakeOpaqueXfermodeProc(SkPMColor src, SkPMColor dst) {
    return dst | (0xFF << SK_A32_SHIFT);
}

void MakeOpaque(SkCanvas* canvas, int x, int y, int width, int height) {
  if (width <= 0 || height <= 0)
    return;

  SkRect rect;
  rect.setXYWH(SkIntToScalar(x), SkIntToScalar(y),
               SkIntToScalar(width), SkIntToScalar(height));
  SkPaint paint;
  // so we don't draw anything on a device that ignores xfermodes
  paint.setColor(0);
  // install our custom mode
  skia::RefPtr<SkProcXfermode> xfermode =
      skia::AdoptRef(new SkProcXfermode(MakeOpaqueXfermodeProc));
  paint.setXfermode(xfermode.get());
  canvas->drawRect(rect, paint);
}

size_t PlatformCanvasStrideForWidth(unsigned width) {
  return 4 * width;
}

SkCanvas* CreateCanvas(const skia::RefPtr<SkDevice>& device, OnFailureType failureType) {
  if (!device) {
    if (CRASH_ON_FAILURE == failureType)
      SK_CRASH();
    return NULL;
  }
  return new SkCanvas(device.get());
}

PlatformBitmap::PlatformBitmap() : surface_(0), platform_extra_(0) {}

}  // namespace skia
