// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_focus_rules.h"

#include "ui/aura/root_window.h"
#include "ui/aura/window.h"

namespace views {

DesktopFocusRules::DesktopFocusRules() {}
DesktopFocusRules::~DesktopFocusRules() {}

bool DesktopFocusRules::SupportsChildActivation(aura::Window* window) const {
  // In Desktop-Aura, only children of the RootWindow are activatable.
  return window->GetRootWindow() == window;
}

}  // namespace views
