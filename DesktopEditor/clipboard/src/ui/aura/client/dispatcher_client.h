// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_DISPATCHER_CLIENT_H_
#define UI_AURA_CLIENT_DISPATCHER_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"
#include "base/message_loop.h"

namespace aura {
class RootWindow;
namespace client {

// An interface implemented by an object which handles nested dispatchers.
class AURA_EXPORT DispatcherClient {
 public:
  virtual void RunWithDispatcher(base::MessageLoop::Dispatcher* dispatcher,
                                 aura::Window* associated_window,
                                 bool nestable_tasks_allowed) = 0;
};

AURA_EXPORT void SetDispatcherClient(RootWindow* root_window,
                                     DispatcherClient* client);
AURA_EXPORT DispatcherClient* GetDispatcherClient(RootWindow* root_window);

}  // namespace client
}  // namespace aura

#endif  // UI_AURA_CLIENT_DISPATCHER_CLIENT_H_
