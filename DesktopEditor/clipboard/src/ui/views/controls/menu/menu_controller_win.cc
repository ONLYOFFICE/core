// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_controller.h"

#include "base/run_loop.h"
#include "ui/gfx/screen.h"

namespace views {

void MenuController::RunMessageLoop(bool nested_menu) {
  base::MessageLoopForUI* loop = base::MessageLoopForUI::current();
  base::MessageLoop::ScopedNestableTaskAllower allow(loop);
  base::RunLoop run_loop(this);
  run_loop.Run();
}

bool MenuController::ShouldQuitNow() const {
  return true;
}

gfx::Screen* MenuController::GetScreen() {
  return gfx::Screen::GetNativeScreen();
}

}  // namespace views
