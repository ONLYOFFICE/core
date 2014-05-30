// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/native_theme_checkbox_example.h"

#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/radio_button.h"
#include "ui/views/layout/fill_layout.h"

namespace views {
namespace examples {

NativeThemeCheckboxExample::NativeThemeCheckboxExample()
    : ExampleBase("Native Theme Checkbox"),
      count_(0) {
}

NativeThemeCheckboxExample::~NativeThemeCheckboxExample() {
}

void NativeThemeCheckboxExample::CreateExampleView(View* container) {
  button_ = new Checkbox(ASCIIToUTF16("Checkbox"));
  button_->set_listener(this);
  container->SetLayoutManager(new FillLayout);
  container->AddChildView(button_);
}

void NativeThemeCheckboxExample::ButtonPressed(Button* sender,
                                               const ui::Event& event) {
  PrintStatus(base::StringPrintf("Pressed! count: %d", ++count_).c_str());
}

}  // namespace examples
}  // namespace views
