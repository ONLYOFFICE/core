// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_TEST_APP_LIST_TEST_MODEL_H_
#define UI_APP_LIST_TEST_APP_LIST_TEST_MODEL_H_

#include <string>

#include "ui/app_list/app_list_model.h"

namespace app_list {

class AppListItemModel;

namespace test {

// Extends AppListModel with helper functions for use in tests.
class AppListTestModel : public AppListModel {
 public:
  AppListTestModel() {}

  // Populate the model with |n| items titled "Item #".
  void PopulateApps(int n);

  // Populate the model with an item titled "Item |id|".
  void PopulateAppWithId(int id);

  // Get a string of all apps in |model| joined with ','.
  std::string GetModelContent();

  AppListItemModel* CreateItem(const std::string& title);
  void AddItem(const std::string& title);

  // Call SetHighlighted on the specified item.
  void HighlightItemAt(int index);

 private:
  DISALLOW_COPY_AND_ASSIGN(AppListTestModel);
};

}  // namespace test
}  // namespace app_list

#endif  // UI_APP_LIST_TEST_APP_LIST_TEST_MODEL_H_
