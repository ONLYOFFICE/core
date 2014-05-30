// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TEXT_CONSTANTS_H_
#define UI_GFX_TEXT_CONSTANTS_H_

namespace gfx {

// TODO(msw): Distinguish between logical character stops and glyph stops?
// TODO(msw): Merge with base::i18n::BreakIterator::BreakType.
enum BreakType {
  // Stop cursor movement on neighboring characters.
  CHARACTER_BREAK = 0,
  // Stop cursor movement on nearest word boundaries.
  WORD_BREAK,
  // Stop cursor movement on line ends as shown on screen.
  LINE_BREAK,
};

// Horizontal text alignment modes.
enum HorizontalAlignment {
  // Align the text's left edge with that of its display area.
  ALIGN_LEFT = 0,
  // Align the text's center with that of its display area.
  ALIGN_CENTER,
  // Align the text's right edge with that of its display area.
  ALIGN_RIGHT,
};

// The directionality modes used to determine the base text direction.
enum DirectionalityMode {
  // Use the first strong character's direction.
  DIRECTIONALITY_FROM_TEXT = 0,
  // Use the UI locale's text reading direction.
  DIRECTIONALITY_FROM_UI,
  // Use LTR regardless of content or UI locale.
  DIRECTIONALITY_FORCE_LTR,
  // Use RTL regardless of content or UI locale.
  DIRECTIONALITY_FORCE_RTL,
};

// Text styles and adornments.
// TODO(msw): Merge with gfx::Font::FontStyle.
enum TextStyle {
  BOLD = 0,
  ITALIC,
  STRIKE,
  DIAGONAL_STRIKE,
  UNDERLINE,
  NUM_TEXT_STYLES,
};

}  // namespace gfx

#endif  // UI_GFX_TEXT_CONSTANTS_H_
