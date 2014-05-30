// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_FONT_LIST_H_
#define UI_GFX_FONT_LIST_H_

#include <string>
#include <vector>

#include "ui/base/ui_export.h"
#include "ui/gfx/font.h"

namespace gfx {

// FontList represents a list of fonts either in the form of Font vector or in
// the form of a string representing font names, styles, and size.
//
// The string representation is in the form "FAMILY_LIST [STYLE_OPTIONS] SIZE",
// where FAMILY_LIST is a comma separated list of families terminated by a
// comma, STYLE_OPTIONS is a whitespace separated list of words where each word
// describes one of style, variant, weight, stretch, or gravity, and SIZE is
// a decimal number followed by "px" for absolute size. STYLE_OPTIONS may be
// absent.
//
// The string format complies with that of Pango detailed at
// http://developer.gnome.org/pango/stable/pango-Fonts.html#pango-font-description-from-string
//
// FontList could be initialized either way without conversion to the other
// form. The conversion to the other form is done only when asked to get the
// other form.
//
// FontList allows operator= since FontList is a data member type in RenderText,
// and operator= is used in RenderText::SetFontList().
class UI_EXPORT FontList {
 public:
  // Creates a font list with a Font with default name and style.
  FontList();

  // Creates a font list from a string representing font names, styles, and
  // size.
  explicit FontList(const std::string& font_description_string);

  // Creates a font list from a Font vector.
  // All fonts in this vector should have the same style and size.
  explicit FontList(const std::vector<Font>& fonts);

  // Creates a font list from a Font.
  explicit FontList(const Font& font);

  ~FontList();

  // Returns a new FontList with the given |font_style| flags.
  FontList DeriveFontList(int font_style) const;

  // Returns a new FontList with the same font names and style but with the
  // given font |size| in pixels.
  FontList DeriveFontListWithSize(int size) const;

  // Returns the |gfx::Font::FontStyle| style flags for this font list.
  int GetFontStyle() const;

  // Returns a string representing font names, styles, and size. If the FontList
  // is initialized by a vector of Font, use the first font's style and size
  // for the description.
  const std::string& GetFontDescriptionString() const;

  // Returns the Font vector.
  const std::vector<Font>& GetFonts() const;

 private:
  // A vector of Font. If FontList is constructed with font description string,
  // |fonts_| is not initialized during construction. Instead, it is computed
  // lazily when user asked to get the font vector.
  mutable std::vector<Font> fonts_;

  // A string representing font names, styles, and sizes.
  // Please refer to the comments before class declaration for details on string
  // format.
  // If FontList is constructed with a vector of font,
  // |font_description_string_| is not initialized during construction. Instead,
  // it is computed lazily when user asked to get the font description string.
  mutable std::string font_description_string_;
};

}  // namespace gfx

#endif  // UI_GFX_FONT_LIST_H_
