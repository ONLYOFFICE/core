// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/metrics.h"

#include <windows.h>

namespace views {

int GetDoubleClickInterval() {
  return ::GetDoubleClickTime();
}

int GetMenuShowDelay() {
  static DWORD delay = 0;
  if (!delay && !SystemParametersInfo(SPI_GETMENUSHOWDELAY, 0, &delay, 0))
    delay = kDefaultMenuShowDelay;
  return delay;
}

}  // namespace views
