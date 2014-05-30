// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ui_base_switches_util.h"

#include "base/command_line.h"
#include "ui/base/ui_base_switches.h"

namespace switches {

bool IsTouchDragDropEnabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableTouchDragDrop);
}

bool IsTouchEditingEnabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableTouchEditing);
}

}  // namespace switches
