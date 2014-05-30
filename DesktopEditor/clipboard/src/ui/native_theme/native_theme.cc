// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme.h"

#include "base/command_line.h"
#include "base/metrics/field_trial.h"


// Constants for the new menu style field trial.
const char kMenuVariationFieldTrialName[] = "NewMenuStyle";
const char kMenuVariationFieldTrialGroupNameCompact1[] = "Compact1";
const char kMenuVariationFieldTrialGroupNameCompact2[] = "Compact2";
const char kMenuVariationFieldTrialGroupNameHigherContrast[] = "HigherContrast";

namespace ui {

void NativeTheme::SetScrollbarColors(unsigned inactive_color,
                                     unsigned active_color,
                                     unsigned track_color) {
  thumb_inactive_color_ = inactive_color;
  thumb_active_color_ = active_color;
  track_color_ = track_color;
}

// NativeTheme::instance() is implemented in the platform specific source files,
// such as native_theme_win.cc or native_theme_linux.cc

// static
NativeTheme::MenuVariation NativeTheme::GetMenuVariation() {
  std::string trial_group_name =
      base::FieldTrialList::FindFullName(kMenuVariationFieldTrialName);
  if (trial_group_name == kMenuVariationFieldTrialGroupNameCompact1)
    return MENU_VARIATION_COMPACT_1;
  if (trial_group_name == kMenuVariationFieldTrialGroupNameCompact2)
    return MENU_VARIATION_COMPACT_2;
  if (trial_group_name == kMenuVariationFieldTrialGroupNameHigherContrast)
    return MENU_VARIATION_CONTRAST;

  return MENU_VARIATION_NORMAL;
}

NativeTheme::NativeTheme()
    : thumb_inactive_color_(0xeaeaea),
      thumb_active_color_(0xf4f4f4),
      track_color_(0xd3d3d3) {
}

NativeTheme::~NativeTheme() {}

}  // namespace ui
