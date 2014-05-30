// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BORDER_H_
#define UI_VIEWS_BORDER_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/insets.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace gfx{
class Canvas;
}

namespace views {

class Painter;
class View;

////////////////////////////////////////////////////////////////////////////////
//
// Border class.
//
// The border class is used to display a border around a view.
// To set a border on a view, just call SetBorder on the view, for example:
// view->set_border(Border::CreateSolidBorder(1, SkColorSetRGB(25, 25, 112));
// Once set on a view, the border is owned by the view.
//
// IMPORTANT NOTE: not all views support borders at this point. In order to
// support the border, views should make sure to use bounds excluding the
// border (by calling View::GetLocalBoundsExcludingBorder) when doing layout and
// painting.
//
////////////////////////////////////////////////////////////////////////////////

class TextButtonBorder;

class VIEWS_EXPORT Border {
 public:
  Border();
  virtual ~Border();

  // Creates a border that is a simple line of the specified thickness and
  // color.
  static Border* CreateSolidBorder(int thickness, SkColor color);

  // Creates a border for reserving space. The returned border does not
  // paint anything.
  static Border* CreateEmptyBorder(int top, int left, int bottom, int right);

  // Creates a border of the specified color, and specified thickness on each
  // side.
  static Border* CreateSolidSidedBorder(int top,
                                        int left,
                                        int bottom,
                                        int right,
                                        SkColor color);

  // Creates a Border from the specified Painter. The border owns the painter,
  // thus the painter is deleted when the Border is deleted.
  // |insets| define size of an area allocated for a Border.
  static Border* CreateBorderPainter(Painter* painter,
                                     const gfx::Insets& insets);

  // Renders the border for the specified view.
  virtual void Paint(const View& view, gfx::Canvas* canvas) = 0;

  // Sets the specified insets to the the border insets.
  virtual gfx::Insets GetInsets() const = 0;

  // Manual RTTI for text buttons.
  virtual TextButtonBorder* AsTextButtonBorder();
  virtual const TextButtonBorder* AsTextButtonBorder() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Border);
};

}  // namespace views

#endif  // UI_VIEWS_BORDER_H_
