// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_ANIMATION_HOST_H_
#define UI_AURA_CLIENT_ANIMATION_HOST_H_

#include "base/compiler_specific.h"
#include "ui/aura/aura_export.h"

namespace gfx {
class Rect;
}

namespace aura {
class Window;
namespace client {

// Interface for top level window host of animation. Communicates additional
// bounds required for animation as well as animation completion for deferring
// window closes on hide.
class AURA_EXPORT AnimationHost {
 public:
  // Ensure the host window is at least this large so that transitions have
  // sufficient space.
  virtual void SetHostTransitionBounds(const gfx::Rect& bounds) = 0;

  // Called after the window has faded out on a hide.
  virtual void OnWindowHidingAnimationCompleted() = 0;

 protected:
  virtual ~AnimationHost() {}
};

AURA_EXPORT void SetAnimationHost(Window* window,
                                  AnimationHost* animation_host);
AURA_EXPORT AnimationHost* GetAnimationHost(Window* window);

}  // namespace client
}  // namespace aura

#endif  // UI_AURA_CLIENT_ANIMATION_HOST_H_
