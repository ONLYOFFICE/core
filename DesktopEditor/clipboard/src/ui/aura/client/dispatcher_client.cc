// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/client/dispatcher_client.h"

#include "ui/aura/root_window.h"
#include "ui/aura/window_property.h"

DECLARE_WINDOW_PROPERTY_TYPE(aura::client::DispatcherClient*);

namespace aura {
namespace client {

DEFINE_LOCAL_WINDOW_PROPERTY_KEY(DispatcherClient*, kDispatcherClientKey, NULL);

void SetDispatcherClient(RootWindow* root_window, DispatcherClient* client) {
  root_window->SetProperty(kDispatcherClientKey, client);
}

DispatcherClient* GetDispatcherClient(RootWindow* root_window) {
  return root_window ? root_window->GetProperty(kDispatcherClientKey) : NULL;
}

}  // namespace client
}  // namespace aura
