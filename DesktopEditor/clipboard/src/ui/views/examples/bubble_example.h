// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_BUBBLE_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_BUBBLE_EXAMPLE_H_

#include "ui/views/controls/button/button.h"
#include "ui/views/examples/example_base.h"

namespace views {
namespace examples {

// A Bubble example.
class BubbleExample : public ExampleBase, public ButtonListener {
 public:
  BubbleExample();
  virtual ~BubbleExample();

  // Overridden from ExampleBase.
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  Button* shadow_;
  Button* no_shadow_;
  Button* big_shadow_;
  Button* small_shadow_;
  Button* align_to_edge_;
  Button* persistent_;
  Button* fade_in_;

  DISALLOW_COPY_AND_ASSIGN(BubbleExample);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_BUBBLE_EXAMPLE_H_
