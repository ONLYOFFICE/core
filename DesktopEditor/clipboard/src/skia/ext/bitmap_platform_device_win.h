// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BITMAP_PLATFORM_DEVICE_WIN_H_
#define SKIA_EXT_BITMAP_PLATFORM_DEVICE_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "skia/ext/platform_device.h"
#include "skia/ext/refptr.h"

namespace skia {

// A device is basically a wrapper around SkBitmap that provides a surface for
// SkCanvas to draw into. Our device provides a surface Windows can also write
// to. BitmapPlatformDevice creates a bitmap using CreateDIBSection() in a
// format that Skia supports and can then use this to draw ClearType into, etc.
// This pixel data is provided to the bitmap that the device contains so that it
// can be shared.
//
// The device owns the pixel data, when the device goes away, the pixel data
// also becomes invalid. THIS IS DIFFERENT THAN NORMAL SKIA which uses
// reference counting for the pixel data. In normal Skia, you could assign
// another bitmap to this device's bitmap and everything will work properly.
// For us, that other bitmap will become invalid as soon as the device becomes
// invalid, which may lead to subtle bugs. Therefore, DO NOT ASSIGN THE
// DEVICE'S PIXEL DATA TO ANOTHER BITMAP, make sure you copy instead.
class SK_API BitmapPlatformDevice : public SkDevice, public PlatformDevice {
 public:
  // Factory function. is_opaque should be set if the caller knows the bitmap
  // will be completely opaque and allows some optimizations.
  //
  // The |shared_section| parameter is optional (pass NULL for default
  // behavior). If |shared_section| is non-null, then it must be a handle to a
  // file-mapping object returned by CreateFileMapping.  See CreateDIBSection
  // for details. If |shared_section| is null, the bitmap backing store is not
  // initialized.
  static BitmapPlatformDevice* Create(int width, int height,
                                      bool is_opaque, HANDLE shared_section);

  // Create a BitmapPlatformDevice with no shared section. The bitmap is not
  // initialized to 0.
  static BitmapPlatformDevice* Create(int width, int height, bool is_opaque);

  // Creates a BitmapPlatformDevice instance respecting the parameters as above.
  // If |is_opaque| is false, then the bitmap is initialzed to 0.
  static BitmapPlatformDevice* CreateAndClear(int width, int height,
                                              bool is_opaque);

  virtual ~BitmapPlatformDevice();

  // PlatformDevice overrides
  // Retrieves the bitmap DC, which is the memory DC for our bitmap data. The
  // bitmap DC is lazy created.
  virtual PlatformSurface BeginPlatformPaint() OVERRIDE;
  virtual void EndPlatformPaint() OVERRIDE;

  virtual void DrawToNativeContext(HDC dc, int x, int y,
                                   const RECT* src_rect) OVERRIDE;

  // Loads the given transform and clipping region into the HDC. This is
  // overridden from SkDevice.
  virtual void setMatrixClip(const SkMatrix& transform, const SkRegion& region,
                             const SkClipStack&) OVERRIDE;

 protected:
  // Flushes the Windows device context so that the pixel data can be accessed
  // directly by Skia. Overridden from SkDevice, this is called when Skia
  // starts accessing pixel data.
  virtual const SkBitmap& onAccessBitmap(SkBitmap* bitmap) OVERRIDE;

  virtual SkDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                             int height, bool isOpaque,
                                             Usage usage) OVERRIDE;

 private:
  // Reference counted data that can be shared between multiple devices. This
  // allows copy constructors and operator= for devices to work properly. The
  // bitmaps used by the base device class are already refcounted and copyable.
  class BitmapPlatformDeviceData;

  // Private constructor.
  BitmapPlatformDevice(const skia::RefPtr<BitmapPlatformDeviceData>& data,
                       const SkBitmap& bitmap);

  // Data associated with this device, guaranteed non-null.
  skia::RefPtr<BitmapPlatformDeviceData> data_;

#ifdef SK_DEBUG
  int begin_paint_count_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BitmapPlatformDevice);
};

}  // namespace skia

#endif  // SKIA_EXT_BITMAP_PLATFORM_DEVICE_WIN_H_
