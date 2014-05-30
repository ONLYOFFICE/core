// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/app_list_view.h"

#include "base/string_util.h"
#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/app_list_view_delegate.h"
#include "ui/app_list/pagination_model.h"
#include "ui/app_list/signin_delegate.h"
#include "ui/app_list/views/app_list_background.h"
#include "ui/app_list/views/app_list_main_view.h"
#include "ui/app_list/views/search_box_view.h"
#include "ui/app_list/views/signin_view.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/path.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/bubble/bubble_frame_view.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/aura/window.h"
#endif

namespace app_list {

namespace {

// The distance between the arrow tip and edge of the anchor view.
const int kArrowOffset = 10;

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// AppListView:

AppListView::AppListView(AppListViewDelegate* delegate)
    : model_(new AppListModel),
      delegate_(delegate),
      app_list_main_view_(NULL),
      signin_view_(NULL) {
  if (delegate_)
    delegate_->SetModel(model_.get());
  if (GetSigninDelegate())
    GetSigninDelegate()->AddObserver(this);
}

AppListView::~AppListView() {
  if (GetSigninDelegate())
    GetSigninDelegate()->RemoveObserver(this);

  // Models are going away, ensure their references are cleared.
  RemoveAllChildViews(true);
}

void AppListView::InitAsBubble(gfx::NativeView parent,
                               PaginationModel* pagination_model,
                               views::View* anchor,
                               const gfx::Point& anchor_point,
                               views::BubbleBorder::Arrow arrow,
                               bool border_accepts_events) {
  app_list_main_view_ = new AppListMainView(delegate_.get(),
                                            model_.get(),
                                            pagination_model,
                                            anchor);
  AddChildView(app_list_main_view_);
#if defined(USE_AURA)
  app_list_main_view_->SetPaintToLayer(true);
  app_list_main_view_->SetFillsBoundsOpaquely(false);
  app_list_main_view_->layer()->SetMasksToBounds(true);
#endif

  signin_view_ = new SigninView(
      GetSigninDelegate(),
      app_list_main_view_->GetPreferredSize().width());
  AddChildView(signin_view_);

  OnSigninStatusChanged();

  set_anchor_view(anchor);
  set_anchor_rect(gfx::Rect(anchor_point, gfx::Size()));
  set_color(kContentsBackgroundColor);
  set_margins(gfx::Insets());
  set_move_with_anchor(true);
  set_parent_window(parent);
  set_close_on_deactivate(false);
  set_close_on_esc(false);
  set_anchor_view_insets(gfx::Insets(kArrowOffset, kArrowOffset,
                                     kArrowOffset, kArrowOffset));
  set_border_accepts_events(border_accepts_events);
  set_shadow(views::BubbleBorder::BIG_SHADOW);
  views::BubbleDelegateView::CreateBubble(this);
  SetBubbleArrow(arrow);

#if defined(USE_AURA)
  GetWidget()->GetNativeWindow()->layer()->SetMasksToBounds(true);
  GetBubbleFrameView()->set_background(new AppListBackground(
      GetBubbleFrameView()->bubble_border()->GetBorderCornerRadius(),
      app_list_main_view_->search_box_view()));
  set_background(NULL);
#else
  set_background(new AppListBackground(
      GetBubbleFrameView()->bubble_border()->GetBorderCornerRadius(),
      app_list_main_view_->search_box_view()));

  // On non-aura the bubble has two widgets, and it's possible for the border
  // to be shown independently in odd situations. Explicitly hide the bubble
  // widget to ensure that any WM_WINDOWPOSCHANGED messages triggered by the
  // window manager do not have the SWP_SHOWWINDOW flag set which would cause
  // the border to be shown. See http://crbug.com/231687 .
  GetWidget()->Hide();
#endif
}

void AppListView::SetBubbleArrow(views::BubbleBorder::Arrow arrow) {
  GetBubbleFrameView()->bubble_border()->set_arrow(arrow);
  SizeToContents();  // Recalcuates with new border.
  GetBubbleFrameView()->SchedulePaint();
}

void AppListView::SetAnchorPoint(const gfx::Point& anchor_point) {
  set_anchor_rect(gfx::Rect(anchor_point, gfx::Size()));
  SizeToContents();  // Repositions view relative to the anchor.
}

void AppListView::ShowWhenReady() {
  app_list_main_view_->ShowAppListWhenReady();
}

void AppListView::Close() {
  app_list_main_view_->Close();

  if (delegate_)
    delegate_->Dismiss();
  else
    GetWidget()->Close();
}

void AppListView::UpdateBounds() {
  SizeToContents();
}

gfx::Size AppListView::GetPreferredSize() {
  return app_list_main_view_->GetPreferredSize();
}

bool AppListView::ShouldHandleSystemCommands() const {
  return true;
}

void AppListView::Prerender() {
  app_list_main_view_->Prerender();
}

void AppListView::OnSigninStatusChanged() {
  const bool needs_signin =
      GetSigninDelegate() && GetSigninDelegate()->NeedSignin();

  signin_view_->SetVisible(needs_signin);
  app_list_main_view_->SetVisible(!needs_signin);
}

views::View* AppListView::GetInitiallyFocusedView() {
  return app_list_main_view_->search_box_view()->search_box();
}

gfx::ImageSkia AppListView::GetWindowIcon() {
  if (delegate_)
    return delegate_->GetWindowIcon();

  return gfx::ImageSkia();
}

bool AppListView::WidgetHasHitTestMask() const {
  return true;
}

void AppListView::GetWidgetHitTestMask(gfx::Path* mask) const {
  DCHECK(mask);
  mask->addRect(gfx::RectToSkRect(
      GetBubbleFrameView()->GetContentsBounds()));
}

bool AppListView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  // The accelerator is added by BubbleDelegateView.
  if (accelerator.key_code() == ui::VKEY_ESCAPE) {
    if (app_list_main_view_->search_box_view()->HasSearch())
      app_list_main_view_->search_box_view()->ClearSearch();
    else
      Close();
    return true;
  }

  return false;
}

void AppListView::Layout() {
  const gfx::Rect contents_bounds = GetContentsBounds();
  app_list_main_view_->SetBoundsRect(contents_bounds);
  signin_view_->SetBoundsRect(contents_bounds);
}

void AppListView::OnWidgetDestroying(views::Widget* widget) {
  BubbleDelegateView::OnWidgetDestroying(widget);
  if (delegate_ && widget == GetWidget())
    delegate_->ViewClosing();
}

void AppListView::OnWidgetActivationChanged(views::Widget* widget,
                                            bool active) {
  // Do not called inherited function as the bubble delegate auto close
  // functionality is not used.
  if (delegate_ && widget == GetWidget())
    delegate_->ViewActivationChanged(active);
}

void AppListView::OnWidgetVisibilityChanged(views::Widget* widget,
                                            bool visible) {
  BubbleDelegateView::OnWidgetVisibilityChanged(widget, visible);

  if (widget != GetWidget())
    return;

  // We clear the search when hiding so the next time the app list appears it is
  // not showing search results.
  if (!visible)
    app_list_main_view_->search_box_view()->ClearSearch();

  // Whether we need to signin or not may have changed since last time we were
  // shown.
  Layout();
}

void AppListView::OnSigninSuccess() {
  OnSigninStatusChanged();
}

SigninDelegate* AppListView::GetSigninDelegate() {
  return delegate_ ? delegate_->GetSigninDelegate() : NULL;
}

}  // namespace app_list
