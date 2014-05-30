// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_ITEM_MODEL_OBSERVER_H_
#define UI_APP_LIST_APP_LIST_ITEM_MODEL_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

class APP_LIST_EXPORT AppListItemModelObserver {
 public:
  // Invoked after item's icon is changed.
  virtual void ItemIconChanged() = 0;

  // Invoked after item's title is changed.
  virtual void ItemTitleChanged() = 0;

  // Invoked after item's highlighted state is changed.
  virtual void ItemHighlightedChanged() = 0;

  // Invoked after item begins or finishes installing.
  virtual void ItemIsInstallingChanged() = 0;

  // Invoked after item's download percentage changes.
  virtual void ItemPercentDownloadedChanged() = 0;

 protected:
  virtual ~AppListItemModelObserver() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_ITEM_MODEL_OBSERVER_H_
