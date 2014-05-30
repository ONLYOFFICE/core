// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_BORDER_H_
#define UI_VIEWS_FOCUS_BORDER_H_

#include "ui/views/views_export.h"
#include "base/basictypes.h"

namespace gfx {
class Canvas;
}

namespace views {
class View;

////////////////////////////////////////////////////////////////////////////////
//
// Focus border class.
//
// The focus border class is used to display an indication of focus on a view.
// To set a focus border on a view, call SetFocusBorder on the view.  Once set
// on a view, the focus border is owned by the view.
//
////////////////////////////////////////////////////////////////////////////////

class VIEWS_EXPORT FocusBorder {
 public:
  virtual ~FocusBorder();

  // Creates the default inset dashed line focus border.
  static FocusBorder* CreateDashedFocusBorder();
  static FocusBorder* CreateDashedFocusBorder(
      int left, int top, int right, int bottom);

  // Renders the focus border for the specified view.
  virtual void Paint(const View& view, gfx::Canvas* canvas) const = 0;

 protected:
  FocusBorder();

 private:
  DISALLOW_COPY_AND_ASSIGN(FocusBorder);
};

}  // namespace views

#endif  // UI_VIEWS_FOCUS_BORDER_H_
