// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_USER_ACTION_CLIENT_H_
#define UI_AURA_CLIENT_USER_ACTION_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace aura {
class RootWindow;
namespace client {

// An interface for handling a user action that isn't handled by the standard
// event path.
class AURA_EXPORT UserActionClient {
 public:
  enum Command {
    BACK = 0,
    FORWARD,
  };

  // Returns true if the command was handled and false otherwise.
  virtual bool OnUserAction(Command command) = 0;

  virtual ~UserActionClient() {}
};

// Sets/gets the client for handling user action on the specified root window.
AURA_EXPORT void SetUserActionClient(RootWindow* root_window,
                                     UserActionClient* client);
AURA_EXPORT UserActionClient* GetUserActionClient(RootWindow* root_window);

}  // namespace client
}  // namespace aura

#endif  // UI_AURA_CLIENT_USER_ACTION_CLIENT_H_
