// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/search_box_model.h"

#include "ui/app_list/search_box_model_observer.h"

namespace app_list {

SearchBoxModel::SearchBoxModel() {
}

SearchBoxModel::~SearchBoxModel() {
}

void SearchBoxModel::SetIcon(const gfx::ImageSkia& icon) {
  icon_ = icon;
  FOR_EACH_OBSERVER(SearchBoxModelObserver, observers_, IconChanged());
}

void SearchBoxModel::SetHintText(const string16& hint_text) {
  if (hint_text_ == hint_text)
    return;

  hint_text_ = hint_text;
  FOR_EACH_OBSERVER(SearchBoxModelObserver, observers_, HintTextChanged());
}

void SearchBoxModel::SetSelectionModel(const gfx::SelectionModel& sel) {
  if (selection_model_ == sel)
    return;

  selection_model_ = sel;
  FOR_EACH_OBSERVER(SearchBoxModelObserver,
                    observers_,
                    SelectionModelChanged());
}

void SearchBoxModel::SetText(const string16& text) {
  if (text_ == text)
    return;

  text_ = text;
  FOR_EACH_OBSERVER(SearchBoxModelObserver, observers_, TextChanged());
}

void SearchBoxModel::AddObserver(SearchBoxModelObserver* observer) {
  observers_.AddObserver(observer);
}

void SearchBoxModel::RemoveObserver(SearchBoxModelObserver* observer) {
  observers_.RemoveObserver(observer);
}

}  // namespace app_list
