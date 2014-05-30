// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MSVC++ requires this to be set before any other includes to get M_PI.
#define _USE_MATH_DEFINES

#include "ui/gfx/transform.h"

#include <cmath>

#include "base/stringprintf.h"
#include "ui/gfx/point.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/vector3d_f.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/safe_integer_conversions.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/transform_util.h"

namespace gfx {

namespace {

// Taken from SkMatrix44.
const double kTooSmallForDeterminant = 1e-8;

double TanDegrees(double degrees) {
  double radians = degrees * M_PI / 180;
  return std::tan(radians);
}

}  // namespace

Transform::Transform(
    double col1row1, double col2row1, double col3row1, double col4row1,
    double col1row2, double col2row2, double col3row2, double col4row2,
    double col1row3, double col2row3, double col3row3, double col4row3,
    double col1row4, double col2row4, double col3row4, double col4row4)
    : matrix_(SkMatrix44::kUninitialized_Constructor)
{
  matrix_.setDouble(0, 0, col1row1);
  matrix_.setDouble(1, 0, col1row2);
  matrix_.setDouble(2, 0, col1row3);
  matrix_.setDouble(3, 0, col1row4);

  matrix_.setDouble(0, 1, col2row1);
  matrix_.setDouble(1, 1, col2row2);
  matrix_.setDouble(2, 1, col2row3);
  matrix_.setDouble(3, 1, col2row4);

  matrix_.setDouble(0, 2, col3row1);
  matrix_.setDouble(1, 2, col3row2);
  matrix_.setDouble(2, 2, col3row3);
  matrix_.setDouble(3, 2, col3row4);

  matrix_.setDouble(0, 3, col4row1);
  matrix_.setDouble(1, 3, col4row2);
  matrix_.setDouble(2, 3, col4row3);
  matrix_.setDouble(3, 3, col4row4);
}

Transform::Transform(
    double col1row1, double col2row1,
    double col1row2, double col2row2,
    double x_translation, double y_translation)
    : matrix_(SkMatrix44::kIdentity_Constructor)
{
  matrix_.setDouble(0, 0, col1row1);
  matrix_.setDouble(1, 0, col1row2);
  matrix_.setDouble(0, 1, col2row1);
  matrix_.setDouble(1, 1, col2row2);
  matrix_.setDouble(0, 3, x_translation);
  matrix_.setDouble(1, 3, y_translation);
}

void Transform::RotateAboutXAxis(double degrees) {
  double radians = degrees * M_PI / 180;
  double cosTheta = std::cos(radians);
  double sinTheta = std::sin(radians);
  if (matrix_.isIdentity()) {
      matrix_.set3x3(1, 0, 0,
                     0, cosTheta, sinTheta,
                     0, -sinTheta, cosTheta);
  } else {
    SkMatrix44 rot(SkMatrix44::kUninitialized_Constructor);
    rot.set3x3(1, 0, 0,
               0, cosTheta, sinTheta,
               0, -sinTheta, cosTheta);
    matrix_.preConcat(rot);
  }
}

void Transform::RotateAboutYAxis(double degrees) {
  double radians = degrees * M_PI / 180;
  double cosTheta = std::cos(radians);
  double sinTheta = std::sin(radians);
  if (matrix_.isIdentity()) {
      // Note carefully the placement of the -sinTheta for rotation about
      // y-axis is different than rotation about x-axis or z-axis.
      matrix_.set3x3(cosTheta, 0, -sinTheta,
                     0, 1, 0,
                     sinTheta, 0, cosTheta);
  } else {
    SkMatrix44 rot(SkMatrix44::kUninitialized_Constructor);
    rot.set3x3(cosTheta, 0, -sinTheta,
               0, 1, 0,
               sinTheta, 0, cosTheta);
    matrix_.preConcat(rot);
  }
}

void Transform::RotateAboutZAxis(double degrees) {
  double radians = degrees * M_PI / 180;
  double cosTheta = std::cos(radians);
  double sinTheta = std::sin(radians);
  if (matrix_.isIdentity()) {
      matrix_.set3x3(cosTheta, sinTheta, 0,
                     -sinTheta, cosTheta, 0,
                     0, 0, 1);
  } else {
    SkMatrix44 rot(SkMatrix44::kUninitialized_Constructor);
    rot.set3x3(cosTheta, sinTheta, 0,
               -sinTheta, cosTheta, 0,
               0, 0, 1);
    matrix_.preConcat(rot);
  }
}

void Transform::RotateAbout(const Vector3dF& axis, double degrees) {
  if (matrix_.isIdentity()) {
    matrix_.setRotateDegreesAbout(SkDoubleToMScalar(axis.x()),
                                  SkDoubleToMScalar(axis.y()),
                                  SkDoubleToMScalar(axis.z()),
                                  SkDoubleToMScalar(degrees));
  } else {
    SkMatrix44 rot(SkMatrix44::kUninitialized_Constructor);
    rot.setRotateDegreesAbout(SkDoubleToMScalar(axis.x()),
                              SkDoubleToMScalar(axis.y()),
                              SkDoubleToMScalar(axis.z()),
                              SkDoubleToMScalar(degrees));
    matrix_.preConcat(rot);
  }
}

void Transform::Scale(double x, double y) {
  matrix_.preScale(SkDoubleToMScalar(x), SkDoubleToMScalar(y), 1);
}

void Transform::Scale3d(double x, double y, double z) {
  matrix_.preScale(SkDoubleToMScalar(x),
                   SkDoubleToMScalar(y),
                   SkDoubleToMScalar(z));
}

void Transform::Translate(double x, double y) {
  matrix_.preTranslate(SkDoubleToMScalar(x), SkDoubleToMScalar(y), 0);
}

void Transform::Translate3d(double x, double y, double z) {
  matrix_.preTranslate(SkDoubleToMScalar(x),
                       SkDoubleToMScalar(y),
                       SkDoubleToMScalar(z));
}

void Transform::SkewX(double angle_x) {
  if (matrix_.isIdentity())
    matrix_.setDouble(0, 1, TanDegrees(angle_x));
  else {
    SkMatrix44 skew(SkMatrix44::kIdentity_Constructor);
    skew.setDouble(0, 1, TanDegrees(angle_x));
    matrix_.preConcat(skew);
  }
}

void Transform::SkewY(double angle_y) {
  if (matrix_.isIdentity())
    matrix_.setDouble(1, 0, TanDegrees(angle_y));
  else {
    SkMatrix44 skew(SkMatrix44::kIdentity_Constructor);
    skew.setDouble(1, 0, TanDegrees(angle_y));
    matrix_.preConcat(skew);
  }
}

void Transform::ApplyPerspectiveDepth(double depth) {
  if (depth == 0)
    return;
  if (matrix_.isIdentity())
    matrix_.setDouble(3, 2, -1.0 / depth);
  else {
    SkMatrix44 m(SkMatrix44::kIdentity_Constructor);
    m.setDouble(3, 2, -1.0 / depth);
    matrix_.preConcat(m);
  }
}

void Transform::PreconcatTransform(const Transform& transform) {
  matrix_.preConcat(transform.matrix_);
}

void Transform::ConcatTransform(const Transform& transform) {
  matrix_.postConcat(transform.matrix_);
}

bool Transform::IsIdentityOrIntegerTranslation() const {
  if (!IsIdentityOrTranslation())
    return false;

  bool no_fractional_translation =
      static_cast<int>(matrix_.getDouble(0, 3)) == matrix_.getDouble(0, 3) &&
      static_cast<int>(matrix_.getDouble(1, 3)) == matrix_.getDouble(1, 3) &&
      static_cast<int>(matrix_.getDouble(2, 3)) == matrix_.getDouble(2, 3);

  return no_fractional_translation;
}

bool Transform::IsBackFaceVisible() const {
  // Compute whether a layer with a forward-facing normal of (0, 0, 1, 0)
  // would have its back face visible after applying the transform.
  if (matrix_.isIdentity())
    return false;

  // This is done by transforming the normal and seeing if the resulting z
  // value is positive or negative. However, note that transforming a normal
  // actually requires using the inverse-transpose of the original transform.
  //
  // We can avoid inverting and transposing the matrix since we know we want
  // to transform only the specific normal vector (0, 0, 1, 0). In this case,
  // we only need the 3rd row, 3rd column of the inverse-transpose. We can
  // calculate only the 3rd row 3rd column element of the inverse, skipping
  // everything else.
  //
  // For more information, refer to:
  //   http://en.wikipedia.org/wiki/Invertible_matrix#Analytic_solution
  //

  double determinant = matrix_.determinant();

  // If matrix was not invertible, then just assume back face is not visible.
  if (std::abs(determinant) <= kTooSmallForDeterminant)
    return false;

  // Compute the cofactor of the 3rd row, 3rd column.
  double cofactor_part_1 =
      matrix_.getDouble(0, 0) *
      matrix_.getDouble(1, 1) *
      matrix_.getDouble(3, 3);

  double cofactor_part_2 =
      matrix_.getDouble(0, 1) *
      matrix_.getDouble(1, 3) *
      matrix_.getDouble(3, 0);

  double cofactor_part_3 =
      matrix_.getDouble(0, 3) *
      matrix_.getDouble(1, 0) *
      matrix_.getDouble(3, 1);

  double cofactor_part_4 =
      matrix_.getDouble(0, 0) *
      matrix_.getDouble(1, 3) *
      matrix_.getDouble(3, 1);

  double cofactor_part_5 =
      matrix_.getDouble(0, 1) *
      matrix_.getDouble(1, 0) *
      matrix_.getDouble(3, 3);

  double cofactor_part_6 =
      matrix_.getDouble(0, 3) *
      matrix_.getDouble(1, 1) *
      matrix_.getDouble(3, 0);

  double cofactor33 =
      cofactor_part_1 +
      cofactor_part_2 +
      cofactor_part_3 -
      cofactor_part_4 -
      cofactor_part_5 -
      cofactor_part_6;

  // Technically the transformed z component is cofactor33 / determinant.  But
  // we can avoid the costly division because we only care about the resulting
  // +/- sign; we can check this equivalently by multiplication.
  return cofactor33 * determinant < 0;
}

bool Transform::GetInverse(Transform* transform) const {
  if (!matrix_.invert(&transform->matrix_)) {
    // Initialize the return value to identity if this matrix turned
    // out to be un-invertible.
    transform->MakeIdentity();
    return false;
  }

  return true;
}

void Transform::Transpose() {
  matrix_.transpose();
}

void Transform::FlattenTo2d() {
  matrix_.setDouble(2, 0, 0.0);
  matrix_.setDouble(2, 1, 0.0);
  matrix_.setDouble(0, 2, 0.0);
  matrix_.setDouble(1, 2, 0.0);
  matrix_.setDouble(2, 2, 1.0);
  matrix_.setDouble(3, 2, 0.0);
  matrix_.setDouble(2, 3, 0.0);
}

void Transform::TransformPoint(Point& point) const {
  TransformPointInternal(matrix_, point);
}

void Transform::TransformPoint(Point3F& point) const {
  TransformPointInternal(matrix_, point);
}

bool Transform::TransformPointReverse(Point& point) const {
  // TODO(sad): Try to avoid trying to invert the matrix.
  SkMatrix44 inverse(SkMatrix44::kUninitialized_Constructor);
  if (!matrix_.invert(&inverse))
    return false;

  TransformPointInternal(inverse, point);
  return true;
}

bool Transform::TransformPointReverse(Point3F& point) const {
  // TODO(sad): Try to avoid trying to invert the matrix.
  SkMatrix44 inverse(SkMatrix44::kUninitialized_Constructor);
  if (!matrix_.invert(&inverse))
    return false;

  TransformPointInternal(inverse, point);
  return true;
}

void Transform::TransformRect(RectF* rect) const {
  if (matrix_.isIdentity())
    return;

  SkRect src = RectFToSkRect(*rect);
  const SkMatrix& matrix = matrix_;
  matrix.mapRect(&src);
  *rect = SkRectToRectF(src);
}

bool Transform::TransformRectReverse(RectF* rect) const {
  if (matrix_.isIdentity())
    return true;

  SkMatrix44 inverse(SkMatrix44::kUninitialized_Constructor);
  if (!matrix_.invert(&inverse))
    return false;

  const SkMatrix& matrix = inverse;
  SkRect src = RectFToSkRect(*rect);
  matrix.mapRect(&src);
  *rect = SkRectToRectF(src);
  return true;
}

bool Transform::Blend(const Transform& from, double progress) {
  if (progress <= 0.0) {
    *this = from;
    return true;
  }

  if (progress >= 1.0)
    return true;

  DecomposedTransform to_decomp;
  DecomposedTransform from_decomp;
  if (!DecomposeTransform(&to_decomp, *this) ||
      !DecomposeTransform(&from_decomp, from))
    return false;

  if (!BlendDecomposedTransforms(&to_decomp, to_decomp, from_decomp, progress))
    return false;

  matrix_ = ComposeTransform(to_decomp).matrix();
  return true;
}

void Transform::TransformPointInternal(const SkMatrix44& xform,
                                       Point3F& point) const {
  if (xform.isIdentity())
    return;

  SkMScalar p[4] = {
    SkDoubleToMScalar(point.x()),
    SkDoubleToMScalar(point.y()),
    SkDoubleToMScalar(point.z()),
    SkDoubleToMScalar(1)
  };

  xform.mapMScalars(p);

  if (p[3] != 1 && abs(p[3]) > 0) {
    point.SetPoint(p[0] / p[3], p[1] / p[3], p[2]/ p[3]);
  } else {
    point.SetPoint(p[0], p[1], p[2]);
  }
}

void Transform::TransformPointInternal(const SkMatrix44& xform,
                                       Point& point) const {
  if (xform.isIdentity())
    return;

  SkMScalar p[4] = {
    SkDoubleToMScalar(point.x()),
    SkDoubleToMScalar(point.y()),
    SkDoubleToMScalar(0),
    SkDoubleToMScalar(1)
  };

  xform.mapMScalars(p);

  point.SetPoint(ToRoundedInt(p[0]), ToRoundedInt(p[1]));
}

std::string Transform::ToString() const {
  return base::StringPrintf(
      "[ %+0.4f %+0.4f %+0.4f %+0.4f  \n"
      "  %+0.4f %+0.4f %+0.4f %+0.4f  \n"
      "  %+0.4f %+0.4f %+0.4f %+0.4f  \n"
      "  %+0.4f %+0.4f %+0.4f %+0.4f ]\n",
      matrix_.getDouble(0, 0),
      matrix_.getDouble(0, 1),
      matrix_.getDouble(0, 2),
      matrix_.getDouble(0, 3),
      matrix_.getDouble(1, 0),
      matrix_.getDouble(1, 1),
      matrix_.getDouble(1, 2),
      matrix_.getDouble(1, 3),
      matrix_.getDouble(2, 0),
      matrix_.getDouble(2, 1),
      matrix_.getDouble(2, 2),
      matrix_.getDouble(2, 3),
      matrix_.getDouble(3, 0),
      matrix_.getDouble(3, 1),
      matrix_.getDouble(3, 2),
      matrix_.getDouble(3, 3));
}

}  // namespace gfx
