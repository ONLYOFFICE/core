// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APPS_GRID_VIEW_H_
#define UI_APP_LIST_VIEWS_APPS_GRID_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/timer.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/app_list_model_observer.h"
#include "ui/app_list/pagination_model_observer.h"
#include "ui/base/models/list_model_observer.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"
#include "ui/views/view_model.h"

namespace views {
class ButtonListener;
}

namespace app_list {

namespace test {
class AppsGridViewTestApi;
}

class AppListItemView;
class AppsGridViewDelegate;
class PageSwitcher;
class PaginationModel;

// AppsGridView displays a grid for AppListModel::Apps sub model.
class APP_LIST_EXPORT AppsGridView : public views::View,
                                     public views::ButtonListener,
                                     public ui::ListModelObserver,
                                     public PaginationModelObserver,
                                     public AppListModelObserver {
 public:
  enum Pointer {
    NONE,
    MOUSE,
    TOUCH,
  };

  AppsGridView(AppsGridViewDelegate* delegate,
               PaginationModel* pagination_model);
  virtual ~AppsGridView();

  // Sets fixed layout parameters. After setting this, CalculateLayout below
  // is no longer called to dynamically choosing those layout params.
  void SetLayout(int icon_size, int cols, int rows_per_page);

  // Sets |model| to use. Note this does not take ownership of |model|.
  void SetModel(AppListModel* model);

  void SetSelectedView(views::View* view);
  void ClearSelectedView(views::View* view);
  bool IsSelectedView(const views::View* view) const;

  // Ensures the view is visible. Note that if there is a running page
  // transition, this does nothing.
  void EnsureViewVisible(const views::View* view);

  void InitiateDrag(views::View* view,
                    Pointer pointer,
                    const ui::LocatedEvent& event);
  void UpdateDrag(views::View* view,
                  Pointer pointer,
                  const ui::LocatedEvent& event);
  void EndDrag(bool cancel);
  bool IsDraggedView(const views::View* view) const;

  // Prerenders the icons on and around |page_index|.
  void Prerender(int page_index);

  bool has_dragged_view() const { return drag_view_ != NULL; }
  bool dragging() const { return drag_pointer_ != NONE; }

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;

 private:
  friend class app_list::test::AppsGridViewTestApi;

  // Represents the index to an item view in the grid.
  struct Index {
    Index() : page(-1), slot(-1) {}
    Index(int page, int slot) : page(page), slot(slot) {}

    bool operator==(const Index& other) const {
      return page == other.page && slot == other.slot;
    }
    bool operator!=(const Index& other) const {
      return page != other.page || slot != other.slot;
    }

    int page;  // Which page an item view is on.
    int slot;  // Which slot in the page an item view is in.
  };

  int tiles_per_page() const { return cols_ * rows_per_page_; }

  // Updates from model.
  void Update();

  // Updates page splits for item views.
  void UpdatePaging();

  // Updates the number of pulsing block views based on AppListModel status and
  // number of apps.
  void UpdatePulsingBlockViews();

  views::View* CreateViewForItemAtIndex(size_t index);

  void SetSelectedItemByIndex(const Index& index);
  bool IsValidIndex(const Index& index) const;

  Index GetIndexOfView(const views::View* view) const;
  views::View* GetViewAtIndex(const Index& index) const;

  void MoveSelected(int page_delta, int slot_delta);

  void CalculateIdealBounds();
  void AnimateToIdealBounds();

  // Invoked when the given |view|'s current bounds and target bounds are on
  // different rows. To avoid moving diagonally, |view| would be put into a
  // slot prior |target| and fade in while moving to |target|. In the meanwhile,
  // a layer copy of |view| would start at |current| and fade out while moving
  // to succeeding slot of |current|. |animate_current| controls whether to run
  // fading out animation from |current|. |animate_target| controls whether to
  // run fading in animation to |target|.
  void AnimationBetweenRows(views::View* view,
                            bool animate_current,
                            const gfx::Rect& current,
                            bool animate_target,
                            const gfx::Rect& target);

  // Extracts drag location info from |event| into |drag_point|.
  void ExtractDragLocation(const ui::LocatedEvent& event,
                           gfx::Point* drag_point);

  // Calculates |drop_target_| based on |drag_point|. |drag_point| is in the
  // grid view's coordinates. When |use_page_button_hovering| is true and
  // |drag_point| is hovering on a page button, use the last slot on that page
  // as drop target.
  void CalculateDropTarget(const gfx::Point& drag_point,
                           bool use_page_button_hovering);

  // Starts the page flip timer if |drag_point| is in left/right side page flip
  // zone or is over page switcher.
  void MaybeStartPageFlipTimer(const gfx::Point& drag_point);

  // Invoked when |page_flip_timer_| fires.
  void OnPageFlipTimer();

  // Updates |model_| to move item represented by |item_view| to |target| slot.
  void MoveItemInModel(views::View* item_view, const Index& target);

  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  // Overridden from ListModelObserver:
  virtual void ListItemsAdded(size_t start, size_t count) OVERRIDE;
  virtual void ListItemsRemoved(size_t start, size_t count) OVERRIDE;
  virtual void ListItemMoved(size_t index, size_t target_index) OVERRIDE;
  virtual void ListItemsChanged(size_t start, size_t count) OVERRIDE;

  // Overridden from PaginationModelObserver:
  virtual void TotalPagesChanged() OVERRIDE;
  virtual void SelectedPageChanged(int old_selected, int new_selected) OVERRIDE;
  virtual void TransitionChanged() OVERRIDE;

  // Overridden from AppListModelObserver:
  virtual void OnAppListModelStatusChanged() OVERRIDE;

  AppListModel* model_;  // Owned by AppListView.
  AppsGridViewDelegate* delegate_;
  PaginationModel* pagination_model_;  // Owned by AppListController.
  PageSwitcher* page_switcher_view_;  // Owned by views hierarchy.

  gfx::Size icon_size_;
  int cols_;
  int rows_per_page_;

  // Tracks app item views. There is a view per item in |model_|.
  views::ViewModel view_model_;

  // Tracks pulsing block views.
  views::ViewModel pulsing_blocks_model_;

  views::View* selected_view_;

  views::View* drag_view_;
  gfx::Point drag_start_;
  Pointer drag_pointer_;
  Index drop_target_;

  // Last mouse drag location in this view's coordinates.
  gfx::Point last_drag_point_;

  // Timer to auto flip page when dragging an item near the left/right edges.
  base::OneShotTimer<AppsGridView> page_flip_timer_;

  // Target page to switch to when |page_flip_timer_| fires.
  int page_flip_target_;

  // Delay in milliseconds of when |page_flip_timer_| should fire after user
  // drags an item near the edges.
  int page_flip_delay_in_ms_;

  views::BoundsAnimator bounds_animator_;

  DISALLOW_COPY_AND_ASSIGN(AppsGridView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_APPS_GRID_VIEW_H_
