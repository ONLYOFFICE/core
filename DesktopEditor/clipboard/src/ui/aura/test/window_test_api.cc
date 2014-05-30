// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/window_test_api.h"

#include "ui/aura/window.h"

namespace aura {
namespace test {

WindowTestApi::WindowTestApi(Window* window) : window_(window) {
}

bool WindowTestApi::OwnsLayer() const {
  return window_->layer_owner_.get() != NULL;
}

bool WindowTestApi::ContainsMouse() const {
  return window_->ContainsMouse();
}

}  // namespace test
}  // namespace aura
