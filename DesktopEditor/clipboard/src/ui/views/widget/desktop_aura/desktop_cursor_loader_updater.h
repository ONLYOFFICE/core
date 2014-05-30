// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CURSOR_LOADER_UPDATER_H_

namespace aura {
class RootWindow;
}

namespace gfx {
class Display;
}

namespace ui {
class CursorLoader;
}

namespace views {

// An interface to optionally update the state of a cursor loader. Only used on
// desktop AuraX11.
class DesktopCursorLoaderUpdater {
 public:
  virtual ~DesktopCursorLoaderUpdater() {}

  // Called when a CursorLoader is created.
  virtual void OnCreate(aura::RootWindow* window,
                        ui::CursorLoader* loader) = 0;

  // Called when the display has changed (as we may need to reload the cursor
  // assets in response to a device scale factor or rotation change).
  virtual void OnDisplayUpdated(const gfx::Display& display,
                                ui::CursorLoader* loader) = 0;
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DISPLAY_CHANGE_HANDLER_H_
