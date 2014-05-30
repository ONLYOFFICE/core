// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_host.h"

#include "base/debug/trace_event.h"
#include "ui/gfx/path.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/menu/menu_controller.h"
#include "ui/views/controls/menu/menu_host_root_view.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/controls/menu/menu_scroll_view_container.h"
#include "ui/views/controls/menu/submenu_view.h"
#include "ui/views/round_rect_painter.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/views/corewm/shadow_types.h"
#endif

namespace views {

////////////////////////////////////////////////////////////////////////////////
// MenuHost, public:

MenuHost::MenuHost(SubmenuView* submenu)
    : submenu_(submenu),
      destroying_(false),
      ignore_capture_lost_(false) {
}

MenuHost::~MenuHost() {
}

void MenuHost::InitMenuHost(Widget* parent,
                            const gfx::Rect& bounds,
                            View* contents_view,
                            bool do_capture) {
  TRACE_EVENT0("views", "MenuHost::InitMenuHost");
  Widget::InitParams params(Widget::InitParams::TYPE_MENU);
  const MenuController* menu_controller =
      submenu_->GetMenuItem()->GetMenuController();
  const MenuConfig& menu_config = submenu_->GetMenuItem()->GetMenuConfig();
  bool rounded_border = menu_controller && menu_config.corner_radius > 0;
  bool bubble_border = submenu_->GetScrollViewContainer() &&
                       submenu_->GetScrollViewContainer()->HasBubbleBorder();
  params.has_dropshadow = !bubble_border;
  params.transparent = bubble_border || rounded_border;
  params.parent = parent ? parent->GetNativeView() : NULL;
  params.bounds = bounds;
  Init(params);

#if defined(USE_AURA)
  if (bubble_border)
    SetShadowType(GetNativeView(), views::corewm::SHADOW_TYPE_NONE);
#endif

  SetContentsView(contents_view);
  if (bubble_border || rounded_border)
    SetOpacity(0);
  ShowMenuHost(do_capture);
}

bool MenuHost::IsMenuHostVisible() {
  return IsVisible();
}

void MenuHost::ShowMenuHost(bool do_capture) {
  // Doing a capture may make us get capture lost. Ignore it while we're in the
  // process of showing.
  ignore_capture_lost_ = true;
  Show();
  if (do_capture)
    native_widget_private()->SetCapture();
  ignore_capture_lost_ = false;
}

void MenuHost::HideMenuHost() {
  ignore_capture_lost_ = true;
  ReleaseMenuHostCapture();
  Hide();
  ignore_capture_lost_ = false;
}

void MenuHost::DestroyMenuHost() {
  HideMenuHost();
  destroying_ = true;
  static_cast<MenuHostRootView*>(GetRootView())->ClearSubmenu();
  Close();
}

void MenuHost::SetMenuHostBounds(const gfx::Rect& bounds) {
  SetBounds(bounds);
}

void MenuHost::ReleaseMenuHostCapture() {
  if (native_widget_private()->HasCapture())
    native_widget_private()->ReleaseCapture();
}

////////////////////////////////////////////////////////////////////////////////
// MenuHost, Widget overrides:

internal::RootView* MenuHost::CreateRootView() {
  return new MenuHostRootView(this, submenu_);
}

bool MenuHost::ShouldReleaseCaptureOnMouseReleased() const {
  return false;
}

void MenuHost::OnMouseCaptureLost() {
  if (destroying_ || ignore_capture_lost_)
    return;
  MenuController* menu_controller =
      submenu_->GetMenuItem()->GetMenuController();
  if (menu_controller && !menu_controller->drag_in_progress())
    menu_controller->CancelAll();
  Widget::OnMouseCaptureLost();
}

void MenuHost::OnNativeWidgetDestroyed() {
  if (!destroying_) {
    // We weren't explicitly told to destroy ourselves, which means the menu was
    // deleted out from under us (the window we're parented to was closed). Tell
    // the SubmenuView to drop references to us.
    submenu_->MenuHostDestroyed();
  }
  Widget::OnNativeWidgetDestroyed();
}

void MenuHost::OnOwnerClosing() {
  if (destroying_)
    return;

  MenuController* menu_controller =
      submenu_->GetMenuItem()->GetMenuController();
  if (menu_controller && !menu_controller->drag_in_progress())
    menu_controller->CancelAll();
}

}  // namespace views
