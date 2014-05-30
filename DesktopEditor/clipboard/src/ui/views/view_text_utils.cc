// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/view_text_utils.h"

#include "base/i18n/bidi_line_iterator.h"
#include "base/i18n/break_iterator.h"
#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/link.h"

namespace view_text_utils {

void DrawTextAndPositionUrl(gfx::Canvas* canvas,
                            views::Label* label,
                            const string16& text,
                            views::Link* link,
                            gfx::Rect* rect,
                            gfx::Size* position,
                            bool text_direction_is_rtl,
                            const gfx::Rect& bounds,
                            const gfx::Font& font) {
  DCHECK(canvas && position);

  // The |text| parameter is potentially a mix of LTR and RTL "runs," where
  // a run is a sequence of words that share the same directionality. We
  // initialize a bidirectional ICU line iterator and split the text into
  // runs that are either strictly LTR or strictly RTL, with no mix.
  base::i18n::BiDiLineIterator bidi_line;
  if (!bidi_line.Open(text, true, false))
    return;

  // Iterate over each run and draw it.
  int run_start = 0;
  int run_end = 0;
  const int runs = bidi_line.CountRuns();
  for (int run = 0; run < runs; ++run) {
    UBiDiLevel level = 0;
    bidi_line.GetLogicalRun(run_start, &run_end, &level);
    DCHECK(run_end > run_start);
    string16 fragment = text.substr(run_start, run_end - run_start);

    // A flag that tells us whether we found LTR text inside RTL text.
    bool ltr_inside_rtl_text =
        ((level & 1) == UBIDI_LTR) && text_direction_is_rtl;

    // Draw text chunk contained in |fragment|. |position| is relative to the
    // top left corner of the label we draw inside, even when drawing RTL.
    DrawTextStartingFrom(canvas, label, fragment, position, bounds, font,
        text_direction_is_rtl, ltr_inside_rtl_text);

    run_start = run_end;  // Advance over what we just drew.
  }

  // If the caller is interested in placing a link after this text blurb, we
  // figure out here where to place it.
  if (link && rect) {
    gfx::Size sz = link->GetPreferredSize();
    gfx::Insets insets = link->GetInsets();
    WrapIfWordDoesntFit(sz.width(), font.GetHeight(), position, bounds);
    int x = position->width();
    int y = position->height();

    // Links have a border to allow them to be focused.
    y -= insets.top();

    *rect = gfx::Rect(x, y, sz.width(), sz.height());

    // Go from relative pixel coordinates (within the label we are drawing
    // on) to absolute pixel coordinates (relative to the top left corner of
    // the dialog content).
    rect->Offset(bounds.x(), bounds.y());
    // And leave some space to draw the link in.
    position->Enlarge(sz.width(), 0);
  }
}

void DrawTextStartingFrom(gfx::Canvas* canvas,
                          views::Label* label,
                          const string16& text,
                          gfx::Size* position,
                          const gfx::Rect& bounds,
                          const gfx::Font& font,
                          bool text_direction_is_rtl,
                          bool ltr_within_rtl) {
  // Iterate through line breaking opportunities (which in English would be
  // spaces and such). This tells us where to wrap.
  base::i18n::BreakIterator iter(text,
                                 base::i18n::BreakIterator::BREAK_SPACE);
  if (!iter.Init())
    return;

  int flags = (text_direction_is_rtl ? gfx::Canvas::TEXT_ALIGN_RIGHT :
                                       gfx::Canvas::TEXT_ALIGN_LEFT);
  flags |= gfx::Canvas::HIDE_PREFIX | gfx::Canvas::NO_ELLIPSIS;

  // Iterate over each word in the text, or put in a more locale-neutral way:
  // iterate to the next line breaking opportunity.
  while (iter.Advance()) {
    // Get the word and figure out the dimensions.
    string16 word;
    if (!ltr_within_rtl)
      word = iter.GetString();  // Get the next word.
    else
      word = text;  // Draw the whole text at once.

    int w = 0, h = 0;
    gfx::Canvas::SizeStringInt(word, font, &w, &h, 0, flags);

    // If we exceed the boundaries, we need to wrap.
    WrapIfWordDoesntFit(w, font.GetHeight(), position, bounds);

    int x = label->GetMirroredXInView(position->width()) + bounds.x();
    if (text_direction_is_rtl) {
      x -= w;
      // When drawing LTR strings inside RTL text we need to make sure we
      // draw the trailing space (if one exists after the LTR text) to the
      // left of the LTR string.
      if (ltr_within_rtl && word[word.size() - 1] == ' ')
        x += gfx::Canvas::GetStringWidth(ASCIIToUTF16(" "), font);
    }
    int y = position->height() + bounds.y();

    // Draw the text on the screen (mirrored, if RTL run).
    canvas->DrawStringInt(word, font, label->enabled_color(), x, y, w,
                          font.GetHeight(), flags);

    if (!word.empty() && word[word.size() - 1] == '\x0a') {
      // When we come across '\n', we move to the beginning of the next line.
      position->set_width(0);
      position->Enlarge(0, font.GetHeight());
    } else {
      // Otherwise, we advance position to the next word.
      position->Enlarge(w, 0);
    }

    if (ltr_within_rtl)
      break;  // LTR within RTL is drawn as one unit, so we are done.
  }
}

void WrapIfWordDoesntFit(int word_width,
                         int font_height,
                         gfx::Size* position,
                         const gfx::Rect& bounds) {
  if (position->width() + word_width > bounds.right()) {
    position->set_width(0);
    position->Enlarge(0, font_height);
  }
}

}  // namespace view_text_utils
