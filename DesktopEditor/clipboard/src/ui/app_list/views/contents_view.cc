// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/contents_view.h"

#include <algorithm>

#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/pagination_model.h"
#include "ui/app_list/views/app_list_main_view.h"
#include "ui/app_list/views/apps_grid_view.h"
#include "ui/app_list/views/search_result_list_view.h"
#include "ui/base/events/event.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/view_model.h"
#include "ui/views/view_model_utils.h"

namespace app_list {

namespace {

const int kPreferredIconDimension = 48;

// Indexes of interesting views in ViewModel of ContentsView.
const int kIndexAppsGrid = 0;
const int kIndexSearchResults = 1;

const int kMinMouseWheelToSwitchPage = 20;
const int kMinScrollToSwitchPage = 20;
const int kMinHorizVelocityToSwitchPage = 800;

const double kFinishTransitionThreshold = 0.33;

// Helpers to get certain child view from |model|.
AppsGridView* GetAppsGridView(views::ViewModel* model) {
  return static_cast<AppsGridView*>(model->view_at(kIndexAppsGrid));
}

SearchResultListView* GetSearchResultListView(views::ViewModel* model) {
  return static_cast<SearchResultListView*>(
      model->view_at(kIndexSearchResults));
}

}  // namespace

ContentsView::ContentsView(AppListMainView* app_list_main_view,
                           PaginationModel* pagination_model)
    : show_state_(SHOW_APPS),
      pagination_model_(pagination_model),
      view_model_(new views::ViewModel),
      bounds_animator_(new views::BoundsAnimator(this)) {
  pagination_model_->SetTransitionDurations(
      kPageTransitionDurationInMs,
      kOverscrollPageTransitionDurationMs);

  AppsGridView* apps_grid_view = new AppsGridView(app_list_main_view,
                                                  pagination_model);
  apps_grid_view->SetLayout(kPreferredIconDimension,
                            kPreferredCols,
                            kPreferredRows);
  AddChildView(apps_grid_view);
  view_model_->Add(apps_grid_view, kIndexAppsGrid);

  SearchResultListView* search_results_view = new SearchResultListView(
      app_list_main_view);
  AddChildView(search_results_view);
  view_model_->Add(search_results_view, kIndexSearchResults);
}

ContentsView::~ContentsView() {
}

void ContentsView::SetModel(AppListModel* model) {
  if (model) {
    GetAppsGridView(view_model_.get())->SetModel(model);
    GetSearchResultListView(view_model_.get())->SetResults(model->results());
  } else {
    GetAppsGridView(view_model_.get())->SetModel(NULL);
    GetSearchResultListView(view_model_.get())->SetResults(NULL);
  }
}

void ContentsView::SetShowState(ShowState show_state) {
  if (show_state_ == show_state)
    return;

  show_state_ = show_state;
  ShowStateChanged();
}

void ContentsView::ShowStateChanged() {
  if (show_state_ == SHOW_SEARCH_RESULTS) {
    // TODO(xiyuan): Highlight default match instead of the first.
    SearchResultListView* results_view =
        GetSearchResultListView(view_model_.get());
    if (results_view->visible())
      results_view->SetSelectedIndex(0);
  }

  AnimateToIdealBounds();
}

void ContentsView::CalculateIdealBounds() {
  gfx::Rect rect(GetContentsBounds());
  if (rect.IsEmpty())
    return;

  gfx::Rect grid_frame(rect);
  gfx::Rect results_frame(rect);

  // Offsets apps grid and result list based on |show_state_|.
  // SearchResultListView is on top of apps grid. Visible view is left in
  // visible area and invisible ones is put out of the visible area.
  int contents_area_height = rect.height();
  switch (show_state_) {
    case SHOW_APPS:
      results_frame.Offset(0, -contents_area_height);
      break;
    case SHOW_SEARCH_RESULTS:
      grid_frame.Offset(0, contents_area_height);
      break;
    default:
      NOTREACHED() << "Unknown show_state_ " << show_state_;
      break;
  }

  view_model_->set_ideal_bounds(kIndexAppsGrid, grid_frame);
  view_model_->set_ideal_bounds(kIndexSearchResults, results_frame);
}

void ContentsView::AnimateToIdealBounds() {
  CalculateIdealBounds();
  for (int i = 0; i < view_model_->view_size(); ++i) {
    bounds_animator_->AnimateViewTo(view_model_->view_at(i),
                                    view_model_->ideal_bounds(i));
  }
}

void ContentsView::ShowSearchResults(bool show) {
  SetShowState(show ? SHOW_SEARCH_RESULTS : SHOW_APPS);
}

void ContentsView::Prerender() {
  const int selected_page = std::max(0, pagination_model_->selected_page());
  GetAppsGridView(view_model_.get())->Prerender(selected_page);
}

gfx::Size ContentsView::GetPreferredSize() {
  const gfx::Size grid_size =
      GetAppsGridView(view_model_.get())->GetPreferredSize();
  const gfx::Size results_size =
      GetSearchResultListView(view_model_.get())->GetPreferredSize();

  int width = std::max(grid_size.width(), results_size.width());
  int height = std::max(grid_size.height(), results_size.height());
  return gfx::Size(width, height);
}

void ContentsView::Layout() {
  CalculateIdealBounds();
  views::ViewModelUtils::SetViewBoundsToIdealBounds(*view_model_);
}

bool ContentsView::OnKeyPressed(const ui::KeyEvent& event) {
  switch (show_state_) {
    case SHOW_APPS:
      return GetAppsGridView(view_model_.get())->OnKeyPressed(event);
    case SHOW_SEARCH_RESULTS:
      return GetSearchResultListView(view_model_.get())->OnKeyPressed(event);
    default:
      NOTREACHED() << "Unknown show state " << show_state_;
  }
  return false;
}

bool ContentsView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  if (show_state_ != SHOW_APPS)
    return false;

  if (abs(event.y_offset()) > kMinMouseWheelToSwitchPage) {
    if (!pagination_model_->has_transition()) {
      pagination_model_->SelectPageRelative(
          event.y_offset() > 0 ? -1 : 1, true);
    }
    return true;
  }

  return false;
}

void ContentsView::OnGestureEvent(ui::GestureEvent* event) {
  if (show_state_ != SHOW_APPS)
    return;

  switch (event->type()) {
    case ui::ET_GESTURE_SCROLL_BEGIN:
      pagination_model_->StartScroll();
      event->SetHandled();
      return;
    case ui::ET_GESTURE_SCROLL_UPDATE:
      // event->details.scroll_x() > 0 means moving contents to right. That is,
      // transitioning to previous page.
      pagination_model_->UpdateScroll(
          event->details().scroll_x() / GetContentsBounds().width());
      event->SetHandled();
      return;
    case ui::ET_GESTURE_SCROLL_END:
      pagination_model_->EndScroll(pagination_model_->
          transition().progress < kFinishTransitionThreshold);
      event->SetHandled();
      return;
    case ui::ET_SCROLL_FLING_START: {
      pagination_model_->EndScroll(true);
      if (fabs(event->details().velocity_x()) > kMinHorizVelocityToSwitchPage) {
        pagination_model_->SelectPageRelative(
            event->details().velocity_x() < 0 ? 1 : -1,
            true);
      }
      event->SetHandled();
      return;
    }
    default:
      break;
  }
}

void ContentsView::OnScrollEvent(ui::ScrollEvent* event) {
  if (show_state_ != SHOW_APPS ||
      event->type() == ui::ET_SCROLL_FLING_CANCEL ||
      abs(event->x_offset()) < kMinScrollToSwitchPage) {
    return;
  }

  if (!pagination_model_->has_transition()) {
    pagination_model_->SelectPageRelative(event->x_offset() > 0 ? -1 : 1,
                                          true);
  }
  event->SetHandled();
  event->StopPropagation();
}

}  // namespace app_list
