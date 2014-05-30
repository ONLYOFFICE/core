// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_NATIVE_THEME_PAINTER_H_
#define UI_VIEWS_NATIVE_THEME_PAINTER_H_

#include "base/compiler_specific.h"
#include "ui/views/painter.h"

namespace gfx {
class Canvas;
class Size;
}

namespace views {

class NativeThemeDelegate;
class View;

// A Painter that uses NativeTheme to implement painting and sizing.  A
// theme delegate must be given at construction time so that the appropriate
// painting and sizing can be done.
class VIEWS_EXPORT NativeThemePainter : public Painter {
 public:
  NativeThemePainter(NativeThemeDelegate* delegate, views::View* view);

  virtual ~NativeThemePainter() {}

  // Returns the preferred size of the native part being painted.
  gfx::Size GetPreferredSize();

 private:
  // Overridden from Painter:
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) OVERRIDE;

  // The delegate the controls the appearance of this painter.
  NativeThemeDelegate* delegate_;

  views::View* view_;

  DISALLOW_COPY_AND_ASSIGN(NativeThemePainter);
};

}  // namespace views

#endif  // UI_VIEWS_NATIVE_THEME_PAINTER_H_
