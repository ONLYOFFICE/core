// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/transform_util.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/point.h"

namespace gfx {
namespace {

TEST(TransformUtilTest, GetScaleTransform) {
  const Point kAnchor(20, 40);
  const float kScale = 0.5f;

  Transform scale = GetScaleTransform(kAnchor, kScale);

  const int kOffset = 10;
  for (int sign_x = -1; sign_x <= 1; ++sign_x) {
    for (int sign_y = -1; sign_y <= 1; ++sign_y) {
      Point test(kAnchor.x() + sign_x * kOffset,
                 kAnchor.y() + sign_y * kOffset);
      scale.TransformPoint(test);

      EXPECT_EQ(Point(kAnchor.x() + sign_x * kOffset * kScale,
                      kAnchor.y() + sign_y * kOffset * kScale),
                test);
    }
  }
}

}  // namespace
}  // namespace gfx
