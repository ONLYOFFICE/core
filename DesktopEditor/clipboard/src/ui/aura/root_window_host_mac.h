// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_HOST_MAC_H_
#define UI_AURA_ROOT_WINDOW_HOST_MAC_H_

#include "base/basictypes.h"
#include "base/event_types.h"

namespace aura {

// An interface establishing event dispatch from the Mac native window and the
// Aura host.
class RootWindowHostMacDelegate {
 public:
  RootWindowHostMacDelegate();
  virtual ~RootWindowHostMacDelegate();

  // Route events from platform code to the RootWindowHost.
  virtual void SendEvent(const base::NativeEvent& native_event) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(RootWindowHostMacDelegate);
};

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_HOST_MAC_H_
