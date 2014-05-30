// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_BUTTON_DROPDOWN_H_
#define UI_VIEWS_CONTROLS_BUTTON_BUTTON_DROPDOWN_H_

#include "base/memory/weak_ptr.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/image_button.h"

namespace ui {
class MenuModel;
}

namespace views {

class MenuRunner;

////////////////////////////////////////////////////////////////////////////////
//
// ButtonDropDown
//
// A button class that when pressed (and held) or pressed (and drag down) will
// display a menu
//
////////////////////////////////////////////////////////////////////////////////
class VIEWS_EXPORT ButtonDropDown : public ImageButton,
                                    public ContextMenuController {
 public:
  // The button's class name.
  static const char kViewClassName[];

  // Takes ownership of the |model|.
  ButtonDropDown(ButtonListener* listener, ui::MenuModel* model);
  virtual ~ButtonDropDown();

  // If menu is currently pending for long press - stop it.
  void ClearPendingMenu();

  // Indicates if menu is currently showing.
  bool IsMenuShowing() const;

  // Overridden from views::View
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;
  // Showing the drop down results in a MouseCaptureLost, we need to ignore it.
  virtual void OnMouseCaptureLost() OVERRIDE {}
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  // Overridden from views::ContextMenuController
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point) OVERRIDE;

 protected:
  // Overridden from CustomButton. Returns true if the button should become
  // pressed when a user holds the mouse down over the button. For this
  // implementation, both left and right mouse buttons can trigger a change
  // to the PUSHED state.
  virtual bool ShouldEnterPushedState(const ui::Event& event) OVERRIDE;

  // Returns if menu should be shown. Override this to change default behavior.
  virtual bool ShouldShowMenu();

  // Function to show the dropdown menu.
  virtual void ShowDropDownMenu();

 private:
  // The model that populates the attached menu.
  scoped_ptr<ui::MenuModel> model_;

  // Indicates if menu is currently showing.
  bool menu_showing_;

  // Y position of mouse when left mouse button is pressed
  int y_position_on_lbuttondown_;

  // Menu runner to display drop down menu.
  scoped_ptr<MenuRunner> menu_runner_;

  // A factory for tasks that show the dropdown context menu for the button.
  base::WeakPtrFactory<ButtonDropDown> show_menu_factory_;

  DISALLOW_COPY_AND_ASSIGN(ButtonDropDown);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_BUTTON_BUTTON_DROPDOWN_H_
