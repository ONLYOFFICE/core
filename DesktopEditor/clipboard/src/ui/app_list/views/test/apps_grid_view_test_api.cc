// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/test/apps_grid_view_test_api.h"

#include "ui/app_list/views/apps_grid_view.h"

namespace app_list {
namespace test {

AppsGridViewTestApi::AppsGridViewTestApi(AppsGridView* view)
    : view_(view) {
}

AppsGridViewTestApi::~AppsGridViewTestApi() {
}

views::View* AppsGridViewTestApi::GetViewAtModelIndex(int index) const {
  return view_->view_model_.view_at(index);
}

void AppsGridViewTestApi::LayoutToIdealBounds() {
  view_->bounds_animator_.Cancel();
  view_->Layout();
}

void AppsGridViewTestApi::SetPageFlipDelay(int page_flip_delay_in_ms) {
  view_->page_flip_delay_in_ms_ = page_flip_delay_in_ms;
}

}  // namespace test;
}  // namespace app_list
