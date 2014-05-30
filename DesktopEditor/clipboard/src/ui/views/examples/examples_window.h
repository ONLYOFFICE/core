// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_
#define UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_

#include "ui/views/examples/views_examples_export.h"

namespace views {
namespace examples {

enum Operation {
  DO_NOTHING_ON_CLOSE = 0,
  QUIT_ON_CLOSE,
};

// Shows a window with the views examples in it.
VIEWS_EXAMPLES_EXPORT void ShowExamplesWindow(Operation operation);

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_H_
