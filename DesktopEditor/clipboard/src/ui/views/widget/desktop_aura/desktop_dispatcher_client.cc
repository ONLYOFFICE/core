// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_dispatcher_client.h"

#include "base/run_loop.h"

namespace views {

DesktopDispatcherClient::DesktopDispatcherClient() {}

DesktopDispatcherClient::~DesktopDispatcherClient() {}

void DesktopDispatcherClient::RunWithDispatcher(
    base::MessageLoop::Dispatcher* nested_dispatcher,
    aura::Window* associated_window,
    bool nestable_tasks_allowed) {
  // TODO(erg): This class has been copypastad from
  // ash/accelerators/nested_dispatcher_controller.cc. I have left my changes
  // commented out because I don't entirely understand the implications of the
  // change.
  base::MessageLoopForUI* loop = base::MessageLoopForUI::current();
  bool did_allow_task_nesting = loop->NestableTasksAllowed();
  loop->SetNestableTasksAllowed(nestable_tasks_allowed);

  // DefaultAcceleratorDispatcher dispatcher(nested_dispatcher,
  //                                         associated_window);
  base::RunLoop run_loop(nested_dispatcher);
  run_loop.Run();
  loop->SetNestableTasksAllowed(did_allow_task_nesting);
}

}  // namespace views
