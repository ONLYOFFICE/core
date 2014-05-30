// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_ITEM_MODEL_H_
#define UI_APP_LIST_APP_LIST_ITEM_MODEL_H_

#include <string>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/app_list/app_list_export.h"
#include "ui/gfx/image/image_skia.h"

namespace ui {
class MenuModel;
}

namespace app_list {

class AppListItemModelObserver;

// AppListItemModel provides icon and title to be shown in a AppListItemView
// and action to be executed when the AppListItemView is activated.
class APP_LIST_EXPORT AppListItemModel {
 public:
  AppListItemModel();
  virtual ~AppListItemModel();

  void SetIcon(const gfx::ImageSkia& icon, bool has_shadow);
  const gfx::ImageSkia& icon() const { return icon_; }
  bool has_shadow() const { return has_shadow_; }

  void SetTitle(const std::string& title);
  const std::string& title() const { return title_; }

  void SetHighlighted(bool highlighted);
  bool highlighted() const { return highlighted_; }

  void SetIsInstalling(bool is_installing);
  bool is_installing() const { return is_installing_; }

  void SetPercentDownloaded(int percent_downloaded);
  int percent_downloaded() const { return percent_downloaded_; }

  void AddObserver(AppListItemModelObserver* observer);
  void RemoveObserver(AppListItemModelObserver* observer);

  // Returns the context menu model for this item.
  // Note the returned menu model is owned by this item.
  virtual ui::MenuModel* GetContextMenuModel();

 private:
  gfx::ImageSkia icon_;
  bool has_shadow_;
  std::string title_;
  bool highlighted_;
  bool is_installing_;
  int percent_downloaded_;

  ObserverList<AppListItemModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(AppListItemModel);
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_ITEM_MODEL_H_
