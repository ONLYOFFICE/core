// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/corewm_switches.h"

#include "base/command_line.h"

namespace views {
namespace corewm {
namespace switches {

// When set uses the old ActivationController/FocusManager instead of the new
// CoreWM FocusController.
const char kDisableFocusController[] = "disable-focus-controller";

// When set uses the FocusController in desktop mode.
const char kDisableFocusControllerOnDesktop[] =
    "disable-focus-controller-on-desktop";

const char kNoDropShadows[] = "aura-no-shadows";

// If present animations are disabled.
const char kWindowAnimationsDisabled[] =
    "views-corewm-window-animations-disabled";

}  // namespace switches

bool UseFocusController() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableFocusController);
}

bool UseFocusControllerOnDesktop() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableFocusControllerOnDesktop);
}

}  // namespace corewm
}  // namespace views
