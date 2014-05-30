// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_MODEL_OBSERVER_H_
#define UI_APP_LIST_APP_LIST_MODEL_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class APP_LIST_EXPORT AppListModelObserver {
 public:
  // Invoked when AppListModel's status has changed.
  virtual void OnAppListModelStatusChanged() = 0;

 protected:
  virtual ~AppListModelObserver() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_MODEL_OBSERVER_H_
