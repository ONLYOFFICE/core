// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop.h"

namespace aura {

class DispatcherWin : public base::MessageLoop::Dispatcher {
 public:
  DispatcherWin() {}
  virtual ~DispatcherWin() {}

  // Overridden from MessageLoop::Dispatcher:
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DispatcherWin);
};

bool DispatcherWin::Dispatch(const base::NativeEvent& msg) {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  return true;
}

MessageLoop::Dispatcher* CreateDispatcher() {
  return new DispatcherWin;
}

}  // namespace aura
