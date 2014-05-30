// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_TEST_APP_LIST_TEST_VIEW_DELEGATE_H
#define UI_APP_LIST_TEST_APP_LIST_TEST_VIEW_DELEGATE_H

#include "base/compiler_specific.h"
#include "ui/app_list/app_list_view_delegate.h"

namespace app_list {
namespace test{

// A concrete AppListViewDelegate for unit tests.
class AppListTestViewDelegate  : public AppListViewDelegate {
 public:
  AppListTestViewDelegate();
  virtual ~AppListTestViewDelegate();

  int activate_count() { return activate_count_; }
  int dismiss_count() { return dismiss_count_; }
  AppListItemModel* last_activated() { return last_activated_; }

  // AppListViewDelegate overrides:
  virtual void SetModel(AppListModel* model) OVERRIDE {}
  virtual SigninDelegate* GetSigninDelegate() OVERRIDE;
  virtual void ActivateAppListItem(AppListItemModel* item,
                                   int event_flags) OVERRIDE;
  virtual void StartSearch() OVERRIDE {}
  virtual void StopSearch() OVERRIDE {}
  virtual void OpenSearchResult(const SearchResult& result,
                                int event_flags) OVERRIDE {}
  virtual void InvokeSearchResultAction(const SearchResult& result,
                                        int action_index,
                                        int event_flags) OVERRIDE {}
  virtual void Dismiss() OVERRIDE;
  virtual void ViewClosing() OVERRIDE {}
  virtual void ViewActivationChanged(bool active) OVERRIDE {}
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual string16 GetCurrentUserName() OVERRIDE;
  virtual string16 GetCurrentUserEmail() OVERRIDE;
  virtual void OpenSettings() OVERRIDE {}
  virtual void OpenFeedback() OVERRIDE {}

private:
  int activate_count_;
  int dismiss_count_;
  AppListItemModel* last_activated_;
};

}  // namespace test
}  // namespace app_list

#endif  // UI_APP_LIST_TEST_APP_LIST_TEST_VIEW_DELEGATE_H
