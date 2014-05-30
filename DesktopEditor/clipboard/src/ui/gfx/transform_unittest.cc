// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MSVC++ requires this to be set before any other includes to get M_PI.
#define _USE_MATH_DEFINES

#include "ui/gfx/transform.h"

#include <cmath>
#include <ostream>
#include <limits>

#include "base/basictypes.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/point.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/transform_util.h"
#include "ui/gfx/vector3d_f.h"

namespace gfx {

namespace {

#define EXPECT_ROW1_EQ(a, b, c, d, transform)               \
    EXPECT_FLOAT_EQ((a), (transform).matrix().get(0, 0));   \
    EXPECT_FLOAT_EQ((b), (transform).matrix().get(0, 1));   \
    EXPECT_FLOAT_EQ((c), (transform).matrix().get(0, 2));   \
    EXPECT_FLOAT_EQ((d), (transform).matrix().get(0, 3));

#define EXPECT_ROW2_EQ(a, b, c, d, transform)               \
    EXPECT_FLOAT_EQ((a), (transform).matrix().get(1, 0));   \
    EXPECT_FLOAT_EQ((b), (transform).matrix().get(1, 1));   \
    EXPECT_FLOAT_EQ((c), (transform).matrix().get(1, 2));   \
    EXPECT_FLOAT_EQ((d), (transform).matrix().get(1, 3));

#define EXPECT_ROW3_EQ(a, b, c, d, transform)               \
    EXPECT_FLOAT_EQ((a), (transform).matrix().get(2, 0));   \
    EXPECT_FLOAT_EQ((b), (transform).matrix().get(2, 1));   \
    EXPECT_FLOAT_EQ((c), (transform).matrix().get(2, 2));   \
    EXPECT_FLOAT_EQ((d), (transform).matrix().get(2, 3));

#define EXPECT_ROW4_EQ(a, b, c, d, transform)               \
    EXPECT_FLOAT_EQ((a), (transform).matrix().get(3, 0));   \
    EXPECT_FLOAT_EQ((b), (transform).matrix().get(3, 1));   \
    EXPECT_FLOAT_EQ((c), (transform).matrix().get(3, 2));   \
    EXPECT_FLOAT_EQ((d), (transform).matrix().get(3, 3));   \

// Checking float values for equality close to zero is not robust using
// EXPECT_FLOAT_EQ (see gtest documentation). So, to verify rotation matrices,
// we must use a looser absolute error threshold in some places.
#define EXPECT_ROW1_NEAR(a, b, c, d, transform, errorThreshold)         \
    EXPECT_NEAR((a), (transform).matrix().get(0, 0), (errorThreshold)); \
    EXPECT_NEAR((b), (transform).matrix().get(0, 1), (errorThreshold)); \
    EXPECT_NEAR((c), (transform).matrix().get(0, 2), (errorThreshold)); \
    EXPECT_NEAR((d), (transform).matrix().get(0, 3), (errorThreshold));

#define EXPECT_ROW2_NEAR(a, b, c, d, transform, errorThreshold)         \
    EXPECT_NEAR((a), (transform).matrix().get(1, 0), (errorThreshold)); \
    EXPECT_NEAR((b), (transform).matrix().get(1, 1), (errorThreshold)); \
    EXPECT_NEAR((c), (transform).matrix().get(1, 2), (errorThreshold)); \
    EXPECT_NEAR((d), (transform).matrix().get(1, 3), (errorThreshold));

#define EXPECT_ROW3_NEAR(a, b, c, d, transform, errorThreshold)         \
    EXPECT_NEAR((a), (transform).matrix().get(2, 0), (errorThreshold)); \
    EXPECT_NEAR((b), (transform).matrix().get(2, 1), (errorThreshold)); \
    EXPECT_NEAR((c), (transform).matrix().get(2, 2), (errorThreshold)); \
    EXPECT_NEAR((d), (transform).matrix().get(2, 3), (errorThreshold));

bool PointsAreNearlyEqual(const Point3F& lhs,
                          const Point3F& rhs) {
  float epsilon = 0.0001f;
  return lhs.SquaredDistanceTo(rhs) < epsilon;
}

bool MatricesAreNearlyEqual(const Transform& lhs,
                            const Transform& rhs) {
  float epsilon = 0.0001f;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      if (std::abs(lhs.matrix().get(row, col) -
                   rhs.matrix().get(row, col)) > epsilon)
        return false;
    }
  }
  return true;
}

void InitializeTestMatrix(Transform* transform) {
  SkMatrix44& matrix = transform->matrix();
  matrix.setDouble(0, 0, 10.0);
  matrix.setDouble(1, 0, 11.0);
  matrix.setDouble(2, 0, 12.0);
  matrix.setDouble(3, 0, 13.0);
  matrix.setDouble(0, 1, 14.0);
  matrix.setDouble(1, 1, 15.0);
  matrix.setDouble(2, 1, 16.0);
  matrix.setDouble(3, 1, 17.0);
  matrix.setDouble(0, 2, 18.0);
  matrix.setDouble(1, 2, 19.0);
  matrix.setDouble(2, 2, 20.0);
  matrix.setDouble(3, 2, 21.0);
  matrix.setDouble(0, 3, 22.0);
  matrix.setDouble(1, 3, 23.0);
  matrix.setDouble(2, 3, 24.0);
  matrix.setDouble(3, 3, 25.0);

  // Sanity check
  EXPECT_ROW1_EQ(10.0f, 14.0f, 18.0f, 22.0f, (*transform));
  EXPECT_ROW2_EQ(11.0f, 15.0f, 19.0f, 23.0f, (*transform));
  EXPECT_ROW3_EQ(12.0f, 16.0f, 20.0f, 24.0f, (*transform));
  EXPECT_ROW4_EQ(13.0f, 17.0f, 21.0f, 25.0f, (*transform));
}

void InitializeTestMatrix2(Transform* transform) {
  SkMatrix44& matrix = transform->matrix();
  matrix.setDouble(0, 0, 30.0);
  matrix.setDouble(1, 0, 31.0);
  matrix.setDouble(2, 0, 32.0);
  matrix.setDouble(3, 0, 33.0);
  matrix.setDouble(0, 1, 34.0);
  matrix.setDouble(1, 1, 35.0);
  matrix.setDouble(2, 1, 36.0);
  matrix.setDouble(3, 1, 37.0);
  matrix.setDouble(0, 2, 38.0);
  matrix.setDouble(1, 2, 39.0);
  matrix.setDouble(2, 2, 40.0);
  matrix.setDouble(3, 2, 41.0);
  matrix.setDouble(0, 3, 42.0);
  matrix.setDouble(1, 3, 43.0);
  matrix.setDouble(2, 3, 44.0);
  matrix.setDouble(3, 3, 45.0);

  // Sanity check
  EXPECT_ROW1_EQ(30.0f, 34.0f, 38.0f, 42.0f, (*transform));
  EXPECT_ROW2_EQ(31.0f, 35.0f, 39.0f, 43.0f, (*transform));
  EXPECT_ROW3_EQ(32.0f, 36.0f, 40.0f, 44.0f, (*transform));
  EXPECT_ROW4_EQ(33.0f, 37.0f, 41.0f, 45.0f, (*transform));
}

#ifdef SK_MSCALAR_IS_DOUBLE
#define ERROR_THRESHOLD 1e-14
#else
#define ERROR_THRESHOLD 1e-7
#endif
#define LOOSE_ERROR_THRESHOLD 1e-7

TEST(XFormTest, Equality) {
  Transform lhs, rhs, interpolated;
  rhs.matrix().set3x3(1, 2, 3,
                      4, 5, 6,
                      7, 8, 9);
  interpolated = lhs;
  for (int i = 0; i <= 100; ++i) {
    for (int row = 0; row < 4; ++row) {
      for (int col = 0; col < 4; ++col) {
        float a = lhs.matrix().get(row, col);
        float b = rhs.matrix().get(row, col);
        float t = i / 100.0f;
        interpolated.matrix().set(row, col, a + (b - a) * t);
      }
    }
    if (i == 100) {
      EXPECT_TRUE(rhs == interpolated);
    } else {
      EXPECT_TRUE(rhs != interpolated);
    }
  }
  lhs = Transform();
  rhs = Transform();
  for (int i = 1; i < 100; ++i) {
    lhs.MakeIdentity();
    rhs.MakeIdentity();
    lhs.Translate(i, i);
    rhs.Translate(-i, -i);
    EXPECT_TRUE(lhs != rhs);
    rhs.Translate(2*i, 2*i);
    EXPECT_TRUE(lhs == rhs);
  }
}

TEST(XFormTest, ConcatTranslate) {
  static const struct TestCase {
    int x1;
    int y1;
    float tx;
    float ty;
    int x2;
    int y2;
  } test_cases[] = {
    { 0, 0, 10.0f, 20.0f, 10, 20 },
    { 0, 0, -10.0f, -20.0f, 0, 0 },
    { 0, 0, -10.0f, -20.0f, -10, -20 },
    { 0, 0,
      std::numeric_limits<float>::quiet_NaN(),
      std::numeric_limits<float>::quiet_NaN(),
      10, 20 },
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform translation;
    translation.Translate(value.tx, value.ty);
    xform = translation * xform;
    Point3F p1(value.x1, value.y1, 0);
    Point3F p2(value.x2, value.y2, 0);
    xform.TransformPoint(p1);
    if (value.tx == value.tx &&
        value.ty == value.ty) {
      EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
    }
  }
}

TEST(XFormTest, ConcatScale) {
  static const struct TestCase {
    int before;
    float scale;
    int after;
  } test_cases[] = {
    { 1, 10.0f, 10 },
    { 1, .1f, 1 },
    { 1, 100.0f, 100 },
    { 1, -1.0f, -100 },
    { 1, std::numeric_limits<float>::quiet_NaN(), 1 }
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform scale;
    scale.Scale(value.scale, value.scale);
    xform = scale * xform;
    Point3F p1(value.before, value.before, 0);
    Point3F p2(value.after, value.after, 0);
    xform.TransformPoint(p1);
    if (value.scale == value.scale) {
      EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
    }
  }
}

TEST(XFormTest, ConcatRotate) {
  static const struct TestCase {
    int x1;
    int y1;
    float degrees;
    int x2;
    int y2;
  } test_cases[] = {
    { 1, 0, 90.0f, 0, 1 },
    { 1, 0, -90.0f, 1, 0 },
    { 1, 0, 90.0f, 0, 1 },
    { 1, 0, 360.0f, 0, 1 },
    { 1, 0, 0.0f, 0, 1 },
    { 1, 0, std::numeric_limits<float>::quiet_NaN(), 1, 0 }
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform rotation;
    rotation.Rotate(value.degrees);
    xform = rotation * xform;
    Point3F p1(value.x1, value.y1, 0);
    Point3F p2(value.x2, value.y2, 0);
    xform.TransformPoint(p1);
    if (value.degrees == value.degrees) {
      EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
    }
  }
}

TEST(XFormTest, SetTranslate) {
  static const struct TestCase {
    int x1; int y1;
    float tx; float ty;
    int x2; int y2;
  } test_cases[] = {
    { 0, 0, 10.0f, 20.0f, 10, 20 },
    { 10, 20, 10.0f, 20.0f, 20, 40 },
    { 10, 20, 0.0f, 0.0f, 10, 20 },
    { 0, 0,
      std::numeric_limits<float>::quiet_NaN(),
      std::numeric_limits<float>::quiet_NaN(),
      0, 0 }
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    for (int k = 0; k < 3; ++k) {
      Point3F p0, p1, p2;
      Transform xform;
      switch (k) {
      case 0:
        p1.SetPoint(value.x1, 0, 0);
        p2.SetPoint(value.x2, 0, 0);
        xform.Translate(value.tx, 0.0);
        break;
      case 1:
        p1.SetPoint(0, value.y1, 0);
        p2.SetPoint(0, value.y2, 0);
        xform.Translate(0.0, value.ty);
        break;
      case 2:
        p1.SetPoint(value.x1, value.y1, 0);
        p2.SetPoint(value.x2, value.y2, 0);
        xform.Translate(value.tx, value.ty);
        break;
      }
      p0 = p1;
      xform.TransformPoint(p1);
      if (value.tx == value.tx &&
          value.ty == value.ty) {
        EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
        xform.TransformPointReverse(p1);
        EXPECT_TRUE(PointsAreNearlyEqual(p1, p0));
      }
    }
  }
}

TEST(XFormTest, SetScale) {
  static const struct TestCase {
    int before;
    float s;
    int after;
  } test_cases[] = {
    { 1, 10.0f, 10 },
    { 1, 1.0f, 1 },
    { 1, 0.0f, 0 },
    { 0, 10.0f, 0 },
    { 1, std::numeric_limits<float>::quiet_NaN(), 0 },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    for (int k = 0; k < 3; ++k) {
      Point3F p0, p1, p2;
      Transform xform;
      switch (k) {
      case 0:
        p1.SetPoint(value.before, 0, 0);
        p2.SetPoint(value.after, 0, 0);
        xform.Scale(value.s, 1.0);
        break;
      case 1:
        p1.SetPoint(0, value.before, 0);
        p2.SetPoint(0, value.after, 0);
        xform.Scale(1.0, value.s);
        break;
      case 2:
        p1.SetPoint(value.before, value.before, 0);
        p2.SetPoint(value.after, value.after, 0);
        xform.Scale(value.s, value.s);
        break;
      }
      p0 = p1;
      xform.TransformPoint(p1);
      if (value.s == value.s) {
        EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
        if (value.s != 0.0f) {
          xform.TransformPointReverse(p1);
          EXPECT_TRUE(PointsAreNearlyEqual(p1, p0));
        }
      }
    }
  }
}

TEST(XFormTest, SetRotate) {
  static const struct SetRotateCase {
    int x;
    int y;
    float degree;
    int xprime;
    int yprime;
  } set_rotate_cases[] = {
    { 100, 0, 90.0f, 0, 100 },
    { 0, 0, 90.0f, 0, 0 },
    { 0, 100, 90.0f, -100, 0 },
    { 0, 1, -90.0f, 1, 0 },
    { 100, 0, 0.0f, 100, 0 },
    { 0, 0, 0.0f, 0, 0 },
    { 0, 0, std::numeric_limits<float>::quiet_NaN(), 0, 0 },
    { 100, 0, 360.0f, 100, 0 }
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(set_rotate_cases); ++i) {
    const SetRotateCase& value = set_rotate_cases[i];
    Point3F p0;
    Point3F p1(value.x, value.y, 0);
    Point3F p2(value.xprime, value.yprime, 0);
    p0 = p1;
    Transform xform;
    xform.Rotate(value.degree);
    // just want to make sure that we don't crash in the case of NaN.
    if (value.degree == value.degree) {
      xform.TransformPoint(p1);
      EXPECT_TRUE(PointsAreNearlyEqual(p1, p2));
      xform.TransformPointReverse(p1);
      EXPECT_TRUE(PointsAreNearlyEqual(p1, p0));
    }
  }
}

// 2D tests
TEST(XFormTest, ConcatTranslate2D) {
  static const struct TestCase {
    int x1;
    int y1;
    float tx;
    float ty;
    int x2;
    int y2;
  } test_cases[] = {
    { 0, 0, 10.0f, 20.0f, 10, 20},
    { 0, 0, -10.0f, -20.0f, 0, 0},
    { 0, 0, -10.0f, -20.0f, -10, -20},
    { 0, 0,
      std::numeric_limits<float>::quiet_NaN(),
      std::numeric_limits<float>::quiet_NaN(),
      10, 20},
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform translation;
    translation.Translate(value.tx, value.ty);
    xform = translation * xform;
    Point p1(value.x1, value.y1);
    Point p2(value.x2, value.y2);
    xform.TransformPoint(p1);
    if (value.tx == value.tx &&
        value.ty == value.ty) {
      EXPECT_EQ(p1.x(), p2.x());
      EXPECT_EQ(p1.y(), p2.y());
    }
  }
}

TEST(XFormTest, ConcatScale2D) {
  static const struct TestCase {
    int before;
    float scale;
    int after;
  } test_cases[] = {
    { 1, 10.0f, 10},
    { 1, .1f, 1},
    { 1, 100.0f, 100},
    { 1, -1.0f, -100},
    { 1, std::numeric_limits<float>::quiet_NaN(), 1}
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform scale;
    scale.Scale(value.scale, value.scale);
    xform = scale * xform;
    Point p1(value.before, value.before);
    Point p2(value.after, value.after);
    xform.TransformPoint(p1);
    if (value.scale == value.scale) {
      EXPECT_EQ(p1.x(), p2.x());
      EXPECT_EQ(p1.y(), p2.y());
    }
  }
}

TEST(XFormTest, ConcatRotate2D) {
  static const struct TestCase {
    int x1;
    int y1;
    float degrees;
    int x2;
    int y2;
  } test_cases[] = {
    { 1, 0, 90.0f, 0, 1},
    { 1, 0, -90.0f, 1, 0},
    { 1, 0, 90.0f, 0, 1},
    { 1, 0, 360.0f, 0, 1},
    { 1, 0, 0.0f, 0, 1},
    { 1, 0, std::numeric_limits<float>::quiet_NaN(), 1, 0}
  };

  Transform xform;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    Transform rotation;
    rotation.Rotate(value.degrees);
    xform = rotation * xform;
    Point p1(value.x1, value.y1);
    Point p2(value.x2, value.y2);
    xform.TransformPoint(p1);
    if (value.degrees == value.degrees) {
      EXPECT_EQ(p1.x(), p2.x());
      EXPECT_EQ(p1.y(), p2.y());
    }
  }
}

TEST(XFormTest, SetTranslate2D) {
  static const struct TestCase {
    int x1; int y1;
    float tx; float ty;
    int x2; int y2;
  } test_cases[] = {
    { 0, 0, 10.0f, 20.0f, 10, 20},
    { 10, 20, 10.0f, 20.0f, 20, 40},
    { 10, 20, 0.0f, 0.0f, 10, 20},
    { 0, 0,
      std::numeric_limits<float>::quiet_NaN(),
      std::numeric_limits<float>::quiet_NaN(),
      0, 0}
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    for (int j = -1; j < 2; ++j) {
      for (int k = 0; k < 3; ++k) {
        float epsilon = 0.0001f;
        Point p0, p1, p2;
        Transform xform;
        switch (k) {
        case 0:
          p1.SetPoint(value.x1, 0);
          p2.SetPoint(value.x2, 0);
          xform.Translate(value.tx + j * epsilon, 0.0);
          break;
        case 1:
          p1.SetPoint(0, value.y1);
          p2.SetPoint(0, value.y2);
          xform.Translate(0.0, value.ty + j * epsilon);
          break;
        case 2:
          p1.SetPoint(value.x1, value.y1);
          p2.SetPoint(value.x2, value.y2);
          xform.Translate(value.tx + j * epsilon,
                          value.ty + j * epsilon);
          break;
        }
        p0 = p1;
        xform.TransformPoint(p1);
        if (value.tx == value.tx &&
            value.ty == value.ty) {
          EXPECT_EQ(p1.x(), p2.x());
          EXPECT_EQ(p1.y(), p2.y());
          xform.TransformPointReverse(p1);
          EXPECT_EQ(p1.x(), p0.x());
          EXPECT_EQ(p1.y(), p0.y());
        }
      }
    }
  }
}

TEST(XFormTest, SetScale2D) {
  static const struct TestCase {
    int before;
    float s;
    int after;
  } test_cases[] = {
    { 1, 10.0f, 10},
    { 1, 1.0f, 1},
    { 1, 0.0f, 0},
    { 0, 10.0f, 0},
    { 1, std::numeric_limits<float>::quiet_NaN(), 0},
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(test_cases); ++i) {
    const TestCase& value = test_cases[i];
    for (int j = -1; j < 2; ++j) {
      for (int k = 0; k < 3; ++k) {
        float epsilon = 0.0001f;
        Point p0, p1, p2;
        Transform xform;
        switch (k) {
        case 0:
          p1.SetPoint(value.before, 0);
          p2.SetPoint(value.after, 0);
          xform.Scale(value.s + j * epsilon, 1.0);
          break;
        case 1:
          p1.SetPoint(0, value.before);
          p2.SetPoint(0, value.after);
          xform.Scale(1.0, value.s + j * epsilon);
          break;
        case 2:
          p1.SetPoint(value.before,
                      value.before);
          p2.SetPoint(value.after,
                      value.after);
          xform.Scale(value.s + j * epsilon,
                      value.s + j * epsilon);
          break;
        }
        p0 = p1;
        xform.TransformPoint(p1);
        if (value.s == value.s) {
          EXPECT_EQ(p1.x(), p2.x());
          EXPECT_EQ(p1.y(), p2.y());
          if (value.s != 0.0f) {
            xform.TransformPointReverse(p1);
            EXPECT_EQ(p1.x(), p0.x());
            EXPECT_EQ(p1.y(), p0.y());
          }
        }
      }
    }
  }
}

TEST(XFormTest, SetRotate2D) {
  static const struct SetRotateCase {
    int x;
    int y;
    float degree;
    int xprime;
    int yprime;
  } set_rotate_cases[] = {
    { 100, 0, 90.0f, 0, 100},
    { 0, 0, 90.0f, 0, 0},
    { 0, 100, 90.0f, -100, 0},
    { 0, 1, -90.0f, 1, 0},
    { 100, 0, 0.0f, 100, 0},
    { 0, 0, 0.0f, 0, 0},
    { 0, 0, std::numeric_limits<float>::quiet_NaN(), 0, 0},
    { 100, 0, 360.0f, 100, 0}
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(set_rotate_cases); ++i) {
    const SetRotateCase& value = set_rotate_cases[i];
    for (int j = 1; j >= -1; --j) {
      float epsilon = 0.1f;
      Point pt(value.x, value.y);
      Transform xform;
      // should be invariant to small floating point errors.
      xform.Rotate(value.degree + j * epsilon);
      // just want to make sure that we don't crash in the case of NaN.
      if (value.degree == value.degree) {
        xform.TransformPoint(pt);
        EXPECT_EQ(value.xprime, pt.x());
        EXPECT_EQ(value.yprime, pt.y());
        xform.TransformPointReverse(pt);
        EXPECT_EQ(pt.x(), value.x);
        EXPECT_EQ(pt.y(), value.y);
      }
    }
  }
}

TEST(XFormTest, BlendTranslate) {
  Transform from;
  for (int i = 0; i < 10; ++i) {
    Transform to;
    to.Translate3d(1, 1, 1);
    double t = i / 9.0;
    EXPECT_TRUE(to.Blend(from, t));
    EXPECT_FLOAT_EQ(t, to.matrix().get(0, 3));
    EXPECT_FLOAT_EQ(t, to.matrix().get(1, 3));
    EXPECT_FLOAT_EQ(t, to.matrix().get(2, 3));
  }
}

TEST(XFormTest, BlendRotate) {
  Vector3dF axes[] = {
    Vector3dF(1, 0, 0),
    Vector3dF(0, 1, 0),
    Vector3dF(0, 0, 1),
    Vector3dF(1, 1, 1)
  };
  Transform from;
  for (size_t index = 0; index < ARRAYSIZE_UNSAFE(axes); ++index) {
    for (int i = 0; i < 10; ++i) {
      Transform to;
      to.RotateAbout(axes[index], 90);
      double t = i / 9.0;
      EXPECT_TRUE(to.Blend(from, t));

      Transform expected;
      expected.RotateAbout(axes[index], 90 * t);

      EXPECT_TRUE(MatricesAreNearlyEqual(expected, to));
    }
  }
}

TEST(XFormTest, CanBlend180DegreeRotation) {
  Vector3dF axes[] = {
    Vector3dF(1, 0, 0),
    Vector3dF(0, 1, 0),
    Vector3dF(0, 0, 1),
    Vector3dF(1, 1, 1)
  };
  Transform from;
  for (size_t index = 0; index < ARRAYSIZE_UNSAFE(axes); ++index) {
    for (int i = 0; i < 10; ++i) {
      Transform to;
      to.RotateAbout(axes[index], 180);
      double t = i / 9.0;
      EXPECT_TRUE(to.Blend(from, t));

      Transform expected;
      expected.RotateAbout(axes[index], 180 * t);

      EXPECT_TRUE(MatricesAreNearlyEqual(expected, to));
    }
  }
}

TEST(XFormTest, BlendScale) {
  Transform from;
  for (int i = 0; i < 10; ++i) {
    Transform to;
    to.Scale3d(5, 4, 3);
    double t = i / 9.0;
    EXPECT_TRUE(to.Blend(from, t));
    EXPECT_FLOAT_EQ(t * 4 + 1, to.matrix().get(0, 0));
    EXPECT_FLOAT_EQ(t * 3 + 1, to.matrix().get(1, 1));
    EXPECT_FLOAT_EQ(t * 2 + 1, to.matrix().get(2, 2));
  }
}

TEST(XFormTest, BlendSkew) {
  Transform from;
  for (int i = 0; i < 2; ++i) {
    Transform to;
    to.SkewX(20);
    to.SkewY(10);
    double t = i;
    Transform expected;
    expected.SkewX(t * 20);
    expected.SkewY(t * 10);
    EXPECT_TRUE(to.Blend(from, t));
    EXPECT_TRUE(MatricesAreNearlyEqual(expected, to));
  }
}

TEST(XFormTest, BlendPerspective) {
  Transform from;
  from.ApplyPerspectiveDepth(200);
  for (int i = 0; i < 2; ++i) {
    Transform to;
    to.ApplyPerspectiveDepth(800);
    double t = i;
    Transform expected;
    expected.ApplyPerspectiveDepth(t * 600 + 200);
    EXPECT_TRUE(to.Blend(from, t));
    EXPECT_TRUE(MatricesAreNearlyEqual(expected, to));
  }
}

TEST(XFormTest, BlendIdentity) {
  Transform from;
  Transform to;
  EXPECT_TRUE(to.Blend(from, 0.5));
  EXPECT_EQ(to, from);
}

TEST(XFormTest, CannotBlendSingularMatrix) {
  Transform from;
  Transform to;
  to.matrix().set(1, 1, SkDoubleToMScalar(0));
  EXPECT_FALSE(to.Blend(from, 0.5));
}

TEST(XFormTest, VerifyBlendForTranslation) {
  Transform from;
  from.Translate3d(100.0, 200.0, 100.0);

  Transform to;

  to.Translate3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  to = Transform();
  to.Translate3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 125.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 175.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 150.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f,  1.0f,  to);

  to = Transform();
  to.Translate3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 150.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 150.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 200.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f,  1.0f,  to);

  to = Transform();
  to.Translate3d(200.0, 100.0, 300.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 200.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 100.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 300.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f,  1.0f,  to);
}

TEST(XFormTest, VerifyBlendForScale) {
  Transform from;
  from.Scale3d(100.0, 200.0, 100.0);

  Transform to;

  to.Scale3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  to = Transform();
  to.Scale3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_EQ(125.0f, 0.0f,  0.0f,  0.0f, to);
  EXPECT_ROW2_EQ(0.0f,  175.0f, 0.0f,  0.0f, to);
  EXPECT_ROW3_EQ(0.0f,   0.0f, 150.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f,   0.0f,  0.0f,  1.0f, to);

  to = Transform();
  to.Scale3d(200.0, 100.0, 300.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_EQ(150.0f, 0.0f,  0.0f,  0.0f, to);
  EXPECT_ROW2_EQ(0.0f,  150.0f, 0.0f,  0.0f, to);
  EXPECT_ROW3_EQ(0.0f,   0.0f, 200.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f,   0.0f,  0.0f,  1.0f, to);

  to = Transform();
  to.Scale3d(200.0, 100.0, 300.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_EQ(200.0f, 0.0f,  0.0f,  0.0f, to);
  EXPECT_ROW2_EQ(0.0f,  100.0f, 0.0f,  0.0f, to);
  EXPECT_ROW3_EQ(0.0f,   0.0f, 300.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f,   0.0f,  0.0f,  1.0f, to);
}

TEST(XFormTest, VerifyBlendForSkewX) {
  Transform from;
  from.SkewX(0.0);

  Transform to;

  to.SkewX(45.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  to = Transform();
  to.SkewX(45.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_EQ(1.0f, 0.5f, 0.0f, 0.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.SkewX(45.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_EQ(1.0f, 0.25f, 0.0f, 0.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f,  0.0f, 0.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f,  1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f,  0.0f, 1.0f, to);

  to = Transform();
  to.SkewX(45.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_EQ(1.0f, 1.0f, 0.0f, 0.0f, to);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, to);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);
}

TEST(XFormTest, VerifyBlendForSkewY) {
  // NOTE CAREFULLY: Decomposition of skew and rotation terms of the matrix
  // is inherently underconstrained, and so it does not always compute the
  // originally intended skew parameters. The current implementation uses QR
  // decomposition, which decomposes the shear into a rotation + non-uniform
  // scale.
  //
  // It is unlikely that the decomposition implementation will need to change
  // very often, so to get any test coverage, the compromise is to verify the
  // exact matrix that the.Blend() operation produces.
  //
  // This problem also potentially exists for skewX, but the current QR
  // decomposition implementation just happens to decompose those test
  // matrices intuitively.
  //
  // Unfortunately, this case suffers from uncomfortably large precision
  // error.

  Transform from;
  from.SkewY(0.0);

  Transform to;

  to.SkewY(45.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  to = Transform();
  to.SkewY(45.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_NEAR(1.0823489449280947471976333,
                   0.0464370719145053845178239,
                   0.0,
                   0.0,
                   to,
                   LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.2152925909665224513123150,
                   0.9541702441750861130032035,
                   0.0,
                   0.0,
                   to,
                   LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.SkewY(45.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_NEAR(1.1152212925809066312865525,
                   0.0676495144007326631996335,
                   0.0,
                   0.0,
                   to,
                   LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.4619397844342648662419037,
                   0.9519009045724774464858342,
                   0.0,
                   0.0,
                   to,
                   LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.SkewY(45.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_NEAR(1.0, 0.0, 0.0, 0.0, to, LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(1.0, 1.0, 0.0, 0.0, to, LOOSE_ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, to);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);
}

TEST(XFormTest, VerifyBlendForRotationAboutX) {
  // Even though.Blending uses quaternions, axis-aligned rotations should.
  // Blend the same with quaternions or Euler angles. So we can test
  // rotation.Blending by comparing against manually specified matrices from
  // Euler angles.

  Transform from;
  from.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 0.0);

  Transform to;

  to.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 90.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  double expectedRotationAngle = 22.5 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 90.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_NEAR(1.0, 0.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0,
                   std::cos(expectedRotationAngle),
                   -std::sin(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0,
                   std::sin(expectedRotationAngle),
                   std::cos(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  expectedRotationAngle = 45.0 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 90.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_NEAR(1.0, 0.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0,
                   std::cos(expectedRotationAngle),
                   -std::sin(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0,
                   std::sin(expectedRotationAngle),
                   std::cos(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 90.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_NEAR(1.0, 0.0,  0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0, 0.0, -1.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 1.0,  0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);
}

TEST(XFormTest, VerifyBlendForRotationAboutY) {
  Transform from;
  from.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 0.0);

  Transform to;

  to.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 90.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  double expectedRotationAngle = 22.5 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 90.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_NEAR(std::cos(expectedRotationAngle),
                   0.0,
                   std::sin(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0, 1.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(-std::sin(expectedRotationAngle),
                   0.0,
                   std::cos(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  expectedRotationAngle = 45.0 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 90.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_NEAR(std::cos(expectedRotationAngle),
                   0.0,
                   std::sin(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0, 1.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(-std::sin(expectedRotationAngle),
                   0.0,
                   std::cos(expectedRotationAngle),
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 90.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_NEAR(0.0,  0.0, 1.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0,  1.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(-1.0, 0.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);
}

TEST(XFormTest, VerifyBlendForRotationAboutZ) {
  Transform from;
  from.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 0.0);

  Transform to;

  to.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 90.0);
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  double expectedRotationAngle = 22.5 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 90.0);
  to.Blend(from, 0.25);
  EXPECT_ROW1_NEAR(std::cos(expectedRotationAngle),
                   -std::sin(expectedRotationAngle),
                   0.0,
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(std::sin(expectedRotationAngle),
                   std::cos(expectedRotationAngle),
                   0.0,
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 0.0, 1.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  expectedRotationAngle = 45.0 * M_PI / 180.0;
  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 90.0);
  to.Blend(from, 0.5);
  EXPECT_ROW1_NEAR(std::cos(expectedRotationAngle),
                   -std::sin(expectedRotationAngle),
                   0.0,
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(std::sin(expectedRotationAngle),
                   std::cos(expectedRotationAngle),
                   0.0,
                   0.0,
                   to,
                   ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 0.0, 1.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);

  to = Transform();
  to.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 90.0);
  to.Blend(from, 1.0);
  EXPECT_ROW1_NEAR(0.0, -1.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(1.0,  0.0, 0.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0,  0.0, 1.0, 0.0, to, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, to);
}

TEST(XFormTest, VerifyBlendForCompositeTransform) {
  // Verify that the.Blending was done with a decomposition in correct order
  // by blending a composite transform. Using matrix x vector notation
  // (Ax = b, where x is column vector), the ordering should be:
  // perspective * translation * rotation * skew * scale
  //
  // It is not as important (or meaningful) to check intermediate
  // interpolations; order of operations will be tested well enough by the
  // end cases that are easier to specify.

  Transform from;
  Transform to;

  Transform expectedEndOfAnimation;
  expectedEndOfAnimation.ApplyPerspectiveDepth(1.0);
  expectedEndOfAnimation.Translate3d(10.0, 20.0, 30.0);
  expectedEndOfAnimation.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 25.0);
  expectedEndOfAnimation.SkewY(45.0);
  expectedEndOfAnimation.Scale3d(6.0, 7.0, 8.0);

  to = expectedEndOfAnimation;
  to.Blend(from, 0.0);
  EXPECT_EQ(from, to);

  to = expectedEndOfAnimation;
  // We short circuit if blend is >= 1, so to check the numerics, we will
  // check that we get close to what we expect when we're nearly done
  // interpolating.
  to.Blend(from, .99999);

  // Recomposing the matrix results in a normalized matrix, so to verify we
  // need to normalize the expectedEndOfAnimation before comparing elements.
  // Normalizing means dividing everything by expectedEndOfAnimation.m44().
  Transform normalizedExpectedEndOfAnimation = expectedEndOfAnimation;
  Transform normalizationMatrix;
  normalizationMatrix.matrix().set(
      0.0,
      0.0,
      SkDoubleToMScalar(1 / expectedEndOfAnimation.matrix().get(3.0, 3.0)));
  normalizationMatrix.matrix().set(
      1.0,
      1.0,
      SkDoubleToMScalar(1 / expectedEndOfAnimation.matrix().get(3.0, 3.0)));
  normalizationMatrix.matrix().set(
      2.0,
      2.0,
      SkDoubleToMScalar(1 / expectedEndOfAnimation.matrix().get(3.0, 3.0)));
  normalizationMatrix.matrix().set(
      3.0,
      3.0,
      SkDoubleToMScalar(1 / expectedEndOfAnimation.matrix().get(3.0, 3.0)));
  normalizedExpectedEndOfAnimation.PreconcatTransform(normalizationMatrix);

  EXPECT_TRUE(MatricesAreNearlyEqual(normalizedExpectedEndOfAnimation, to));
}

TEST(XFormTest, DecomposedTransformCtor) {
  DecomposedTransform decomp;
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(0.0, decomp.translate[i]);
    EXPECT_EQ(1.0, decomp.scale[i]);
    EXPECT_EQ(0.0, decomp.skew[i]);
    EXPECT_EQ(0.0, decomp.quaternion[i]);
    EXPECT_EQ(0.0, decomp.perspective[i]);
  }
  EXPECT_EQ(1.0, decomp.quaternion[3]);
  EXPECT_EQ(1.0, decomp.perspective[3]);
  Transform identity;
  Transform composed = ComposeTransform(decomp);
  EXPECT_TRUE(MatricesAreNearlyEqual(identity, composed));
}

TEST(XFormTest, FactorTRS) {
  for (int degrees = 0; degrees < 180; ++degrees) {
    // build a transformation matrix.
    gfx::Transform transform;
    transform.Translate(degrees * 2, -degrees * 3);
    transform.Rotate(degrees);
    transform.Scale(degrees + 1, 2 * degrees + 1);

    // factor the matrix
    DecomposedTransform decomp;
    bool success = DecomposeTransform(&decomp, transform);
    EXPECT_TRUE(success);
    EXPECT_FLOAT_EQ(decomp.translate[0], degrees * 2);
    EXPECT_FLOAT_EQ(decomp.translate[1], -degrees * 3);
    double rotation = std::acos(decomp.quaternion[3]) * 360.0 / M_PI;
    while (rotation < 0.0)
      rotation += 360.0;
    while (rotation > 360.0)
      rotation -= 360.0;
    EXPECT_FLOAT_EQ(rotation, degrees);
    EXPECT_FLOAT_EQ(decomp.scale[0], degrees + 1);
    EXPECT_FLOAT_EQ(decomp.scale[1], 2 * degrees + 1);
  }
}

TEST(XFormTest, IntegerTranslation) {
  gfx::Transform transform;
  EXPECT_TRUE(transform.IsIdentityOrIntegerTranslation());

  transform.Translate3d(1, 2, 3);
  EXPECT_TRUE(transform.IsIdentityOrIntegerTranslation());

  transform.MakeIdentity();
  transform.Translate3d(-1, -2, -3);
  EXPECT_TRUE(transform.IsIdentityOrIntegerTranslation());

  transform.MakeIdentity();
  transform.Translate3d(4.5, 0, 0);
  EXPECT_FALSE(transform.IsIdentityOrIntegerTranslation());

  transform.MakeIdentity();
  transform.Translate3d(0, -6.7, 0);
  EXPECT_FALSE(transform.IsIdentityOrIntegerTranslation());

  transform.MakeIdentity();
  transform.Translate3d(0, 0, 8.9);
  EXPECT_FALSE(transform.IsIdentityOrIntegerTranslation());
}

TEST(XFormTest, verifyMatrixInversion) {
  {
    // Invert a translation
    gfx::Transform translation;
    translation.Translate3d(2.0, 3.0, 4.0);
    EXPECT_TRUE(translation.IsInvertible());

    gfx::Transform inverse_translation;
    bool is_invertible = translation.GetInverse(&inverse_translation);
    EXPECT_TRUE(is_invertible);
    EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, -2.0f, inverse_translation);
    EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, -3.0f, inverse_translation);
    EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, -4.0f, inverse_translation);
    EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f,  1.0f, inverse_translation);
  }

  {
    // Invert a non-uniform scale
    gfx::Transform scale;
    scale.Scale3d(4.0, 10.0, 100.0);
    EXPECT_TRUE(scale.IsInvertible());

    gfx::Transform inverse_scale;
    bool is_invertible = scale.GetInverse(&inverse_scale);
    EXPECT_TRUE(is_invertible);
    EXPECT_ROW1_EQ(0.25f, 0.0f, 0.0f, 0.0f, inverse_scale);
    EXPECT_ROW2_EQ(0.0f,  0.1f, 0.0f, 0.0f, inverse_scale);
    EXPECT_ROW3_EQ(0.0f,  0.0f, 0.01f, 0.0f, inverse_scale);
    EXPECT_ROW4_EQ(0.0f,  0.0f, 0.0f, 1.0f, inverse_scale);
  }

  {
    // Try to invert a matrix that is not invertible.
    // The inverse() function should reset the output matrix to identity.
    gfx::Transform uninvertible;
    uninvertible.matrix().setDouble(0, 0, 0.0);
    uninvertible.matrix().setDouble(1, 1, 0.0);
    uninvertible.matrix().setDouble(2, 2, 0.0);
    uninvertible.matrix().setDouble(3, 3, 0.0);
    EXPECT_FALSE(uninvertible.IsInvertible());

    gfx::Transform inverse_of_uninvertible;

    // Add a scale just to more easily ensure that inverse_of_uninvertible is
    // reset to identity.
    inverse_of_uninvertible.Scale3d(4.0, 10.0, 100.0);

    bool is_invertible = uninvertible.GetInverse(&inverse_of_uninvertible);
    EXPECT_FALSE(is_invertible);
    EXPECT_TRUE(inverse_of_uninvertible.IsIdentity());
    EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, inverse_of_uninvertible);
    EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, inverse_of_uninvertible);
    EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, inverse_of_uninvertible);
    EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, inverse_of_uninvertible);
  }
}

TEST(XFormTest, verifyBackfaceVisibilityBasicCases) {
  Transform transform;

  transform.MakeIdentity();
  EXPECT_FALSE(transform.IsBackFaceVisible());

  transform.MakeIdentity();
  transform.RotateAboutYAxis(80.0);
  EXPECT_FALSE(transform.IsBackFaceVisible());

  transform.MakeIdentity();
  transform.RotateAboutYAxis(100.0);
  EXPECT_TRUE(transform.IsBackFaceVisible());

  // Edge case, 90 degree rotation should return false.
  transform.MakeIdentity();
  transform.RotateAboutYAxis(90.0);
  EXPECT_FALSE(transform.IsBackFaceVisible());
}

TEST(XFormTest, verifyBackfaceVisibilityForPerspective) {
  Transform layer_space_to_projection_plane;

  // This tests if IsBackFaceVisible works properly under perspective
  // transforms.  Specifically, layers that may have their back face visible in
  // orthographic projection, may not actually have back face visible under
  // perspective projection.

  // Case 1: Layer is rotated by slightly more than 90 degrees, at the center
  //         of the prespective projection. In this case, the layer's back-side
  //         is visible to the camera.
  layer_space_to_projection_plane.MakeIdentity();
  layer_space_to_projection_plane.ApplyPerspectiveDepth(1.0);
  layer_space_to_projection_plane.Translate3d(0.0, 0.0, 0.0);
  layer_space_to_projection_plane.RotateAboutYAxis(100.0);
  EXPECT_TRUE(layer_space_to_projection_plane.IsBackFaceVisible());

  // Case 2: Layer is rotated by slightly more than 90 degrees, but shifted off
  //         to the side of the camera. Because of the wide field-of-view, the
  //         layer's front side is still visible.
  //
  //                       |<-- front side of layer is visible to camera
  //                    \  |            /
  //                     \ |           /
  //                      \|          /
  //                       |         /
  //                       |\       /<-- camera field of view
  //                       | \     /
  // back side of layer -->|  \   /
  //                           \./ <-- camera origin
  //
  layer_space_to_projection_plane.MakeIdentity();
  layer_space_to_projection_plane.ApplyPerspectiveDepth(1.0);
  layer_space_to_projection_plane.Translate3d(-10.0, 0.0, 0.0);
  layer_space_to_projection_plane.RotateAboutYAxis(100.0);
  EXPECT_FALSE(layer_space_to_projection_plane.IsBackFaceVisible());

  // Case 3: Additionally rotating the layer by 180 degrees should of course
  //         show the opposite result of case 2.
  layer_space_to_projection_plane.RotateAboutYAxis(180.0);
  EXPECT_TRUE(layer_space_to_projection_plane.IsBackFaceVisible());
}

TEST(XFormTest, verifyDefaultConstructorCreatesIdentityMatrix) {
  Transform A;
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
  EXPECT_TRUE(A.IsIdentity());
}

TEST(XFormTest, verifyCopyConstructor) {
  Transform A;
  InitializeTestMatrix(&A);

  // Copy constructor should produce exact same elements as matrix A.
  Transform B(A);
  EXPECT_ROW1_EQ(10.0f, 14.0f, 18.0f, 22.0f, B);
  EXPECT_ROW2_EQ(11.0f, 15.0f, 19.0f, 23.0f, B);
  EXPECT_ROW3_EQ(12.0f, 16.0f, 20.0f, 24.0f, B);
  EXPECT_ROW4_EQ(13.0f, 17.0f, 21.0f, 25.0f, B);
}

TEST(XFormTest, verifyConstructorFor16Elements) {
  Transform transform(1.0, 2.0, 3.0, 4.0,
                      5.0, 6.0, 7.0, 8.0,
                      9.0, 10.0, 11.0, 12.0,
                      13.0, 14.0, 15.0, 16.0);

  EXPECT_ROW1_EQ(1.0f, 2.0f, 3.0f, 4.0f, transform);
  EXPECT_ROW2_EQ(5.0f, 6.0f, 7.0f, 8.0f, transform);
  EXPECT_ROW3_EQ(9.0f, 10.0f, 11.0f, 12.0f, transform);
  EXPECT_ROW4_EQ(13.0f, 14.0f, 15.0f, 16.0f, transform);
}

TEST(XFormTest, verifyConstructorFor2dElements) {
  Transform transform(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

  EXPECT_ROW1_EQ(1.0f, 2.0f, 0.0f, 5.0f, transform);
  EXPECT_ROW2_EQ(3.0f, 4.0f, 0.0f, 6.0f, transform);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, transform);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, transform);
}


TEST(XFormTest, verifyAssignmentOperator) {
  Transform A;
  InitializeTestMatrix(&A);
  Transform B;
  InitializeTestMatrix2(&B);
  Transform C;
  InitializeTestMatrix2(&C);
  C = B = A;

  // Both B and C should now have been re-assigned to the value of A.
  EXPECT_ROW1_EQ(10.0f, 14.0f, 18.0f, 22.0f, B);
  EXPECT_ROW2_EQ(11.0f, 15.0f, 19.0f, 23.0f, B);
  EXPECT_ROW3_EQ(12.0f, 16.0f, 20.0f, 24.0f, B);
  EXPECT_ROW4_EQ(13.0f, 17.0f, 21.0f, 25.0f, B);

  EXPECT_ROW1_EQ(10.0f, 14.0f, 18.0f, 22.0f, C);
  EXPECT_ROW2_EQ(11.0f, 15.0f, 19.0f, 23.0f, C);
  EXPECT_ROW3_EQ(12.0f, 16.0f, 20.0f, 24.0f, C);
  EXPECT_ROW4_EQ(13.0f, 17.0f, 21.0f, 25.0f, C);
}

TEST(XFormTest, verifyEqualsBooleanOperator) {
  Transform A;
  InitializeTestMatrix(&A);

  Transform B;
  InitializeTestMatrix(&B);
  EXPECT_TRUE(A == B);

  // Modifying multiple elements should cause equals operator to return false.
  Transform C;
  InitializeTestMatrix2(&C);
  EXPECT_FALSE(A == C);

  // Modifying any one individual element should cause equals operator to
  // return false.
  Transform D;
  D = A;
  D.matrix().setDouble(0, 0, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(1, 0, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(2, 0, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(3, 0, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(0, 1, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(1, 1, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(2, 1, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(3, 1, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(0, 2, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(1, 2, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(2, 2, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(3, 2, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(0, 3, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(1, 3, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(2, 3, 0.0);
  EXPECT_FALSE(A == D);

  D = A;
  D.matrix().setDouble(3, 3, 0.0);
  EXPECT_FALSE(A == D);
}

TEST(XFormTest, verifyMultiplyOperator) {
  Transform A;
  InitializeTestMatrix(&A);

  Transform B;
  InitializeTestMatrix2(&B);

  Transform C = A * B;
  EXPECT_ROW1_EQ(2036.0f, 2292.0f, 2548.0f, 2804.0f, C);
  EXPECT_ROW2_EQ(2162.0f, 2434.0f, 2706.0f, 2978.0f, C);
  EXPECT_ROW3_EQ(2288.0f, 2576.0f, 2864.0f, 3152.0f, C);
  EXPECT_ROW4_EQ(2414.0f, 2718.0f, 3022.0f, 3326.0f, C);

  // Just an additional sanity check; matrix multiplication is not commutative.
  EXPECT_FALSE(A * B == B * A);
}

TEST(XFormTest, verifyMultiplyAndAssignOperator) {
  Transform A;
  InitializeTestMatrix(&A);

  Transform B;
  InitializeTestMatrix2(&B);

  A *= B;
  EXPECT_ROW1_EQ(2036.0f, 2292.0f, 2548.0f, 2804.0f, A);
  EXPECT_ROW2_EQ(2162.0f, 2434.0f, 2706.0f, 2978.0f, A);
  EXPECT_ROW3_EQ(2288.0f, 2576.0f, 2864.0f, 3152.0f, A);
  EXPECT_ROW4_EQ(2414.0f, 2718.0f, 3022.0f, 3326.0f, A);

  // Just an additional sanity check; matrix multiplication is not commutative.
  Transform C = A;
  C *= B;
  Transform D = B;
  D *= A;
  EXPECT_FALSE(C == D);
}

TEST(XFormTest, verifyMatrixMultiplication) {
  Transform A;
  InitializeTestMatrix(&A);

  Transform B;
  InitializeTestMatrix2(&B);

  A.PreconcatTransform(B);
  EXPECT_ROW1_EQ(2036.0f, 2292.0f, 2548.0f, 2804.0f, A);
  EXPECT_ROW2_EQ(2162.0f, 2434.0f, 2706.0f, 2978.0f, A);
  EXPECT_ROW3_EQ(2288.0f, 2576.0f, 2864.0f, 3152.0f, A);
  EXPECT_ROW4_EQ(2414.0f, 2718.0f, 3022.0f, 3326.0f, A);
}

TEST(XFormTest, verifyMakeIdentiy) {
  Transform A;
  InitializeTestMatrix(&A);
  A.MakeIdentity();
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
  EXPECT_TRUE(A.IsIdentity());
}

TEST(XFormTest, verifyTranslate) {
  Transform A;
  A.Translate(2.0, 3.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 2.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 3.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that Translate() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale(5.0, 5.0);
  A.Translate(2.0, 3.0);
  EXPECT_ROW1_EQ(5.0f, 0.0f, 0.0f, 10.0f, A);
  EXPECT_ROW2_EQ(0.0f, 5.0f, 0.0f, 15.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f,  A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f,  A);
}

TEST(XFormTest, verifyTranslate3d) {
  Transform A;
  A.Translate3d(2.0, 3.0, 4.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 2.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 3.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 4.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that Translate3d() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.Translate3d(2.0, 3.0, 4.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 12.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 21.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 32.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f,  A);
}

TEST(XFormTest, verifyScale) {
  Transform A;
  A.Scale(6.0, 7.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that Scale() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Translate3d(2.0, 3.0, 4.0);
  A.Scale(6.0, 7.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 2.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 3.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 4.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyScale3d) {
  Transform A;
  A.Scale3d(6.0, 7.0, 8.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that scale3d() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Translate3d(2.0, 3.0, 4.0);
  A.Scale3d(6.0, 7.0, 8.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 2.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 3.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 4.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotate) {
  Transform A;
  A.Rotate(90.0);
  EXPECT_ROW1_NEAR(0.0, -1.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(1.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that Rotate() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.Rotate(90.0);
  EXPECT_ROW1_NEAR(0.0, -6.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(7.0, 0.0,  0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutXAxis) {
  Transform A;
  double sin45 = 0.5 * sqrt(2.0);
  double cos45 = sin45;

  A.MakeIdentity();
  A.RotateAboutXAxis(90.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_NEAR(0.0, 0.0, -1.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 1.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  A.MakeIdentity();
  A.RotateAboutXAxis(45.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_NEAR(0.0, cos45, -sin45, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, sin45, cos45, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that RotateAboutXAxis(angle) post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.RotateAboutXAxis(90.0);
  EXPECT_ROW1_NEAR(6.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0, 0.0, -7.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 8.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutYAxis) {
  Transform A;
  double sin45 = 0.5 * sqrt(2.0);
  double cos45 = sin45;

  // Note carefully, the expected pattern is inverted compared to rotating
  // about x axis or z axis.
  A.MakeIdentity();
  A.RotateAboutYAxis(90.0);
  EXPECT_ROW1_NEAR(0.0, 0.0, 1.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_NEAR(-1.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  A.MakeIdentity();
  A.RotateAboutYAxis(45.0);
  EXPECT_ROW1_NEAR(cos45, 0.0, sin45, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_NEAR(-sin45, 0.0, cos45, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that RotateAboutYAxis(angle) post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.RotateAboutYAxis(90.0);
  EXPECT_ROW1_NEAR(0.0, 0.0, 6.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.0, 7.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(-8.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutZAxis) {
  Transform A;
  double sin45 = 0.5 * sqrt(2.0);
  double cos45 = sin45;

  A.MakeIdentity();
  A.RotateAboutZAxis(90.0);
  EXPECT_ROW1_NEAR(0.0, -1.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(1.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  A.MakeIdentity();
  A.RotateAboutZAxis(45.0);
  EXPECT_ROW1_NEAR(cos45, -sin45, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(sin45, cos45, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that RotateAboutZAxis(angle) post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.RotateAboutZAxis(90.0);
  EXPECT_ROW1_NEAR(0.0, -6.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(7.0, 0.0,  0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutForAlignedAxes) {
  Transform A;

  // Check rotation about z-axis
  A.MakeIdentity();
  A.RotateAbout(Vector3dF(0.0, 0.0, 1.0), 90.0);
  EXPECT_ROW1_NEAR(0.0, -1.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(1.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Check rotation about x-axis
  A.MakeIdentity();
  A.RotateAbout(Vector3dF(1.0, 0.0, 0.0), 90.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_NEAR(0.0, 0.0, -1.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(0.0, 1.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Check rotation about y-axis. Note carefully, the expected pattern is
  // inverted compared to rotating about x axis or z axis.
  A.MakeIdentity();
  A.RotateAbout(Vector3dF(0.0, 1.0, 0.0), 90.0);
  EXPECT_ROW1_NEAR(0.0, 0.0, 1.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_NEAR(-1.0, 0.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that rotate3d(axis, angle) post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.RotateAboutZAxis(90.0);
  EXPECT_ROW1_NEAR(0.0, -6.0, 0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(7.0, 0.0,  0.0, 0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutForArbitraryAxis) {
  // Check rotation about an arbitrary non-axis-aligned vector.
  Transform A;
  A.RotateAbout(Vector3dF(1.0, 1.0, 1.0), 90.0);
  EXPECT_ROW1_NEAR(0.3333333333333334258519187,
                   -0.2440169358562924717404030,
                   0.9106836025229592124219380,
                   0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW2_NEAR(0.9106836025229592124219380,
                   0.3333333333333334258519187,
                   -0.2440169358562924717404030,
                   0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW3_NEAR(-0.2440169358562924717404030,
                   0.9106836025229592124219380,
                   0.3333333333333334258519187,
                   0.0, A, ERROR_THRESHOLD);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyRotateAboutForDegenerateAxis) {
  // Check rotation about a degenerate zero vector.
  // It is expected to skip applying the rotation.
  Transform A;

  A.RotateAbout(Vector3dF(0.0, 0.0, 0.0), 45.0);
  // Verify that A remains unchanged.
  EXPECT_TRUE(A.IsIdentity());

  InitializeTestMatrix(&A);
  A.RotateAbout(Vector3dF(0.0, 0.0, 0.0), 35.0);

  // Verify that A remains unchanged.
  EXPECT_ROW1_EQ(10.0f, 14.0f, 18.0f, 22.0f, A);
  EXPECT_ROW2_EQ(11.0f, 15.0f, 19.0f, 23.0f, A);
  EXPECT_ROW3_EQ(12.0f, 16.0f, 20.0f, 24.0f, A);
  EXPECT_ROW4_EQ(13.0f, 17.0f, 21.0f, 25.0f, A);
}

TEST(XFormTest, verifySkewX) {
  Transform A;
  A.SkewX(45.0);
  EXPECT_ROW1_EQ(1.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that skewX() post-multiplies the existing matrix. Row 1, column 2,
  // would incorrectly have value "7" if the matrix is pre-multiplied instead
  // of post-multiplied.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.SkewX(45.0);
  EXPECT_ROW1_EQ(6.0f, 6.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 7.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifySkewY) {
  Transform A;
  A.SkewY(45.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(1.0f, 1.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);

  // Verify that skewY() post-multiplies the existing matrix. Row 2, column 1 ,
  // would incorrectly have value "6" if the matrix is pre-multiplied instead
  // of post-multiplied.
  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  A.SkewY(45.0);
  EXPECT_ROW1_EQ(6.0f, 0.0f, 0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(7.0f, 7.0f, 0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, 8.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, 0.0f, 1.0f, A);
}

TEST(XFormTest, verifyPerspectiveDepth) {
  Transform A;
  A.ApplyPerspectiveDepth(1.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f,  0.0f, 0.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f,  0.0f, 0.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f,  1.0f, 0.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, -1.0f, 1.0f, A);

  // Verify that PerspectiveDepth() post-multiplies the existing matrix.
  A.MakeIdentity();
  A.Translate3d(2.0, 3.0, 4.0);
  A.ApplyPerspectiveDepth(1.0);
  EXPECT_ROW1_EQ(1.0f, 0.0f, -2.0f, 2.0f, A);
  EXPECT_ROW2_EQ(0.0f, 1.0f, -3.0f, 3.0f, A);
  EXPECT_ROW3_EQ(0.0f, 0.0f, -3.0f, 4.0f, A);
  EXPECT_ROW4_EQ(0.0f, 0.0f, -1.0f, 1.0f, A);
}

TEST(XFormTest, verifyHasPerspective) {
  Transform A;
  A.ApplyPerspectiveDepth(1.0);
  EXPECT_TRUE(A.HasPerspective());

  A.MakeIdentity();
  A.ApplyPerspectiveDepth(0.0);
  EXPECT_FALSE(A.HasPerspective());

  A.MakeIdentity();
  A.matrix().setDouble(3, 0, -1.0);
  EXPECT_TRUE(A.HasPerspective());

  A.MakeIdentity();
  A.matrix().setDouble(3, 1, -1.0);
  EXPECT_TRUE(A.HasPerspective());

  A.MakeIdentity();
  A.matrix().setDouble(3, 2, -0.3);
  EXPECT_TRUE(A.HasPerspective());

  A.MakeIdentity();
  A.matrix().setDouble(3, 3, 0.5);
  EXPECT_TRUE(A.HasPerspective());

  A.MakeIdentity();
  A.matrix().setDouble(3, 3, 0.0);
  EXPECT_TRUE(A.HasPerspective());
}

TEST(XFormTest, verifyIsInvertible) {
  Transform A;

  // Translations, rotations, scales, skews and arbitrary combinations of them
  // are invertible.
  A.MakeIdentity();
  EXPECT_TRUE(A.IsInvertible());

  A.MakeIdentity();
  A.Translate3d(2.0, 3.0, 4.0);
  EXPECT_TRUE(A.IsInvertible());

  A.MakeIdentity();
  A.Scale3d(6.0, 7.0, 8.0);
  EXPECT_TRUE(A.IsInvertible());

  A.MakeIdentity();
  A.RotateAboutXAxis(10.0);
  A.RotateAboutYAxis(20.0);
  A.RotateAboutZAxis(30.0);
  EXPECT_TRUE(A.IsInvertible());

  A.MakeIdentity();
  A.SkewX(45.0);
  EXPECT_TRUE(A.IsInvertible());

  // A perspective matrix (projection plane at z=0) is invertible. The
  // intuitive explanation is that perspective is eqivalent to a skew of the
  // w-axis; skews are invertible.
  A.MakeIdentity();
  A.ApplyPerspectiveDepth(1.0);
  EXPECT_TRUE(A.IsInvertible());

  // A "pure" perspective matrix derived by similar triangles, with m44() set
  // to zero (i.e. camera positioned at the origin), is not invertible.
  A.MakeIdentity();
  A.ApplyPerspectiveDepth(1.0);
  A.matrix().setDouble(3, 3, 0.0);
  EXPECT_FALSE(A.IsInvertible());

  // Adding more to a non-invertible matrix will not make it invertible in the
  // general case.
  A.MakeIdentity();
  A.ApplyPerspectiveDepth(1.0);
  A.matrix().setDouble(3, 3, 0.0);
  A.Scale3d(6.0, 7.0, 8.0);
  A.RotateAboutXAxis(10.0);
  A.RotateAboutYAxis(20.0);
  A.RotateAboutZAxis(30.0);
  A.Translate3d(6.0, 7.0, 8.0);
  EXPECT_FALSE(A.IsInvertible());

  // A degenerate matrix of all zeros is not invertible.
  A.MakeIdentity();
  A.matrix().setDouble(0, 0, 0.0);
  A.matrix().setDouble(1, 1, 0.0);
  A.matrix().setDouble(2, 2, 0.0);
  A.matrix().setDouble(3, 3, 0.0);
  EXPECT_FALSE(A.IsInvertible());
}

TEST(XFormTest, verifyIsIdentity) {
  Transform A;

  InitializeTestMatrix(&A);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  EXPECT_TRUE(A.IsIdentity());

  // Modifying any one individual element should cause the matrix to no longer
  // be identity.
  A.MakeIdentity();
  A.matrix().setDouble(0, 0, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(1, 0, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(2, 0, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(3, 0, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(0, 1, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(1, 1, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(2, 1, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(3, 1, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(0, 2, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(1, 2, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(2, 2, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(3, 2, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(0, 3, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(1, 3, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(2, 3, 2.0);
  EXPECT_FALSE(A.IsIdentity());

  A.MakeIdentity();
  A.matrix().setDouble(3, 3, 2.0);
  EXPECT_FALSE(A.IsIdentity());
}

TEST(XFormTest, verifyIsIdentityOrTranslation) {
  Transform A;

  InitializeTestMatrix(&A);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  EXPECT_TRUE(A.IsIdentityOrTranslation());

  // Modifying any non-translation components should cause
  // IsIdentityOrTranslation() to return false. NOTE: (0, 3), (1, 3), and
  // (2, 3) are the translation components, so modifying them should still
  // return true.
  A.MakeIdentity();
  A.matrix().setDouble(0, 0, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(1, 0, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(2, 0, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 0, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(0, 1, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(1, 1, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(2, 1, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 1, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(0, 2, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(1, 2, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(2, 2, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 2, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(0, 3, 2.0);
  EXPECT_TRUE(A.IsIdentityOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(1, 3, 2.0);
  EXPECT_TRUE(A.IsIdentityOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(2, 3, 2.0);
  EXPECT_TRUE(A.IsIdentityOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 3, 2.0);
  EXPECT_FALSE(A.IsIdentityOrTranslation());
}

TEST(XFormTest, verifyIsScaleOrTranslation) {
  Transform A;

  InitializeTestMatrix(&A);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  EXPECT_TRUE(A.IsScaleOrTranslation());

  // Modifying any non-scale or non-translation components should cause
  // IsScaleOrTranslation() to return false. (0, 0), (1, 1), (2, 2), (0, 3),
  // (1, 3), and (2, 3) are the scale and translation components, so
  // modifying them should still return true.

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(0, 0, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(1, 0, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(2, 0, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 0, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(0, 1, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(1, 1, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(2, 1, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 1, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(0, 2, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(1, 2, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(2, 2, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 2, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(0, 3, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(1, 3, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  // Note carefully - expecting true here.
  A.MakeIdentity();
  A.matrix().setDouble(2, 3, 2.0);
  EXPECT_TRUE(A.IsScaleOrTranslation());

  A.MakeIdentity();
  A.matrix().setDouble(3, 3, 2.0);
  EXPECT_FALSE(A.IsScaleOrTranslation());
}

TEST(XFormTest, verifyFlattenTo2d) {
  Transform A;
  InitializeTestMatrix(&A);

  A.FlattenTo2d();
  EXPECT_ROW1_EQ(10.0f, 14.0f, 0.0f, 22.0f, A);
  EXPECT_ROW2_EQ(11.0f, 15.0f, 0.0f, 23.0f, A);
  EXPECT_ROW3_EQ(0.0f,  0.0f,  1.0f, 0.0f,  A);
  EXPECT_ROW4_EQ(13.0f, 17.0f, 0.0f, 25.0f, A);
}

}  // namespace

}  // namespace gfx
