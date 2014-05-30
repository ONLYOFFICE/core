// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/touch/touch_device.h"
#include "base/win/windows_version.h"
#include <windows.h>

namespace ui {

bool IsTouchDevicePresent() {
  int value = GetSystemMetrics(SM_DIGITIZER);
  return (value & (NID_READY | NID_INTEGRATED_TOUCH)) ==
             (NID_READY | NID_INTEGRATED_TOUCH);
}

}  // namespace ui
