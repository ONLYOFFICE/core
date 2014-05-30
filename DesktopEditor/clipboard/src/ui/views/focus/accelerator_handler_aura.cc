// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/focus/accelerator_handler.h"

namespace views {

AcceleratorHandler::AcceleratorHandler() {
}

bool AcceleratorHandler::Dispatch(const base::NativeEvent& event) {
#if defined(OS_WIN)
  TranslateMessage(&event);
  DispatchMessage(&event);
#endif  // defined(OS_WIN)
  return true;
}

#if defined(USE_X11)
bool DispatchXEvent(XEvent* xev) {
  return false;
}
#endif  // defined(USE_X11)

}  // namespace views
