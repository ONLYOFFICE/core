// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LAYOUT_BOX_LAYOUT_H_
#define UI_VIEWS_LAYOUT_BOX_LAYOUT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/insets.h"
#include "ui/views/layout/layout_manager.h"

namespace gfx {
class Size;
}

namespace views {

class View;

// A Layout manager that arranges child views vertically or horizontally in a
// side-by-side fashion with spacing around and between the child views. The
// child views are always sized according to their preferred size. If the
// host's bounds provide insufficient space, child views will be clamped.
// Excess space will not be distributed.
class VIEWS_EXPORT BoxLayout : public LayoutManager {
 public:
  enum Orientation {
    kHorizontal,
    kVertical,
  };

  // Use |inside_border_horizontal_spacing| and
  // |inside_border_vertical_spacing| to add additional space between the child
  // view area and the host view border. |between_child_spacing| controls the
  // space in between child views.
  BoxLayout(Orientation orientation,
            int inside_border_horizontal_spacing,
            int inside_border_vertical_spacing,
            int between_child_spacing);
  virtual ~BoxLayout();

  void set_spread_blank_space(bool spread) {
    spread_blank_space_ = spread;
  }

  // Overridden from views::LayoutManager:
  virtual void Layout(View* host) OVERRIDE;
  virtual gfx::Size GetPreferredSize(View* host) OVERRIDE;
  virtual int GetPreferredHeightForWidth(View* host, int width) OVERRIDE;

 private:
  // The preferred size for the dialog given the width of the child area.
  gfx::Size GetPreferredSizeForChildWidth(View* host, int child_area_width);

  // The amount of space the layout requires in addition to any space for the
  // child views.
  gfx::Size NonChildSize(View* host);

  const Orientation orientation_;

  // Spacing between child views and host view border.
  gfx::Insets inside_border_insets_;

  // Spacing to put in between child views.
  const int between_child_spacing_;

  // Whether the available extra space should be distributed among the child
  // views.
  bool spread_blank_space_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(BoxLayout);
};

} // namespace views

#endif  // UI_VIEWS_LAYOUT_BOX_LAYOUT_H_
