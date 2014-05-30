// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/models/menu_model.h"

namespace ui {

bool MenuModel::IsVisibleAt(int index) const {
  return true;
}

// static
bool MenuModel::GetModelAndIndexForCommandId(int command_id,
                                             MenuModel** model,
                                             int* index) {
  const int item_count = (*model)->GetItemCount();
  for (int i = 0; i < item_count; ++i) {
    const int candidate_index = i;
    if ((*model)->GetTypeAt(candidate_index) == TYPE_SUBMENU) {
      MenuModel* submenu_model = (*model)->GetSubmenuModelAt(candidate_index);
      if (GetModelAndIndexForCommandId(command_id, &submenu_model, index)) {
        *model = submenu_model;
        return true;
      }
    }
    if ((*model)->GetCommandIdAt(candidate_index) == command_id) {
      *index = candidate_index;
      return true;
    }
  }
  return false;
}

string16 MenuModel::GetSublabelAt(int index) const {
  return string16();
}

const gfx::Font* MenuModel::GetLabelFontAt(int index) const {
  return NULL;
}

const gfx::Font* MenuModel::GetSublabelFontAt(int index) const {
  return NULL;
}

// Default implementation ignores the event flags.
void MenuModel::ActivatedAt(int index, int event_flags) {
  ActivatedAt(index);
}

}  // namespace ui
