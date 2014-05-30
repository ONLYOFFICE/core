// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_screen_position_client.h"

#include "ui/aura/root_window.h"
#include "ui/gfx/display.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/screen.h"

#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

namespace {

gfx::Point GetOrigin(const aura::RootWindow* root_window) {
  gfx::Point origin_in_pixels = root_window->GetHostOrigin();
  aura::RootWindow* window = const_cast<aura::RootWindow*>(root_window);
  float scale = gfx::Screen::GetScreenFor(window)->
       GetDisplayNearestWindow(window).device_scale_factor();
  return gfx::ToFlooredPoint(
      gfx::ScalePoint(origin_in_pixels, 1 / scale));
}

}  // namespace

namespace views {

DesktopScreenPositionClient::DesktopScreenPositionClient() {
}

DesktopScreenPositionClient::~DesktopScreenPositionClient() {
}

void DesktopScreenPositionClient::ConvertPointToScreen(
    const aura::Window* window, gfx::Point* point) {
  const aura::RootWindow* root_window = window->GetRootWindow();
  aura::Window::ConvertPointToTarget(window, root_window, point);
  gfx::Point origin = GetOrigin(root_window);
  point->Offset(origin.x(), origin.y());
}

void DesktopScreenPositionClient::ConvertPointFromScreen(
    const aura::Window* window, gfx::Point* point) {
  const aura::RootWindow* root_window = window->GetRootWindow();
  gfx::Point origin = GetOrigin(root_window);
  point->Offset(-origin.x(), -origin.y());
  aura::Window::ConvertPointToTarget(root_window, window, point);
}

void DesktopScreenPositionClient::ConvertHostPointToScreen(
    aura::RootWindow* window, gfx::Point* point) {
  ConvertPointToScreen(window, point);
}

void DesktopScreenPositionClient::SetBounds(
    aura::Window* window,
    const gfx::Rect& bounds,
    const gfx::Display& display) {
  // TODO: Use the 3rd parameter, |display|.
  gfx::Point origin = bounds.origin();
  aura::RootWindow* root = window->GetRootWindow();
  aura::Window::ConvertPointToTarget(window->parent(), root, &origin);

  if  (window->type() == aura::client::WINDOW_TYPE_CONTROL) {
    window->SetBounds(gfx::Rect(origin, bounds.size()));
    return;
  } else if (window->type() == aura::client::WINDOW_TYPE_POPUP ||
             window->type() == aura::client::WINDOW_TYPE_TOOLTIP) {
    // The caller expects windows we consider "embedded" to be placed in the
    // screen coordinate system. So we need to offset the root window's
    // position (which is in screen coordinates) from these bounds.
    gfx::Point host_origin = GetOrigin(root);
    origin.Offset(-host_origin.x(), -host_origin.y());
    window->SetBounds(gfx::Rect(origin, bounds.size()));
    return;
  }
  DesktopNativeWidgetAura* desktop_native_widget =
      DesktopNativeWidgetAura::ForWindow(window);
  if (desktop_native_widget) {
    root->SetHostBounds(bounds);
    // Setting bounds of root resizes |window|.
  } else {
    window->SetBounds(bounds);
  }
}

}  // namespace views
