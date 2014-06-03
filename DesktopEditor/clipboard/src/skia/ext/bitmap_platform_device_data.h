// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_DATA_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_DATA_H_

#include "skia/ext/bitmap_platform_device.h"

namespace skia {

class BitmapPlatformDevice::BitmapPlatformDeviceData :
#if defined(WIN32) || defined(__APPLE__)
    public SkRefCnt {
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
    // These objects are reference counted and own a Cairo surface. The surface
    // is the backing store for a Skia bitmap and we reference count it so that
    // we can copy BitmapPlatformDevice objects without having to copy all the
    // image data.
    public base::RefCounted<BitmapPlatformDeviceData> {
#endif

 public:
#if defined(WIN32)
  typedef HBITMAP PlatformContext;
#elif defined(__APPLE__)
  typedef CGContextRef PlatformContext;
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
  typedef cairo_t* PlatformContext;
#endif

#if defined(WIN32) || defined(__APPLE__)
  explicit BitmapPlatformDeviceData(PlatformContext bitmap);
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
  explicit BitmapPlatformDeviceData(cairo_surface_t* surface);
#endif

#if defined(WIN32)
  // Create/destroy hdc_, which is the memory DC for our bitmap data.
  HDC GetBitmapDC();
  void ReleaseBitmapDC();
  bool IsBitmapDCCreated() const;
#endif

#if defined(__APPLE__)
  void ReleaseBitmapContext();
#endif  // defined(__APPLE__)

  // Sets the transform and clip operations. This will not update the CGContext,
  // but will mark the config as dirty. The next call of LoadConfig will
  // pick up these changes.
  void SetMatrixClip(const SkMatrix& transform, const SkRegion& region);

  // Loads the current transform and clip into the context. Can be called even
  // when |bitmap_context_| is NULL (will be a NOP).
  void LoadConfig();

  const SkMatrix& transform() const {
    return transform_;
  }

  PlatformContext bitmap_context() {
    return bitmap_context_;
  }

 private:
#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
  friend class base::RefCounted<BitmapPlatformDeviceData>;
#endif
  virtual ~BitmapPlatformDeviceData();

  // Lazily-created graphics context used to draw into the bitmap.
  PlatformContext bitmap_context_;

#if defined(WIN32)
  // Lazily-created DC used to draw into the bitmap, see GetBitmapDC().
  HDC hdc_;
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)
  cairo_surface_t *const surface_;
#endif

  // True when there is a transform or clip that has not been set to the
  // context.  The context is retrieved for every text operation, and the
  // transform and clip do not change as much. We can save time by not loading
  // the clip and transform for every one.
  bool config_dirty_;

  // Translation assigned to the context: we need to keep track of this
  // separately so it can be updated even if the context isn't created yet.
  SkMatrix transform_;

  // The current clipping
  SkRegion clip_region_;

  // Disallow copy & assign.
  BitmapPlatformDeviceData(const BitmapPlatformDeviceData&);
  BitmapPlatformDeviceData& operator=(const BitmapPlatformDeviceData&);
};

}  // namespace skia

#endif  // SKIA_EXT_BITMAP_PLATFORM_DEVICE_DATA_H_
