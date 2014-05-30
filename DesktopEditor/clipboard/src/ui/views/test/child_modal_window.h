// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_CHILD_WMODAL_WINDOW_H_
#define UI_VIEWS_TEST_CHILD_WMODAL_WINDOW_H_

#include "ui/views/controls/button/button.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

namespace views {
class LabelButton;
class NativeViewHost;
class Textfield;
class View;
class Widget;
namespace test {

void CreateChildModalParent(gfx::NativeView context);

class ChildModalParent : public WidgetDelegateView,
                         public ButtonListener,
                         public WidgetObserver {
 public:
  ChildModalParent(gfx::NativeView context);
  virtual ~ChildModalParent();

  void ShowChild();
  gfx::NativeWindow GetModalParent() const;
  gfx::NativeWindow GetChild() const;

 private:
  Widget* CreateChild();

  // Overridden from WidgetDelegate:
  virtual View* GetContentsView() OVERRIDE;
  virtual string16 GetWindowTitle() const OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;

  // Overridden from ButtonListener:
  virtual void ButtonPressed(Button* sender,
                             const ui::Event& event) OVERRIDE;

  // Overridden from WidgetObserver:
  virtual void OnWidgetDestroying(Widget* widget) OVERRIDE;

  // The button to toggle showing and hiding the child window. The child window
  // does not block input to this button.
  LabelButton* button_;

  // The text field to indicate the keyboard focus.
  Textfield* textfield_;

  // The host for the modal parent.
  NativeViewHost* host_;

  // The modal parent of the child window. The child window blocks input to this
  // view.
  gfx::NativeWindow modal_parent_;

  // The child window.
  Widget* child_;

  DISALLOW_COPY_AND_ASSIGN(ChildModalParent);
};

}  // namespace test
}  // namespace views

#endif  // UI_VIEWS_TEST_CHILD_WMODAL_WINDOW_H_
