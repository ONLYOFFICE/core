// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_AURA_WINDOW_OBSERVER_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_AURA_WINDOW_OBSERVER_H_

#include "base/compiler_specific.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/widget/native_widget_private.h"

namespace views {

// A helper class shared between NativeWidgetAura implementations.  Used when
// SetInactiveRenderingDisabled() is invoked to track when active status
// changes in such a way that we should enable inactive rendering.
class NativeWidgetAuraWindowObserver
    : public aura::WindowObserver,
      public aura::client::ActivationChangeObserver {
 public:
  explicit NativeWidgetAuraWindowObserver(
      gfx::NativeView native_view,
      internal::NativeWidgetDelegate* delegate);
  virtual ~NativeWidgetAuraWindowObserver();

  // Overridden from aura::client::ActivationChangeObserver:
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  // Overridden from aura::WindowObserver:
  virtual void OnWindowRemovingFromRootWindow(aura::Window* window) OVERRIDE;

 private:
  void CleanUpObservers();

  gfx::NativeView native_view_;
  internal::NativeWidgetDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(NativeWidgetAuraWindowObserver);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_NATIVE_WIDGET_AURA_WINDOW_OBSERVER_H_
