// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_TRANSFORMER_H_
#define UI_AURA_ROOT_WINDOW_TRANSFORMER_H_

#include "ui/aura/aura_export.h"

namespace gfx {
class Insets;
class Rect;
class Size;
class Transform;
}

namespace aura {

// RootWindowTransformer controls how RootWindow should be placed and
// transformed inside the host window.
class AURA_EXPORT RootWindowTransformer {
 public:
  virtual ~RootWindowTransformer() {}

  // Returns the transform the root window in DIP.
  virtual gfx::Transform GetTransform() const = 0;

  // Returns the inverse of the transform above. This method is to
  // provie an accurate inverse of the transform because the result of
  // |gfx::Transform::GetInverse| may contains computational error.
  virtual gfx::Transform GetInverseTransform() const = 0;

  // Returns the root window's bounds for given host window size in DIP.
  // This is necessary to handle the case where the root window's size
  // is bigger than the host window. (Screen magnifier for example).
  virtual gfx::Rect GetRootWindowBounds(const gfx::Size& host_size) const = 0;

  // Returns the insets that specifies the effective area of
  // the host window.
  virtual gfx::Insets GetHostInsets() const = 0;
};

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_TRANSFORMER_H_
