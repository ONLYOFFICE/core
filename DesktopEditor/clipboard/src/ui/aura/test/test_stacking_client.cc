// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/test_stacking_client.h"

#include "ui/aura/root_window.h"

namespace aura {
namespace test {

TestStackingClient::TestStackingClient(RootWindow* root_window)
    : root_window_(root_window) {
  client::SetStackingClient(root_window_, this);
}

TestStackingClient::~TestStackingClient() {
  client::SetStackingClient(root_window_, NULL);
}

Window* TestStackingClient::GetDefaultParent(Window* context,
                                             Window* window,
                                             const gfx::Rect& bounds) {
  return root_window_;
}

}  // namespace test
}  // namespace aura
