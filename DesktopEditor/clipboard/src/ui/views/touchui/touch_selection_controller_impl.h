// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_UI_VIEWS_TOUCHUI_TOUCH_SELECTION_CONTROLLER_IMPL_H_
#define UI_UI_VIEWS_TOUCHUI_TOUCH_SELECTION_CONTROLLER_IMPL_H_

#include "base/timer.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/gfx/point.h"
#include "ui/views/touchui/touch_editing_menu.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

// Touch specific implementation of TouchSelectionController. Responsible for
// displaying selection handles and menu elements relevant in a touch interface.
class VIEWS_EXPORT TouchSelectionControllerImpl
    : public ui::TouchSelectionController,
      public TouchEditingMenuController,
      public WidgetObserver {
 public:
  // Use TextSelectionController::create().
  explicit TouchSelectionControllerImpl(
      ui::TouchEditable* client_view);

  virtual ~TouchSelectionControllerImpl();

  // TextSelectionController.
  virtual void SelectionChanged() OVERRIDE;
  virtual bool IsHandleDragInProgress() OVERRIDE;

 private:
  friend class TouchSelectionControllerImplTest;
  class EditingHandleView;

  void SetDraggingHandle(EditingHandleView* handle);

  // Callback to inform the client view that the selection handle has been
  // dragged, hence selection may need to be updated.
  void SelectionHandleDragged(const gfx::Point& drag_pos);

  // Convenience method to convert a point from a selection handle's coordinate
  // system to that of the client view.
  void ConvertPointToClientView(EditingHandleView* source, gfx::Point* point);

  // Overridden from TouchEditingMenuController.
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual void OpenContextMenu() OVERRIDE;
  virtual void OnMenuClosed(TouchEditingMenuView* menu) OVERRIDE;

  // Overridden from WidgetObserver. We will observe the widget backing the
  // |client_view_| so that when its moved/resized, we can update the selection
  // handles appropriately.
  virtual void OnWidgetClosing(Widget* widget) OVERRIDE;
  virtual void OnWidgetBoundsChanged(Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE;

  // Time to show context menu.
  void ContextMenuTimerFired();

  void StartContextMenuTimer();

  // Convenience method to update the position/visibility of the context menu.
  void UpdateContextMenu(const gfx::Point& p1, const gfx::Point& p2);

  // Convenience method for hiding context menu.
  void HideContextMenu();

  // Convenience methods for testing.
  gfx::Point GetSelectionHandle1Position();
  gfx::Point GetSelectionHandle2Position();
  gfx::Point GetCursorHandlePosition();
  bool IsSelectionHandle1Visible();
  bool IsSelectionHandle2Visible();
  bool IsCursorHandleVisible();

  ui::TouchEditable* client_view_;
  Widget* client_widget_;
  scoped_ptr<EditingHandleView> selection_handle_1_;
  scoped_ptr<EditingHandleView> selection_handle_2_;
  scoped_ptr<EditingHandleView> cursor_handle_;
  TouchEditingMenuView* context_menu_;

  // Timer to trigger |context_menu| (|context_menu| is not shown if the
  // selection handles are being updated. It appears only when the handles are
  // stationary for a certain amount of time).
  base::OneShotTimer<TouchSelectionControllerImpl> context_menu_timer_;

  // Pointer to the SelectionHandleView being dragged during a drag session.
  EditingHandleView* dragging_handle_;

  DISALLOW_COPY_AND_ASSIGN(TouchSelectionControllerImpl);
};

class VIEWS_EXPORT ViewsTouchSelectionControllerFactory
    : public ui::TouchSelectionControllerFactory {
 public:
  ViewsTouchSelectionControllerFactory();

  // Overridden from ui::TouchSelectionControllerFactory.
  virtual ui::TouchSelectionController* create(
      ui::TouchEditable* client_view) OVERRIDE;
};

}  // namespace views

#endif  // UI_UI_VIEWS_TOUCHUI_TOUCH_SELECTION_CONTROLLER_IMPL_H_
