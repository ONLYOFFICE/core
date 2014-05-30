// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_NATIVE_CURSOR_MANAGER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_NATIVE_CURSOR_MANAGER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/corewm/native_cursor_manager.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
}

namespace ui {
class CursorLoader;
}

namespace views {
class DesktopCursorLoaderUpdater;

namespace corewm {
class NativeCursorManagerDelegate;
}

// A NativeCursorManager that interacts with only one RootWindow. (Unlike the
// one in ash, which interacts with all the RootWindows that ash knows about.)
class VIEWS_EXPORT DesktopNativeCursorManager
    : public views::corewm::NativeCursorManager {
 public:
  DesktopNativeCursorManager(
      aura::RootWindow* window,
      scoped_ptr<DesktopCursorLoaderUpdater> cursor_loader_updater);
  virtual ~DesktopNativeCursorManager();

 private:
  // Overridden from views::corewm::NativeCursorManager:
  virtual void SetDisplay(
      const gfx::Display& display,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetCursor(
      gfx::NativeCursor cursor,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetVisibility(
      bool visible,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetMouseEventsEnabled(
      bool enabled,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetCursorResourceModule(const string16& module_name) OVERRIDE;

  aura::RootWindow* root_window_;
  scoped_ptr<DesktopCursorLoaderUpdater> cursor_loader_updater_;
  scoped_ptr<ui::CursorLoader> cursor_loader_;

  DISALLOW_COPY_AND_ASSIGN(DesktopNativeCursorManager);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_NATIVE_CURSOR_MANAGER_H_

