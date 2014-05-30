// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/window_destruction_observer.h"

namespace aura {

WindowDestructionObserver::WindowDestructionObserver(Window* window)
    : window_(window), destroyed_(false) {
  DCHECK(window_);
  window_->AddObserver(this);
}

WindowDestructionObserver::~WindowDestructionObserver() {
  if (!destroyed_)
    window_->RemoveObserver(this);
}

void WindowDestructionObserver::OnWindowDestroyed(Window* window) {
  if (window == window_)
    destroyed_ = true;
}

} // namespace aura
