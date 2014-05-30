// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/layout/box_layout.h"

#include "ui/gfx/rect.h"
#include "ui/views/view.h"

namespace views {

BoxLayout::BoxLayout(BoxLayout::Orientation orientation,
                     int inside_border_horizontal_spacing,
                     int inside_border_vertical_spacing,
                     int between_child_spacing)
    : orientation_(orientation),
      inside_border_insets_(inside_border_vertical_spacing,
                            inside_border_horizontal_spacing,
                            inside_border_vertical_spacing,
                            inside_border_horizontal_spacing),
      between_child_spacing_(between_child_spacing),
      spread_blank_space_(false) {
}

BoxLayout::~BoxLayout() {
}

void BoxLayout::Layout(View* host) {
  gfx::Rect child_area(host->GetLocalBounds());
  child_area.Inset(host->GetInsets());
  child_area.Inset(inside_border_insets_);
  int x = child_area.x();
  int y = child_area.y();

  int padding = 0;
  if (spread_blank_space_) {
    int total = 0;
    int visible = 0;
    for (int i = 0; i < host->child_count(); ++i) {
      View* child = host->child_at(i);
      if (!child->visible())
        continue;
      if (orientation_ == kHorizontal) {
        total += child->GetPreferredSize().width() + between_child_spacing_;
      } else {
        total += child->GetHeightForWidth(child_area.width()) +
            between_child_spacing_;
      }
      ++visible;
    }

    if (visible) {
      total -= between_child_spacing_;
      if (orientation_ == kHorizontal)
        padding = (child_area.width() - total) / visible;
      else
        padding = (child_area.height() - total) / visible;

      if (padding < 0)
        padding = 0;
    }
  }

  for (int i = 0; i < host->child_count(); ++i) {
    View* child = host->child_at(i);
    if (child->visible()) {
      gfx::Rect bounds(x, y, child_area.width(), child_area.height());
      if (orientation_ == kHorizontal) {
        bounds.set_width(child->GetPreferredSize().width() + padding);
        x += bounds.width() + between_child_spacing_;
      } else {
        bounds.set_height(child->GetHeightForWidth(bounds.width()) + padding);
        y += bounds.height() + between_child_spacing_;
      }
      // Clamp child view bounds to |child_area|.
      bounds.Intersect(child_area);
      child->SetBoundsRect(bounds);
    }
  }
}

gfx::Size BoxLayout::GetPreferredSize(View* host) {
  // Calculate the child views' preferred width.
  int width = 0;
  if (orientation_ == kVertical) {
    for (int i = 0; i < host->child_count(); ++i) {
      View* child = host->child_at(i);
      if (!child->visible())
        continue;

      width = std::max(width, child->GetPreferredSize().width());
    }
  }

  return GetPreferredSizeForChildWidth(host, width);
}

int BoxLayout::GetPreferredHeightForWidth(View* host, int width) {
  int child_width = width - NonChildSize(host).width();
  return GetPreferredSizeForChildWidth(host, child_width).height();
}

gfx::Size BoxLayout::GetPreferredSizeForChildWidth(View* host,
                                                   int child_area_width) {
  gfx::Rect child_area_bounds;

  if (orientation_ == kHorizontal) {
    // Horizontal layouts ignore |child_area_width|, meaning they mimic the
    // default behavior of GridLayout::GetPreferredHeightForWidth().
    // TODO(estade): fix this if it ever becomes a problem.
    int position = 0;
    for (int i = 0; i < host->child_count(); ++i) {
      View* child = host->child_at(i);
      if (!child->visible())
        continue;

      gfx::Size size(child->GetPreferredSize());
      gfx::Rect child_bounds(position, 0, size.width(), size.height());
      child_area_bounds.Union(child_bounds);
      position += size.width() + between_child_spacing_;
    }
  } else {
    int height = 0;
    for (int i = 0; i < host->child_count(); ++i) {
      View* child = host->child_at(i);
      if (!child->visible())
        continue;

      height += child->GetHeightForWidth(child_area_width);
      if (i != 0)
        height += between_child_spacing_;
    }

    child_area_bounds.set_width(child_area_width);
    child_area_bounds.set_height(height);
  }

  gfx::Size non_child_size = NonChildSize(host);
  return gfx::Size(child_area_bounds.width() + non_child_size.width(),
                   child_area_bounds.height() + non_child_size.height());
}

gfx::Size BoxLayout::NonChildSize(View* host) {
  gfx::Insets insets(host->GetInsets());
  return gfx::Size(insets.width() + inside_border_insets_.width(),
                   insets.height() + inside_border_insets_.height());
}

} // namespace views
