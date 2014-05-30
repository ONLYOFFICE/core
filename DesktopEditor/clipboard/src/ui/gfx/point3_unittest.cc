// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/point3_f.h"

#include "base/basictypes.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace gfx {

TEST(Point3Test, VectorArithmetic) {
  gfx::Point3F a(1.6f, 5.1f, 3.2f);
  gfx::Vector3dF v1(3.1f, -3.2f, 9.3f);
  gfx::Vector3dF v2(-8.1f, 1.2f, 3.3f);

  static const struct {
    gfx::Point3F expected;
    gfx::Point3F actual;
  } tests[] = {
    { gfx::Point3F(4.7f, 1.9f, 12.5f), a + v1 },
    { gfx::Point3F(-1.5f, 8.3f, -6.1f), a - v1 },
    { a, a - v1 + v1 },
    { a, a + v1 - v1 },
    { a, a + gfx::Vector3dF() },
    { gfx::Point3F(12.8f, 0.7f, 9.2f), a + v1 - v2 },
    { gfx::Point3F(-9.6f, 9.5f, -2.8f), a - v1 + v2 }
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(tests); ++i)
    EXPECT_EQ(tests[i].expected.ToString(),
              tests[i].actual.ToString());
}

TEST(Point3Test, VectorFromPoints) {
  gfx::Point3F a(1.6f, 5.2f, 3.2f);
  gfx::Vector3dF v1(3.1f, -3.2f, 9.3f);

  gfx::Point3F b(a + v1);
  EXPECT_EQ((b - a).ToString(), v1.ToString());
}

}  // namespace gfx
