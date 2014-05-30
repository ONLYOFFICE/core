// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>

#include "base/logging.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/strings/string_split.h"
#include "ui/gfx/font_list.h"

namespace {

// Parses font description into |font_names|, |font_style| and |font_size|.
void ParseFontDescriptionString(const std::string& font_description_string,
                                std::vector<std::string>* font_names,
                                int* font_style,
                                int* font_size) {
  base::SplitString(font_description_string, ',', font_names);
  DCHECK_GT(font_names->size(), 1U);

  // The last item is [STYLE_OPTIONS] SIZE.
  std::vector<std::string> styles_size;
  base::SplitString(font_names->back(), ' ', &styles_size);
  DCHECK(!styles_size.empty());
  base::StringToInt(styles_size.back(), font_size);
  DCHECK_GT(*font_size, 0);
  font_names->pop_back();

  // Font supports BOLD and ITALIC; underline is supported via RenderText.
  *font_style = 0;
  for (size_t i = 0; i < styles_size.size() - 1; ++i) {
    // Styles are separated by white spaces. base::SplitString splits styles
    // by space, and it inserts empty string for continuous spaces.
    if (styles_size[i].empty())
      continue;
    if (!styles_size[i].compare("Bold"))
      *font_style |= gfx::Font::BOLD;
    else if (!styles_size[i].compare("Italic"))
      *font_style |= gfx::Font::ITALIC;
    else
      NOTREACHED();
  }
}

// Returns the font style and size as a string.
std::string FontStyleAndSizeToString(int font_style, int font_size) {
  std::string result;
  if (font_style & gfx::Font::BOLD)
    result += "Bold ";
  if (font_style & gfx::Font::ITALIC)
    result += "Italic ";
  result += base::IntToString(font_size);
  result += "px";
  return result;
}

// Returns font description from |font_names|, |font_style|, and |font_size|.
std::string BuildFontDescription(const std::vector<std::string>& font_names,
                                 int font_style,
                                 int font_size) {
  std::string description = JoinString(font_names, ',');
  description += "," + FontStyleAndSizeToString(font_style, font_size);
  return description;
}

}  // namespace

namespace gfx {

FontList::FontList() {
  fonts_.push_back(Font());
}

FontList::FontList(const std::string& font_description_string)
    : font_description_string_(font_description_string) {
  DCHECK(!font_description_string.empty());
  // DCHECK description string ends with "px" for size in pixel.
  DCHECK(EndsWith(font_description_string, "px", true));
}

FontList::FontList(const std::vector<Font>& fonts)
    : fonts_(fonts) {
  DCHECK(!fonts.empty());
  if (DCHECK_IS_ON()) {
    int style = fonts[0].GetStyle();
    int size = fonts[0].GetFontSize();
    for (size_t i = 1; i < fonts.size(); ++i) {
      DCHECK_EQ(fonts[i].GetStyle(), style);
      DCHECK_EQ(fonts[i].GetFontSize(), size);
    }
  }
}

FontList::FontList(const Font& font) {
  fonts_.push_back(font);
}

FontList::~FontList() {
}

FontList FontList::DeriveFontList(int font_style) const {
  // If there is a font vector, derive from that.
  if (!fonts_.empty()) {
    std::vector<Font> fonts = fonts_;
    for (size_t i = 0; i < fonts.size(); ++i)
      fonts[i] = fonts[i].DeriveFont(0, font_style);
    return FontList(fonts);
  }

  // Otherwise, parse the font description string to derive from it.
  std::vector<std::string> font_names;
  int old_style;
  int font_size;
  ParseFontDescriptionString(font_description_string_, &font_names,
                             &old_style, &font_size);
  return FontList(BuildFontDescription(font_names, font_style, font_size));
}

FontList FontList::DeriveFontListWithSize(int size) const {
  DCHECK_GT(size, 0);

  // If there is a font vector, derive from that.
  int old_size = 0;
  if (!fonts_.empty()) {
    old_size = fonts_[0].GetFontSize();
    if (old_size == size)
      return FontList(fonts_);

    std::vector<Font> fonts = fonts_;
    for (size_t i = 0; i < fonts.size(); ++i)
      fonts[i] = fonts[i].DeriveFont(size - old_size);
    return FontList(fonts);
  }

  // Otherwise, parse the font description string to derive from it.
  std::vector<std::string> font_names;
  int font_style = 0;
  ParseFontDescriptionString(font_description_string_, &font_names,
                             &font_style, &old_size);

  if (old_size == size)
    return FontList(font_description_string_);

  return FontList(BuildFontDescription(font_names, font_style, size));
}

int FontList::GetFontStyle() const {
  if (!fonts_.empty())
    return fonts_[0].GetStyle();

  std::vector<std::string> font_names;
  int font_style;
  int font_size;
  ParseFontDescriptionString(font_description_string_, &font_names,
                             &font_style, &font_size);
  return font_style;
}

const std::string& FontList::GetFontDescriptionString() const {
  if (font_description_string_.empty()) {
    DCHECK(!fonts_.empty());
    for (size_t i = 0; i < fonts_.size(); ++i) {
      std::string name = fonts_[i].GetFontName();
      font_description_string_ += name;
      font_description_string_ += ',';
    }
    // All fonts have the same style and size.
    font_description_string_ +=
        FontStyleAndSizeToString(fonts_[0].GetStyle(), fonts_[0].GetFontSize());
  }
  return font_description_string_;
}

const std::vector<Font>& FontList::GetFonts() const {
  if (fonts_.empty()) {
    DCHECK(!font_description_string_.empty());

    std::vector<std::string> font_names;
    int font_style;
    int font_size;
    ParseFontDescriptionString(font_description_string_, &font_names,
                               &font_style, &font_size);
    for (size_t i = 0; i < font_names.size(); ++i) {
      DCHECK(!font_names[i].empty());

      Font font(font_names[i], font_size);
      if (font_style == Font::NORMAL)
        fonts_.push_back(font);
      else
        fonts_.push_back(font.DeriveFont(0, font_style));
    }
  }
  return fonts_;
}

}  // namespace gfx
