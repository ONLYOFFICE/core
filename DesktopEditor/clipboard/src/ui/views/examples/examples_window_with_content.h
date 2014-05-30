// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_WITH_CONTENT_H_
#define UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_WITH_CONTENT_H_

#include "ui/views/examples/views_examples_with_content_export.h"

namespace content {
class BrowserContext;
}

namespace views {
namespace examples {

enum Operation {
  DO_NOTHING_ON_CLOSE = 0,
  QUIT_ON_CLOSE,
};

// Shows a window with the views examples in it.
VIEWS_EXAMPLES_WITH_CONTENT_EXPORT void ShowExamplesWindowWithContent(
    Operation operation,
    content::BrowserContext* browser_context);

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_EXAMPLES_WINDOW_WITH_CONTENT_H_
