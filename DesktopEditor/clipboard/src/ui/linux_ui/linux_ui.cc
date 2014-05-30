// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/linux_ui/linux_ui.h"

#include "ui/shell_dialogs/linux_shell_dialog.h"

namespace {

ui::LinuxUI* g_linux_ui = NULL;

}  // namespace

namespace ui {

void LinuxUI::SetInstance(LinuxUI* instance) {
  delete g_linux_ui;
  g_linux_ui = instance;

  LinuxShellDialog::SetInstance(instance);
}

const LinuxUI* LinuxUI::instance() {
  return g_linux_ui;
}

}  // namespace ui
