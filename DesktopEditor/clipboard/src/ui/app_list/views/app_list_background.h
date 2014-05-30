// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_VIEWS_BACKGROUND_H_
#define UI_APP_LIST_APP_LIST_VIEWS_BACKGROUND_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/background.h"

namespace views {
class View;
}

namespace app_list {

// A class to paint bubble background.
class AppListBackground : public views::Background {
 public:
  AppListBackground(int corner_radius,
                    views::View* search_box_view);
  virtual ~AppListBackground();

 private:
  // views::Background overrides:
  virtual void Paint(gfx::Canvas* canvas, views::View* view) const OVERRIDE;

  const int corner_radius_;
  const views::View* search_box_view_;  // Owned by views hierarchy.

  DISALLOW_COPY_AND_ASSIGN(AppListBackground);
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_VIEWS_BACKGROUND_H_
