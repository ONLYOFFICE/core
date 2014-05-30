// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_MENU_H_
#define UI_APP_LIST_APP_LIST_MENU_H_

#include "ui/base/models/simple_menu_model.h"

namespace app_list {

class AppListViewDelegate;

// Menu for the app list. This is shown in the top right hand corner of the
// app list.
// TODO(benwells): We should consider moving this into Chrome.
class AppListMenu : public ui::SimpleMenuModel::Delegate {
 public:
  enum AppListMenuCommands {
    CURRENT_USER,
    SHOW_SETTINGS,
    SHOW_FEEDBACK,
  };

  explicit AppListMenu(AppListViewDelegate* delegate);
  virtual ~AppListMenu();

 protected:
  ui::SimpleMenuModel* menu_model() { return &menu_model_; }

 private:
  void InitMenu();

  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  ui::SimpleMenuModel menu_model_;
  AppListViewDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(AppListMenu);
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_MENU_H_
