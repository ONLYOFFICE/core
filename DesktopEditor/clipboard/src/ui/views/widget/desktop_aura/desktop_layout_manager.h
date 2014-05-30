// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_LAYOUT_MANAGER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_LAYOUT_MANAGER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/layout_manager.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
class Window;
}

namespace views {

// A LayoutManager that by default makes the first Window added to a
// RootWindow the full size, and will be resized as the RootWindow is resized.
class VIEWS_EXPORT DesktopLayoutManager : public aura::LayoutManager {
 public:
  explicit DesktopLayoutManager(aura::RootWindow* root_window);
  virtual ~DesktopLayoutManager();

  // Overridden from aura::LayoutManager:
  virtual void OnWindowResized() OVERRIDE;
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE;
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE;
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visible) OVERRIDE;
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE;

 private:
  // Sets the size of |main_window_| to the internal bounds of |root_window_|.
  void SetMainWindowSize();

  aura::RootWindow* root_window_;
  aura::Window* main_window_;

  DISALLOW_COPY_AND_ASSIGN(DesktopLayoutManager);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_LAYOUT_MANAGER_H_
