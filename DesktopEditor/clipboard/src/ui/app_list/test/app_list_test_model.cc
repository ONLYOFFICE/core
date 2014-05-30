// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/test/app_list_test_model.h"

#include "base/stringprintf.h"
#include "ui/app_list/app_list_item_model.h"

namespace app_list {
namespace test {

void AppListTestModel::PopulateApps(int n) {
  for (int i = 0; i < n; ++i)
    AddItem(base::StringPrintf("Item %d", i));
}

void AppListTestModel::PopulateAppWithId(int id) {
  AddItem(base::StringPrintf("Item %d", id));
}

std::string AppListTestModel::GetModelContent() {
  std::string content;
  for (size_t i = 0; i < apps()->item_count(); ++i) {
    if (i > 0)
      content += ',';
    content += apps()->GetItemAt(i)->title();
  }
  return content;
}

AppListItemModel* AppListTestModel::CreateItem(const std::string& title) {
  AppListItemModel* item = new AppListItemModel;
  item->SetTitle(title);
  return item;
}

void AppListTestModel::AddItem(const std::string& title) {
  apps()->Add(CreateItem(title));
}

void AppListTestModel::HighlightItemAt(int index) {
  AppListItemModel* item = apps()->GetItemAt(index);
  item->SetHighlighted(true);
}

}  // namespace test
}  // namespace app_list
