// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_BOX_VIEW_DELEGATE_H_
#define UI_APP_LIST_SEARCH_BOX_VIEW_DELEGATE_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class SearchBoxView;

class APP_LIST_EXPORT SearchBoxViewDelegate {
 public:
  // Invoked when query text has changed by the user.
  virtual void QueryChanged(SearchBoxView* sender) = 0;

 protected:
  virtual ~SearchBoxViewDelegate() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_SEARCH_BOX_VIEW_DELEGATE_H_
