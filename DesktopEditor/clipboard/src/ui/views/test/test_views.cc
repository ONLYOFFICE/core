// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/test/test_views.h"

namespace views {

StaticSizedView::StaticSizedView(const gfx::Size& size) : size_(size) {}

StaticSizedView::~StaticSizedView() {}

gfx::Size StaticSizedView::GetPreferredSize() {
  return size_;
}

ProportionallySizedView::ProportionallySizedView(int factor)
    : factor_(factor) {}

ProportionallySizedView::~ProportionallySizedView() {}

int ProportionallySizedView::GetHeightForWidth(int w) {
  return w * factor_;
}

}  // namespace views
