// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEW_TEXT_UTILS_H_
#define UI_VIEWS_VIEW_TEXT_UTILS_H_

#include "base/string16.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Font;
class Rect;
class Size;
}

namespace views {
class Label;
class Link;
}

// This file defines utility functions for working with text in views.

namespace view_text_utils {

// Draws a string onto the canvas (wrapping if needed) while also keeping
// track of where it ends so we can position a URL after the text. The
// parameter |bounds| represents the boundary we have to work with, |position|
// specifies where to draw the string (relative to the top left corner of the
// |bounds| rectangle and |font| specifies the font to use when drawing. When
// the function returns, the parameter |rect| contains where to draw the URL
// (to the right of where we just drew the text) and |position| is updated to
// reflect where to draw the next string after the URL.  |label| is a dummy
// label with the correct width and origin for the text to be written; it's
// used so that the x position can be correctly mirrored in RTL languages.
// |text_direction_is_rtl| is true if an RTL language is being used.
// NOTE: The reason why we need this function is because while Skia knows how
// to wrap text appropriately, it doesn't tell us where it drew the last
// character, which we need to position the URLs within the text.
VIEWS_EXPORT void DrawTextAndPositionUrl(gfx::Canvas* canvas,
                                         views::Label* label,
                                         const string16& text,
                                         views::Link* link,
                                         gfx::Rect* rect,
                                         gfx::Size* position,
                                         bool text_direction_is_rtl,
                                         const gfx::Rect& bounds,
                                         const gfx::Font& font);

// A helper function for DrawTextAndPositionUrl, which simply draws the text
// from a certain starting point |position| and wraps within bounds.
// |word_for_word| specifies whether to draw the text word for word or whether
// to treat the text as one blurb (similar to the way URL's are treated inside
// RTL text. For details on the other parameters, see DrawTextAndPositionUrl.
void DrawTextStartingFrom(gfx::Canvas* canvas,
                          views::Label* label,
                          const string16& text,
                          gfx::Size* position,
                          const gfx::Rect& bounds,
                          const gfx::Font& font,
                          bool text_direction_is_rtl,
                          bool word_for_word);

// A simply utility function that calculates whether a word of width
// |word_width| fits at position |position| within the |bounds| rectangle. If
// not, |position| is updated to wrap to the beginning of the next line.
void WrapIfWordDoesntFit(int word_width,
                         int font_height,
                         gfx::Size* position,
                         const gfx::Rect& bounds);

}  // namespace view_text_utils

#endif  // UI_VIEWS_VIEW_TEXT_UTILS_H_
