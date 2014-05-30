// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/utf_string_conversions.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/size.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/test/views_test_base.h"

namespace {

gfx::ImageSkia CreateTestImage(int width, int height) {
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, width, height);
  bitmap.allocPixels();
  return gfx::ImageSkia::CreateFrom1xBitmap(bitmap);
}

}  // namespace

namespace views {

typedef ViewsTestBase LabelButtonTest;

TEST_F(LabelButtonTest, Init) {
  const string16 text(ASCIIToUTF16("abc"));
  LabelButton button(NULL, text);

  EXPECT_TRUE(button.GetImage(Button::STATE_NORMAL).isNull());
  EXPECT_TRUE(button.GetImage(Button::STATE_HOVERED).isNull());
  EXPECT_TRUE(button.GetImage(Button::STATE_PRESSED).isNull());
  EXPECT_TRUE(button.GetImage(Button::STATE_DISABLED).isNull());

  EXPECT_EQ(text, button.GetText());
  EXPECT_EQ(gfx::ALIGN_LEFT, button.GetHorizontalAlignment());
  EXPECT_FALSE(button.is_default());
  EXPECT_EQ(button.style(), Button::STYLE_TEXTBUTTON);
  EXPECT_EQ(Button::STATE_NORMAL, button.state());

  EXPECT_EQ(button.image_->parent(), &button);
  EXPECT_EQ(button.label_->parent(), &button);
}

TEST_F(LabelButtonTest, Label) {
  LabelButton button(NULL, string16());
  EXPECT_TRUE(button.GetText().empty());

  const gfx::Font font;
  const string16 short_text(ASCIIToUTF16("abcdefghijklm"));
  const string16 long_text(ASCIIToUTF16("abcdefghijklmnopqrstuvwxyz"));
  const int short_text_width = gfx::Canvas::GetStringWidth(short_text, font);
  const int long_text_width = gfx::Canvas::GetStringWidth(long_text, font);

  // The width increases monotonically with string size (it does not shrink).
  EXPECT_LT(button.GetPreferredSize().width(), short_text_width);
  button.SetText(short_text);
  EXPECT_GT(button.GetPreferredSize().height(), font.GetHeight());
  EXPECT_GT(button.GetPreferredSize().width(), short_text_width);
  EXPECT_LT(button.GetPreferredSize().width(), long_text_width);
  button.SetText(long_text);
  EXPECT_GT(button.GetPreferredSize().width(), long_text_width);
  button.SetText(short_text);
  EXPECT_GT(button.GetPreferredSize().width(), long_text_width);

  // Clamp the size to a maximum value.
  button.set_max_size(gfx::Size(long_text_width, 1));
  EXPECT_EQ(button.GetPreferredSize(), gfx::Size(long_text_width, 1));

  // Clear the monotonically increasing minimum size.
  button.set_min_size(gfx::Size());
  EXPECT_GT(button.GetPreferredSize().width(), short_text_width);
  EXPECT_LT(button.GetPreferredSize().width(), long_text_width);
}

TEST_F(LabelButtonTest, Image) {
  LabelButton button(NULL, string16());

  const int small_size = 50, large_size = 100;
  const gfx::ImageSkia small_image = CreateTestImage(small_size, small_size);
  const gfx::ImageSkia large_image = CreateTestImage(large_size, large_size);

  // The width increases monotonically with image size (it does not shrink).
  EXPECT_LT(button.GetPreferredSize().width(), small_size);
  EXPECT_LT(button.GetPreferredSize().height(), small_size);
  button.SetImage(Button::STATE_NORMAL, small_image);
  EXPECT_GT(button.GetPreferredSize().width(), small_size);
  EXPECT_GT(button.GetPreferredSize().height(), small_size);
  EXPECT_LT(button.GetPreferredSize().width(), large_size);
  EXPECT_LT(button.GetPreferredSize().height(), large_size);
  button.SetImage(Button::STATE_NORMAL, large_image);
  EXPECT_GT(button.GetPreferredSize().width(), large_size);
  EXPECT_GT(button.GetPreferredSize().height(), large_size);
  button.SetImage(Button::STATE_NORMAL, small_image);
  EXPECT_GT(button.GetPreferredSize().width(), large_size);
  EXPECT_GT(button.GetPreferredSize().height(), large_size);

  // Clamp the size to a maximum value.
  button.set_max_size(gfx::Size(large_size, 1));
  EXPECT_EQ(button.GetPreferredSize(), gfx::Size(large_size, 1));

  // Clear the monotonically increasing minimum size.
  button.set_min_size(gfx::Size());
  EXPECT_GT(button.GetPreferredSize().width(), small_size);
  EXPECT_LT(button.GetPreferredSize().width(), large_size);
}

TEST_F(LabelButtonTest, LabelAndImage) {
  LabelButton button(NULL, string16());

  const gfx::Font font;
  const string16 text(ASCIIToUTF16("abcdefghijklm"));
  const int text_width = gfx::Canvas::GetStringWidth(text, font);

  const int image_size = 50;
  const gfx::ImageSkia image = CreateTestImage(image_size, image_size);
  ASSERT_LT(font.GetHeight(), image_size);

  // The width increases monotonically with content size (it does not shrink).
  EXPECT_LT(button.GetPreferredSize().width(), text_width);
  EXPECT_LT(button.GetPreferredSize().width(), image_size);
  EXPECT_LT(button.GetPreferredSize().height(), image_size);
  button.SetText(text);
  EXPECT_GT(button.GetPreferredSize().width(), text_width);
  EXPECT_GT(button.GetPreferredSize().height(), font.GetHeight());
  EXPECT_LT(button.GetPreferredSize().width(), text_width + image_size);
  EXPECT_LT(button.GetPreferredSize().height(), image_size);
  button.SetImage(Button::STATE_NORMAL, image);
  EXPECT_GT(button.GetPreferredSize().width(), text_width + image_size);
  EXPECT_GT(button.GetPreferredSize().height(), image_size);

  // Layout and ensure the image is left of the label except for ALIGN_RIGHT.
  // (A proper parent view or layout manager would Layout on its invalidations).
  button.SetSize(button.GetPreferredSize());
  button.Layout();
  EXPECT_EQ(gfx::ALIGN_LEFT, button.GetHorizontalAlignment());
  EXPECT_LT(button.image_->bounds().right(), button.label_->bounds().x());
  button.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  button.Layout();
  EXPECT_EQ(gfx::ALIGN_CENTER, button.GetHorizontalAlignment());
  EXPECT_LT(button.image_->bounds().right(), button.label_->bounds().x());
  button.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  button.Layout();
  EXPECT_EQ(gfx::ALIGN_RIGHT, button.GetHorizontalAlignment());
  EXPECT_LT(button.label_->bounds().right(), button.image_->bounds().x());

  button.SetText(string16());
  EXPECT_GT(button.GetPreferredSize().width(), text_width + image_size);
  EXPECT_GT(button.GetPreferredSize().height(), image_size);
  button.SetImage(Button::STATE_NORMAL, gfx::ImageSkia());
  EXPECT_GT(button.GetPreferredSize().width(), text_width + image_size);
  EXPECT_GT(button.GetPreferredSize().height(), image_size);

  // Clamp the size to a maximum value.
  button.set_max_size(gfx::Size(image_size, 1));
  EXPECT_EQ(button.GetPreferredSize(), gfx::Size(image_size, 1));

  // Clear the monotonically increasing minimum size.
  button.set_min_size(gfx::Size());
  EXPECT_LT(button.GetPreferredSize().width(), text_width);
  EXPECT_LT(button.GetPreferredSize().width(), image_size);
  EXPECT_LT(button.GetPreferredSize().height(), image_size);
}

TEST_F(LabelButtonTest, Font) {
  const string16 text(ASCIIToUTF16("abc"));
  LabelButton button(NULL, text);

  const gfx::Font original_font = button.GetFont();
  const gfx::Font large_font = original_font.DeriveFont(100);
  const int original_width = button.GetPreferredSize().width();
  const int original_height = button.GetPreferredSize().height();

  // The button size increases when the font size is increased.
  button.SetFont(large_font);
  EXPECT_GT(button.GetPreferredSize().width(), original_width);
  EXPECT_GT(button.GetPreferredSize().height(), original_height);

  // The button returns to its original size when the minimal size is cleared
  // and the original font size is restored.
  button.set_min_size(gfx::Size());
  button.SetFont(original_font);
  EXPECT_EQ(original_width, button.GetPreferredSize().width());
  EXPECT_EQ(original_height, button.GetPreferredSize().height());
}

}  // namespace views
