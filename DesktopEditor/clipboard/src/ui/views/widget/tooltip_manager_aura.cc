// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/tooltip_manager_aura.h"

#include "base/logging.h"
#include "ui/aura/client/tooltip_client.h"
#include "ui/aura/root_window.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/screen.h"
#include "ui/views/widget/widget.h"

namespace views {

// static
int TooltipManager::GetTooltipHeight() {
  // Not used for linux and chromeos.
  NOTIMPLEMENTED();
  return 0;
}

// static
gfx::Font TooltipManager::GetDefaultFont() {
  return ui::ResourceBundle::GetSharedInstance().GetFont(
      ui::ResourceBundle::BaseFont);
}

// static
int TooltipManager::GetMaxWidth(int x, int y, gfx::NativeView context) {
  gfx::Rect monitor_bounds =
      gfx::Screen::GetScreenFor(context)->GetDisplayNearestPoint(
          gfx::Point(x, y)).bounds();
  return (monitor_bounds.width() + 1) / 2;
}

////////////////////////////////////////////////////////////////////////////////
// TooltipManagerAura public:

TooltipManagerAura::TooltipManagerAura(aura::Window* window, Widget* widget)
    : window_(window),
      widget_(widget) {
  aura::client::SetTooltipText(window_, &tooltip_text_);
}

TooltipManagerAura::~TooltipManagerAura() {
  aura::client::SetTooltipText(window_, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// TooltipManagerAura, TooltipManager implementation:

void TooltipManagerAura::UpdateTooltip() {
  aura::RootWindow* root_window = window_->GetRootWindow();
  if (aura::client::GetTooltipClient(root_window)) {
    gfx::Point view_point = root_window->GetLastMouseLocationInRoot();
    aura::Window::ConvertPointToTarget(root_window, window_, &view_point);
    View* view = GetViewUnderPoint(view_point);
    UpdateTooltipForTarget(view, view_point, root_window);
  }
}

void TooltipManagerAura::TooltipTextChanged(View* view)  {
  aura::RootWindow* root_window = window_->GetRootWindow();
  if (aura::client::GetTooltipClient(root_window)) {
    gfx::Point view_point = root_window->GetLastMouseLocationInRoot();
    aura::Window::ConvertPointToTarget(root_window, window_, &view_point);
    View* target = GetViewUnderPoint(view_point);
    if (target != view)
      return;
    UpdateTooltipForTarget(view, view_point, root_window);
  }
}

void TooltipManagerAura::ShowKeyboardTooltip(View* view) {
  NOTREACHED();
}

void TooltipManagerAura::HideKeyboardTooltip()  {
  NOTREACHED();
}

View* TooltipManagerAura::GetViewUnderPoint(const gfx::Point& point) {
  View* root_view = widget_->GetRootView();
  if (root_view)
    return root_view->GetTooltipHandlerForPoint(point);
  return NULL;
}

void TooltipManagerAura::UpdateTooltipForTarget(View* target,
                                                const gfx::Point& point,
                                                aura::RootWindow* root_window) {
  if (target) {
    gfx::Point view_point = point;
    View::ConvertPointFromWidget(target, &view_point);
    string16 new_tooltip_text;
    if (!target->GetTooltipText(view_point, &new_tooltip_text))
      tooltip_text_.clear();
    else
      tooltip_text_ = new_tooltip_text;
  } else {
    tooltip_text_.clear();
  }
  aura::client::GetTooltipClient(root_window)->UpdateTooltip(window_);
}

}  // namespace views.
