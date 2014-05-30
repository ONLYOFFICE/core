// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_STYLED_LABEL_H_
#define UI_VIEWS_CONTROLS_STYLED_LABEL_H_

#include <map>
#include <queue>

#include "base/basictypes.h"
#include "base/string16.h"
#include "ui/base/range/range.h"
#include "ui/gfx/size.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/view.h"

namespace views {

class Link;
class StyledLabelListener;

// A class which can apply mixed styles to a block of text. Currently, text is
// always multiline. Trailing whitespace in the styled label text is not
// supported and will be trimmed on StyledLabel construction. Leading
// whitespace is respected, provided not only whitespace fits in the first line.
// In this case, leading whitespace is ignored.
class VIEWS_EXPORT StyledLabel : public View, public LinkListener {
 public:
  // Parameters that define label style for a styled label's text range.
  struct VIEWS_EXPORT RangeStyleInfo {
    RangeStyleInfo();
    ~RangeStyleInfo();

    // Creates a range style info with default values for link.
    static RangeStyleInfo CreateForLink();

    // The font style that will be applied to the range. Should be a bitmask of
    // values defined in gfx::Font::FontStyle (BOLD, ITALIC, UNDERLINE).
    int font_style;

    // Tooltip for the range.
    string16 tooltip;

    // If set, the whole range will be put on a single line.
    bool disable_line_wrapping;

    // If set, the range will be created as a link.
    bool is_link;
  };

  // Note that any trailing whitespace in |text| will be trimmed.
  StyledLabel(const string16& text, StyledLabelListener* listener);
  virtual ~StyledLabel();

  // Sets the text to be displayed, and clears any previous styling.
  void SetText(const string16& text);

  // Marks the given range within |text_| with style defined by |style_info|.
  // |range| must be contained in |text_|.
  void AddStyleRange(const ui::Range& range, const RangeStyleInfo& style_info);

  // View implementation:
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual int GetHeightForWidth(int w) OVERRIDE;
  virtual void Layout() OVERRIDE;

  // LinkListener implementation:
  virtual void LinkClicked(Link* source, int event_flags) OVERRIDE;

 private:
  struct StyleRange {
    StyleRange(const ui::Range& range,
               const RangeStyleInfo& style_info)
        : range(range),
          style_info(style_info) {
    }
    ~StyleRange() {}

    bool operator<(const StyleRange& other) const;

    ui::Range range;
    RangeStyleInfo style_info;
  };

  // Calculates how to layout child views, creates them and sets their size
  // and position. |width| is the horizontal space, in pixels, that the view
  // has to work with. If |dry_run| is true, the view hierarchy is not touched.
  // The return value is the height in pixels.
  int CalculateAndDoLayout(int width, bool dry_run);

  // The text to display.
  string16 text_;

  // The listener that will be informed of link clicks.
  StyledLabelListener* listener_;

  // The ranges that should be linkified, sorted by start position.
  std::priority_queue<StyleRange> style_ranges_;

  // A mapping from a view to the range it corresponds to in |text_|. Only views
  // that correspond to ranges with is_link style set will be added to the map.
  std::map<View*, ui::Range> link_targets_;

  // This variable saves the result of the last GetHeightForWidth call in order
  // to avoid repeated calculation.
  gfx::Size calculated_size_;

  DISALLOW_COPY_AND_ASSIGN(StyledLabel);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_STYLED_LABEL_H_
