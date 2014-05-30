// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/font.h"

#include "base/string16.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
#include <pango/pango.h>
#elif defined(OS_WIN)
#include "ui/gfx/platform_font_win.h"
#endif

namespace gfx {
namespace {

class FontTest : public testing::Test {
 public:
  // Fulfills the memory management contract as outlined by the comment at
  // gfx::Font::GetNativeFont().
  void FreeIfNecessary(NativeFont font) {
#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
    pango_font_description_free(font);
#endif
  }
};

#if defined(OS_WIN)
class ScopedMinimumFontSizeCallback {
 public:
  explicit ScopedMinimumFontSizeCallback(int minimum_size) {
    minimum_size_ = minimum_size;
    old_callback_ = PlatformFontWin::get_minimum_font_size_callback;
    PlatformFontWin::get_minimum_font_size_callback = &GetMinimumFontSize;
  }

  ~ScopedMinimumFontSizeCallback() {
    PlatformFontWin::get_minimum_font_size_callback = old_callback_;
  }

 private:
  static int GetMinimumFontSize() {
    return minimum_size_;
  }

  PlatformFontWin::GetMinimumFontSizeCallback old_callback_;
  static int minimum_size_;

  DISALLOW_COPY_AND_ASSIGN(ScopedMinimumFontSizeCallback);
};

int ScopedMinimumFontSizeCallback::minimum_size_ = 0;
#endif  // defined(OS_WIN)


TEST_F(FontTest, LoadArial) {
  Font cf("Arial", 16);
  NativeFont native = cf.GetNativeFont();
  ASSERT_TRUE(native);
  ASSERT_EQ(cf.GetStyle(), Font::NORMAL);
  ASSERT_EQ(cf.GetFontSize(), 16);
  ASSERT_EQ(cf.GetFontName(), "Arial");
  FreeIfNecessary(native);
}

TEST_F(FontTest, LoadArialBold) {
  Font cf("Arial", 16);
  Font bold(cf.DeriveFont(0, Font::BOLD));
  NativeFont native = bold.GetNativeFont();
  ASSERT_TRUE(native);
  ASSERT_EQ(bold.GetStyle(), Font::BOLD);
  FreeIfNecessary(native);
}

TEST_F(FontTest, Ascent) {
  Font cf("Arial", 16);
  ASSERT_GT(cf.GetBaseline(), 2);
  ASSERT_LE(cf.GetBaseline(), 22);
}

TEST_F(FontTest, Height) {
  Font cf("Arial", 16);
  ASSERT_GE(cf.GetHeight(), 16);
  // TODO(akalin): Figure out why height is so large on Linux.
  ASSERT_LE(cf.GetHeight(), 26);
}

TEST_F(FontTest, AvgWidths) {
  Font cf("Arial", 16);
  ASSERT_EQ(cf.GetExpectedTextWidth(0), 0);
  ASSERT_GT(cf.GetExpectedTextWidth(1), cf.GetExpectedTextWidth(0));
  ASSERT_GT(cf.GetExpectedTextWidth(2), cf.GetExpectedTextWidth(1));
  ASSERT_GT(cf.GetExpectedTextWidth(3), cf.GetExpectedTextWidth(2));
}

TEST_F(FontTest, AvgCharWidth) {
  Font cf("Arial", 16);
  ASSERT_GT(cf.GetAverageCharacterWidth(), 0);
}

TEST_F(FontTest, Widths) {
  Font cf("Arial", 16);
  ASSERT_EQ(cf.GetStringWidth(string16()), 0);
  ASSERT_GT(cf.GetStringWidth(ASCIIToUTF16("a")),
            cf.GetStringWidth(string16()));
  ASSERT_GT(cf.GetStringWidth(ASCIIToUTF16("ab")),
            cf.GetStringWidth(ASCIIToUTF16("a")));
  ASSERT_GT(cf.GetStringWidth(ASCIIToUTF16("abc")),
            cf.GetStringWidth(ASCIIToUTF16("ab")));
}

#if defined(OS_WIN)
TEST_F(FontTest, DeriveFontResizesIfSizeTooSmall) {
  Font cf("Arial", 8);
  // The minimum font size is set to 5 in browser_main.cc.
  ScopedMinimumFontSizeCallback minimum_size(5);

  Font derived_font = cf.DeriveFont(-4);
  EXPECT_EQ(5, derived_font.GetFontSize());
}

TEST_F(FontTest, DeriveFontKeepsOriginalSizeIfHeightOk) {
  Font cf("Arial", 8);
  // The minimum font size is set to 5 in browser_main.cc.
  ScopedMinimumFontSizeCallback minimum_size(5);

  Font derived_font = cf.DeriveFont(-2);
  EXPECT_EQ(6, derived_font.GetFontSize());
}
#endif  // defined(OS_WIN)

}  // namespace
}  // namespace gfx
