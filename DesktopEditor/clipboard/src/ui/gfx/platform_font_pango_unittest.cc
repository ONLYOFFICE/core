// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/platform_font_pango.h"

#include <string>

#include "base/memory/ref_counted.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/pango_util.h"

namespace gfx {

// Test that PlatformFontPango is able to cope with PangoFontDescriptions
// containing multiple font families.  The first family should be preferred.
TEST(PlatformFontPangoTest, FamilyList) {
  ScopedPangoFontDescription desc(
      pango_font_description_from_string("Arial,Times New Roman, 13px"));
  scoped_refptr<gfx::PlatformFontPango> font(
      new gfx::PlatformFontPango(desc.get()));
  EXPECT_EQ("Arial", font->GetFontName());
  EXPECT_EQ(13, font->GetFontSize());

  ScopedPangoFontDescription desc2(
      pango_font_description_from_string("Times New Roman,Arial, 15px"));
  scoped_refptr<gfx::PlatformFontPango> font2(
      new gfx::PlatformFontPango(desc2.get()));
  EXPECT_EQ("Times New Roman", font2->GetFontName());
  EXPECT_EQ(15, font2->GetFontSize());
}

}  // namespace gfx
