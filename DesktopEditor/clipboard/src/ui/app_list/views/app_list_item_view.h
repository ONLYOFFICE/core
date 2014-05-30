// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_ITEM_VIEW_H_
#define UI_APP_LIST_VIEWS_APP_LIST_ITEM_VIEW_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_item_model_observer.h"
#include "ui/gfx/shadow_value.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/custom_button.h"

class SkBitmap;

namespace views {
class ImageView;
class Label;
class MenuRunner;
}

namespace app_list {

class AppListItemModel;
class AppsGridView;
class CachedLabel;

class APP_LIST_EXPORT AppListItemView : public views::CustomButton,
                                        public views::ContextMenuController,
                                        public AppListItemModelObserver {
 public:
  // Internal class name.
  static const char kViewClassName[];

  AppListItemView(AppsGridView* apps_grid_view, AppListItemModel* model);
  virtual ~AppListItemView();

  void SetIconSize(const gfx::Size& size);

  void Prerender();

  AppListItemModel* model() const { return model_; }

 private:
  enum UIState {
    UI_STATE_NORMAL,    // Normal UI (icon + label)
    UI_STATE_DRAGGING,  // Dragging UI (scaled icon only)
  };

  // Get icon from model and schedule background processing.
  void UpdateIcon();

  void SetUIState(UIState state);

  // Sets |touch_dragging_| flag and updates UI.
  void SetTouchDragging(bool touch_dragging);

  // Invoked when |mouse_drag_timer_| fires to show dragging UI.
  void OnMouseDragTimer();

  // AppListItemModelObserver overrides:
  virtual void ItemIconChanged() OVERRIDE;
  virtual void ItemTitleChanged() OVERRIDE;
  virtual void ItemHighlightedChanged() OVERRIDE;
  virtual void ItemIsInstallingChanged() OVERRIDE;
  virtual void ItemPercentDownloadedChanged() OVERRIDE;

  // views::View overrides:
  virtual std::string GetClassName() const OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  // views::ContextMenuController overrides:
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point) OVERRIDE;

  // views::CustomButton overrides:
  virtual void StateChanged() OVERRIDE;
  virtual bool ShouldEnterPushedState(const ui::Event& event) OVERRIDE;

  // views::View overrides:
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;

  // ui::EventHandler overrides:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  AppListItemModel* model_;  // Owned by AppListModel::Apps.

  AppsGridView* apps_grid_view_;  // Owned by views hierarchy.
  views::ImageView* icon_;  // Owned by views hierarchy.
  CachedLabel* title_;  // Owned by views hierarchy.

  scoped_ptr<views::MenuRunner> context_menu_runner_;

  gfx::Size icon_size_;
  gfx::ShadowValues icon_shadows_;

  UIState ui_state_;

  // True if scroll gestures should contribute to dragging.
  bool touch_dragging_;

  // A timer to defer showing drag UI when mouse is pressed.
  base::OneShotTimer<AppListItemView> mouse_drag_timer_;

  DISALLOW_COPY_AND_ASSIGN(AppListItemView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_APP_LIST_ITEM_VIEW_H_
