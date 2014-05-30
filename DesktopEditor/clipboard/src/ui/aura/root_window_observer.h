// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_OBSERVER_H_
#define UI_AURA_ROOT_WINDOW_OBSERVER_H_

#include "ui/aura/aura_export.h"

namespace gfx {
class Point;
class Size;
}

namespace aura {
class RootWindow;
class Window;

class AURA_EXPORT RootWindowObserver {
 public:
  // Invoked after the RootWindow is resized.
  virtual void OnRootWindowResized(const RootWindow* root,
                                   const gfx::Size& old_size) {}

  // Invoked after the RootWindow has been moved on screen.
  virtual void OnRootWindowMoved(const RootWindow* root,
                                 const gfx::Point& new_origin) {}

  // Invoked when the native windowing system sends us a request to close our
  // window.
  virtual void OnRootWindowHostCloseRequested(const RootWindow* root) {}

  // Invoked when the keyboard mapping has changed.
  virtual void OnKeyboardMappingChanged(const RootWindow* root) {}

 protected:
  virtual ~RootWindowObserver() {}
};

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_OBSERVER_H_
