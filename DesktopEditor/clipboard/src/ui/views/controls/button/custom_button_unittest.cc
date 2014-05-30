// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/custom_button.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/layout.h"
#include "ui/gfx/screen.h"
#include "ui/views/test/views_test_base.h"

namespace views {

namespace {

class TestCustomButton : public CustomButton {
 public:
  explicit TestCustomButton(ButtonListener* listener)
      : CustomButton(listener) {
  }

  virtual ~TestCustomButton() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TestCustomButton);
};

}  // namespace

typedef ViewsTestBase CustomButtonTest;

// Tests that hover state changes correctly when visiblity/enableness changes.
TEST_F(CustomButtonTest, HoverStateOnVisibilityChange) {
  // Create a widget so that the CustomButton can query the hover state
  // correctly.
  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(0, 0, 650, 650);
  widget->Init(params);
  widget->Show();

  // Position the widget in a way so that it is under the cursor.
  gfx::Point cursor = gfx::Screen::GetScreenFor(
      widget->GetNativeView())->GetCursorScreenPoint();
  gfx::Rect widget_bounds = widget->GetWindowBoundsInScreen();
  widget_bounds.set_origin(cursor);
  widget->SetBounds(widget_bounds);

  TestCustomButton* button = new TestCustomButton(NULL);
  widget->SetContentsView(button);

  gfx::Point center(10, 10);
  button->OnMousePressed(ui::MouseEvent(ui::ET_MOUSE_PRESSED, center, center,
                                        ui::EF_LEFT_MOUSE_BUTTON));
  EXPECT_EQ(CustomButton::STATE_PRESSED, button->state());

  button->OnMouseReleased(ui::MouseEvent(ui::ET_MOUSE_RELEASED, center, center,
                                         ui::EF_LEFT_MOUSE_BUTTON));
  EXPECT_EQ(CustomButton::STATE_HOVERED, button->state());

  button->SetEnabled(false);
  EXPECT_EQ(CustomButton::STATE_DISABLED, button->state());

  button->SetEnabled(true);
  EXPECT_EQ(CustomButton::STATE_HOVERED, button->state());

  button->SetVisible(false);
  EXPECT_EQ(CustomButton::STATE_NORMAL, button->state());

  button->SetVisible(true);
  EXPECT_EQ(CustomButton::STATE_HOVERED, button->state());
}

}  // namespace views
