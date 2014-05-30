// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_MENU_VIEWS_H_
#define UI_APP_LIST_VIEWS_APP_LIST_MENU_VIEWS_H_

#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_menu.h"
#include "ui/views/controls/menu/menu_model_adapter.h"
#include "ui/views/controls/menu/menu_runner.h"

namespace gfx {
class Point;
}

namespace views {
class MenuButton;
}

namespace app_list {

// Views implementation of the app list menu.
// TODO(benwells): We should consider moving this into Chrome.
class AppListMenuViews : public app_list::AppListMenu {
 public:
  explicit AppListMenuViews(AppListViewDelegate* delegate);
  virtual ~AppListMenuViews();

  void RunMenuAt(views::MenuButton* button, const gfx::Point& point);
  void Cancel();

 private:
  // The context menu itself and its contents.
  scoped_ptr<views::MenuModelAdapter> menu_delegate_;
  views::MenuItemView* menu_;  // Owned by menu_runner_.
  scoped_ptr<views::MenuRunner> menu_runner_;

  DISALLOW_COPY_AND_ASSIGN(AppListMenuViews);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_APP_LIST_MENU_VIEWS_H_
