// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_COREWM_SWITCHES_H_
#define UI_VIEWS_COREWM_COREWM_SWITCHES_H_

#include "build/build_config.h"
#include "ui/views/views_export.h"

namespace views {
namespace corewm {
namespace switches {

// Note: If you add a switch, consider if it needs to be copied to a subsequent
// command line if the process executes a new copy of itself.  (For example,
// see chromeos::LoginUtil::GetOffTheRecordCommandLine().)

// Please keep alphabetized.
VIEWS_EXPORT extern const char kNoDropShadows[];
VIEWS_EXPORT extern const char kWindowAnimationsDisabled[];

}  // namespace switches

// Returns true if the new FocusController should be used as the
// Activation/FocusClient.
VIEWS_EXPORT bool UseFocusController();

// Same as above but for Desktop mode.
VIEWS_EXPORT bool UseFocusControllerOnDesktop();

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_COREWM_SWITCHES_H_
