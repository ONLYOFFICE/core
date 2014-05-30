// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/test/app_list_test_view_delegate.h"

#include "ui/gfx/image/image_skia.h"

namespace app_list {
namespace test {

AppListTestViewDelegate::AppListTestViewDelegate()
    : activate_count_(0),
      dismiss_count_(0),
      last_activated_(NULL) {
}

AppListTestViewDelegate::~AppListTestViewDelegate() {}

SigninDelegate* AppListTestViewDelegate::GetSigninDelegate() {
  return NULL;
}

void AppListTestViewDelegate::ActivateAppListItem(AppListItemModel* item,
                                                  int event_flags) {
  last_activated_ = item;
  ++activate_count_;
}

void AppListTestViewDelegate::Dismiss() {
  ++dismiss_count_;
}

gfx::ImageSkia AppListTestViewDelegate::GetWindowIcon() {
  return gfx::ImageSkia();
}

string16 AppListTestViewDelegate::GetCurrentUserName() {
  return string16();
}

string16 AppListTestViewDelegate::GetCurrentUserEmail() {
  return string16();
}

}  // namespace test
}  // namespace app_list
