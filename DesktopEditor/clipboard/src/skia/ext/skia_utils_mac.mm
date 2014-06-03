// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "skia/ext/skia_utils_mac.h"

#import <AppKit/AppKit.h>

#include "base/logging.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "skia/ext/bitmap_platform_device_mac.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "third_party/skia/include/utils/mac/SkCGUtils.h"

namespace {

// Draws an NSImage or an NSImageRep with a given size into a SkBitmap.
SkBitmap NSImageOrNSImageRepToSkBitmap(
    NSImage* image,
    NSImageRep* image_rep,
    NSSize size,
    bool is_opaque) {
  // Only image or image_rep should be provided, not both.
  DCHECK((image != 0) ^ (image_rep != 0));

  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, size.width, size.height);
  if (!bitmap.allocPixels())
    return bitmap;  // Return |bitmap| which should respond true to isNull().

  bitmap.setIsOpaque(is_opaque);

  base::mac::ScopedCFTypeRef<CGColorSpaceRef> color_space(
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB));
  void* data = bitmap.getPixels();

  // Allocate a bitmap context with 4 components per pixel (BGRA). Apple
  // recommends these flags for improved CG performance.
#define HAS_ARGB_SHIFTS(a, r, g, b) \
            (SK_A32_SHIFT == (a) && SK_R32_SHIFT == (r) \
             && SK_G32_SHIFT == (g) && SK_B32_SHIFT == (b))
#if defined(SK_CPU_LENDIAN) && HAS_ARGB_SHIFTS(24, 16, 8, 0)
  base::mac::ScopedCFTypeRef<CGContextRef> context(
    CGBitmapContextCreate(data, size.width, size.height, 8, size.width*4,
                          color_space,
                          kCGImageAlphaPremultipliedFirst |
                              kCGBitmapByteOrder32Host));
#else
#error We require that Skia's and CoreGraphics's recommended \
       image memory layout match.
#endif
#undef HAS_ARGB_SHIFTS

  // Something went really wrong. Best guess is that the bitmap data is invalid.
  DCHECK(context);

  [NSGraphicsContext saveGraphicsState];

  NSGraphicsContext* context_cocoa =
      [NSGraphicsContext graphicsContextWithGraphicsPort:context flipped:NO];
  [NSGraphicsContext setCurrentContext:context_cocoa];

  NSRect drawRect = NSMakeRect(0, 0, size.width, size.height);
  if (image) {
    [image drawInRect:drawRect
             fromRect:NSZeroRect
            operation:NSCompositeCopy
             fraction:1.0];
  } else {
    [image_rep drawInRect:drawRect
                 fromRect:NSZeroRect
                operation:NSCompositeCopy
                 fraction:1.0
           respectFlipped:NO
                    hints:nil];
  }

  [NSGraphicsContext restoreGraphicsState];

  return bitmap;
}

} // namespace

namespace gfx {

CGAffineTransform SkMatrixToCGAffineTransform(const SkMatrix& matrix) {
  // CGAffineTransforms don't support perspective transforms, so make sure
  // we don't get those.
  DCHECK(matrix[SkMatrix::kMPersp0] == 0.0f);
  DCHECK(matrix[SkMatrix::kMPersp1] == 0.0f);
  DCHECK(matrix[SkMatrix::kMPersp2] == 1.0f);

  return CGAffineTransformMake(matrix[SkMatrix::kMScaleX],
                               matrix[SkMatrix::kMSkewY],
                               matrix[SkMatrix::kMSkewX],
                               matrix[SkMatrix::kMScaleY],
                               matrix[SkMatrix::kMTransX],
                               matrix[SkMatrix::kMTransY]);
}

SkRect CGRectToSkRect(const CGRect& rect) {
  SkRect sk_rect = {
    rect.origin.x, rect.origin.y, CGRectGetMaxX(rect), CGRectGetMaxY(rect)
  };
  return sk_rect;
}

CGRect SkIRectToCGRect(const SkIRect& rect) {
  CGRect cg_rect = {
    { rect.fLeft, rect.fTop },
    { rect.fRight - rect.fLeft, rect.fBottom - rect.fTop }
  };
  return cg_rect;
}

CGRect SkRectToCGRect(const SkRect& rect) {
  CGRect cg_rect = {
    { rect.fLeft, rect.fTop },
    { rect.fRight - rect.fLeft, rect.fBottom - rect.fTop }
  };
  return cg_rect;
}

// Converts CGColorRef to the ARGB layout Skia expects.
SkColor CGColorRefToSkColor(CGColorRef color) {
  DCHECK(CGColorGetNumberOfComponents(color) == 4);
  const CGFloat* components = CGColorGetComponents(color);
  return SkColorSetARGB(SkScalarRound(255.0 * components[3]), // alpha
                        SkScalarRound(255.0 * components[0]), // red
                        SkScalarRound(255.0 * components[1]), // green
                        SkScalarRound(255.0 * components[2])); // blue
}

// Converts ARGB to CGColorRef.
CGColorRef CGColorCreateFromSkColor(SkColor color) {
  return CGColorCreateGenericRGB(SkColorGetR(color) / 255.0,
                                 SkColorGetG(color) / 255.0,
                                 SkColorGetB(color) / 255.0,
                                 SkColorGetA(color) / 255.0);
}

// Converts NSColor to ARGB
SkColor NSDeviceColorToSkColor(NSColor* color) {
  DCHECK([color colorSpace] == [NSColorSpace genericRGBColorSpace] ||
         [color colorSpace] == [NSColorSpace deviceRGBColorSpace]);
  CGFloat red, green, blue, alpha;
  color = [color colorUsingColorSpace:[NSColorSpace deviceRGBColorSpace]];
  [color getRed:&red green:&green blue:&blue alpha:&alpha];
  return SkColorSetARGB(SkScalarRound(255.0 * alpha),
                        SkScalarRound(255.0 * red),
                        SkScalarRound(255.0 * green),
                        SkScalarRound(255.0 * blue));
}

// Converts ARGB to NSColor.
NSColor* SkColorToCalibratedNSColor(SkColor color) {
  return [NSColor colorWithCalibratedRed:SkColorGetR(color) / 255.0
                                   green:SkColorGetG(color) / 255.0
                                    blue:SkColorGetB(color) / 255.0
                                   alpha:SkColorGetA(color) / 255.0];
}

NSColor* SkColorToDeviceNSColor(SkColor color) {
  return [NSColor colorWithDeviceRed:SkColorGetR(color) / 255.0
                               green:SkColorGetG(color) / 255.0
                                blue:SkColorGetB(color) / 255.0
                               alpha:SkColorGetA(color) / 255.0];
}

SkBitmap CGImageToSkBitmap(CGImageRef image) {
  if (!image)
    return SkBitmap();

  int width = CGImageGetWidth(image);
  int height = CGImageGetHeight(image);

  scoped_ptr<SkDevice> device(
      skia::BitmapPlatformDevice::Create(NULL, width, height, false));

  CGContextRef context = skia::GetBitmapContext(device.get());

  // We need to invert the y-axis of the canvas so that Core Graphics drawing
  // happens right-side up. Skia has an upper-left origin and CG has a lower-
  // left one.
  CGContextScaleCTM(context, 1.0, -1.0);
  CGContextTranslateCTM(context, 0, -height);

  // We want to copy transparent pixels from |image|, instead of blending it
  // onto uninitialized pixels.
  CGContextSetBlendMode(context, kCGBlendModeCopy);

  CGRect rect = CGRectMake(0, 0, width, height);
  CGContextDrawImage(context, rect, image);

  // Because |device| will be cleaned up and will take its pixels with it, we
  // copy it to the stack and return it.
  SkBitmap bitmap = device->accessBitmap(false);

  return bitmap;
}

SkBitmap NSImageToSkBitmap(NSImage* image, NSSize size, bool is_opaque) {
  return NSImageOrNSImageRepToSkBitmap(image, nil, size, is_opaque);
}

SkBitmap NSImageRepToSkBitmap(
    NSImageRep* image_rep, NSSize size, bool is_opaque) {
  return NSImageOrNSImageRepToSkBitmap(nil, image_rep, size, is_opaque);
}

NSBitmapImageRep* SkBitmapToNSBitmapImageRep(const SkBitmap& skiaBitmap) {
  base::mac::ScopedCFTypeRef<CGColorSpaceRef> color_space(
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB));
  return SkBitmapToNSBitmapImageRepWithColorSpace(skiaBitmap, color_space);
}

NSBitmapImageRep* SkBitmapToNSBitmapImageRepWithColorSpace(
    const SkBitmap& skiaBitmap,
    CGColorSpaceRef colorSpace) {
  // First convert SkBitmap to CGImageRef.
  base::mac::ScopedCFTypeRef<CGImageRef> cgimage(
      SkCreateCGImageRefWithColorspace(skiaBitmap, colorSpace));

  // Now convert to NSBitmapImageRep.
  scoped_nsobject<NSBitmapImageRep> bitmap(
      [[NSBitmapImageRep alloc] initWithCGImage:cgimage]);
  return [bitmap.release() autorelease];
}

NSImage* SkBitmapToNSImageWithColorSpace(const SkBitmap& skiaBitmap,
                                         CGColorSpaceRef colorSpace) {
  if (skiaBitmap.isNull())
    return nil;

  scoped_nsobject<NSImage> image([[NSImage alloc] init]);
  [image addRepresentation:
      SkBitmapToNSBitmapImageRepWithColorSpace(skiaBitmap, colorSpace)];
  [image setSize:NSMakeSize(skiaBitmap.width(), skiaBitmap.height())];
  return [image.release() autorelease];
}

NSImage* SkBitmapToNSImage(const SkBitmap& skiaBitmap) {
  base::mac::ScopedCFTypeRef<CGColorSpaceRef> colorSpace(
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB));
  return SkBitmapToNSImageWithColorSpace(skiaBitmap, colorSpace.get());
}

SkiaBitLocker::SkiaBitLocker(SkCanvas* canvas)
    : canvas_(canvas),
      cgContext_(0) {
}

SkiaBitLocker::~SkiaBitLocker() {
  releaseIfNeeded();
}

// This must be called to balance calls to cgContext
void SkiaBitLocker::releaseIfNeeded() {
  if (!cgContext_)
    return;
  if (useDeviceBits_) {
    bitmap_.unlockPixels();
  } else {
    // Find the bits that were drawn to.
    SkAutoLockPixels lockedPixels(bitmap_);
    const uint32_t* pixelBase
        = reinterpret_cast<uint32_t*>(bitmap_.getPixels());
    int rowPixels = bitmap_.rowBytesAsPixels();
    int width = bitmap_.width();
    int height = bitmap_.height();
    SkIRect bounds;
    bounds.fTop = 0;
    int x;
    int y = -1;
    const uint32_t* pixels = pixelBase;
    while (++y < height) {
      for (x = 0; x < width; ++x) {
        if (pixels[x]) {
          bounds.fTop = y;
          goto foundTop;
        }
      }
      pixels += rowPixels;
    }
foundTop:
    bounds.fBottom = height;
    y = height;
    pixels = pixelBase + rowPixels * (y - 1);
    while (--y > bounds.fTop) {
      for (x = 0; x < width; ++x) {
        if (pixels[x]) {
          bounds.fBottom = y + 1;
          goto foundBottom;
        }
      }
      pixels -= rowPixels;
    }
foundBottom:
    bounds.fLeft = 0;
    x = -1;
    while (++x < width) {
      pixels = pixelBase + rowPixels * bounds.fTop;
      for (y = bounds.fTop; y < bounds.fBottom; ++y) {
        if (pixels[x]) {
          bounds.fLeft = x;
          goto foundLeft;
        }
        pixels += rowPixels;
      }
    }
foundLeft:
    bounds.fRight = width;
    x = width;
    while (--x > bounds.fLeft) {
      pixels = pixelBase + rowPixels * bounds.fTop;
      for (y = bounds.fTop; y < bounds.fBottom; ++y) {
        if (pixels[x]) {
          bounds.fRight = x + 1;
          goto foundRight;
        }
        pixels += rowPixels;
      }
    }
foundRight:
    SkBitmap subset;
    if (!bitmap_.extractSubset(&subset, bounds)) {
        return;
    }
    // Neutralize the global matrix by concatenating the inverse. In the
    // future, Skia may provide some mechanism to set the device portion of
    // the matrix to identity without clobbering any hosting matrix (e.g., the
    // picture's matrix).
    const SkMatrix& skMatrix = canvas_->getTotalMatrix();
    SkMatrix inverse;
    if (!skMatrix.invert(&inverse))
      return;
    canvas_->save();
    canvas_->concat(inverse);
    canvas_->drawBitmap(subset, bounds.fLeft, bounds.fTop);
    canvas_->restore();
  }
  CGContextRelease(cgContext_);
  cgContext_ = 0;
}

CGContextRef SkiaBitLocker::cgContext() {
  SkDevice* device = canvas_->getTopDevice();
  DCHECK(device);
  if (!device)
    return 0;
  releaseIfNeeded(); // This flushes any prior bitmap use
  const SkBitmap& deviceBits = device->accessBitmap(true);
  useDeviceBits_ = deviceBits.getPixels();
  if (useDeviceBits_) {
    bitmap_ = deviceBits;
    bitmap_.lockPixels();
  } else {
    bitmap_.setConfig(
        SkBitmap::kARGB_8888_Config, deviceBits.width(), deviceBits.height());
    bitmap_.allocPixels();
    bitmap_.eraseColor(0);
  }
  base::mac::ScopedCFTypeRef<CGColorSpaceRef> colorSpace(
      CGColorSpaceCreateDeviceRGB());
  cgContext_ = CGBitmapContextCreate(bitmap_.getPixels(), bitmap_.width(),
    bitmap_.height(), 8, bitmap_.rowBytes(), colorSpace, 
    kCGBitmapByteOrder32Host | kCGImageAlphaPremultipliedFirst);

  // Apply device matrix.
  CGAffineTransform contentsTransform = CGAffineTransformMakeScale(1, -1);
  contentsTransform = CGAffineTransformTranslate(contentsTransform, 0,
      -device->height());
  CGContextConcatCTM(cgContext_, contentsTransform);

  const SkIPoint& pt = device->getOrigin();
  // Skip applying the clip when not writing directly to device.
  // They're applied in the offscreen case when the bitmap is drawn.
  if (useDeviceBits_) {
      // Apply clip in device coordinates.
      CGMutablePathRef clipPath = CGPathCreateMutable();
      const SkRegion& clipRgn = canvas_->getTotalClip();
      if (clipRgn.isEmpty()) {
        // CoreGraphics does not consider a newly created path to be empty.
        // Explicitly set it to empty so the subsequent drawing is clipped out.
        // It would be better to make the CGContext hidden if there was a CG
        // call that does that.
        CGPathAddRect(clipPath, 0, CGRectMake(0, 0, 0, 0));
      }
      SkRegion::Iterator iter(clipRgn);
      const SkIPoint& pt = device->getOrigin();
      for (; !iter.done(); iter.next()) {
        SkIRect skRect = iter.rect();
        skRect.offset(-pt);
        CGRect cgRect = SkIRectToCGRect(skRect);
        CGPathAddRect(clipPath, 0, cgRect);
      }
      CGContextAddPath(cgContext_, clipPath);
      CGContextClip(cgContext_);
      CGPathRelease(clipPath);
  }

  // Apply content matrix.
  SkMatrix skMatrix = canvas_->getTotalMatrix();
  skMatrix.postTranslate(-SkIntToScalar(pt.fX), -SkIntToScalar(pt.fY));
  CGAffineTransform affine = SkMatrixToCGAffineTransform(skMatrix);
  CGContextConcatCTM(cgContext_, affine);
  
  return cgContext_;
}

}  // namespace gfx
