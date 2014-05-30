// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/native_widget_aura_window_observer.h"

#include "ui/aura/client/activation_client.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"

namespace views {

NativeWidgetAuraWindowObserver::NativeWidgetAuraWindowObserver(
    gfx::NativeView native_view,
    internal::NativeWidgetDelegate* delegate)
    : native_view_(native_view),
      delegate_(delegate) {
  native_view_->GetRootWindow()->AddObserver(this);
  native_view_->AddObserver(this);
  aura::client::GetActivationClient(native_view_->GetRootWindow())->
      AddObserver(this);
}

NativeWidgetAuraWindowObserver::~NativeWidgetAuraWindowObserver() {
  CleanUpObservers();
}

void NativeWidgetAuraWindowObserver::OnWindowActivated(
    aura::Window* gained_active,
    aura::Window* lost_active) {
  if (!gained_active || gained_active->transient_parent() != native_view_)
    delegate_->EnableInactiveRendering();
}

void NativeWidgetAuraWindowObserver::OnWindowRemovingFromRootWindow(
    aura::Window* window) {
  if (window != native_view_)
    return;
  CleanUpObservers();
}

void NativeWidgetAuraWindowObserver::CleanUpObservers() {
  if (!native_view_)
    return;
  native_view_->GetRootWindow()->RemoveObserver(this);
  native_view_->RemoveObserver(this);
  aura::client::GetActivationClient(native_view_->GetRootWindow())->
      RemoveObserver(this);
  native_view_ = NULL;
}

}  // namespace views
