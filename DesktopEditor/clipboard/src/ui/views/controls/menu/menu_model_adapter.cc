// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_model_adapter.h"

#include "base/logging.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/menu_model.h"
#include "ui/views/controls/menu/submenu_view.h"
#include "ui/views/views_delegate.h"

namespace views {

MenuModelAdapter::MenuModelAdapter(ui::MenuModel* menu_model)
    : menu_model_(menu_model),
      triggerable_event_flags_(ui::EF_LEFT_MOUSE_BUTTON |
                               ui::EF_RIGHT_MOUSE_BUTTON) {
  DCHECK(menu_model);
}

MenuModelAdapter::~MenuModelAdapter() {
}

void MenuModelAdapter::BuildMenu(MenuItemView* menu) {
  DCHECK(menu);

  // Clear the menu.
  if (menu->HasSubmenu()) {
    const int subitem_count = menu->GetSubmenu()->child_count();
    for (int i = 0; i < subitem_count; ++i)
      menu->RemoveMenuItemAt(0);
  }

  // Leave entries in the map if the menu is being shown.  This
  // allows the map to find the menu model of submenus being closed
  // so ui::MenuModel::MenuClosed() can be called.
  if (!menu->GetMenuController())
    menu_map_.clear();
  menu_map_[menu] = menu_model_;

  // Repopulate the menu.
  BuildMenuImpl(menu, menu_model_);
  menu->ChildrenChanged();
}

MenuItemView* MenuModelAdapter::CreateMenu() {
  MenuItemView* item = new MenuItemView(this);
  BuildMenu(item);
  return item;
}

MenuItemView* MenuModelAdapter::AppendMenuItem(MenuItemView* menu,
                                               ui::MenuModel* model,
                                               int index) {
  return menu->AppendMenuItemFromModel(model, index,
                                       model->GetCommandIdAt(index));
}

// MenuModelAdapter, MenuDelegate implementation:

void MenuModelAdapter::ExecuteCommand(int id) {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index)) {
    model->ActivatedAt(index);
    return;
  }

  NOTREACHED();
}

void MenuModelAdapter::ExecuteCommand(int id, int mouse_event_flags) {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index)) {
    model->ActivatedAt(index, mouse_event_flags);
    return;
  }

  NOTREACHED();
}

bool MenuModelAdapter::IsTriggerableEvent(MenuItemView* source,
                                          const ui::Event& e) {
  return e.type() == ui::ET_GESTURE_TAP ||
         e.type() == ui::ET_GESTURE_TAP_DOWN ||
         (e.IsMouseEvent() && (triggerable_event_flags_ & e.flags()) != 0);
}

bool MenuModelAdapter::GetAccelerator(int id,
                                      ui::Accelerator* accelerator) {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index))
    return model->GetAcceleratorAt(index, accelerator);

  NOTREACHED();
  return false;
}

string16 MenuModelAdapter::GetLabel(int id) const {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index))
    return model->GetLabelAt(index);

  NOTREACHED();
  return string16();
}

const gfx::Font* MenuModelAdapter::GetLabelFont(int id) const {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index)) {
    const gfx::Font* font = model->GetLabelFontAt(index);
    if (font)
      return font;
  }

  // This line may be reached for the empty menu item.
  return MenuDelegate::GetLabelFont(id);
}

bool MenuModelAdapter::IsCommandEnabled(int id) const {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index))
    return model->IsEnabledAt(index);

  NOTREACHED();
  return false;
}

bool MenuModelAdapter::IsItemChecked(int id) const {
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index))
    return model->IsItemCheckedAt(index);

  NOTREACHED();
  return false;
}

void MenuModelAdapter::SelectionChanged(MenuItemView* menu) {
  // Ignore selection of the root menu.
  if (menu == menu->GetRootMenuItem())
    return;

  const int id = menu->GetCommand();
  ui::MenuModel* model = menu_model_;
  int index = 0;
  if (ui::MenuModel::GetModelAndIndexForCommandId(id, &model, &index)) {
    model->HighlightChangedTo(index);
    return;
  }

  NOTREACHED();
}

void MenuModelAdapter::WillShowMenu(MenuItemView* menu) {
  // Look up the menu model for this menu.
  const std::map<MenuItemView*, ui::MenuModel*>::const_iterator map_iterator =
      menu_map_.find(menu);
  if (map_iterator != menu_map_.end()) {
    map_iterator->second->MenuWillShow();
    return;
  }

  NOTREACHED();
}

void MenuModelAdapter::WillHideMenu(MenuItemView* menu) {
  // Look up the menu model for this menu.
  const std::map<MenuItemView*, ui::MenuModel*>::const_iterator map_iterator =
      menu_map_.find(menu);
  if (map_iterator != menu_map_.end()) {
    map_iterator->second->MenuClosed();
    return;
  }

  NOTREACHED();
}

// MenuModelAdapter, private:

void MenuModelAdapter::BuildMenuImpl(MenuItemView* menu, ui::MenuModel* model) {
  DCHECK(menu);
  DCHECK(model);
  bool has_icons = model->HasIcons();
  const int item_count = model->GetItemCount();
  for (int i = 0; i < item_count; ++i) {
    MenuItemView* item = AppendMenuItem(menu, model, i);

    if (item)
      item->SetVisible(model->IsVisibleAt(i));

    if (model->GetTypeAt(i) == ui::MenuModel::TYPE_SUBMENU) {
      DCHECK(item);
      DCHECK_EQ(MenuItemView::SUBMENU, item->GetType());
      ui::MenuModel* submodel = model->GetSubmenuModelAt(i);
      DCHECK(submodel);
      BuildMenuImpl(item, submodel);
      has_icons = has_icons || item->has_icons();

      menu_map_[item] = submodel;
    }
  }

  menu->set_has_icons(has_icons);
}

}  // namespace views
