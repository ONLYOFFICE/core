// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/client/user_action_client.h"

#include "ui/aura/root_window.h"
#include "ui/aura/window_property.h"

namespace aura {
namespace client {

DEFINE_WINDOW_PROPERTY_KEY(UserActionClient*,
                           kRootWindowUserActionClientKey,
                           NULL);

void SetUserActionClient(RootWindow* root_window, UserActionClient* client) {
  root_window->SetProperty(kRootWindowUserActionClientKey, client);
}

UserActionClient* GetUserActionClient(RootWindow* root_window) {
  return root_window ?
      root_window->GetProperty(kRootWindowUserActionClientKey) : NULL;
}

}  // namespace client
}  // namespace aura
