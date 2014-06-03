// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "skia/ext/bitmap_platform_device_linux.h"
#include "skia/ext/bitmap_platform_device_data.h"
#include "skia/ext/platform_canvas.h"

#if defined(OS_OPENBSD)
#include <cairo.h>
#else
#include <cairo/cairo.h>
#endif

namespace skia {

namespace {

void LoadMatrixToContext(cairo_t* context, const SkMatrix& matrix) {
  cairo_matrix_t cairo_matrix;
  cairo_matrix_init(&cairo_matrix,
                    SkScalarToFloat(matrix.getScaleX()),
                    SkScalarToFloat(matrix.getSkewY()),
                    SkScalarToFloat(matrix.getSkewX()),
                    SkScalarToFloat(matrix.getScaleY()),
                    SkScalarToFloat(matrix.getTranslateX()),
                    SkScalarToFloat(matrix.getTranslateY()));
  cairo_set_matrix(context, &cairo_matrix);
}

void LoadClipToContext(cairo_t* context, const SkRegion& clip) {
  cairo_reset_clip(context);

  // TODO(brettw) support non-rect clips.
  SkIRect bounding = clip.getBounds();
  cairo_rectangle(context, bounding.fLeft, bounding.fTop,
                  bounding.fRight - bounding.fLeft,
                  bounding.fBottom - bounding.fTop);
  cairo_clip(context);
}

}  // namespace

BitmapPlatformDevice::BitmapPlatformDeviceData::BitmapPlatformDeviceData(
    cairo_surface_t* surface)
    : surface_(surface),
      config_dirty_(true),
      transform_(SkMatrix::I()) {  // Want to load the config next time.
  bitmap_context_ = cairo_create(surface);
}

BitmapPlatformDevice::BitmapPlatformDeviceData::~BitmapPlatformDeviceData() {
  cairo_destroy(bitmap_context_);
  cairo_surface_destroy(surface_);
}

void BitmapPlatformDevice::BitmapPlatformDeviceData::SetMatrixClip(
    const SkMatrix& transform,
    const SkRegion& region) {
  transform_ = transform;
  clip_region_ = region;
  config_dirty_ = true;
}

void BitmapPlatformDevice::BitmapPlatformDeviceData::LoadConfig() {
  if (!config_dirty_ || !bitmap_context_)
    return;  // Nothing to do.
  config_dirty_ = false;

  // Load the identity matrix since this is what our clip is relative to.
  cairo_matrix_t cairo_matrix;
  cairo_matrix_init_identity(&cairo_matrix);
  cairo_set_matrix(bitmap_context_, &cairo_matrix);

  LoadClipToContext(bitmap_context_, clip_region_);
  LoadMatrixToContext(bitmap_context_, transform_);
}

// We use this static factory function instead of the regular constructor so
// that we can create the pixel data before calling the constructor. This is
// required so that we can call the base class' constructor with the pixel
// data.
BitmapPlatformDevice* BitmapPlatformDevice::Create(int width, int height,
                                                   bool is_opaque,
                                                   cairo_surface_t* surface) {
  if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surface);
    return NULL;
  }
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, width, height,
                   cairo_image_surface_get_stride(surface));
  bitmap.setPixels(cairo_image_surface_get_data(surface));
  bitmap.setIsOpaque(is_opaque);

  // The device object will take ownership of the graphics context.
  return new BitmapPlatformDevice
      (bitmap, new BitmapPlatformDeviceData(surface));
}

BitmapPlatformDevice* BitmapPlatformDevice::Create(int width, int height,
                                                   bool is_opaque) {
  // This initializes the bitmap to all zeros.
  cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                                        width, height);

  BitmapPlatformDevice* device = Create(width, height, is_opaque, surface);

#ifndef NDEBUG
  if (device && is_opaque)  // Fill with bright bluish green
    device->eraseColor(SkColorSetARGB(255, 0, 255, 128));
#endif

  return device;
}

BitmapPlatformDevice* BitmapPlatformDevice::CreateAndClear(int width,
                                                           int height,
                                                           bool is_opaque) {
  // The Linux port always constructs initialized bitmaps, so there is no extra
  // work to perform here.
  return Create(width, height, is_opaque);
}

BitmapPlatformDevice* BitmapPlatformDevice::Create(int width, int height,
                                                   bool is_opaque,
                                                   uint8_t* data) {
  cairo_surface_t* surface = cairo_image_surface_create_for_data(
      data, CAIRO_FORMAT_ARGB32, width, height,
      cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width));

  return Create(width, height, is_opaque, surface);
}

// The device will own the bitmap, which corresponds to also owning the pixel
// data. Therefore, we do not transfer ownership to the SkDevice's bitmap.
BitmapPlatformDevice::BitmapPlatformDevice(
    const SkBitmap& bitmap,
    BitmapPlatformDeviceData* data)
    : SkDevice(bitmap),
      data_(data) {
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

cairo_t* BitmapPlatformDevice::BeginPlatformPaint() {
  data_->LoadConfig();
  cairo_t* cairo = data_->bitmap_context();
  cairo_surface_t* surface = cairo_get_target(cairo);
  // Tell cairo to flush anything it has pending.
  cairo_surface_flush(surface);
  // Tell Cairo that we (probably) modified (actually, will modify) its pixel
  // buffer directly.
  cairo_surface_mark_dirty(surface);
  return cairo;
}

void BitmapPlatformDevice::DrawToNativeContext(
    PlatformSurface surface, int x, int y, const PlatformRect* src_rect) {
  // Should never be called on Linux.
  SkASSERT(false);
}

void BitmapPlatformDevice::setMatrixClip(const SkMatrix& transform,
                                         const SkRegion& region,
                                         const SkClipStack&) {
  data_->SetMatrixClip(transform, region);
}

// PlatformCanvas impl

SkCanvas* CreatePlatformCanvas(int width, int height, bool is_opaque,
                               uint8_t* data, OnFailureType failureType) {
  skia::RefPtr<SkDevice> dev = skia::AdoptRef(
      BitmapPlatformDevice::Create(width, height, is_opaque, data));
  return CreateCanvas(dev, failureType);
}

// Port of PlatformBitmap to linux
PlatformBitmap::~PlatformBitmap() {
  cairo_destroy(surface_);
}

bool PlatformBitmap::Allocate(int width, int height, bool is_opaque) {
  // The SkBitmap allocates and owns the bitmap memory; PlatformBitmap owns the
  // cairo drawing context tied to the bitmap. The SkBitmap's pixelRef can
  // outlive the PlatformBitmap if additional copies are made.
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
  bitmap_.setConfig(SkBitmap::kARGB_8888_Config, width, height, stride);
  if (!bitmap_.allocPixels())  // Using the default allocator.
    return false;
  bitmap_.setIsOpaque(is_opaque);

  cairo_surface_t* surf = cairo_image_surface_create_for_data(
      reinterpret_cast<unsigned char*>(bitmap_.getPixels()),
      CAIRO_FORMAT_ARGB32,
      width,
      height,
      stride);
  if (cairo_surface_status(surf) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surf);
    return false;
  }

  surface_ = cairo_create(surf);
  cairo_surface_destroy(surf);
  return true;
}

}  // namespace skia
