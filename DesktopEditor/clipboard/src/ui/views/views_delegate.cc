// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/views_delegate.h"

#include "ui/views/touchui/touch_selection_controller_impl.h"

namespace views {

ViewsDelegate::ViewsDelegate()
    : views_tsc_factory_(new views::ViewsTouchSelectionControllerFactory) {
  ui::TouchSelectionControllerFactory::SetInstance(views_tsc_factory_.get());
}

ViewsDelegate::~ViewsDelegate() {
  ui::TouchSelectionControllerFactory::SetInstance(NULL);
}

}  // namespace views
