// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_RESULT_LIST_VIEW_H_
#define UI_APP_LIST_VIEWS_SEARCH_RESULT_LIST_VIEW_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/search_result_view_delegate.h"
#include "ui/base/models/list_model_observer.h"
#include "ui/views/view.h"

namespace app_list {

class SearchResultListViewDelegate;
class SearchResultView;

// SearchResultListView displays AppListModel::SearchResults with a list of
// SearchResultView.
class SearchResultListView : public views::View,
                             public ui::ListModelObserver,
                             public SearchResultViewDelegate {
 public:
  explicit SearchResultListView(SearchResultListViewDelegate* delegate);
  virtual ~SearchResultListView();

  void SetResults(AppListModel::SearchResults* results);

  void SetSelectedIndex(int selected_index);

  bool IsResultViewSelected(const SearchResultView* result_view) const;

  // Overridden from views::View:
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;

 private:
  // Helper function to get SearchResultView at given |index|.
  SearchResultView* GetResultViewAt(int index);

  // Updates UI with model.
  void Update();

  // Schedules an Update call using |update_factory_|. Do nothing if there is a
  // pending call.
  void ScheduleUpdate();

  // Overridden from ListModelObserver:
  virtual void ListItemsAdded(size_t start, size_t count) OVERRIDE;
  virtual void ListItemsRemoved(size_t start, size_t count) OVERRIDE;
  virtual void ListItemMoved(size_t index, size_t target_index) OVERRIDE;
  virtual void ListItemsChanged(size_t start, size_t count) OVERRIDE;

  // Overridden from SearchResultViewDelegate:
  virtual void SearchResultActivated(SearchResultView* view,
                                     const ui::Event& event) OVERRIDE;
  virtual void SearchResultActionActivated(SearchResultView* view,
                                           int action_index,
                                           const ui::Event& event) OVERRIDE;

  SearchResultListViewDelegate* delegate_;  // Not owned.
  AppListModel::SearchResults* results_;  // Owned by AppListModel.

  int last_visible_index_;
  int selected_index_;

  // The factory that consolidates multiple Update calls into one.
  base::WeakPtrFactory<SearchResultListView> update_factory_;

  DISALLOW_COPY_AND_ASSIGN(SearchResultListView);
};

}  // namespace app_ist

#endif  // UI_APP_LIST_VIEWS_SEARCH_RESULT_LIST_VIEW_H_
