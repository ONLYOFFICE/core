// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "skia/ext/bitmap_platform_device_android.h"
#include "skia/ext/platform_canvas.h"

namespace skia {

BitmapPlatformDevice* BitmapPlatformDevice::Create(int width, int height,
                                                   bool is_opaque) {
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, width, height);
  if (bitmap.allocPixels()) {
    bitmap.setIsOpaque(is_opaque);
    // Follow the logic in SkCanvas::createDevice(), initialize the bitmap if it
    // is not opaque.
    if (!is_opaque)
      bitmap.eraseARGB(0, 0, 0, 0);
    return new BitmapPlatformDevice(bitmap);
  }
  return NULL;
}

BitmapPlatformDevice* BitmapPlatformDevice::CreateAndClear(int width,
                                                           int height,
                                                           bool is_opaque) {
  BitmapPlatformDevice* device = Create(width, height, is_opaque);
  if (!is_opaque)
    device->accessBitmap(true).eraseARGB(0, 0, 0, 0);
  return device;
}

BitmapPlatformDevice* BitmapPlatformDevice::Create(int width, int height,
                                                   bool is_opaque,
                                                   uint8_t* data) {
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, width, height);
  if (data)
    bitmap.setPixels(data);
  else if (!bitmap.allocPixels())
    return NULL;

  bitmap.setIsOpaque(is_opaque);
  return new BitmapPlatformDevice(bitmap);
}

BitmapPlatformDevice::BitmapPlatformDevice(const SkBitmap& bitmap)
    : SkDevice(bitmap) {
  SetPlatformDevice(this, this);
}

BitmapPlatformDevice::~BitmapPlatformDevice() {
}

SkDevice* BitmapPlatformDevice::onCreateCompatibleDevice(
    SkBitmap::Config config, int width, int height, bool isOpaque,
    Usage /*usage*/) {
  SkASSERT(config == SkBitmap::kARGB_8888_Config);
  return BitmapPlatformDevice::Create(width, height, isOpaque);
}

PlatformSurface BitmapPlatformDevice::BeginPlatformPaint() {
  // TODO(zhenghao): What should we return? The ptr to the address of the
  // pixels? Maybe this won't be called at all.
  return accessBitmap(true).getPixels();
}

void BitmapPlatformDevice::DrawToNativeContext(
    PlatformSurface surface, int x, int y, const PlatformRect* src_rect) {
  // Should never be called on Android.
  SkASSERT(false);
}

// PlatformCanvas impl

SkCanvas* CreatePlatformCanvas(int width, int height, bool is_opaque,
                               uint8_t* data, OnFailureType failureType) {
  skia::RefPtr<SkDevice> dev = skia::AdoptRef(
      BitmapPlatformDevice::Create(width, height, is_opaque, data));
  return CreateCanvas(dev, failureType);
}

// Port of PlatformBitmap to android
PlatformBitmap::~PlatformBitmap() {
  // Nothing to do.
}

bool PlatformBitmap::Allocate(int width, int height, bool is_opaque) {
  bitmap_.setConfig(SkBitmap::kARGB_8888_Config, width, height);
  if (!bitmap_.allocPixels())
    return false;

  bitmap_.setIsOpaque(is_opaque);
  surface_ = bitmap_.getPixels();
  return true;
}

}  // namespace skia
