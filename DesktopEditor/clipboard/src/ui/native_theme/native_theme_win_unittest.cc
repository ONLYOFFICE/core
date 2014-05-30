// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme_win.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace ui {

TEST(NativeThemeTest, Init) {
  ASSERT_TRUE(NativeThemeWin::instance() != NULL);
}

}  // namespace ui
