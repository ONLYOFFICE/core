// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_MAIN_VIEW_H_
#define UI_APP_LIST_VIEWS_APP_LIST_MAIN_VIEW_H_

#include "base/memory/scoped_vector.h"
#include "base/timer.h"
#include "ui/app_list/apps_grid_view_delegate.h"
#include "ui/app_list/search_box_view_delegate.h"
#include "ui/app_list/search_result_list_view_delegate.h"
#include "ui/views/view.h"

namespace views {
class Widget;
}

namespace app_list {

class AppListModel;
class AppListItemModel;
class AppListViewDelegate;
class ContentsView;
class PaginationModel;
class SearchBoxView;

// AppListMainView contains the normal view of the app list, which is shown
// when the user is signed in.
class AppListMainView : public views::View,
                        public AppsGridViewDelegate,
                        public SearchBoxViewDelegate,
                        public SearchResultListViewDelegate {
 public:
  // Takes ownership of |delegate|.
  explicit AppListMainView(AppListViewDelegate* delegate,
                           AppListModel* model,
                           PaginationModel* pagination_model,
                           views::View* anchor);
  virtual ~AppListMainView();

  void ShowAppListWhenReady();

  void Close();

  void Prerender();

  SearchBoxView* search_box_view() { return search_box_view_; }

 private:
  class IconLoader;

  // Loads icon image for the apps in the selected page of |pagination_model|.
  // |anchor| is used to determine the image scale factor to use.
  void PreloadIcons(PaginationModel* pagination_model,
                    views::View* anchor);

  // Invoked when |icon_loading_wait_timer_| fires.
  void OnIconLoadingWaitTimer();

  // Invoked from an IconLoader when icon loading is finished.
  void OnItemIconLoaded(IconLoader* loader);

  // Overridden from AppsGridViewDelegate:
  virtual void ActivateApp(AppListItemModel* item, int event_flags) OVERRIDE;

  // Overridden from SearchBoxViewDelegate:
  virtual void QueryChanged(SearchBoxView* sender) OVERRIDE;

  // Overridden from SearchResultListViewDelegate:
  virtual void OpenResult(const SearchResult& result,
                          int event_flags) OVERRIDE;
  virtual void InvokeResultAction(const SearchResult& result,
                                  int action_index,
                                  int event_flags) OVERRIDE;

  AppListViewDelegate* delegate_;
  AppListModel* model_;

  SearchBoxView* search_box_view_;  // Owned by views hierarchy.
  ContentsView* contents_view_;  // Owned by views hierarchy.

  // A timer that fires when maximum allowed time to wait for icon loading has
  // passed.
  base::OneShotTimer<AppListMainView> icon_loading_wait_timer_;

  ScopedVector<IconLoader> pending_icon_loaders_;

  DISALLOW_COPY_AND_ASSIGN(AppListMainView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_APP_LIST_MAIN_VIEW_H_
