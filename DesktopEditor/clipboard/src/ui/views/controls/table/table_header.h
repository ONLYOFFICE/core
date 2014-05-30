// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_HEADER_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_HEADER_H_

#include "ui/gfx/font.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

class TableView;

// Views used to render the header for the table.
class VIEWS_EXPORT TableHeader : public views::View {
 public:
  // Amount the text is padded on the left/right side.
  static const int kHorizontalPadding;

  // Amount of space reserved for the indicator and padding.
  static const int kSortIndicatorWidth;

  explicit TableHeader(TableView* table);
  virtual ~TableHeader();

  const gfx::Font& font() const { return font_; }

  // views::View overrides.
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

 private:
  // Used to track the column being resized.
  struct ColumnResizeDetails {
    ColumnResizeDetails() : column_index(0), initial_x(0), initial_width(0) {}

    // Index into table_->visible_columns() that is being resized.
    int column_index;

    // X-coordinate of the mouse at the time the resize started.
    int initial_x;

    // Width of the column when the drag started.
    int initial_width;
  };

  // Returns the column to resize given the specified x-coordinate, or -1 if |x|
  // is not in the resize range of any columns.
  int GetResizeColumn(int x) const;

  bool is_resizing() const { return resize_details_.get() != NULL; }

  const gfx::Font font_;

  TableView* table_;

  // If non-null a resize is in progress.
  scoped_ptr<ColumnResizeDetails> resize_details_;

  DISALLOW_COPY_AND_ASSIGN(TableHeader);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TABLE_TABLE_HEADER_H_
