// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/window_util.h"

#include "ui/aura/client/activation_client.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer.h"

namespace views {
namespace corewm {

void ActivateWindow(aura::Window* window) {
  DCHECK(window);
  DCHECK(window->GetRootWindow());
  aura::client::GetActivationClient(window->GetRootWindow())->ActivateWindow(
      window);
}

void DeactivateWindow(aura::Window* window) {
  DCHECK(window);
  DCHECK(window->GetRootWindow());
  aura::client::GetActivationClient(window->GetRootWindow())->DeactivateWindow(
      window);
}

bool IsActiveWindow(aura::Window* window) {
  DCHECK(window);
  if (!window->GetRootWindow())
    return false;
  aura::client::ActivationClient* client =
      aura::client::GetActivationClient(window->GetRootWindow());
  return client && client->GetActiveWindow() == window;
}

bool CanActivateWindow(aura::Window* window) {
  DCHECK(window);
  if (!window->GetRootWindow())
    return false;
  aura::client::ActivationClient* client =
      aura::client::GetActivationClient(window->GetRootWindow());
  return client && client->CanActivateWindow(window);
}

aura::Window* GetActivatableWindow(aura::Window* window) {
  aura::client::ActivationClient* client =
      aura::client::GetActivationClient(window->GetRootWindow());
  return client ? client->GetActivatableWindow(window) : NULL;
}

aura::Window* GetToplevelWindow(aura::Window* window) {
  aura::client::ActivationClient* client =
      aura::client::GetActivationClient(window->GetRootWindow());
  return client ? client->GetToplevelWindow(window) : NULL;
}

void DeepDeleteLayers(ui::Layer* layer) {
  std::vector<ui::Layer*> children = layer->children();
  for (std::vector<ui::Layer*>::const_iterator it = children.begin();
       it != children.end();
       ++it) {
    ui::Layer* child = *it;
    DeepDeleteLayers(child);
  }
  delete layer;
}

ui::Layer* RecreateWindowLayers(aura::Window* window, bool set_bounds) {
  const gfx::Rect bounds = window->bounds();
  ui::Layer* old_layer = window->RecreateLayer();
  DCHECK(old_layer);
  for (aura::Window::Windows::const_iterator it = window->children().begin();
       it != window->children().end();
       ++it) {
    // Maintain the hierarchy of the detached layers.
    old_layer->Add(RecreateWindowLayers(*it, set_bounds));
  }
  if (set_bounds)
    window->SetBounds(bounds);
  return old_layer;
}

}  // namespace corewm
}  // namespace views
