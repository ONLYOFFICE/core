// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_INSETS_H_
#define UI_GFX_INSETS_H_

#include <string>

#include "build/build_config.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/insets_base.h"

#if defined(TOOLKIT_GTK)
typedef struct _GtkBorder GtkBorder;
#endif

namespace gfx {

// An integer versin of gfx::Insets.
class UI_EXPORT Insets : public InsetsBase<Insets, int> {
 public:
  Insets();
  Insets(int top, int left, int bottom, int right);
#if defined(TOOLKIT_GTK)
  explicit Insets(const GtkBorder& border);
#endif

  ~Insets();

  Insets Scale(float scale) const {
    return Scale(scale, scale);
  }

  Insets Scale(float x_scale, float y_scale) const {
    return Insets(static_cast<int>(top() * y_scale),
                  static_cast<int>(left() * x_scale),
                  static_cast<int>(bottom() * y_scale),
                  static_cast<int>(right() * x_scale));
  }

  // Returns a string representation of the insets.
  std::string ToString() const;
};

#if !defined(COMPILER_MSVC)
extern template class InsetsBase<Insets, int>;
#endif

}  // namespace gfx

#endif  // UI_GFX_INSETS_H_
