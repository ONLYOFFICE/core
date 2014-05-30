// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_TEST_APPS_GRID_VIEW_TEST_API_H_
#define UI_APP_LIST_VIEWS_TEST_APPS_GRID_VIEW_TEST_API_H_

#include "base/basictypes.h"

namespace views {
class View;
}

namespace app_list {

class AppsGridView;

namespace test {

class AppsGridViewTestApi {
 public:
  explicit AppsGridViewTestApi(AppsGridView* view);
  ~AppsGridViewTestApi();

  views::View* GetViewAtModelIndex(int index) const;

  void LayoutToIdealBounds();

  void SetPageFlipDelay(int page_flip_delay_in_ms);

 private:
  AppsGridView* view_;

  DISALLOW_COPY_AND_ASSIGN(AppsGridViewTestApi);
};

}  // namespace test
}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_TEST_APPS_GRID_VIEW_TEST_API_H_
