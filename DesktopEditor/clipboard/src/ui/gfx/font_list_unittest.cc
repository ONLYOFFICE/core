// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/font_list.h"

#include <string>
#include <vector>

#include "base/string_number_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

// Helper function for comparing fonts for equality.
std::string FontToString(const gfx::Font& font) {
  std::string font_string = font.GetFontName();
  font_string += "|";
  font_string += base::IntToString(font.GetFontSize());
  int style = font.GetStyle();
  if (style & gfx::Font::BOLD)
    font_string += "|bold";
  if (style & gfx::Font::ITALIC)
    font_string += "|italic";
  return font_string;
}

}  // namespace

namespace gfx {

TEST(FontListTest, FontDescString_FromDescString) {
  // Test init from font name style size string.
  FontList font_list = FontList("Droid Sans serif, Sans serif, 10px");
  const std::string& font_str = font_list.GetFontDescriptionString();
  EXPECT_EQ("Droid Sans serif, Sans serif, 10px", font_str);
}

TEST(FontListTest, FontDescString_FromFont) {
  // Test init from Font.
  Font font("Arial", 8);
  FontList font_list = FontList(font);
  const std::string& font_str = font_list.GetFontDescriptionString();
  EXPECT_EQ("Arial,8px", font_str);
}

TEST(FontListTest, FontDescString_FromFontWithNonNormalStyle) {
  // Test init from Font with non-normal style.
  Font font("Arial", 8);
  FontList font_list = FontList(font.DeriveFont(2, Font::BOLD));
  EXPECT_EQ("Arial,Bold 10px", font_list.GetFontDescriptionString());

  font_list = FontList(font.DeriveFont(-2, Font::ITALIC));
  EXPECT_EQ("Arial,Italic 6px", font_list.GetFontDescriptionString());
}

TEST(FontListTest, FontDescString_FromFontVector) {
  // Test init from Font vector.
  Font font("Arial", 8);
  Font font_1("Sans serif", 10);
  std::vector<Font> fonts;
  fonts.push_back(font.DeriveFont(0, Font::BOLD));
  fonts.push_back(font_1.DeriveFont(-2, Font::BOLD));
  FontList font_list = FontList(fonts);
  const std::string& font_str = font_list.GetFontDescriptionString();
  EXPECT_EQ("Arial,Sans serif,Bold 8px", font_str);
}

TEST(FontListTest, Fonts_FromDescString) {
  // Test init from font name size string.
  FontList font_list = FontList("serif,Sans serif, 13px");
  const std::vector<Font>& fonts = font_list.GetFonts();
  EXPECT_EQ(2U, fonts.size());
  EXPECT_EQ("serif|13", FontToString(fonts[0]));
  EXPECT_EQ("Sans serif|13", FontToString(fonts[1]));
}

TEST(FontListTest, Fonts_FromDescStringInFlexibleFormat) {
  // Test init from font name size string with flexible format.
  FontList font_list = FontList("  serif   ,   Sans serif ,   13px");
  const std::vector<Font>& fonts = font_list.GetFonts();
  EXPECT_EQ(2U, fonts.size());
  EXPECT_EQ("serif|13", FontToString(fonts[0]));
  EXPECT_EQ("Sans serif|13", FontToString(fonts[1]));
}

TEST(FontListTest, Fonts_FromDescStringWithStyleInFlexibleFormat) {
  // Test init from font name style size string with flexible format.
  FontList font_list = FontList("  serif  ,  Sans serif ,  Bold   "
                                "  Italic   13px");
  const std::vector<Font>& fonts = font_list.GetFonts();
  EXPECT_EQ(2U, fonts.size());
  EXPECT_EQ("serif|13|bold|italic", FontToString(fonts[0]));
  EXPECT_EQ("Sans serif|13|bold|italic", FontToString(fonts[1]));
}

TEST(FontListTest, Fonts_FromFont) {
  // Test init from Font.
  Font font("Arial", 8);
  FontList font_list = FontList(font);
  const std::vector<Font>& fonts = font_list.GetFonts();
  EXPECT_EQ(1U, fonts.size());
  EXPECT_EQ("Arial|8", FontToString(fonts[0]));
}

TEST(FontListTest, Fonts_FromFontWithNonNormalStyle) {
  // Test init from Font with non-normal style.
  Font font("Arial", 8);
  FontList font_list = FontList(font.DeriveFont(2, Font::BOLD));
  std::vector<Font> fonts = font_list.GetFonts();
  EXPECT_EQ(1U, fonts.size());
  EXPECT_EQ("Arial|10|bold", FontToString(fonts[0]));

  font_list = FontList(font.DeriveFont(-2, Font::ITALIC));
  fonts = font_list.GetFonts();
  EXPECT_EQ(1U, fonts.size());
  EXPECT_EQ("Arial|6|italic", FontToString(fonts[0]));
}

TEST(FontListTest, Fonts_FromFontVector) {
  // Test init from Font vector.
  Font font("Arial", 8);
  Font font_1("Sans serif", 10);
  std::vector<Font> input_fonts;
  input_fonts.push_back(font.DeriveFont(0, Font::BOLD));
  input_fonts.push_back(font_1.DeriveFont(-2, Font::BOLD));
  FontList font_list = FontList(input_fonts);
  const std::vector<Font>& fonts = font_list.GetFonts();
  EXPECT_EQ(2U, fonts.size());
  EXPECT_EQ("Arial|8|bold", FontToString(fonts[0]));
  EXPECT_EQ("Sans serif|8|bold", FontToString(fonts[1]));
}

TEST(FontListTest, Fonts_DescStringWithStyleInFlexibleFormat_RoundTrip) {
  // Test round trip from font description string to font vector to
  // font description string.
  FontList font_list = FontList("  serif  ,  Sans serif ,  Bold   "
                                "  Italic   13px");

  const std::vector<Font>& fonts = font_list.GetFonts();
  FontList font_list_1 = FontList(fonts);
  const std::string& desc_str = font_list_1.GetFontDescriptionString();

  EXPECT_EQ("serif,Sans serif,Bold Italic 13px", desc_str);
}

TEST(FontListTest, Fonts_FontVector_RoundTrip) {
  // Test round trip from font vector to font description string to font vector.
  Font font("Arial", 8);
  Font font_1("Sans serif", 10);
  std::vector<Font> input_fonts;
  input_fonts.push_back(font.DeriveFont(0, Font::BOLD));
  input_fonts.push_back(font_1.DeriveFont(-2, Font::BOLD));
  FontList font_list = FontList(input_fonts);

  const std::string& desc_string = font_list.GetFontDescriptionString();
  FontList font_list_1 = FontList(desc_string);
  const std::vector<Font>& round_trip_fonts = font_list_1.GetFonts();

  EXPECT_EQ(2U, round_trip_fonts.size());
  EXPECT_EQ("Arial|8|bold", FontToString(round_trip_fonts[0]));
  EXPECT_EQ("Sans serif|8|bold", FontToString(round_trip_fonts[1]));
}

TEST(FontListTest, FontDescString_GetStyle) {
  FontList font_list = FontList("Arial,Sans serif, 8px");
  EXPECT_EQ(Font::NORMAL, font_list.GetFontStyle());

  font_list = FontList("Arial,Sans serif,Bold 8px");
  EXPECT_EQ(Font::BOLD, font_list.GetFontStyle());

  font_list = FontList("Arial,Sans serif,Italic 8px");
  EXPECT_EQ(Font::ITALIC, font_list.GetFontStyle());

  font_list = FontList("Arial,Italic Bold 8px");
  EXPECT_EQ(Font::BOLD | Font::ITALIC, font_list.GetFontStyle());
}

TEST(FontListTest, Fonts_GetStyle) {
  std::vector<Font> fonts;
  fonts.push_back(gfx::Font("Arial", 8));
  fonts.push_back(gfx::Font("Sans serif", 8));
  FontList font_list = FontList(fonts);
  EXPECT_EQ(Font::NORMAL, font_list.GetFontStyle());
  fonts[0] = fonts[0].DeriveFont(0, Font::ITALIC | Font::BOLD);
  fonts[1] = fonts[1].DeriveFont(0, Font::ITALIC | Font::BOLD);
  font_list = FontList(fonts);
  EXPECT_EQ(Font::ITALIC | Font::BOLD, font_list.GetFontStyle());
}

TEST(FontListTest, FontDescString_DeriveFontList) {
  FontList font_list = FontList("Arial,Sans serif, 8px");

  FontList derived = font_list.DeriveFontList(Font::BOLD | Font::ITALIC);
  EXPECT_EQ("Arial,Sans serif,Bold Italic 8px",
            derived.GetFontDescriptionString());
}

TEST(FontListTest, Fonts_DeriveFontList) {
  std::vector<Font> fonts;
  fonts.push_back(gfx::Font("Arial", 8));
  fonts.push_back(gfx::Font("Sans serif", 8));
  FontList font_list = FontList(fonts);

  FontList derived = font_list.DeriveFontList(Font::BOLD | Font::ITALIC);
  const std::vector<Font>& derived_fonts = derived.GetFonts();

  EXPECT_EQ(2U, derived_fonts.size());
  EXPECT_EQ("Arial|8|bold|italic", FontToString(derived_fonts[0]));
  EXPECT_EQ("Sans serif|8|bold|italic", FontToString(derived_fonts[1]));
}

TEST(FontListTest, FontDescString_DeriveFontListWithSize) {
  FontList font_list = FontList("Arial,Sans serif,Bold Italic  8px");

  FontList derived = font_list.DeriveFontListWithSize(10);
  EXPECT_EQ("Arial,Sans serif,Bold Italic 10px",
            derived.GetFontDescriptionString());
}

TEST(FontListTest, Fonts_DeriveFontListWithSize) {
  std::vector<Font> fonts;
  fonts.push_back(gfx::Font("Arial", 8));
  fonts.push_back(gfx::Font("Sans serif", 8));
  FontList font_list = FontList(fonts);

  FontList derived = font_list.DeriveFontListWithSize(5);
  const std::vector<Font>& derived_fonts = derived.GetFonts();

  EXPECT_EQ(2U, derived_fonts.size());
  EXPECT_EQ("Arial|5", FontToString(derived_fonts[0]));
  EXPECT_EQ("Sans serif|5", FontToString(derived_fonts[1]));
}

}  // namespace gfx
