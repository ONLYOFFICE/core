// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_COMPOSITION_UNDERLINE_H_
#define UI_BASE_IME_COMPOSITION_UNDERLINE_H_

#include <vector>

#include "third_party/skia/include/core/SkColor.h"

namespace ui {

// Intentionally keep sync with WebKit::WebCompositionUnderline defined in:
// third_party/WebKit/Source/WebKit/chromium/public/WebCompositionUnderline.h
struct CompositionUnderline {
  CompositionUnderline()
    : start_offset(0),
      end_offset(0),
      color(0),
      thick(false) {}

  CompositionUnderline(unsigned s, unsigned e, SkColor c, bool t)
    : start_offset(s),
      end_offset(e),
      color(c),
      thick(t) {}

  bool operator==(const CompositionUnderline& rhs) const {
    return (this->start_offset == rhs.start_offset) &&
        (this->end_offset == rhs.end_offset) &&
        (this->color == rhs.color) &&
        (this->thick == rhs.thick);
  }

  bool operator!=(const CompositionUnderline& rhs) const {
    return !(*this == rhs);
  }

  // Though use of unsigned is discouraged, we use it here to make sure it's
  // identical to WebKit::WebCompositionUnderline.
  unsigned start_offset;
  unsigned end_offset;
  SkColor color;
  bool thick;
};

typedef std::vector<CompositionUnderline> CompositionUnderlines;

}  // namespace ui

#endif  // UI_BASE_IME_COMPOSITION_UNDERLINE_H_
