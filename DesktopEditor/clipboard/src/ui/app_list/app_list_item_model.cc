// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/app_list_item_model.h"

#include "base/logging.h"
#include "ui/app_list/app_list_item_model_observer.h"

namespace app_list {

AppListItemModel::AppListItemModel()
    : highlighted_(false),
      is_installing_(false),
      percent_downloaded_(-1) {
}

AppListItemModel::~AppListItemModel() {
}

void AppListItemModel::SetIcon(const gfx::ImageSkia& icon, bool has_shadow) {
  icon_ = icon;
  has_shadow_ = has_shadow;
  FOR_EACH_OBSERVER(AppListItemModelObserver, observers_, ItemIconChanged());
}

void AppListItemModel::SetTitle(const std::string& title) {
  if (title_ == title)
    return;

  title_ = title;
  FOR_EACH_OBSERVER(AppListItemModelObserver, observers_, ItemTitleChanged());
}

void AppListItemModel::SetHighlighted(bool highlighted) {
  if (highlighted_ == highlighted)
    return;

  highlighted_ = highlighted;
  FOR_EACH_OBSERVER(AppListItemModelObserver,
                    observers_,
                    ItemHighlightedChanged());
}

void AppListItemModel::SetIsInstalling(bool is_installing) {
  if (is_installing_ == is_installing)
    return;

  is_installing_ = is_installing;
  FOR_EACH_OBSERVER(AppListItemModelObserver,
                    observers_,
                    ItemIsInstallingChanged());
}

void AppListItemModel::SetPercentDownloaded(int percent_downloaded) {
  if (percent_downloaded_ == percent_downloaded)
    return;

  percent_downloaded_ = percent_downloaded;
  FOR_EACH_OBSERVER(AppListItemModelObserver,
                    observers_,
                    ItemPercentDownloadedChanged());
}

void AppListItemModel::AddObserver(AppListItemModelObserver* observer) {
  observers_.AddObserver(observer);
}

void AppListItemModel::RemoveObserver(AppListItemModelObserver* observer) {
  observers_.RemoveObserver(observer);
}

ui::MenuModel* AppListItemModel::GetContextMenuModel() {
  return NULL;
}

}  // namespace app_list
