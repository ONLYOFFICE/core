// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_layout_manager.h"

#include "ui/aura/root_window.h"
#include "ui/gfx/rect.h"

namespace views {

DesktopLayoutManager::DesktopLayoutManager(aura::RootWindow* root_window)
    : root_window_(root_window),
      main_window_(NULL) {
}

DesktopLayoutManager::~DesktopLayoutManager() {}

void DesktopLayoutManager::OnWindowResized() {
  if (main_window_)
    SetMainWindowSize();
}

void DesktopLayoutManager::OnWindowAddedToLayout(aura::Window* child) {
  if (!main_window_) {
    main_window_ = child;

    SetMainWindowSize();
  }
}

void DesktopLayoutManager::OnWillRemoveWindowFromLayout(aura::Window* child) {
  if (main_window_ == child)
    main_window_ = NULL;
}

void DesktopLayoutManager::OnWindowRemovedFromLayout(aura::Window* child) {
}

void DesktopLayoutManager::OnChildWindowVisibilityChanged(aura::Window* child,
                                                          bool visible) {
}

void DesktopLayoutManager::SetChildBounds(aura::Window* child,
                                          const gfx::Rect& requested_bounds) {
  if (main_window_ != child)
    SetChildBoundsDirect(child, requested_bounds);
}

void DesktopLayoutManager::SetMainWindowSize() {
  gfx::Rect bounds;
  bounds.set_size(root_window_->GetHostSize());
  SetChildBoundsDirect(main_window_, bounds);
}

}  // namespace views
