// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Cocoa/Cocoa.h>

#include "base/logging.h"
#include "base/memory/scoped_nsobject.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_png_rep.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_util_mac.h"
#include "ui/gfx/image/image_unittest_util.h"

namespace {

// Returns true if the structure of |ns_image| matches the structure
// described by |width|, |height|, and |scale_factors|.
// The structure matches if:
// - |ns_image| is not nil.
// - |ns_image| has NSImageReps of |scale_factors|.
// - Each of the NSImageReps has a pixel size of [|ns_image| size] *
//   scale_factor.
bool NSImageStructureMatches(
    NSImage* ns_image,
    int width,
    int height,
    const std::vector<ui::ScaleFactor>& scale_factors) {
  if (!ns_image ||
      [ns_image size].width != width ||
      [ns_image size].height != height ||
      [ns_image representations].count != scale_factors.size()) {
    return false;
  }

  for (size_t i = 0; i < scale_factors.size(); ++i) {
    float scale = ui::GetScaleFactorScale(scale_factors[i]);
    bool found_match = false;
    for (size_t j = 0; j < [ns_image representations].count; ++j) {
      NSImageRep* ns_image_rep = [[ns_image representations] objectAtIndex:j];
      if (ns_image_rep &&
          [ns_image_rep pixelsWide] == width * scale &&
          [ns_image_rep pixelsHigh] == height * scale) {
        found_match = true;
        break;
      }
    }
    if (!found_match)
      return false;
  }
  return true;
}

void BitmapImageRep(int width, int height,
     NSBitmapImageRep** image_rep) {
  *image_rep = [[[NSBitmapImageRep alloc]
      initWithBitmapDataPlanes:NULL
                    pixelsWide:width
                   pixelsHigh:height
                bitsPerSample:8
              samplesPerPixel:3
                     hasAlpha:NO
                     isPlanar:NO
               colorSpaceName:NSDeviceRGBColorSpace
                 bitmapFormat:0
                  bytesPerRow:0
                 bitsPerPixel:0]
      autorelease];
  unsigned char* image_rep_data = [*image_rep bitmapData];
  for (int i = 0; i < width * height * 3; ++i)
    image_rep_data[i] = 255;
}

class ImageMacTest : public testing::Test {
 public:
  ImageMacTest()
      : supported_scale_factors_(gfx::test::Get1xAnd2xScaleFactors()) {
  }

  virtual ~ImageMacTest() {
  }

 private:
  ui::test::ScopedSetSupportedScaleFactors supported_scale_factors_;

  DISALLOW_COPY_AND_ASSIGN(ImageMacTest);
};

namespace gt = gfx::test;

TEST_F(ImageMacTest, MultiResolutionNSImageToImageSkia) {
  const int kWidth1x = 10;
  const int kHeight1x = 12;
  const int kWidth2x = 20;
  const int kHeight2x = 24;

  NSBitmapImageRep* ns_image_rep1;
  BitmapImageRep(kWidth1x, kHeight1x, &ns_image_rep1);
  NSBitmapImageRep* ns_image_rep2;
  BitmapImageRep(kWidth2x, kHeight2x, &ns_image_rep2);
  scoped_nsobject<NSImage> ns_image(
      [[NSImage alloc] initWithSize:NSMakeSize(kWidth1x, kHeight1x)]);
  [ns_image addRepresentation:ns_image_rep1];
  [ns_image addRepresentation:ns_image_rep2];

  gfx::Image image(ns_image.release());

  EXPECT_EQ(1u, image.RepresentationCount());

  const gfx::ImageSkia* image_skia = image.ToImageSkia();

  std::vector<ui::ScaleFactor> scale_factors;
  scale_factors.push_back(ui::SCALE_FACTOR_100P);
  scale_factors.push_back(ui::SCALE_FACTOR_200P);
  EXPECT_TRUE(gt::ImageSkiaStructureMatches(*image_skia, kWidth1x, kHeight1x,
                                            scale_factors));

  // ToImageSkia should create a second representation.
  EXPECT_EQ(2u, image.RepresentationCount());
}

// Test that convertng to an ImageSkia from an NSImage with scale factors
// other than 1x and 2x results in an ImageSkia with scale factors 1x and
// 2x;
TEST_F(ImageMacTest, UnalignedMultiResolutionNSImageToImageSkia) {
  const int kWidth1x = 10;
  const int kHeight1x= 12;
  const int kWidth4x = 40;
  const int kHeight4x = 48;

  NSBitmapImageRep* ns_image_rep4;
  BitmapImageRep(kWidth4x, kHeight4x, &ns_image_rep4);
  scoped_nsobject<NSImage> ns_image(
      [[NSImage alloc] initWithSize:NSMakeSize(kWidth1x, kHeight1x)]);
  [ns_image addRepresentation:ns_image_rep4];

  gfx::Image image(ns_image.release());

  EXPECT_EQ(1u, image.RepresentationCount());

  const gfx::ImageSkia* image_skia = image.ToImageSkia();

  std::vector<ui::ScaleFactor> scale_factors;
  scale_factors.push_back(ui::SCALE_FACTOR_100P);
  scale_factors.push_back(ui::SCALE_FACTOR_200P);
  EXPECT_TRUE(gt::ImageSkiaStructureMatches(*image_skia, kWidth1x, kHeight1x,
                                            scale_factors));

  // ToImageSkia should create a second representation.
  EXPECT_EQ(2u, image.RepresentationCount());
}

TEST_F(ImageMacTest, MultiResolutionImageSkiaToNSImage) {
  const int kWidth1x = 10;
  const int kHeight1x= 12;
  const int kWidth2x = 20;
  const int kHeight2x = 24;

  gfx::ImageSkia image_skia;
  image_skia.AddRepresentation(gfx::ImageSkiaRep(
      gt::CreateBitmap(kWidth1x, kHeight1x), ui::SCALE_FACTOR_100P));
  image_skia.AddRepresentation(gfx::ImageSkiaRep(
      gt::CreateBitmap(kWidth2x, kHeight2x), ui::SCALE_FACTOR_200P));

  gfx::Image image(image_skia);

  EXPECT_EQ(1u, image.RepresentationCount());
  EXPECT_EQ(2u, image.ToImageSkia()->image_reps().size());

  NSImage* ns_image = image.ToNSImage();

  std::vector<ui::ScaleFactor> scale_factors;
  scale_factors.push_back(ui::SCALE_FACTOR_100P);
  scale_factors.push_back(ui::SCALE_FACTOR_200P);
  EXPECT_TRUE(NSImageStructureMatches(ns_image, kWidth1x, kHeight1x,
                                      scale_factors));

  // Request for NSImage* should create a second representation.
  EXPECT_EQ(2u, image.RepresentationCount());
}

TEST_F(ImageMacTest, MultiResolutionPNGToNSImage) {
  const int kSize1x = 25;
  const int kSize2x = 50;

  scoped_refptr<base::RefCountedMemory> bytes1x = gt::CreatePNGBytes(kSize1x);
  scoped_refptr<base::RefCountedMemory> bytes2x = gt::CreatePNGBytes(kSize2x);
  std::vector<gfx::ImagePNGRep> image_png_reps;
  image_png_reps.push_back(gfx::ImagePNGRep(bytes1x, ui::SCALE_FACTOR_100P));
  image_png_reps.push_back(gfx::ImagePNGRep(bytes2x, ui::SCALE_FACTOR_200P));

  gfx::Image image(image_png_reps);

  NSImage* ns_image = image.ToNSImage();
  std::vector<ui::ScaleFactor> scale_factors;
  scale_factors.push_back(ui::SCALE_FACTOR_100P);
  scale_factors.push_back(ui::SCALE_FACTOR_200P);
  EXPECT_TRUE(NSImageStructureMatches(ns_image, kSize1x, kSize1x,
                                      scale_factors));

  // Converting from PNG to NSImage should not go through ImageSkia.
  EXPECT_FALSE(image.HasRepresentation(gfx::Image::kImageRepSkia));

  // Convert to ImageSkia to check pixel contents of NSImageReps.
  gfx::ImageSkia image_skia = gfx::ImageSkiaFromNSImage(ns_image);
  EXPECT_TRUE(gt::IsEqual(bytes1x,
      image_skia.GetRepresentation(ui::SCALE_FACTOR_100P).sk_bitmap()));
  EXPECT_TRUE(gt::IsEqual(bytes2x,
      image_skia.GetRepresentation(ui::SCALE_FACTOR_200P).sk_bitmap()));
}

} // namespace
