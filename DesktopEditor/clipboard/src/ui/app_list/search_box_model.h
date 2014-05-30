// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_BOX_MODEL_H_
#define UI_APP_LIST_SEARCH_BOX_MODEL_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "ui/app_list/app_list_export.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/selection_model.h"

namespace app_list {

class SearchBoxModelObserver;

// SearchBoxModel consisits of an icon, a hint text, a user text and a selection
// model. The icon is rendered to the side of the query editor. The hint text
// is used as query edit control's placeholder text and displayed when there is
// no user text in the control. The selection model and the text represents the
// text, cursor position and selected text in edit control.
class APP_LIST_EXPORT SearchBoxModel {
 public:
  SearchBoxModel();
  ~SearchBoxModel();

  // Sets/gets the icon on side of edit box.
  void SetIcon(const gfx::ImageSkia& icon);
  const gfx::ImageSkia& icon() const { return icon_; }

  // Sets/gets the hint text to display when there is in input.
  void SetHintText(const string16& hint_text);
  const string16& hint_text() const { return hint_text_; }

  // Sets/gets the selection model for the search box's Textfield.
  void SetSelectionModel(const gfx::SelectionModel& sel);
  const gfx::SelectionModel& selection_model() const {
    return selection_model_;
  }

  // Sets/gets the text for the search box's Textfield.
  void SetText(const string16& text);
  const string16& text() const { return text_; }

  void AddObserver(SearchBoxModelObserver* observer);
  void RemoveObserver(SearchBoxModelObserver* observer);

 private:
  gfx::ImageSkia icon_;
  string16 hint_text_;
  gfx::SelectionModel selection_model_;
  string16 text_;

  ObserverList<SearchBoxModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(SearchBoxModel);
};

}  // namespace app_list

#endif  // UI_APP_LIST_SEARCH_BOX_MODEL_H_
