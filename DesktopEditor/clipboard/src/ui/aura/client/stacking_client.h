// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_STACKING_CLIENT_H_
#define UI_AURA_CLIENT_STACKING_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace gfx {
class Rect;
}

namespace aura {
class Window;
namespace client {

// An interface implemented by an object that stacks windows.
class AURA_EXPORT StackingClient {
 public:
  virtual ~StackingClient() {}

  // Called by the Window when it looks for a default parent. Returns the
  // window that |window| should be added to instead. |context| provides a
  // Window (generally a RootWindow) that can be used to determine which
  // desktop type the default parent should be chosen from.  NOTE: this may
  // have side effects. It should only be used when |window| is going to be
  // immediately added.
  //
  // TODO(erg): Remove |context|, and maybe after oshima's patch lands,
  // |bounds|.
  virtual Window* GetDefaultParent(
      Window* context,
      Window* window,
      const gfx::Rect& bounds) = 0;
};

// Set/Get a stacking client for a specific window. Setting the stacking client
// sets the stacking client on the window's RootWindow, not the window itself.
// Likewise getting obtains it from the window's RootWindow. If |window| is
// non-NULL it must be in a RootWindow. If |window| is NULL, the default
// stacking client is used.
AURA_EXPORT void SetStackingClient(Window* window,
                                   StackingClient* stacking_client);
StackingClient* GetStackingClient(Window* window);

}  // namespace client
}  // namespace aura

#endif  // UI_AURA_CLIENT_STACKING_CLIENT_H_
