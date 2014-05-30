// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/window.h"
#include "ui/base/touch/touch_device.h"
#include "ui/base/touch/touch_factory_x11.h"

namespace ui {

bool IsTouchDevicePresent() {
  return ui::TouchFactory::GetInstance()->IsTouchDevicePresent();
}

}  // namespace ui
