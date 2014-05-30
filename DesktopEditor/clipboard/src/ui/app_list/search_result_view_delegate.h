// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_RESULT_VIEW_DELEGATE_H_
#define UI_APP_LIST_SEARCH_RESULT_VIEW_DELEGATE_H_

namespace ui {
class Event;
}

namespace app_list {

class SearchResultView;

class SearchResultViewDelegate {
 public:
  // Called when the search result is activated.
  virtual void SearchResultActivated(SearchResultView* view,
                                     const ui::Event& event) = 0;

  // Called when one of the search result's optional action icons is activated.
  // |action_index| contains the 0-based index of the action.
  virtual void SearchResultActionActivated(SearchResultView* view,
                                           int action_index,
                                           const ui::Event& event) = 0;

 protected:
  virtual ~SearchResultViewDelegate() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_SEARCH_RESULT_VIEW_DELEGATE_H_
