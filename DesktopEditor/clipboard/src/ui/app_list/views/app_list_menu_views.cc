// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/app_list_menu_views.h"

#include "grit/ui_resources.h"
#include "ui/app_list/app_list_view_delegate.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/menu/menu_config.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/controls/menu/submenu_view.h"
#include "ui/views/layout/grid_layout.h"

using views::MenuItemView;

namespace app_list {

namespace {

class CurrentUserView : public views::View {
 public:
  CurrentUserView(const string16& user_name,
                  const string16& user_email,
                  const gfx::ImageSkia& icon) {
    const views::MenuConfig& menu_config = views::MenuConfig::instance(NULL);
    views::GridLayout* layout = new views::GridLayout(this);
    int item_right_padding = menu_config.label_to_arrow_padding +
                             menu_config.arrow_width +
                             menu_config.arrow_to_edge_padding;

    layout->SetInsets(0, 0, 0, item_right_padding);
    SetLayoutManager(layout);

    views::ColumnSet* columns = layout->AddColumnSet(0);
    columns->AddColumn(views::GridLayout::FILL,
                       views::GridLayout::FILL,
                       0,
                       views::GridLayout::USE_PREF,
                       0,
                       menu_config.check_width + menu_config.item_left_margin);
    columns->AddColumn(views::GridLayout::FILL,
                       views::GridLayout::FILL,
                       1,
                       views::GridLayout::USE_PREF,
                       0,
                       0);

    layout->StartRow(0, 0);
    views::ImageView* image_view = new views::ImageView();
    image_view->SetImage(icon);
    layout->AddView(image_view);

    views::Label* user_name_view = new views::Label(user_name);
    user_name_view->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    layout->AddView(user_name_view);

    layout->StartRow(0, 0);
    views::Label* user_email_view = new views::Label(user_email);
    user_email_view->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    user_email_view->SetEnabled(false);
    layout->SkipColumns(1);
    layout->AddView(user_email_view);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CurrentUserView);
};

class CurrentUserMenuItem : public MenuItemView {
 public:
  CurrentUserMenuItem(MenuItemView* parent,
                      int id,
                      const string16& user_name,
                      const string16& user_email,
                      const gfx::ImageSkia& icon)
      : MenuItemView(parent, id, MenuItemView::NORMAL) {
    AddChildView(new CurrentUserView(user_name, user_email, icon));
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CurrentUserMenuItem);
};

class AppListMenuModelAdapter : public views::MenuModelAdapter {
 public:
  AppListMenuModelAdapter(ui::MenuModel* menu_model,
                          AppListViewDelegate* delegate)
      : views::MenuModelAdapter(menu_model),
        delegate_(delegate) {}
  virtual ~AppListMenuModelAdapter() {}

  // Overridden from views::MenuModelAdapter:
  virtual MenuItemView* AppendMenuItem(MenuItemView* menu,
                                       ui::MenuModel* model,
                                       int index) OVERRIDE {
    if (!delegate_)
      return NULL;

    int id = model->GetCommandIdAt(index);
    if (id != AppListMenu::CURRENT_USER)
      return MenuModelAdapter::AppendMenuItem(menu, model, index);

    ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
    MenuItemView* item = new CurrentUserMenuItem(
        menu,
        id,
        delegate_->GetCurrentUserName(),
        delegate_->GetCurrentUserEmail(),
        *rb.GetImageSkiaNamed(IDR_APP_LIST_USER_INDICATOR));
    menu->CreateSubmenu();
    menu->GetSubmenu()->AddChildViewAt(item, index);
    return item;
  }

 private:
  AppListViewDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(AppListMenuModelAdapter);
};

}  // namespace

AppListMenuViews::AppListMenuViews(AppListViewDelegate* delegate)
    : AppListMenu(delegate) {
  menu_delegate_.reset(new AppListMenuModelAdapter(menu_model(), delegate));
  menu_ = new MenuItemView(menu_delegate_.get());
  menu_runner_.reset(new views::MenuRunner(menu_));
  menu_delegate_->BuildMenu(menu_);
}

AppListMenuViews::~AppListMenuViews() {}

void AppListMenuViews::RunMenuAt(views::MenuButton* button,
                                 const gfx::Point& point) {
  ignore_result(menu_runner_->RunMenuAt(button->GetWidget(), button,
                                        gfx::Rect(point, gfx::Size()),
                                        MenuItemView::TOPRIGHT, 0));
}

void AppListMenuViews::Cancel() {
  menu_runner_->Cancel();
}

}  // namespace app_list