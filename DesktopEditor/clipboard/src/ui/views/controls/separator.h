// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SEPARATOR_H_
#define UI_VIEWS_CONTROLS_SEPARATOR_H_

#include <string>

#include "ui/views/view.h"

namespace views {

// The Separator class is a view that shows a line used to visually separate
// other views.  The current implementation is only horizontal.

class VIEWS_EXPORT Separator : public View {
 public:
  // The separator's class name.
  static const char kViewClassName[];

  Separator();
  virtual ~Separator();

  // Overridden from View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(Separator);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_SEPARATOR_H_
