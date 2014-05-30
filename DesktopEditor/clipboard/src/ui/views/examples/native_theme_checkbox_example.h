// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_NATIVE_THEME_CHECKBOX_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_NATIVE_THEME_CHECKBOX_EXAMPLE_H_

#include "base/basictypes.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/examples/example_base.h"

namespace views {
class Checkbox;

namespace examples {

// NativeThemeCheckboxExample exercises a Checkbox control.
class NativeThemeCheckboxExample : public ExampleBase, public ButtonListener {
 public:
  NativeThemeCheckboxExample();
  virtual ~NativeThemeCheckboxExample();

  // Overridden from ExampleBase:
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  // Overridden from ButtonListener:
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  // The only control in this test.
  Checkbox* button_;

  int count_;

  DISALLOW_COPY_AND_ASSIGN(NativeThemeCheckboxExample);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_NATIVE_THEME_CHECKBOX_EXAMPLE_H_
