// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/app_list_main_view.h"

#include <algorithm>

#include "base/string_util.h"
#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/app_list_item_model.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/app_list_view_delegate.h"
#include "ui/app_list/pagination_model.h"
#include "ui/app_list/search_box_model.h"
#include "ui/app_list/views/app_list_item_view.h"
#include "ui/app_list/views/contents_view.h"
#include "ui/app_list/views/search_box_view.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/widget/widget.h"

namespace app_list {

namespace {

// Inner padding space in pixels of bubble contents.
const int kInnerPadding = 1;

// The maximum allowed time to wait for icon loading in milliseconds.
const int kMaxIconLoadingWaitTimeInMs = 50;

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// AppListMainView::IconLoader

class AppListMainView::IconLoader : public AppListItemModelObserver {
 public:
  IconLoader(AppListMainView* owner,
             AppListItemModel* item,
             ui::ScaleFactor scale_factor)
      : owner_(owner),
        item_(item) {
    item_->AddObserver(this);

    // Triggers icon loading for given |scale_factor|.
    item_->icon().GetRepresentation(scale_factor);
  }

  virtual ~IconLoader() {
    item_->RemoveObserver(this);
  }

 private:
  // AppListItemModelObserver overrides:
  virtual void ItemIconChanged() OVERRIDE {
    owner_->OnItemIconLoaded(this);
    // Note that IconLoader is released here.
  }
  virtual void ItemTitleChanged() OVERRIDE {}
  virtual void ItemHighlightedChanged() OVERRIDE {}
  virtual void ItemIsInstallingChanged() OVERRIDE {};
  virtual void ItemPercentDownloadedChanged() OVERRIDE {};

  AppListMainView* owner_;
  AppListItemModel* item_;

  DISALLOW_COPY_AND_ASSIGN(IconLoader);
};

////////////////////////////////////////////////////////////////////////////////
// AppListMainView:

AppListMainView::AppListMainView(AppListViewDelegate* delegate,
                                 AppListModel* model,
                                 PaginationModel* pagination_model,
                                 views::View* anchor)
    : delegate_(delegate),
      model_(model),
      search_box_view_(NULL),
      contents_view_(NULL) {
  // Starts icon loading early.
  PreloadIcons(pagination_model, anchor);

  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical,
                                        kInnerPadding,
                                        kInnerPadding,
                                        kInnerPadding));

  search_box_view_ = new SearchBoxView(this, delegate);
  AddChildView(search_box_view_);

  contents_view_ = new ContentsView(this, pagination_model);
  AddChildView(contents_view_);

  search_box_view_->set_contents_view(contents_view_);

#if defined(USE_AURA)
  contents_view_->SetPaintToLayer(true);
  contents_view_->SetFillsBoundsOpaquely(false);
  contents_view_->layer()->SetMasksToBounds(true);
#endif

  search_box_view_->SetModel(model_->search_box());
  contents_view_->SetModel(model_);
}

AppListMainView::~AppListMainView() {
  pending_icon_loaders_.clear();
}

void AppListMainView::ShowAppListWhenReady() {
  if (pending_icon_loaders_.empty()) {
    icon_loading_wait_timer_.Stop();
    GetWidget()->Show();
    return;
  }

  if (icon_loading_wait_timer_.IsRunning())
    return;

  icon_loading_wait_timer_.Start(
      FROM_HERE,
      base::TimeDelta::FromMilliseconds(kMaxIconLoadingWaitTimeInMs),
      this, &AppListMainView::OnIconLoadingWaitTimer);
}

void AppListMainView::Close() {
  icon_loading_wait_timer_.Stop();
}

void AppListMainView::Prerender() {
  contents_view_->Prerender();
}

void AppListMainView::PreloadIcons(PaginationModel* pagination_model,
                                   views::View* anchor) {
  ui::ScaleFactor scale_factor = ui::SCALE_FACTOR_100P;
  if (anchor && anchor->GetWidget()) {
    scale_factor = ui::GetScaleFactorForNativeView(
        anchor->GetWidget()->GetNativeView());
  }

  // |pagination_model| could have -1 as the initial selected page and
  // assumes first page (i.e. index 0) will be used in this case.
  const int selected_page = std::max(0, pagination_model->selected_page());

  const int tiles_per_page = kPreferredCols * kPreferredRows;
  const int start_model_index = selected_page * tiles_per_page;
  const int end_model_index = std::min(
      static_cast<int>(model_->apps()->item_count()),
      start_model_index + tiles_per_page);

  pending_icon_loaders_.clear();
  for (int i = start_model_index; i < end_model_index; ++i) {
    AppListItemModel* item = model_->apps()->GetItemAt(i);
    if (item->icon().HasRepresentation(scale_factor))
      continue;

    pending_icon_loaders_.push_back(new IconLoader(this, item, scale_factor));
  }
}

void AppListMainView::OnIconLoadingWaitTimer() {
  GetWidget()->Show();
}

void AppListMainView::OnItemIconLoaded(IconLoader* loader) {
  ScopedVector<IconLoader>::iterator it = std::find(
      pending_icon_loaders_.begin(), pending_icon_loaders_.end(), loader);
  DCHECK(it != pending_icon_loaders_.end());
  pending_icon_loaders_.erase(it);

  if (pending_icon_loaders_.empty() && icon_loading_wait_timer_.IsRunning()) {
    icon_loading_wait_timer_.Stop();
    GetWidget()->Show();
  }
}

void AppListMainView::ActivateApp(AppListItemModel* item, int event_flags) {
  if (delegate_)
    delegate_->ActivateAppListItem(item, event_flags);
}

void AppListMainView::QueryChanged(SearchBoxView* sender) {
  string16 query;
  TrimWhitespace(model_->search_box()->text(), TRIM_ALL, &query);
  bool should_show_search = !query.empty();
  contents_view_->ShowSearchResults(should_show_search);

  if (delegate_) {
    if (should_show_search)
      delegate_->StartSearch();
    else
      delegate_->StopSearch();
  }
}

void AppListMainView::OpenResult(const SearchResult& result, int event_flags) {
  if (delegate_)
    delegate_->OpenSearchResult(result, event_flags);
}

void AppListMainView::InvokeResultAction(const SearchResult& result,
                                         int action_index,
                                         int event_flags) {
  if (delegate_)
    delegate_->InvokeSearchResultAction(result, action_index, event_flags);
}

}  // namespace app_list
