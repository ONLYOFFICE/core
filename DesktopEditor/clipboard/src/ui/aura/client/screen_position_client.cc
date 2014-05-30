// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/client/screen_position_client.h"

#include "ui/aura/root_window.h"
#include "ui/aura/window_property.h"

DECLARE_WINDOW_PROPERTY_TYPE(aura::client::ScreenPositionClient*)

namespace aura {
namespace client {

DEFINE_LOCAL_WINDOW_PROPERTY_KEY(ScreenPositionClient*,
                                 kScreenPositionClientKey,
                                 NULL);

void SetScreenPositionClient(RootWindow* window,
                             ScreenPositionClient* client) {
  window->SetProperty(kScreenPositionClientKey, client);
}

ScreenPositionClient* GetScreenPositionClient(const RootWindow* window) {
  return window ? window->GetProperty(kScreenPositionClientKey) : NULL;
}

}  // namespace client
}  // namespace aura
