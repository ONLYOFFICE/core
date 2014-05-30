// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/focus/focus_manager.h"

#include "base/memory/scoped_ptr.h"
#include "base/run_loop.h"
#include "base/utf_string_conversions.h"
#include "ui/base/events/event.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/focus/focus_manager_test.h"
#include "ui/views/widget/widget.h"

namespace views {

namespace {

class MessageTrackingView : public View {
 public:
  MessageTrackingView() : accelerator_pressed_(false) {
  }

  void Reset() {
    accelerator_pressed_ = false;
    keys_pressed_.clear();
    keys_released_.clear();
  }

  const std::vector<ui::KeyboardCode>& keys_pressed() const {
    return keys_pressed_;
  }

  const std::vector<ui::KeyboardCode>& keys_released() const {
    return keys_released_;
  }

  bool accelerator_pressed() const {
    return accelerator_pressed_;
  }

  // Overridden from View:
  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE {
    keys_pressed_.push_back(e.key_code());
    return true;
  }
  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE {
    keys_released_.push_back(e.key_code());
    return true;
  }
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE {
    accelerator_pressed_ = true;
    return true;
  }

 private:
  bool accelerator_pressed_;
  std::vector<ui::KeyboardCode> keys_pressed_;
  std::vector<ui::KeyboardCode> keys_released_;

  DISALLOW_COPY_AND_ASSIGN(MessageTrackingView);
};

}  // namespace

// Test that when activating/deactivating the top window, the focus is stored/
// restored properly.
TEST_F(FocusManagerTest, FocusStoreRestore) {
  // Simulate an activate, otherwise the deactivate isn't going to do anything.
  SimulateActivateWindow();

  LabelButton* button = new LabelButton(NULL, ASCIIToUTF16("Press me"));
  button->SetStyle(Button::STYLE_NATIVE_TEXTBUTTON);
  View* view = new View();
  view->set_focusable(true);

  GetContentsView()->AddChildView(button);
  button->SetBounds(10, 10, 200, 30);
  GetContentsView()->AddChildView(view);
  RunPendingMessages();

  TestFocusChangeListener listener;
  AddFocusChangeListener(&listener);

  view->RequestFocus();
  RunPendingMessages();

  // Required for VS2010: http://connect.microsoft.com/VisualStudio/feedback/details/520043/error-converting-from-null-to-a-pointer-type-in-std-pair
  views::View* null_view = NULL;

  // Deacivate the window, it should store its focus.
  SimulateDeactivateWindow();
  EXPECT_EQ(NULL, GetFocusManager()->GetFocusedView());
  ASSERT_EQ(2, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(null_view, view));
  EXPECT_TRUE(listener.focus_changes()[1] == ViewPair(view, null_view));
  listener.ClearFocusChanges();

  // Reactivate, focus should come-back to the previously focused view.
  SimulateActivateWindow();
  EXPECT_EQ(view, GetFocusManager()->GetFocusedView());
  ASSERT_EQ(1, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(null_view, view));
  listener.ClearFocusChanges();

  // Same test with a NativeControl.
  button->RequestFocus();
  SimulateDeactivateWindow();
  EXPECT_EQ(NULL, GetFocusManager()->GetFocusedView());
  ASSERT_EQ(2, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(view, button));
  EXPECT_TRUE(listener.focus_changes()[1] == ViewPair(button, null_view));
  listener.ClearFocusChanges();

  SimulateActivateWindow();
  EXPECT_EQ(button, GetFocusManager()->GetFocusedView());
  ASSERT_EQ(1, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(null_view, button));
  listener.ClearFocusChanges();

  /*
  // Now test that while the window is inactive we can change the focused view
  // (we do that in several places).
  SimulateDeactivateWindow();
  // TODO: would have to mock the window being inactive (with a TestWidgetWin
  //       that would return false on IsActive()).
  GetFocusManager()->SetFocusedView(view);
  ::SendMessage(window_->GetNativeWindow(), WM_ACTIVATE, WA_ACTIVE, NULL);

  EXPECT_EQ(view, GetFocusManager()->GetFocusedView());
  ASSERT_EQ(2, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(button, null_view));
  EXPECT_TRUE(listener.focus_changes()[1] == ViewPair(null_view, view));
  */
}

// Test that the focus manager is created successfully for the first view
// window parented to a native dialog.
TEST_F(FocusManagerTest, CreationForNativeRoot) {
  // Create a window class.
  WNDCLASSEX class_ex;
  memset(&class_ex, 0, sizeof(class_ex));
  class_ex.cbSize = sizeof(WNDCLASSEX);
  class_ex.lpfnWndProc = &DefWindowProc;
  class_ex.lpszClassName = L"TestWindow";
  ATOM atom = RegisterClassEx(&class_ex);
  ASSERT_TRUE(atom);

  // Create a native dialog window.
  HWND hwnd = CreateWindowEx(0, class_ex.lpszClassName, NULL,
                             WS_OVERLAPPEDWINDOW, 0, 0, 200, 200,
                             NULL, NULL, NULL, NULL);
  ASSERT_TRUE(hwnd);

  // Create a view window parented to native dialog.
  scoped_ptr<Widget> widget1(new Widget);
  Widget::InitParams params(Widget::InitParams::TYPE_CONTROL);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.parent = hwnd;
  params.bounds = gfx::Rect(0, 0, 100, 100);
  params.top_level = true;  // This is top level in views hierarchy.
  widget1->Init(params);

  // Get the focus manager directly from the first window.  Should exist
  // because the first window is the root widget.
  views::FocusManager* focus_manager1 = widget1->GetFocusManager();
  EXPECT_TRUE(focus_manager1);

  // Create another view window parented to the first view window.
  scoped_ptr<Widget> widget2(new Widget);
  params.parent = widget1->GetNativeView();
  params.top_level = false;  // This is child widget.
  widget2->Init(params);

  // Access the shared focus manager directly from the second window.
  views::FocusManager* focus_manager2 = widget2->GetFocusManager();
  EXPECT_EQ(focus_manager2, focus_manager1);

  // Access the shared focus manager indirectly from the first window handle.
  gfx::NativeWindow native_window = widget1->GetNativeWindow();
  views::Widget* widget =
      views::Widget::GetWidgetForNativeWindow(native_window);
  EXPECT_EQ(widget->GetFocusManager(), focus_manager1);

  // Access the shared focus manager indirectly from the second window handle.
  native_window = widget2->GetNativeWindow();
  widget = views::Widget::GetWidgetForNativeWindow(native_window);
  EXPECT_EQ(widget->GetFocusManager(), focus_manager1);

  // Access the shared focus manager indirectly from the first view handle.
  gfx::NativeView native_view = widget1->GetNativeView();
  widget = views::Widget::GetTopLevelWidgetForNativeView(native_view);
  EXPECT_EQ(widget->GetFocusManager(), focus_manager1);

  // Access the shared focus manager indirectly from the second view handle.
  native_view = widget2->GetNativeView();
  widget = views::Widget::GetTopLevelWidgetForNativeView(native_view);
  EXPECT_EQ(widget->GetFocusManager(), focus_manager1);

  DestroyWindow(hwnd);
}

// Tests that the keyup messages are eaten for accelerators.
// Windows-only, Windows is the only platform that handles accelerators in
// AcceleratorHandler. NativeWidgetAura::OnKeyEvent handles them in other
// configurations.
TEST_F(FocusManagerTest, IgnoreKeyupForAccelerators) {
  FocusManager* focus_manager = GetFocusManager();
  MessageTrackingView* mtv = new MessageTrackingView();
  mtv->AddAccelerator(ui::Accelerator(ui::VKEY_0, ui::EF_NONE));
  mtv->AddAccelerator(ui::Accelerator(ui::VKEY_1, ui::EF_NONE));
  GetContentsView()->AddChildView(mtv);
  focus_manager->SetFocusedView(mtv);

  // First send a non-accelerator key sequence.
  PostKeyDown(ui::VKEY_9);
  PostKeyUp(ui::VKEY_9);
  AcceleratorHandler accelerator_handler;
  scoped_ptr<base::RunLoop> run_loop(new base::RunLoop(&accelerator_handler));
  run_loop->RunUntilIdle();
  // Make sure we get a key-up and key-down.
  ASSERT_EQ(1U, mtv->keys_pressed().size());
  EXPECT_EQ(ui::VKEY_9, mtv->keys_pressed()[0]);
  ASSERT_EQ(1U, mtv->keys_released().size());
  EXPECT_EQ(ui::VKEY_9, mtv->keys_released()[0]);
  EXPECT_FALSE(mtv->accelerator_pressed());
  mtv->Reset();

  // Same thing with repeat and more than one key at once.
  PostKeyDown(ui::VKEY_9);
  PostKeyDown(ui::VKEY_9);
  PostKeyDown(ui::VKEY_8);
  PostKeyDown(ui::VKEY_9);
  PostKeyDown(ui::VKEY_7);
  PostKeyUp(ui::VKEY_9);
  PostKeyUp(ui::VKEY_7);
  PostKeyUp(ui::VKEY_8);
  run_loop.reset(new base::RunLoop(&accelerator_handler));
  run_loop->RunUntilIdle();
  // Make sure we get a key-up and key-down.
  ASSERT_EQ(5U, mtv->keys_pressed().size());
  EXPECT_EQ(ui::VKEY_9, mtv->keys_pressed()[0]);
  EXPECT_EQ(ui::VKEY_9, mtv->keys_pressed()[1]);
  EXPECT_EQ(ui::VKEY_8, mtv->keys_pressed()[2]);
  EXPECT_EQ(ui::VKEY_9, mtv->keys_pressed()[3]);
  EXPECT_EQ(ui::VKEY_7, mtv->keys_pressed()[4]);
  ASSERT_EQ(3U, mtv->keys_released().size());
  EXPECT_EQ(ui::VKEY_9, mtv->keys_released()[0]);
  EXPECT_EQ(ui::VKEY_7, mtv->keys_released()[1]);
  EXPECT_EQ(ui::VKEY_8, mtv->keys_released()[2]);
  EXPECT_FALSE(mtv->accelerator_pressed());
  mtv->Reset();

  // Now send an accelerator key sequence.
  PostKeyDown(ui::VKEY_0);
  PostKeyUp(ui::VKEY_0);
  run_loop.reset(new base::RunLoop(&accelerator_handler));
  run_loop->RunUntilIdle();
  EXPECT_TRUE(mtv->keys_pressed().empty());
  EXPECT_TRUE(mtv->keys_released().empty());
  EXPECT_TRUE(mtv->accelerator_pressed());
  mtv->Reset();

  // Same thing with repeat and more than one key at once.
  PostKeyDown(ui::VKEY_0);
  PostKeyDown(ui::VKEY_1);
  PostKeyDown(ui::VKEY_1);
  PostKeyDown(ui::VKEY_0);
  PostKeyDown(ui::VKEY_0);
  PostKeyUp(ui::VKEY_1);
  PostKeyUp(ui::VKEY_0);
  run_loop.reset(new base::RunLoop(&accelerator_handler));
  run_loop->RunUntilIdle();
  EXPECT_TRUE(mtv->keys_pressed().empty());
  EXPECT_TRUE(mtv->keys_released().empty());
  EXPECT_TRUE(mtv->accelerator_pressed());
  mtv->Reset();
}

}  // namespace views
