// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_RESULT_LIST_VIEW_DELEGATE_H_
#define UI_APP_LIST_SEARCH_RESULT_LIST_VIEW_DELEGATE_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class SearchResult;

class APP_LIST_EXPORT SearchResultListViewDelegate {
 public:
  // Invoked to open given |result|. |event_flags| contains the flags of the
  // keyboard/mouse event that triggers the "open" request. Delegate could use
  // the |event_flags| information to choose different ways to open the result.
  virtual void OpenResult(const SearchResult& result,
                          int event_flags) = 0;

  // Called to invoke a custom action on |result|.  |action_index| corresponds
  // to the index of the icon in |result.action_icons()| that was activated.
  virtual void InvokeResultAction(const SearchResult& result,
                                  int action_index,
                                  int event_flags) = 0;

 protected:
  virtual ~SearchResultListViewDelegate() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_SEARCH_RESULT_LIST_VIEW_DELEGATE_H_
