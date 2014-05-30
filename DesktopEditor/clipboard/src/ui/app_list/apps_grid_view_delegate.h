// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APPS_GRID_VIEW_DELEGATE_H_
#define UI_APP_LIST_APPS_GRID_VIEW_DELEGATE_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class AppListItemModel;

class APP_LIST_EXPORT AppsGridViewDelegate {
 public:
  // Invoked when an item is activated on the grid view. |event_flags| contains
  // the flags of the keyboard/mouse event that triggers the activation request.
  virtual void ActivateApp(AppListItemModel* item, int event_flags) = 0;

 protected:
  virtual ~AppsGridViewDelegate() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_APPS_GRID_VIEW_DELEGATE_H_
