// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/focus_manager.h"

#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/focus_change_observer.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window_delegate.h"

namespace aura {

////////////////////////////////////////////////////////////////////////////////
// FocusManager, public:

FocusManager::FocusManager() : focused_window_(NULL) {
}

FocusManager::~FocusManager() {
}

////////////////////////////////////////////////////////////////////////////////
// FocusManager, client::FocusClient implementation:

void FocusManager::AddObserver(client::FocusChangeObserver* observer) {
  observers_.AddObserver(observer);
}

void FocusManager::RemoveObserver(client::FocusChangeObserver* observer) {
  observers_.RemoveObserver(observer);
}

void FocusManager::FocusWindow(Window* focused_window) {
  if (focused_window == focused_window_)
    return;
  if (focused_window && !focused_window->CanFocus())
    return;
  // The NULL-check of |focused_window| is essential here before asking the
  // activation client, since it is valid to clear the focus by calling
  // SetFocusedWindow() to NULL.

  if (focused_window) {
    RootWindow* root = focused_window->GetRootWindow();
    DCHECK(root);
    if (client::GetActivationClient(root) &&
        !client::GetActivationClient(root)->OnWillFocusWindow(
            focused_window, NULL)) {
      return;
    }
  }

  Window* old_focused_window = focused_window_;
  focused_window_ = focused_window;

  FOR_EACH_OBSERVER(client::FocusChangeObserver, observers_,
                    OnWindowFocused(focused_window, old_focused_window));
  client::FocusChangeObserver* observer =
      client::GetFocusChangeObserver(old_focused_window);
  if (observer)
    observer->OnWindowFocused(focused_window_, old_focused_window);
  observer = client::GetFocusChangeObserver(focused_window_);
  if (observer)
    observer->OnWindowFocused(focused_window_, old_focused_window);
}

void FocusManager::ResetFocusWithinActiveWindow(Window* window) {
  FocusWindow(window);
}

Window* FocusManager::GetFocusedWindow() {
  return focused_window_;
}

void FocusManager::OnWindowHiddenInRootWindow(
    aura::Window* window,
    aura::RootWindow* root_window,
    bool destroyed) {
  Window* focused_window =
      client::GetFocusClient(root_window)->GetFocusedWindow();
  if (window->Contains(focused_window)) {
    Window* focus_to = window->transient_parent();
    if (focus_to) {
      // Has to be removed from the transient parent before focusing,
      // otherwise |window| will be focused again.
      if (destroyed)
        focus_to->RemoveTransientChild(window);
    } else {
      // If the invisible view has no visible transient window, focus to the
      // topmost visible parent window.
      focus_to = window->parent();
    }
    if (focus_to &&
        (!focus_to->IsVisible() ||
          !focus_to->CanFocus() ||
          (client::GetActivationClient(root_window) &&
          !client::GetActivationClient(root_window)->OnWillFocusWindow(
              focus_to, NULL)))) {
      focus_to = NULL;
    }
    client::GetFocusClient(root_window)->FocusWindow(focus_to);
  }
}

}  // namespace aura
