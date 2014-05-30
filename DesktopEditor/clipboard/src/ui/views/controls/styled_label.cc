// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/styled_label.h"

#include <vector>

#include "base/string_util.h"
#include "ui/base/text/text_elider.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/link.h"
#include "ui/views/controls/styled_label_listener.h"

namespace views {

namespace {

// Calculates the height of a line of text. Currently returns the height of
// a label.
int CalculateLineHeight() {
  Label label;
  return label.GetPreferredSize().height();
}

scoped_ptr<View> CreateLabelRange(const string16& text,
                                  const StyledLabel::RangeStyleInfo& style_info,
                                  views::LinkListener* link_listener) {
  scoped_ptr<Label> result;

  if (style_info.is_link) {
    Link* link = new Link(text);
    link->set_listener(link_listener);
    link->SetUnderline((style_info.font_style & gfx::Font::UNDERLINE) != 0);
    result.reset(link);
  } else {
    Label* label = new Label(text);
    // Give the label a focus border so that its preferred size matches
    // links' preferred sizes
    label->SetHasFocusBorder(true);

    result.reset(label);
  }

  if (!style_info.tooltip.empty())
    result->SetTooltipText(style_info.tooltip);
  if (style_info.font_style != gfx::Font::NORMAL)
    result->SetFont(result->font().DeriveFont(0, style_info.font_style));

  return scoped_ptr<View>(result.release());
}

}  // namespace


StyledLabel::RangeStyleInfo::RangeStyleInfo()
    : font_style(gfx::Font::NORMAL),
      disable_line_wrapping(false),
      is_link(false) {
}

StyledLabel::RangeStyleInfo::~RangeStyleInfo() {}

// static
StyledLabel::RangeStyleInfo StyledLabel::RangeStyleInfo::CreateForLink() {
  RangeStyleInfo result;
  result.disable_line_wrapping = true;
  result.is_link = true;
  result.font_style = gfx::Font::UNDERLINE;
  return result;
}

bool StyledLabel::StyleRange::operator<(
    const StyledLabel::StyleRange& other) const {
  // Intentionally reversed so the priority queue is sorted by smallest first.
  return range.start() > other.range.start();
}

StyledLabel::StyledLabel(const string16& text, StyledLabelListener* listener)
    : listener_(listener) {
  TrimWhitespace(text, TRIM_TRAILING, &text_);
}

StyledLabel::~StyledLabel() {}

void StyledLabel::SetText(const string16& text) {
  text_ = text;
  calculated_size_ = gfx::Size();
  style_ranges_ = std::priority_queue<StyleRange>();
  RemoveAllChildViews(true);
  PreferredSizeChanged();
}

void StyledLabel::AddStyleRange(const ui::Range& range,
                                const RangeStyleInfo& style_info) {
  DCHECK(!range.is_reversed());
  DCHECK(!range.is_empty());
  DCHECK(ui::Range(0, text_.size()).Contains(range));

  style_ranges_.push(StyleRange(range, style_info));

  calculated_size_ = gfx::Size();
  PreferredSizeChanged();
}

gfx::Insets StyledLabel::GetInsets() const {
  gfx::Insets insets = View::GetInsets();
  const gfx::Insets focus_border_padding(1, 1, 1, 1);
  insets += focus_border_padding;
  return insets;
}

int StyledLabel::GetHeightForWidth(int w) {
  if (w != calculated_size_.width())
    calculated_size_ = gfx::Size(w, CalculateAndDoLayout(w, true));

  return calculated_size_.height();
}

void StyledLabel::Layout() {
  CalculateAndDoLayout(GetLocalBounds().width(), false);
}

void StyledLabel::LinkClicked(Link* source, int event_flags) {
  listener_->StyledLabelLinkClicked(link_targets_[source], event_flags);
}

int StyledLabel::CalculateAndDoLayout(int width, bool dry_run) {
  if (!dry_run) {
    RemoveAllChildViews(true);
    link_targets_.clear();
  }

  width -= GetInsets().width();
  if (width <= 0 || text_.empty())
    return 0;

  const int line_height = CalculateLineHeight();
  // The index of the line we're on.
  int line = 0;
  // The x position (in pixels) of the line we're on, relative to content
  // bounds.
  int x = 0;

  string16 remaining_string = text_;
  std::priority_queue<StyleRange> style_ranges = style_ranges_;

  // Iterate over the text, creating a bunch of labels and links and laying them
  // out in the appropriate positions.
  while (!remaining_string.empty()) {
    // Don't put whitespace at beginning of a line with an exception for the
    // first line (so the text's leading whitespace is respected).
    if (x == 0 && line > 0)
      TrimWhitespace(remaining_string, TRIM_LEADING, &remaining_string);

    ui::Range range(ui::Range::InvalidRange());
    if (!style_ranges.empty())
      range = style_ranges.top().range;

    const size_t position = text_.size() - remaining_string.size();

    const gfx::Rect chunk_bounds(x, 0, width - x, 2 * line_height);
    std::vector<string16> substrings;
    gfx::Font text_font;
    // If the start of the remaining text is inside a styled range, the font
    // style may differ from the base font. The font specified by the range
    // should be used when eliding text.
    if (position >= range.start()) {
      text_font =
          text_font.DeriveFont(0, style_ranges.top().style_info.font_style);
    }
    ui::ElideRectangleText(remaining_string,
                           text_font,
                           chunk_bounds.width(),
                           chunk_bounds.height(),
                           ui::IGNORE_LONG_WORDS,
                           &substrings);

    DCHECK(!substrings.empty());
    string16 chunk = substrings[0];
    if (chunk.empty()) {
      // Nothing fits on this line. Start a new line.
      // If x is 0, first line may have leading whitespace that doesn't fit in a
      // single line, so try trimming those. Otherwise there is no room for
      // anything; abort.
      if (x == 0) {
        if (line == 0) {
          TrimWhitespace(remaining_string, TRIM_LEADING, &remaining_string);
          continue;
        }
        break;
      }

      x = 0;
      line++;
      continue;
    }

    scoped_ptr<View> view;
    if (position >= range.start()) {
      const RangeStyleInfo& style_info = style_ranges.top().style_info;

      if (style_info.disable_line_wrapping && chunk.size() < range.length() &&
          position == range.start() && x != 0) {
        // If the chunk should not be wrapped, try to fit it entirely on the
        // next line.
        x = 0;
        line++;
        continue;
      }

      chunk = chunk.substr(0, std::min(chunk.size(), range.end() - position));

      view = CreateLabelRange(chunk, style_info, this);

      if (style_info.is_link && !dry_run)
        link_targets_[view.get()] = range;

      if (position + chunk.size() >= range.end())
        style_ranges.pop();
    } else {
      // This chunk is normal text.
      if (position + chunk.size() > range.start())
        chunk = chunk.substr(0, range.start() - position);
      view = CreateLabelRange(chunk, RangeStyleInfo(), this);
    }

    // Lay out the views to overlap by 1 pixel to compensate for their border
    // spacing. Otherwise, "<a>link</a>," will render as "link ,".
    const int overlap = 1;
    const gfx::Size view_size = view->GetPreferredSize();
    DCHECK_EQ(line_height, view_size.height() - 2 * overlap);
    if (!dry_run) {
      view->SetBoundsRect(gfx::Rect(
          gfx::Point(GetInsets().left() + x - overlap,
                     GetInsets().top() + line * line_height - overlap),
          view_size));
      AddChildView(view.release());
    }
    x += view_size.width() - 2 * overlap;

    remaining_string = remaining_string.substr(chunk.size());
  }

  return (line + 1) * line_height + GetInsets().height();
}

}  // namespace views
