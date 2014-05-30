// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/app_list_menu.h"

#include "grit/ui_strings.h"
#include "ui/app_list/app_list_view_delegate.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/menu_separator_types.h"
#include "ui/base/resource/resource_bundle.h"

namespace app_list {

AppListMenu::AppListMenu(AppListViewDelegate* delegate)
    : menu_model_(this),
      delegate_(delegate) {
  InitMenu();
}

AppListMenu::~AppListMenu() {}

void AppListMenu::InitMenu() {
  menu_model_.AddItem(CURRENT_USER, string16());
  menu_model_.AddSeparator(ui::NORMAL_SEPARATOR);

  menu_model_.AddItem(SHOW_SETTINGS, l10n_util::GetStringUTF16(
      IDS_APP_LIST_OPEN_SETTINGS));

  menu_model_.AddItem(SHOW_FEEDBACK, l10n_util::GetStringUTF16(
      IDS_APP_LIST_OPEN_FEEDBACK));
}

bool AppListMenu::IsCommandIdChecked(int command_id) const {
  return false;
}

bool AppListMenu::IsCommandIdEnabled(int command_id) const {
  return true;
}

bool AppListMenu::GetAcceleratorForCommandId(int command_id,
                                             ui::Accelerator* accelerator) {
  return false;
}

void AppListMenu::ExecuteCommand(int command_id, int event_flags) {
  switch (command_id) {
    case CURRENT_USER:
      break;  // Do nothing.
    case SHOW_SETTINGS:
      delegate_->OpenSettings();
      break;
    case SHOW_FEEDBACK:
      delegate_->OpenFeedback();
      break;
    default:
      NOTREACHED();
  }
}

}  // namespace app_list
