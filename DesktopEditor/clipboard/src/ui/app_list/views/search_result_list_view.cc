// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/search_result_list_view.h"

#include <algorithm>

#include "base/bind.h"
#include "base/message_loop.h"
#include "ui/app_list/search_result_list_view_delegate.h"
#include "ui/app_list/views/search_result_view.h"
#include "ui/base/events/event.h"
#include "ui/views/layout/box_layout.h"

namespace {

const int kMaxResults = 6;

}  // namespace

namespace app_list {

SearchResultListView::SearchResultListView(
    SearchResultListViewDelegate* delegate)
    : delegate_(delegate),
      results_(NULL),
      last_visible_index_(0),
      selected_index_(-1),
      update_factory_(this) {
  SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0));

  for (int i = 0; i < kMaxResults; ++i)
    AddChildView(new SearchResultView(this, this));
}

SearchResultListView::~SearchResultListView() {
  if (results_)
    results_->RemoveObserver(this);
}

void SearchResultListView::SetResults(AppListModel::SearchResults* results) {
  if (results_)
    results_->RemoveObserver(this);

  results_ = results;
  if (results_)
    results_->AddObserver(this);

  Update();
}

void SearchResultListView::SetSelectedIndex(int selected_index) {
  if (selected_index_ == selected_index)
    return;

  if (selected_index_ >= 0)
    GetResultViewAt(selected_index_)->SchedulePaint();

  selected_index_ = selected_index;

  if (selected_index_ >= 0)
    GetResultViewAt(selected_index_)->SchedulePaint();
}

bool SearchResultListView::IsResultViewSelected(
    const SearchResultView* result_view) const {
  if (selected_index_ < 0)
    return false;

  return static_cast<const SearchResultView*>(child_at(selected_index_)) ==
      result_view;
}

bool SearchResultListView::OnKeyPressed(const ui::KeyEvent& event) {
  switch (event.key_code()) {
    case ui::VKEY_TAB:
      if (event.IsShiftDown())
        SetSelectedIndex(std::max(selected_index_ - 1, 0));
      else
        SetSelectedIndex(std::min(selected_index_ + 1, last_visible_index_));
      return true;
    case ui::VKEY_UP:
      SetSelectedIndex(std::max(selected_index_ - 1, 0));
      return true;
    case ui::VKEY_DOWN:
      SetSelectedIndex(std::min(selected_index_ + 1, last_visible_index_));
      return true;
    case ui::VKEY_RETURN:
      if (selected_index_ >= 0)
        SearchResultActivated(GetResultViewAt(selected_index_), event);
      return true;
    default:
      break;
  }
  return false;
}

SearchResultView* SearchResultListView::GetResultViewAt(int index) {
  DCHECK(index >= 0 && index < child_count());
  return static_cast<SearchResultView*>(child_at(index));
}

void SearchResultListView::Update() {
  last_visible_index_ = 0;
  for (size_t i = 0; i < static_cast<size_t>(child_count()); ++i) {
    SearchResultView* result_view = GetResultViewAt(i);
    if (i < results_->item_count()) {
      result_view->SetResult(results_->GetItemAt(i));
      result_view->SetVisible(true);
      last_visible_index_ = i;
    } else {
      result_view->SetResult(NULL);
      result_view->SetVisible(false);
    }
  }
  if (selected_index_ > last_visible_index_)
    SetSelectedIndex(last_visible_index_);

  Layout();
  update_factory_.InvalidateWeakPtrs();
}

void SearchResultListView::ScheduleUpdate() {
  // When search results are added one by one, each addition generates an update
  // request. Consolidates those update requests into one Update call.
  if (!update_factory_.HasWeakPtrs()) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&SearchResultListView::Update,
                   update_factory_.GetWeakPtr()));
  }
}

void SearchResultListView::ListItemsAdded(size_t start, size_t count) {
  ScheduleUpdate();
}

void SearchResultListView::ListItemsRemoved(size_t start, size_t count) {
  size_t last = std::min(start + count, static_cast<size_t>(child_count()));
  for (size_t i = start; i < last; ++i)
    GetResultViewAt(i)->ClearResultNoRepaint();

  ScheduleUpdate();
}

void SearchResultListView::ListItemMoved(size_t index, size_t target_index) {
  NOTREACHED();
}

void SearchResultListView::ListItemsChanged(size_t start, size_t count) {
  ScheduleUpdate();
}

void SearchResultListView::SearchResultActivated(SearchResultView* view,
                                                 const ui::Event& event) {
  if (delegate_ && view->result())
    delegate_->OpenResult(*(view->result()), event.flags());
}

void SearchResultListView::SearchResultActionActivated(SearchResultView* view,
                                                       int action_index,
                                                       const ui::Event& event) {
  if (delegate_ && view->result()) {
    delegate_->InvokeResultAction(
        *(view->result()), action_index, event.flags());
  }
}

}  // namespace app_list
