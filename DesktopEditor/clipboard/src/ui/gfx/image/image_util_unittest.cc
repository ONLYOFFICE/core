// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/image/image_util.h"

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image_unittest_util.h"

TEST(ImageUtilTest, JPEGEncodeAndDecode) {
  gfx::Image original = gfx::test::CreateImage(100, 100);

  std::vector<unsigned char> encoded;
  ASSERT_TRUE(gfx::JPEG1xEncodedDataFromImage(original, 80, &encoded));

  gfx::Image decoded =
      gfx::ImageFrom1xJPEGEncodedData(&encoded.front(), encoded.size());

  // JPEG is lossy, so simply check that the image decoded successfully.
  EXPECT_FALSE(decoded.IsEmpty());
}
