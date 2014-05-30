// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_CONTENTS_VIEW_H_
#define UI_APP_LIST_VIEWS_CONTENTS_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/view.h"

namespace views {
class BoundsAnimator;
class ViewModel;
}

namespace app_list {

class AppListMainView;
class AppListModel;
class PaginationModel;

// A view to manage sub views under the search box (apps grid view + page
// switcher and search results). The two sets of sub views are mutually
// exclusive. ContentsView manages a show state to choose one set to show
// and animates the transition between show states.
class ContentsView : public views::View {
 public:
  ContentsView(AppListMainView* app_list_main_view,
               PaginationModel* pagination_model);
  virtual ~ContentsView();

  void SetModel(AppListModel* model);

  void ShowSearchResults(bool show);

  void Prerender();

 private:
  enum ShowState {
    SHOW_APPS,
    SHOW_SEARCH_RESULTS,
  };

  // Sets show state.
  void SetShowState(ShowState show_state);

  // Invoked when show state is changed.
  void ShowStateChanged();

  void CalculateIdealBounds();
  void AnimateToIdealBounds();

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  // Overridden from ui::EventHandler:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;

  ShowState show_state_;
  PaginationModel* pagination_model_;  // Owned by AppListController.

  scoped_ptr<views::ViewModel> view_model_;
  scoped_ptr<views::BoundsAnimator> bounds_animator_;

  DISALLOW_COPY_AND_ASSIGN(ContentsView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_CONTENTS_VIEW_H_
