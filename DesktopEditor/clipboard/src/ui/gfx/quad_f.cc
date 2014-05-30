// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/quad_f.h"

#include <cmath>
#include <limits>

#include "base/stringprintf.h"

namespace gfx {

void QuadF::operator=(const RectF& rect) {
  p1_ = PointF(rect.x(), rect.y());
  p2_ = PointF(rect.right(), rect.y());
  p3_ = PointF(rect.right(), rect.bottom());
  p4_ = PointF(rect.x(), rect.bottom());
}

std::string QuadF::ToString() const {
  return base::StringPrintf("%s;%s;%s;%s",
                            p1_.ToString().c_str(),
                            p2_.ToString().c_str(),
                            p3_.ToString().c_str(),
                            p4_.ToString().c_str());
}

static inline bool WithinEpsilon(float a, float b) {
  return std::abs(a - b) < std::numeric_limits<float>::epsilon();
}

bool QuadF::IsRectilinear() const {
  return
      (WithinEpsilon(p1_.x(), p2_.x()) && WithinEpsilon(p2_.y(), p3_.y()) &&
       WithinEpsilon(p3_.x(), p4_.x()) && WithinEpsilon(p4_.y(), p1_.y())) ||
      (WithinEpsilon(p1_.y(), p2_.y()) && WithinEpsilon(p2_.x(), p3_.x()) &&
       WithinEpsilon(p3_.y(), p4_.y()) && WithinEpsilon(p4_.x(), p1_.x()));
}

bool QuadF::IsCounterClockwise() const {
  // This math computes the signed area of the quad. Positive area
  // indicates the quad is clockwise; negative area indicates the quad is
  // counter-clockwise. Note carefully: this is backwards from conventional
  // math because our geometric space uses screen coordiantes with y-axis
  // pointing downards.
  // Reference: http://mathworld.wolfram.com/PolygonArea.html

  // Up-cast to double so this cannot overflow.
  double determinant1 = static_cast<double>(p1_.x()) * p2_.y()
      - static_cast<double>(p2_.x()) * p1_.y();
  double determinant2 = static_cast<double>(p2_.x()) * p3_.y()
      - static_cast<double>(p3_.x()) * p2_.y();
  double determinant3 = static_cast<double>(p3_.x()) * p4_.y()
      - static_cast<double>(p4_.x()) * p3_.y();
  double determinant4 = static_cast<double>(p4_.x()) * p1_.y()
      - static_cast<double>(p1_.x()) * p4_.y();

  return determinant1 + determinant2 + determinant3 + determinant4 < 0;
}

static inline bool PointIsInTriangle(const PointF& point,
                                     const PointF& r1,
                                     const PointF& r2,
                                     const PointF& r3) {
  // Compute the barycentric coordinates of |point| relative to the triangle
  // (r1, r2, r3). This algorithm comes from Christer Ericson's Real-Time
  // Collision Detection.
  Vector2dF v0 = r2 - r1;
  Vector2dF v1 = r3 - r1;
  Vector2dF v2 = point - r1;

  double dot00 = DotProduct(v0, v0);
  double dot01 = DotProduct(v0, v1);
  double dot11 = DotProduct(v1, v1);
  double dot20 = DotProduct(v2, v0);
  double dot21 = DotProduct(v2, v1);

  double denom = dot00 * dot11 - dot01 * dot01;

  double v = (dot11 * dot20 - dot01 * dot21) / denom;
  double w = (dot00 * dot21 - dot01 * dot20) / denom;
  double u = 1 - v - w;

  // Use the barycentric coordinates to test if |point| is inside the
  // triangle (r1, r2, r2).
  return (v >= 0) && (w >= 0) && (u >= 0);
}

bool QuadF::Contains(const PointF& point) const {
  return PointIsInTriangle(point, p1_, p2_, p3_)
      || PointIsInTriangle(point, p1_, p3_, p4_);
}

RectF QuadF::BoundingBox() const {
  float rl = std::min(std::min(p1_.x(), p2_.x()), std::min(p3_.x(), p4_.x()));
  float rr = std::max(std::max(p1_.x(), p2_.x()), std::max(p3_.x(), p4_.x()));
  float rt = std::min(std::min(p1_.y(), p2_.y()), std::min(p3_.y(), p4_.y()));
  float rb = std::max(std::max(p1_.y(), p2_.y()), std::max(p3_.y(), p4_.y()));
  return RectF(rl, rt, rr - rl, rb - rt);
}

void QuadF::Scale(float x_scale, float y_scale) {
  p1_.Scale(x_scale, y_scale);
  p2_.Scale(x_scale, y_scale);
  p3_.Scale(x_scale, y_scale);
  p4_.Scale(x_scale, y_scale);
}

void QuadF::operator+=(const Vector2dF& rhs) {
  p1_ += rhs;
  p2_ += rhs;
  p3_ += rhs;
  p4_ += rhs;
}

void QuadF::operator-=(const Vector2dF& rhs) {
  p1_ -= rhs;
  p2_ -= rhs;
  p3_ -= rhs;
  p4_ -= rhs;
}

QuadF operator+(const QuadF& lhs, const Vector2dF& rhs) {
  QuadF result = lhs;
  result += rhs;
  return result;
}

QuadF operator-(const QuadF& lhs, const Vector2dF& rhs) {
  QuadF result = lhs;
  result -= rhs;
  return result;
}

}  // namespace gfx
