// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image.h"

#import <AppKit/AppKit.h>

#include "base/logging.h"
#include "base/memory/scoped_nsobject.h"
#include "ui/gfx/image/image_png_rep.h"
#include "ui/gfx/size.h"

namespace gfx {
namespace internal {

namespace {

// Returns a 16x16 red NSImage to visually show when a NSImage cannot be
// created from PNG data.
// Caller takes ownership of the returned NSImage.
NSImage* GetErrorNSImage() {
  NSRect rect = NSMakeRect(0, 0, 16, 16);
  NSImage* image = [[NSImage alloc] initWithSize:rect.size];
  [image lockFocus];
  [[NSColor colorWithDeviceRed:1.0 green:0.0 blue:0.0 alpha:1.0] set];
  NSRectFill(rect);
  [image unlockFocus];
  return image;
}

}  // namespace

scoped_refptr<base::RefCountedMemory> Get1xPNGBytesFromNSImage(
    NSImage* nsimage) {
  CGImageRef cg_image = [nsimage CGImageForProposedRect:NULL
                                                context:nil
                                                  hints:nil];
  scoped_nsobject<NSBitmapImageRep> ns_bitmap(
      [[NSBitmapImageRep alloc] initWithCGImage:cg_image]);
  NSData* ns_data = [ns_bitmap representationUsingType:NSPNGFileType
                                            properties:nil];
  const unsigned char* bytes =
      static_cast<const unsigned char*>([ns_data bytes]);
  scoped_refptr<base::RefCountedBytes> refcounted_bytes(
      new base::RefCountedBytes());
  refcounted_bytes->data().assign(bytes, bytes + [ns_data length]);
  return refcounted_bytes;
}

NSImage* NSImageFromPNG(const std::vector<gfx::ImagePNGRep>& image_png_reps) {
  if (image_png_reps.empty()) {
    LOG(ERROR) << "Unable to decode PNG.";
    return GetErrorNSImage();
  }

  scoped_nsobject<NSImage> image;
  for (size_t i = 0; i < image_png_reps.size(); ++i) {
    scoped_refptr<base::RefCountedMemory> png = image_png_reps[i].raw_data;
    CHECK(png.get());
    scoped_nsobject<NSData> ns_data(
        [[NSData alloc] initWithBytes:png->front() length:png->size()]);
    scoped_nsobject<NSBitmapImageRep> ns_image_rep(
        [[NSBitmapImageRep alloc] initWithData:ns_data]);
    if (!ns_image_rep) {
      LOG(ERROR) << "Unable to decode PNG at "
                 << ui::GetScaleFactorScale(image_png_reps[i].scale_factor)
                 << ".";
      return GetErrorNSImage();
    }

    if (!image.get()) {
      float scale = ui::GetScaleFactorScale(image_png_reps[i].scale_factor);
      NSSize image_size = NSMakeSize([ns_image_rep pixelsWide] / scale,
                                     [ns_image_rep pixelsHigh] / scale);
      image.reset([[NSImage alloc] initWithSize:image_size]);
    }
    [image addRepresentation:ns_image_rep];
  }

  return image.release();
}

gfx::Size NSImageSize(NSImage* image) {
  NSSize size = [image size];
  int width = static_cast<int>(size.width);
  int height = static_cast<int>(size.height);
  return gfx::Size(width, height);
}

} // namespace internal
} // namespace gfx

