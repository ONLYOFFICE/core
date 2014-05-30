// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_BOX_VIEW_H_
#define UI_APP_LIST_VIEWS_SEARCH_BOX_VIEW_H_

#include <string>

#include "ui/app_list/search_box_model_observer.h"
#include "ui/app_list/views/app_list_menu_views.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/view.h"

namespace views {
class ImageView;
class MenuButton;
class Textfield;
}  // namespace views

namespace app_list {

class AppListViewDelegate;
class SearchBoxModel;
class SearchBoxViewDelegate;

// SearchBoxView consists of an icon and a Textfield. SearchBoxModel is its data
// model that controls what icon to display, what placeholder text to use for
// Textfield. The text and selection model part could be set to change the
// contents and selection model of the Textfield.
class SearchBoxView : public views::View,
                      public views::TextfieldController,
                      public views::MenuButtonListener,
                      public SearchBoxModelObserver {
 public:
  SearchBoxView(SearchBoxViewDelegate* delegate,
                AppListViewDelegate* view_delegate);
  virtual ~SearchBoxView();

  void SetModel(SearchBoxModel* model);

  bool HasSearch() const;
  void ClearSearch();

  views::Textfield* search_box() { return search_box_; }

  void set_contents_view(View* contents_view) {
    contents_view_ = contents_view;
  }

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

 private:
  // Updates model text and selection model with current Textfield info.
  void UpdateModel();

  // Fires query change notification.
  void NotifyQueryChanged();

  // Overridden from views::TextfieldController:
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  // Overridden from views::MenuButtonListener:
  virtual void OnMenuButtonClicked(View* source,
                                   const gfx::Point& point) OVERRIDE;

  // Overridden from SearchBoxModelObserver:
  virtual void IconChanged() OVERRIDE;
  virtual void HintTextChanged() OVERRIDE;
  virtual void SelectionModelChanged() OVERRIDE;
  virtual void TextChanged() OVERRIDE;

  SearchBoxViewDelegate* delegate_;  // Not owned.
  SearchBoxModel* model_;  // Owned by AppListModel.

  AppListMenuViews menu_;

  views::ImageView* icon_view_;  // Owned by views hierarchy.
  views::MenuButton* menu_button_;  // Owned by views hierarchy.
  views::Textfield* search_box_;  // Owned by views hierarchy.
  views::View* contents_view_;  // Owned by views hierarchy.

  DISALLOW_COPY_AND_ASSIGN(SearchBoxView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_SEARCH_BOX_VIEW_H_
