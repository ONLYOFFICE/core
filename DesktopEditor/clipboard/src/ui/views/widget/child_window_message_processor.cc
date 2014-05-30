// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/child_window_message_processor.h"

#include "base/logging.h"
#include "ui/base/view_prop.h"

namespace views {

static const char* const kChildWindowKey = "__CHILD_WINDOW_MESSAGE_PROCESSOR__";

// static
ui::ViewProp* ChildWindowMessageProcessor::Register(
    HWND hwnd,
    ChildWindowMessageProcessor* processor) {
  DCHECK(processor);
  return new ui::ViewProp(hwnd, kChildWindowKey, processor);
}

// static
ChildWindowMessageProcessor* ChildWindowMessageProcessor::Get(HWND hwnd) {
  return reinterpret_cast<ChildWindowMessageProcessor*>(
    ui::ViewProp::GetValue(hwnd, kChildWindowKey));
}

}  // namespace
