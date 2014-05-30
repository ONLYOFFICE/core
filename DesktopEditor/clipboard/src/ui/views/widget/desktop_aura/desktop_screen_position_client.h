// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_SCREEN_POSITION_CLIENT_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_SCREEN_POSITION_CLIENT_H_

#include "ui/aura/client/screen_position_client.h"
#include "ui/views/views_export.h"

namespace views {

// Client that always offsets by the toplevel RootWindow of the passed
// in child NativeWidgetAura.
class VIEWS_EXPORT DesktopScreenPositionClient
    : public aura::client::ScreenPositionClient {
 public:
  DesktopScreenPositionClient();
  virtual ~DesktopScreenPositionClient();

  // aura::client::ScreenPositionClient overrides:
  virtual void ConvertPointToScreen(const aura::Window* window,
                                    gfx::Point* point) OVERRIDE;
  virtual void ConvertPointFromScreen(const aura::Window* window,
                                      gfx::Point* point) OVERRIDE;
  virtual void ConvertHostPointToScreen(aura::RootWindow* window,
                                        gfx::Point* point) OVERRIDE;
  virtual void SetBounds(aura::Window* window,
                         const gfx::Rect& bounds,
                         const gfx::Display& display) OVERRIDE;
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_SCREEN_POSITION_CLIENT_H_
