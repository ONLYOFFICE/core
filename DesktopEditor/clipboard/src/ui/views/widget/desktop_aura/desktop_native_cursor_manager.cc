// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_native_cursor_manager.h"

#include "ui/aura/root_window.h"
#include "ui/base/cursor/cursor_loader.h"
#include "ui/views/widget/desktop_aura/desktop_cursor_loader_updater.h"

namespace views {

DesktopNativeCursorManager::DesktopNativeCursorManager(
    aura::RootWindow* window,
    scoped_ptr<DesktopCursorLoaderUpdater> cursor_loader_updater)
    : root_window_(window),
      cursor_loader_updater_(cursor_loader_updater.Pass()),
      cursor_loader_(ui::CursorLoader::Create()) {
  if (cursor_loader_updater_.get())
    cursor_loader_updater_->OnCreate(root_window_, cursor_loader_.get());
}

DesktopNativeCursorManager::~DesktopNativeCursorManager() {
}

void DesktopNativeCursorManager::SetDisplay(
    const gfx::Display& display,
    views::corewm::NativeCursorManagerDelegate* delegate) {
  cursor_loader_->UnloadAll();
  cursor_loader_->set_display(display);

  if (cursor_loader_updater_.get())
    cursor_loader_updater_->OnDisplayUpdated(display, cursor_loader_.get());

  SetCursor(delegate->GetCurrentCursor(), delegate);
}

void DesktopNativeCursorManager::SetCursor(
    gfx::NativeCursor cursor,
    views::corewm::NativeCursorManagerDelegate* delegate) {
  gfx::NativeCursor new_cursor = cursor;
  cursor_loader_->SetPlatformCursor(&new_cursor);
  delegate->CommitCursor(new_cursor);

  if (delegate->GetCurrentVisibility())
    root_window_->SetCursor(new_cursor);
}

void DesktopNativeCursorManager::SetVisibility(
    bool visible,
    views::corewm::NativeCursorManagerDelegate* delegate) {
  delegate->CommitVisibility(visible);

  if (visible) {
    SetCursor(delegate->GetCurrentCursor(), delegate);
  } else {
    gfx::NativeCursor invisible_cursor(ui::kCursorNone);
    cursor_loader_->SetPlatformCursor(&invisible_cursor);
    root_window_->SetCursor(invisible_cursor);
  }

  root_window_->OnCursorVisibilityChanged(visible);
}

void DesktopNativeCursorManager::SetMouseEventsEnabled(
    bool enabled,
    views::corewm::NativeCursorManagerDelegate* delegate) {
  delegate->CommitMouseEventsEnabled(enabled);

  // TODO(erg): In the ash version, we set the last mouse location on Env. I'm
  // not sure this concept makes sense on the desktop.

  SetVisibility(delegate->GetCurrentVisibility(), delegate);

  root_window_->OnMouseEventsEnableStateChanged(enabled);
}

void DesktopNativeCursorManager::SetCursorResourceModule(
    const string16& module_name) {
  cursor_loader_->SetCursorResourceModule(module_name);
}

}  // namespace views
