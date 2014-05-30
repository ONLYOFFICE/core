// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TRANSFORM_UTIL_H_
#define UI_GFX_TRANSFORM_UTIL_H_

#include "ui/base/ui_export.h"
#include "ui/gfx/transform.h"

namespace gfx {

class Point;

// Returns a scale transform at |anchor| point.
UI_EXPORT Transform GetScaleTransform(const Point& anchor, float scale);

// Contains the components of a factored transform. These components may be
// blended and recomposed.
struct UI_EXPORT DecomposedTransform {
  // The default constructor initializes the components in such a way that
  // if used with Compose below, will produce the identity transform.
  DecomposedTransform();

  double translate[3];
  double scale[3];
  double skew[3];
  double perspective[4];
  double quaternion[4];

  // Copy and assign are allowed.
};

// Interpolates the decomposed components |to| with |from| using the
// routines described in http://www.w3.org/TR/css3-3d-transform/.
// |progress| is in the range [0, 1] (0 leaves |out| unchanged, and 1
// assigns |from| to |out|).
UI_EXPORT bool BlendDecomposedTransforms(DecomposedTransform* out,
                                         const DecomposedTransform& to,
                                         const DecomposedTransform& from,
                                         double progress);

// Decomposes this transform into its translation, scale, skew, perspective,
// and rotation components following the routines detailed in this spec:
// http://www.w3.org/TR/css3-3d-transforms/.
UI_EXPORT bool DecomposeTransform(DecomposedTransform* out,
                                  const Transform& transform);

// Composes a transform from the given translation, scale, skew, prespective,
// and rotation components following the routines detailed in this spec:
// http://www.w3.org/TR/css3-3d-transforms/.
UI_EXPORT Transform ComposeTransform(const DecomposedTransform& decomp);

}  // namespace gfx

#endif  // UI_GFX_TRANSFORM_UTIL_H_
