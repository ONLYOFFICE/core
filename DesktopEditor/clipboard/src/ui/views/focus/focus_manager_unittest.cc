// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <utility>
#include <vector>

#include "base/utf_string_conversions.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/views/accessible_pane_view.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/focus/focus_manager_factory.h"
#include "ui/views/focus/focus_manager_test.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/aura/client/focus_client.h"
#include "ui/aura/window.h"
#endif

namespace views {

void FocusNativeView(gfx::NativeView view) {
#if defined(USE_AURA)
  aura::client::GetFocusClient(view)->FocusWindow(view);
#elif defined(OS_WIN)
  SetFocus(view);
#else
#error
#endif
}

enum FocusTestEventType {
  ON_FOCUS = 0,
  ON_BLUR
};

struct FocusTestEvent {
  FocusTestEvent(FocusTestEventType type, int view_id)
      : type(type),
        view_id(view_id) {
  }

  FocusTestEventType type;
  int view_id;
};

class SimpleTestView : public View {
 public:
  SimpleTestView(std::vector<FocusTestEvent>* event_list, int view_id)
      : event_list_(event_list) {
    set_focusable(true);
    set_id(view_id);
  }

  virtual void OnFocus() OVERRIDE {
    event_list_->push_back(FocusTestEvent(ON_FOCUS, id()));
  }

  virtual void OnBlur() OVERRIDE {
    event_list_->push_back(FocusTestEvent(ON_BLUR, id()));
  }

 private:
  std::vector<FocusTestEvent>* event_list_;
};

// Tests that the appropriate Focus related methods are called when a View
// gets/loses focus.
TEST_F(FocusManagerTest, ViewFocusCallbacks) {
  std::vector<FocusTestEvent> event_list;
  const int kView1ID = 1;
  const int kView2ID = 2;

  SimpleTestView* view1 = new SimpleTestView(&event_list, kView1ID);
  SimpleTestView* view2 = new SimpleTestView(&event_list, kView2ID);
  GetContentsView()->AddChildView(view1);
  GetContentsView()->AddChildView(view2);

  view1->RequestFocus();
  ASSERT_EQ(1, static_cast<int>(event_list.size()));
  EXPECT_EQ(ON_FOCUS, event_list[0].type);
  EXPECT_EQ(kView1ID, event_list[0].view_id);

  event_list.clear();
  view2->RequestFocus();
  ASSERT_EQ(2, static_cast<int>(event_list.size()));
  EXPECT_EQ(ON_BLUR, event_list[0].type);
  EXPECT_EQ(kView1ID, event_list[0].view_id);
  EXPECT_EQ(ON_FOCUS, event_list[1].type);
  EXPECT_EQ(kView2ID, event_list[1].view_id);

  event_list.clear();
  GetFocusManager()->ClearFocus();
  ASSERT_EQ(1, static_cast<int>(event_list.size()));
  EXPECT_EQ(ON_BLUR, event_list[0].type);
  EXPECT_EQ(kView2ID, event_list[0].view_id);
}

TEST_F(FocusManagerTest, FocusChangeListener) {
  View* view1 = new View();
  view1->set_focusable(true);
  View* view2 = new View();
  view2->set_focusable(true);
  GetContentsView()->AddChildView(view1);
  GetContentsView()->AddChildView(view2);

  TestFocusChangeListener listener;
  AddFocusChangeListener(&listener);

  // Required for VS2010: http://connect.microsoft.com/VisualStudio/feedback/details/520043/error-converting-from-null-to-a-pointer-type-in-std-pair
  views::View* null_view = NULL;

  view1->RequestFocus();
  ASSERT_EQ(1, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(null_view, view1));
  listener.ClearFocusChanges();

  view2->RequestFocus();
  ASSERT_EQ(1, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(view1, view2));
  listener.ClearFocusChanges();

  GetFocusManager()->ClearFocus();
  ASSERT_EQ(1, static_cast<int>(listener.focus_changes().size()));
  EXPECT_TRUE(listener.focus_changes()[0] == ViewPair(view2, null_view));
}

TEST_F(FocusManagerTest, WidgetFocusChangeListener) {
  TestWidgetFocusChangeListener widget_listener;
  AddWidgetFocusChangeListener(&widget_listener);

  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(10, 10, 100, 100);
  params.parent = GetWidget()->GetNativeView();

  scoped_ptr<Widget> widget1(new Widget);
  widget1->Init(params);
  widget1->Show();

  scoped_ptr<Widget> widget2(new Widget);
  widget2->Init(params);
  widget2->Show();

  widget_listener.ClearFocusChanges();
  gfx::NativeView native_view1 = widget1->GetNativeView();
  FocusNativeView(native_view1);
  ASSERT_EQ(2, static_cast<int>(widget_listener.focus_changes().size()));
  EXPECT_EQ(native_view1, widget_listener.focus_changes()[0].second);
  EXPECT_EQ(native_view1, widget_listener.focus_changes()[1].second);

  widget_listener.ClearFocusChanges();
  gfx::NativeView native_view2 = widget2->GetNativeView();
  FocusNativeView(native_view2);
  ASSERT_EQ(2, static_cast<int>(widget_listener.focus_changes().size()));
  EXPECT_EQ(NativeViewPair(native_view1, native_view2),
            widget_listener.focus_changes()[0]);
  EXPECT_EQ(NativeViewPair(native_view1, native_view2),
            widget_listener.focus_changes()[1]);
}

#if !defined(USE_AURA)
class TestTextfield : public Textfield {
 public:
  TestTextfield() {}
  virtual gfx::NativeView TestGetNativeControlView() {
    return native_wrapper_->GetTestingHandle();
  }
};

// Tests that NativeControls do set the focused View appropriately on the
// FocusManager.
TEST_F(FocusManagerTest, DISABLED_FocusNativeControls) {
  TestTextfield* textfield = new TestTextfield();
  GetContentsView()->AddChildView(textfield);
  // Simulate the native view getting the native focus (such as by user click).
  FocusNativeView(textfield->TestGetNativeControlView());
  EXPECT_EQ(textfield, GetFocusManager()->GetFocusedView());
}
#endif

// Counts accelerator calls.
class TestAcceleratorTarget : public ui::AcceleratorTarget {
 public:
  explicit TestAcceleratorTarget(bool process_accelerator)
      : accelerator_count_(0),
        process_accelerator_(process_accelerator),
        can_handle_accelerators_(true) {}

  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE {
    ++accelerator_count_;
    return process_accelerator_;
  }

  virtual bool CanHandleAccelerators() const OVERRIDE {
    return can_handle_accelerators_;
  }

  int accelerator_count() const { return accelerator_count_; }

  void set_can_handle_accelerators(bool can_handle_accelerators) {
    can_handle_accelerators_ = can_handle_accelerators;
  }

 private:
  int accelerator_count_;  // number of times that the accelerator is activated
  bool process_accelerator_;  // return value of AcceleratorPressed
  bool can_handle_accelerators_;  // return value of CanHandleAccelerators

  DISALLOW_COPY_AND_ASSIGN(TestAcceleratorTarget);
};

TEST_F(FocusManagerTest, CallsNormalAcceleratorTarget) {
  FocusManager* focus_manager = GetFocusManager();
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  ui::Accelerator escape_accelerator(ui::VKEY_ESCAPE, ui::EF_NONE);

  TestAcceleratorTarget return_target(true);
  TestAcceleratorTarget escape_target(true);
  EXPECT_EQ(return_target.accelerator_count(), 0);
  EXPECT_EQ(escape_target.accelerator_count(), 0);
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));

  // Register targets.
  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &return_target);
  focus_manager->RegisterAccelerator(escape_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &escape_target);

  // Checks if the correct target is registered.
  EXPECT_EQ(&return_target,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));
  EXPECT_EQ(&escape_target,
            focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));

  // Hitting the return key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 1);
  EXPECT_EQ(escape_target.accelerator_count(), 0);

  // Hitting the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 1);
  EXPECT_EQ(escape_target.accelerator_count(), 1);

  // Register another target for the return key.
  TestAcceleratorTarget return_target2(true);
  EXPECT_EQ(return_target2.accelerator_count(), 0);
  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &return_target2);
  EXPECT_EQ(&return_target2,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Hitting the return key; return_target2 has the priority.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 1);
  EXPECT_EQ(return_target2.accelerator_count(), 1);

  // Register a target that does not process the accelerator event.
  TestAcceleratorTarget return_target3(false);
  EXPECT_EQ(return_target3.accelerator_count(), 0);
  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &return_target3);
  EXPECT_EQ(&return_target3,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Hitting the return key.
  // Since the event handler of return_target3 returns false, return_target2
  // should be called too.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 1);
  EXPECT_EQ(return_target2.accelerator_count(), 2);
  EXPECT_EQ(return_target3.accelerator_count(), 1);

  // Unregister return_target2.
  focus_manager->UnregisterAccelerator(return_accelerator, &return_target2);
  EXPECT_EQ(&return_target3,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Hitting the return key. return_target3 and return_target should be called.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 2);
  EXPECT_EQ(return_target2.accelerator_count(), 2);
  EXPECT_EQ(return_target3.accelerator_count(), 2);

  // Unregister targets.
  focus_manager->UnregisterAccelerator(return_accelerator, &return_target);
  focus_manager->UnregisterAccelerator(return_accelerator, &return_target3);
  focus_manager->UnregisterAccelerator(escape_accelerator, &escape_target);

  // Now there is no target registered.
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));

  // Hitting the return key and the escape key. Nothing should happen.
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(return_target.accelerator_count(), 2);
  EXPECT_EQ(return_target2.accelerator_count(), 2);
  EXPECT_EQ(return_target3.accelerator_count(), 2);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target.accelerator_count(), 1);
}

TEST_F(FocusManagerTest, HighPriorityHandlers) {
  FocusManager* focus_manager = GetFocusManager();
  ui::Accelerator escape_accelerator(ui::VKEY_ESCAPE, ui::EF_NONE);

  TestAcceleratorTarget escape_target_high(true);
  TestAcceleratorTarget escape_target_normal(true);
  EXPECT_EQ(escape_target_high.accelerator_count(), 0);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 0);
  EXPECT_EQ(NULL,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_FALSE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Register high priority target.
  focus_manager->RegisterAccelerator(escape_accelerator,
                                     ui::AcceleratorManager::kHighPriority,
                                     &escape_target_high);
  EXPECT_EQ(&escape_target_high,
     focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_TRUE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 1);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 0);

  // Add a normal priority target and make sure it doesn't see the key.
  focus_manager->RegisterAccelerator(escape_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &escape_target_normal);

  // Checks if the correct target is registered (same as before, the high
  // priority one).
  EXPECT_EQ(&escape_target_high,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_TRUE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 2);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 0);

  // Unregister the high priority accelerator.
  focus_manager->UnregisterAccelerator(escape_accelerator, &escape_target_high);
  EXPECT_EQ(&escape_target_normal,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_FALSE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 2);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 1);

  // Add the high priority target back and make sure it starts seeing the key.
  focus_manager->RegisterAccelerator(escape_accelerator,
                                     ui::AcceleratorManager::kHighPriority,
                                     &escape_target_high);
  EXPECT_EQ(&escape_target_high,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_TRUE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 3);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 1);

  // Unregister the normal priority accelerator.
  focus_manager->UnregisterAccelerator(
      escape_accelerator, &escape_target_normal);
  EXPECT_EQ(&escape_target_high,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_TRUE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 4);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 1);

  // Unregister the high priority accelerator.
  focus_manager->UnregisterAccelerator(escape_accelerator, &escape_target_high);
  EXPECT_EQ(NULL,
      focus_manager->GetCurrentTargetForAccelerator(escape_accelerator));
  EXPECT_FALSE(focus_manager->HasPriorityHandler(escape_accelerator));

  // Hit the escape key (no change, no targets registered).
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(escape_target_high.accelerator_count(), 4);
  EXPECT_EQ(escape_target_normal.accelerator_count(), 1);
}

TEST_F(FocusManagerTest, CallsEnabledAcceleratorTargetsOnly) {
  FocusManager* focus_manager = GetFocusManager();
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);

  TestAcceleratorTarget return_target1(true);
  TestAcceleratorTarget return_target2(true);

  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &return_target1);
  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &return_target2);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(0, return_target1.accelerator_count());
  EXPECT_EQ(1, return_target2.accelerator_count());

  // If CanHandleAccelerators() return false, FocusManager shouldn't call
  // AcceleratorPressed().
  return_target2.set_can_handle_accelerators(false);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, return_target1.accelerator_count());
  EXPECT_EQ(1, return_target2.accelerator_count());

  // If no accelerator targets are enabled, ProcessAccelerator() should fail.
  return_target1.set_can_handle_accelerators(false);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, return_target1.accelerator_count());
  EXPECT_EQ(1, return_target2.accelerator_count());

  // Enabling the target again causes the accelerators to be processed again.
  return_target1.set_can_handle_accelerators(true);
  return_target2.set_can_handle_accelerators(true);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, return_target1.accelerator_count());
  EXPECT_EQ(2, return_target2.accelerator_count());
}

// Unregisters itself when its accelerator is invoked.
class SelfUnregisteringAcceleratorTarget : public ui::AcceleratorTarget {
 public:
  SelfUnregisteringAcceleratorTarget(ui::Accelerator accelerator,
                                     FocusManager* focus_manager)
      : accelerator_(accelerator),
        focus_manager_(focus_manager),
        accelerator_count_(0) {
  }

  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE {
    ++accelerator_count_;
    focus_manager_->UnregisterAccelerator(accelerator, this);
    return true;
  }

  virtual bool CanHandleAccelerators() const OVERRIDE {
    return true;
  }

  int accelerator_count() const { return accelerator_count_; }

 private:
  ui::Accelerator accelerator_;
  FocusManager* focus_manager_;
  int accelerator_count_;

  DISALLOW_COPY_AND_ASSIGN(SelfUnregisteringAcceleratorTarget);
};

TEST_F(FocusManagerTest, CallsSelfDeletingAcceleratorTarget) {
  FocusManager* focus_manager = GetFocusManager();
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  SelfUnregisteringAcceleratorTarget target(return_accelerator, focus_manager);
  EXPECT_EQ(target.accelerator_count(), 0);
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Register the target.
  focus_manager->RegisterAccelerator(return_accelerator,
                                     ui::AcceleratorManager::kNormalPriority,
                                     &target);
  EXPECT_EQ(&target,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Hitting the return key. The target will be unregistered.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(target.accelerator_count(), 1);
  EXPECT_EQ(NULL,
            focus_manager->GetCurrentTargetForAccelerator(return_accelerator));

  // Hitting the return key again; nothing should happen.
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(target.accelerator_count(), 1);
}

class FocusManagerDtorTest : public FocusManagerTest {
 protected:
  typedef std::vector<std::string> DtorTrackVector;

  class FocusManagerDtorTracked : public FocusManager {
   public:
    FocusManagerDtorTracked(Widget* widget, DtorTrackVector* dtor_tracker)
      : FocusManager(widget, NULL /* delegate */),
        dtor_tracker_(dtor_tracker) {
    }

    virtual ~FocusManagerDtorTracked() {
      dtor_tracker_->push_back("FocusManagerDtorTracked");
    }

    DtorTrackVector* dtor_tracker_;

   private:
    DISALLOW_COPY_AND_ASSIGN(FocusManagerDtorTracked);
  };

  class TestFocusManagerFactory : public FocusManagerFactory {
   public:
    explicit TestFocusManagerFactory(DtorTrackVector* dtor_tracker)
        : dtor_tracker_(dtor_tracker) {
    }

    virtual FocusManager* CreateFocusManager(Widget* widget) OVERRIDE {
      return new FocusManagerDtorTracked(widget, dtor_tracker_);
    }

   private:
    DtorTrackVector* dtor_tracker_;
    DISALLOW_COPY_AND_ASSIGN(TestFocusManagerFactory);
  };

  class LabelButtonDtorTracked : public LabelButton {
   public:
    LabelButtonDtorTracked(const string16& text, DtorTrackVector* dtor_tracker)
        : LabelButton(NULL, text),
          dtor_tracker_(dtor_tracker) {
      SetStyle(STYLE_NATIVE_TEXTBUTTON);
    };
    virtual ~LabelButtonDtorTracked() {
      dtor_tracker_->push_back("LabelButtonDtorTracked");
    }

    DtorTrackVector* dtor_tracker_;
  };

  class WindowDtorTracked : public Widget {
   public:
    explicit WindowDtorTracked(DtorTrackVector* dtor_tracker)
        : dtor_tracker_(dtor_tracker) {
    }

    virtual ~WindowDtorTracked() {
      dtor_tracker_->push_back("WindowDtorTracked");
    }

    DtorTrackVector* dtor_tracker_;
  };

  virtual void SetUp() {
    ViewsTestBase::SetUp();
    FocusManagerFactory::Install(new TestFocusManagerFactory(&dtor_tracker_));
    // Create WindowDtorTracked that uses FocusManagerDtorTracked.
    Widget* widget = new WindowDtorTracked(&dtor_tracker_);
    Widget::InitParams params;
    params.delegate = this;
    params.bounds = gfx::Rect(0, 0, 100, 100);
    widget->Init(params);

    tracked_focus_manager_ =
        static_cast<FocusManagerDtorTracked*>(GetFocusManager());
    widget->Show();
  }

  virtual void TearDown() {
    FocusManagerFactory::Install(NULL);
    ViewsTestBase::TearDown();
  }

  FocusManager* tracked_focus_manager_;
  DtorTrackVector dtor_tracker_;
};

#if !defined(USE_AURA)
TEST_F(FocusManagerDtorTest, FocusManagerDestructedLast) {
  // Setup views hierarchy.
  GetContentsView()->AddChildView(new TestTextfield());
  GetContentsView()->AddChildView(new LabelButtonDtorTracked(
      ASCIIToUTF16("button"), &dtor_tracker_));

  // Close the window.
  GetWidget()->Close();
  RunPendingMessages();

  // Test window, button and focus manager should all be destructed.
  ASSERT_EQ(3, static_cast<int>(dtor_tracker_.size()));

  // Focus manager should be the last one to destruct.
  ASSERT_STREQ("FocusManagerDtorTracked", dtor_tracker_[2].c_str());
}
#endif

namespace {

class FocusInAboutToRequestFocusFromTabTraversalView : public View {
 public:
  FocusInAboutToRequestFocusFromTabTraversalView() : view_to_focus_(NULL) {}

  void set_view_to_focus(View* view) { view_to_focus_ = view; }

  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE {
    view_to_focus_->RequestFocus();
  }

 private:
  views::View* view_to_focus_;

  DISALLOW_COPY_AND_ASSIGN(FocusInAboutToRequestFocusFromTabTraversalView);
};
}  // namespace

// Verifies a focus change done during a call to
// AboutToRequestFocusFromTabTraversal() is honored.
TEST_F(FocusManagerTest, FocusInAboutToRequestFocusFromTabTraversal) {
  // Create 3 views focuses the 3 and advances to the second. The 2nd views
  // implementation of AboutToRequestFocusFromTabTraversal() focuses the first.
  views::View* v1 = new View;
  v1->set_focusable(true);
  GetContentsView()->AddChildView(v1);

  FocusInAboutToRequestFocusFromTabTraversalView* v2 =
      new FocusInAboutToRequestFocusFromTabTraversalView;
  v2->set_focusable(true);
  v2->set_view_to_focus(v1);
  GetContentsView()->AddChildView(v2);

  views::View* v3 = new View;
  v3->set_focusable(true);
  GetContentsView()->AddChildView(v3);

  v3->RequestFocus();
  GetWidget()->GetFocusManager()->AdvanceFocus(true);
  EXPECT_TRUE(v1->HasFocus());
}

TEST_F(FocusManagerTest, RotatePaneFocus) {
  views::AccessiblePaneView* pane1 = new AccessiblePaneView();
  GetContentsView()->AddChildView(pane1);

  views::View* v1 = new View;
  v1->set_focusable(true);
  pane1->AddChildView(v1);

  views::View* v2 = new View;
  v2->set_focusable(true);
  pane1->AddChildView(v2);

  views::AccessiblePaneView* pane2 = new AccessiblePaneView();
  GetContentsView()->AddChildView(pane2);

  views::View* v3 = new View;
  v3->set_focusable(true);
  pane2->AddChildView(v3);

  views::View* v4 = new View;
  v4->set_focusable(true);
  pane2->AddChildView(v4);

  std::vector<views::View*> panes;
  panes.push_back(pane1);
  panes.push_back(pane2);
  SetAccessiblePanes(panes);

  FocusManager* focus_manager = GetWidget()->GetFocusManager();

  // Advance forwards. Focus should stay trapped within each pane.
  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kForward, FocusManager::kWrap));
  EXPECT_EQ(v1, focus_manager->GetFocusedView());
  focus_manager->AdvanceFocus(false);
  EXPECT_EQ(v2, focus_manager->GetFocusedView());
  focus_manager->AdvanceFocus(false);
  EXPECT_EQ(v1, focus_manager->GetFocusedView());

  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kForward, FocusManager::kWrap));
  EXPECT_EQ(v3, focus_manager->GetFocusedView());
  focus_manager->AdvanceFocus(false);
  EXPECT_EQ(v4, focus_manager->GetFocusedView());
  focus_manager->AdvanceFocus(false);
  EXPECT_EQ(v3, focus_manager->GetFocusedView());

  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kForward, FocusManager::kWrap));
  EXPECT_EQ(v1, focus_manager->GetFocusedView());

  // Advance backwards.
  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kBackward, FocusManager::kWrap));
  EXPECT_EQ(v3, focus_manager->GetFocusedView());

  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kBackward, FocusManager::kWrap));
  EXPECT_EQ(v1, focus_manager->GetFocusedView());

  // Advance without wrap. When it gets to the end of the list of
  // panes, RotatePaneFocus should return false but the current
  // focused view shouldn't change.
  EXPECT_TRUE(focus_manager->RotatePaneFocus(
      FocusManager::kForward, FocusManager::kNoWrap));
  EXPECT_EQ(v3, focus_manager->GetFocusedView());

  EXPECT_FALSE(focus_manager->RotatePaneFocus(
      FocusManager::kForward, FocusManager::kNoWrap));
  EXPECT_EQ(v3, focus_manager->GetFocusedView());
}

// Verifies the stored focus view tracks the focused view.
TEST_F(FocusManagerTest, ImplicitlyStoresFocus) {
  views::View* v1 = new View;
  v1->set_focusable(true);
  GetContentsView()->AddChildView(v1);

  views::View* v2 = new View;
  v2->set_focusable(true);
  GetContentsView()->AddChildView(v2);

  // Verify a focus request on |v1| implicitly updates the stored focus view.
  v1->RequestFocus();
  EXPECT_TRUE(v1->HasFocus());
  EXPECT_EQ(v1, GetWidget()->GetFocusManager()->GetStoredFocusView());

  // Verify a focus request on |v2| implicitly updates the stored focus view.
  v2->RequestFocus();
  EXPECT_TRUE(v2->HasFocus());
  EXPECT_EQ(v2, GetWidget()->GetFocusManager()->GetStoredFocusView());
}

namespace  {

class FocusManagerArrowKeyTraversalTest : public FocusManagerTest {
 public:
  FocusManagerArrowKeyTraversalTest()
      : previous_arrow_key_traversal_enabled_(false) {
  }
  virtual ~FocusManagerArrowKeyTraversalTest() {}

  // FocusManagerTest overrides:
  virtual void SetUp() OVERRIDE {
    FocusManagerTest::SetUp();

    previous_arrow_key_traversal_enabled_ =
      FocusManager::arrow_key_traversal_enabled();
  }
  virtual void TearDown() OVERRIDE {
    FocusManager::set_arrow_key_traversal_enabled(
        previous_arrow_key_traversal_enabled_);
    FocusManagerTest::TearDown();
  }

 private:
  bool previous_arrow_key_traversal_enabled_;

  DISALLOW_COPY_AND_ASSIGN(FocusManagerArrowKeyTraversalTest);
};

}  // namespace

TEST_F(FocusManagerArrowKeyTraversalTest, ArrowKeyTraversal) {
  FocusManager* focus_manager = GetFocusManager();
  const ui::KeyEvent left_key(
      ui::ET_KEY_PRESSED, ui::VKEY_LEFT, ui::EF_NONE, false);
  const ui::KeyEvent right_key(
      ui::ET_KEY_PRESSED, ui::VKEY_RIGHT, ui::EF_NONE, false);
  const ui::KeyEvent up_key(
      ui::ET_KEY_PRESSED, ui::VKEY_UP, ui::EF_NONE, false);
  const ui::KeyEvent down_key(
      ui::ET_KEY_PRESSED, ui::VKEY_DOWN, ui::EF_NONE, false);

  std::vector<views::View*> v;
  for (size_t i = 0; i < 2; ++i) {
    views::View* view = new View;
    view->set_focusable(true);
    GetContentsView()->AddChildView(view);
    v.push_back(view);
  }

  // Arrow key traversal is off and arrow key does not change focus.
  FocusManager::set_arrow_key_traversal_enabled(false);
  v[0]->RequestFocus();
  focus_manager->OnKeyEvent(right_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(left_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(down_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(up_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());

  // Turn on arrow key traversal.
  FocusManager::set_arrow_key_traversal_enabled(true);
  v[0]->RequestFocus();
  focus_manager->OnKeyEvent(right_key);
  EXPECT_EQ(v[1], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(left_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(down_key);
  EXPECT_EQ(v[1], focus_manager->GetFocusedView());
  focus_manager->OnKeyEvent(up_key);
  EXPECT_EQ(v[0], focus_manager->GetFocusedView());
}

}  // namespace views
