// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/events/event_utils.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/test/test_views_delegate.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/native_widget_delegate.h"
#include "ui/views/window/native_frame_view.h"

#if defined(USE_AURA)
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/test/test_window_delegate.h"
#include "ui/aura/window.h"
#include "ui/views/widget/native_widget_aura.h"
#if !defined(OS_CHROMEOS)
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#endif
#elif defined(OS_WIN)
#include "ui/views/widget/native_widget_win.h"
#endif

namespace views {
namespace {

// A generic typedef to pick up relevant NativeWidget implementations.
#if defined(USE_AURA)
typedef NativeWidgetAura NativeWidgetPlatform;
#elif defined(OS_WIN)
typedef NativeWidgetWin NativeWidgetPlatform;
#endif

// A widget that assumes mouse capture always works. It won't on Aura in
// testing, so we mock it.
#if defined(USE_AURA)
class NativeWidgetCapture : public NativeWidgetPlatform {
 public:
  explicit NativeWidgetCapture(internal::NativeWidgetDelegate* delegate)
      : NativeWidgetPlatform(delegate),
        mouse_capture_(false) {}
  virtual ~NativeWidgetCapture() {}

  virtual void SetCapture() OVERRIDE {
    mouse_capture_ = true;
  }
  virtual void ReleaseCapture() OVERRIDE {
    if (mouse_capture_)
      delegate()->OnMouseCaptureLost();
    mouse_capture_ = false;
  }
  virtual bool HasCapture() const OVERRIDE {
    return mouse_capture_;
  }

 private:
  bool mouse_capture_;

  DISALLOW_COPY_AND_ASSIGN(NativeWidgetCapture);
};
#endif

// A typedef that inserts our mock-capture NativeWidget implementation for
// relevant platforms.
#if defined(USE_AURA)
typedef NativeWidgetCapture NativeWidgetPlatformForTest;
#elif defined(OS_WIN)
typedef NativeWidgetWin NativeWidgetPlatformForTest;
#endif

// A view that always processes all mouse events.
class MouseView : public View {
 public:
  MouseView()
      : View(),
        entered_(0),
        exited_(0),
        pressed_(0) {
  }
  virtual ~MouseView() {}

  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE {
    pressed_++;
    return true;
  }

  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE {
    entered_++;
  }

  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE {
    exited_++;
  }

  // Return the number of OnMouseEntered calls and reset the counter.
  int EnteredCalls() {
    int i = entered_;
    entered_ = 0;
    return i;
  }

  // Return the number of OnMouseExited calls and reset the counter.
  int ExitedCalls() {
    int i = exited_;
    exited_ = 0;
    return i;
  }

  int pressed() const { return pressed_; }

 private:
  int entered_;
  int exited_;

  int pressed_;

  DISALLOW_COPY_AND_ASSIGN(MouseView);
};

// A view that keeps track of the events it receives, but consumes no events.
class EventCountView : public View {
 public:
  EventCountView() {}
  virtual ~EventCountView() {}

  int GetEventCount(ui::EventType type) {
    return event_count_[type];
  }

  void ResetCounts() {
    event_count_.clear();
  }

 protected:
  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE {
    RecordEvent(*event);
  }
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE {
    RecordEvent(*event);
  }
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE {
    RecordEvent(*event);
  }
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE {
    RecordEvent(*event);
  }
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    RecordEvent(*event);
  }

 private:
  void RecordEvent(const ui::Event& event) {
    ++event_count_[event.type()];
  }

  std::map<ui::EventType, int> event_count_;

  DISALLOW_COPY_AND_ASSIGN(EventCountView);
};

// A view that keeps track of the events it receives, and consumes all scroll
// gesture events.
class ScrollableEventCountView : public EventCountView {
 public:
  ScrollableEventCountView() {}
  virtual ~ScrollableEventCountView() {}

 private:
  // Overridden from ui::EventHandler:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    EventCountView::OnGestureEvent(event);
    switch (event->type()) {
      case ui::ET_GESTURE_SCROLL_BEGIN:
      case ui::ET_GESTURE_SCROLL_UPDATE:
      case ui::ET_GESTURE_SCROLL_END:
      case ui::ET_SCROLL_FLING_START:
        event->SetHandled();
        break;
      default:
        break;
    }
  }

  DISALLOW_COPY_AND_ASSIGN(ScrollableEventCountView);
};

// A view that does a capture on gesture-begin events.
class GestureCaptureView : public View {
 public:
  GestureCaptureView() {}
  virtual ~GestureCaptureView() {}

 private:
  // Overridden from View:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    if (event->type() == ui::ET_GESTURE_BEGIN) {
      GetWidget()->SetCapture(this);
      event->StopPropagation();
    }
  }

  DISALLOW_COPY_AND_ASSIGN(GestureCaptureView);
};

// A view that implements GetMinimumSize.
class MinimumSizeFrameView : public NativeFrameView {
 public:
  explicit MinimumSizeFrameView(Widget* frame): NativeFrameView(frame) {}
  virtual ~MinimumSizeFrameView() {}

 private:
  // Overridden from View:
  virtual gfx::Size GetMinimumSize() OVERRIDE {
    return gfx::Size(300, 400);
  }

  DISALLOW_COPY_AND_ASSIGN(MinimumSizeFrameView);
};

// An event handler that simply keeps a count of the different types of events
// it receives.
class EventCountHandler : public ui::EventHandler {
 public:
  EventCountHandler() {}
  virtual ~EventCountHandler() {}

  int GetEventCount(ui::EventType type) {
    return event_count_[type];
  }

  void ResetCounts() {
    event_count_.clear();
  }

 protected:
  // Overridden from ui::EventHandler:
  virtual void OnEvent(ui::Event* event) OVERRIDE {
    RecordEvent(*event);
    ui::EventHandler::OnEvent(event);
  }

 private:
  void RecordEvent(const ui::Event& event) {
    ++event_count_[event.type()];
  }

  std::map<ui::EventType, int> event_count_;

  DISALLOW_COPY_AND_ASSIGN(EventCountHandler);
};

class WidgetTest : public ViewsTestBase {
 public:
  WidgetTest() {}
  virtual ~WidgetTest() {}

  NativeWidget* CreatePlatformNativeWidget(
      internal::NativeWidgetDelegate* delegate) {
    return new NativeWidgetPlatformForTest(delegate);
  }

  Widget* CreateTopLevelPlatformWidget() {
    Widget* toplevel = new Widget;
    Widget::InitParams toplevel_params =
        CreateParams(Widget::InitParams::TYPE_WINDOW);
    toplevel_params.native_widget = CreatePlatformNativeWidget(toplevel);
    toplevel->Init(toplevel_params);
    return toplevel;
  }

  Widget* CreateTopLevelFramelessPlatformWidget() {
    Widget* toplevel = new Widget;
    Widget::InitParams toplevel_params =
        CreateParams(Widget::InitParams::TYPE_WINDOW_FRAMELESS);
    toplevel_params.native_widget = CreatePlatformNativeWidget(toplevel);
    toplevel->Init(toplevel_params);
    return toplevel;
  }

  Widget* CreateChildPlatformWidget(gfx::NativeView parent_native_view) {
    Widget* child = new Widget;
    Widget::InitParams child_params =
        CreateParams(Widget::InitParams::TYPE_CONTROL);
    child_params.native_widget = CreatePlatformNativeWidget(child);
    child_params.parent = parent_native_view;
    child->Init(child_params);
    child->SetContentsView(new View);
    return child;
  }

#if defined(OS_WIN) && !defined(USE_AURA)
  // On Windows, it is possible for us to have a child window that is
  // TYPE_POPUP.
  Widget* CreateChildPopupPlatformWidget(gfx::NativeView parent_native_view) {
    Widget* child = new Widget;
    Widget::InitParams child_params =
        CreateParams(Widget::InitParams::TYPE_POPUP);
    child_params.child = true;
    child_params.native_widget = CreatePlatformNativeWidget(child);
    child_params.parent = parent_native_view;
    child->Init(child_params);
    child->SetContentsView(new View);
    return child;
  }
#endif

  Widget* CreateTopLevelNativeWidget() {
    Widget* toplevel = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
    toplevel->Init(params);
    return toplevel;
  }

  Widget* CreateChildNativeWidgetWithParent(Widget* parent) {
    Widget* child = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_CONTROL);
    params.parent = parent->GetNativeView();
    child->Init(params);
    child->SetContentsView(new View);
    return child;
  }

  Widget* CreateChildNativeWidget() {
    return CreateChildNativeWidgetWithParent(NULL);
  }
};

bool WidgetHasMouseCapture(const Widget* widget) {
  return static_cast<const internal::NativeWidgetPrivate*>(widget->
      native_widget())->HasCapture();
}

ui::WindowShowState GetWidgetShowState(const Widget* widget) {
  // Use IsMaximized/IsMinimized/IsFullScreen instead of GetWindowPlacement
  // because the former is implemented on all platforms but the latter is not.
  return widget->IsFullscreen() ? ui::SHOW_STATE_FULLSCREEN :
      widget->IsMaximized() ? ui::SHOW_STATE_MAXIMIZED :
      widget->IsMinimized() ? ui::SHOW_STATE_MINIMIZED :
                              ui::SHOW_STATE_NORMAL;
}

TEST_F(WidgetTest, WidgetInitParams) {
  ASSERT_FALSE(views_delegate().UseTransparentWindows());

  // Widgets are not transparent by default.
  Widget::InitParams init1;
  EXPECT_FALSE(init1.transparent);

  // Non-window widgets are not transparent either.
  Widget::InitParams init2(Widget::InitParams::TYPE_MENU);
  EXPECT_FALSE(init2.transparent);

  // A ViewsDelegate can set windows transparent by default.
  views_delegate().SetUseTransparentWindows(true);
  Widget::InitParams init3;
  EXPECT_TRUE(init3.transparent);

  // Non-window widgets stay opaque.
  Widget::InitParams init4(Widget::InitParams::TYPE_MENU);
  EXPECT_FALSE(init4.transparent);
}

////////////////////////////////////////////////////////////////////////////////
// Widget::GetTopLevelWidget tests.

TEST_F(WidgetTest, GetTopLevelWidget_Native) {
  // Create a hierarchy of native widgets.
  Widget* toplevel = CreateTopLevelPlatformWidget();
  gfx::NativeView parent = toplevel->GetNativeView();
  Widget* child = CreateChildPlatformWidget(parent);

  EXPECT_EQ(toplevel, toplevel->GetTopLevelWidget());
  EXPECT_EQ(toplevel, child->GetTopLevelWidget());

  toplevel->CloseNow();
  // |child| should be automatically destroyed with |toplevel|.
}

// Tests some grab/ungrab events.
TEST_F(WidgetTest, DISABLED_GrabUngrab) {
  Widget* toplevel = CreateTopLevelPlatformWidget();
  Widget* child1 = CreateChildNativeWidgetWithParent(toplevel);
  Widget* child2 = CreateChildNativeWidgetWithParent(toplevel);

  toplevel->SetBounds(gfx::Rect(0, 0, 500, 500));

  child1->SetBounds(gfx::Rect(10, 10, 300, 300));
  View* view = new MouseView();
  view->SetBounds(0, 0, 300, 300);
  child1->GetRootView()->AddChildView(view);

  child2->SetBounds(gfx::Rect(200, 10, 200, 200));
  view = new MouseView();
  view->SetBounds(0, 0, 200, 200);
  child2->GetRootView()->AddChildView(view);

  toplevel->Show();
  RunPendingMessages();

  // Click on child1
  gfx::Point p1(45, 45);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1,
                         ui::EF_LEFT_MOUSE_BUTTON);
  toplevel->OnMouseEvent(&pressed);

  EXPECT_TRUE(WidgetHasMouseCapture(toplevel));
  EXPECT_TRUE(WidgetHasMouseCapture(child1));
  EXPECT_FALSE(WidgetHasMouseCapture(child2));

  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, p1, p1,
                          ui::EF_LEFT_MOUSE_BUTTON);
  toplevel->OnMouseEvent(&released);

  EXPECT_FALSE(WidgetHasMouseCapture(toplevel));
  EXPECT_FALSE(WidgetHasMouseCapture(child1));
  EXPECT_FALSE(WidgetHasMouseCapture(child2));

  RunPendingMessages();

  // Click on child2
  gfx::Point p2(315, 45);
  ui::MouseEvent pressed2(ui::ET_MOUSE_PRESSED, p2, p2,
                          ui::EF_LEFT_MOUSE_BUTTON);
  toplevel->OnMouseEvent(&pressed2);
  EXPECT_TRUE(pressed2.handled());
  EXPECT_TRUE(WidgetHasMouseCapture(toplevel));
  EXPECT_TRUE(WidgetHasMouseCapture(child2));
  EXPECT_FALSE(WidgetHasMouseCapture(child1));

  ui::MouseEvent released2(ui::ET_MOUSE_RELEASED, p2, p2,
                           ui::EF_LEFT_MOUSE_BUTTON);
  toplevel->OnMouseEvent(&released2);
  EXPECT_FALSE(WidgetHasMouseCapture(toplevel));
  EXPECT_FALSE(WidgetHasMouseCapture(child1));
  EXPECT_FALSE(WidgetHasMouseCapture(child2));

  toplevel->CloseNow();
}

// Tests mouse move outside of the window into the "resize controller" and back
// will still generate an OnMouseEntered and OnMouseExited event..
TEST_F(WidgetTest, CheckResizeControllerEvents) {
  Widget* toplevel = CreateTopLevelPlatformWidget();

  toplevel->SetBounds(gfx::Rect(0, 0, 100, 100));

  MouseView* view = new MouseView();
  view->SetBounds(90, 90, 10, 10);
  toplevel->GetRootView()->AddChildView(view);

  toplevel->Show();
  RunPendingMessages();

  // Move to an outside position.
  gfx::Point p1(200, 200);
  ui::MouseEvent moved_out(ui::ET_MOUSE_MOVED, p1, p1, ui::EF_NONE);
  toplevel->OnMouseEvent(&moved_out);
  EXPECT_EQ(0, view->EnteredCalls());
  EXPECT_EQ(0, view->ExitedCalls());

  // Move onto the active view.
  gfx::Point p2(95, 95);
  ui::MouseEvent moved_over(ui::ET_MOUSE_MOVED, p2, p2, ui::EF_NONE);
  toplevel->OnMouseEvent(&moved_over);
  EXPECT_EQ(1, view->EnteredCalls());
  EXPECT_EQ(0, view->ExitedCalls());

  // Move onto the outer resizing border.
  gfx::Point p3(102, 95);
  ui::MouseEvent moved_resizer(ui::ET_MOUSE_MOVED, p3, p3, ui::EF_NONE);
  toplevel->OnMouseEvent(&moved_resizer);
  EXPECT_EQ(0, view->EnteredCalls());
  EXPECT_EQ(1, view->ExitedCalls());

  // Move onto the view again.
  toplevel->OnMouseEvent(&moved_over);
  EXPECT_EQ(1, view->EnteredCalls());
  EXPECT_EQ(0, view->ExitedCalls());

  RunPendingMessages();

  toplevel->CloseNow();
}

// Test if a focus manager and an inputmethod work without CHECK failure
// when window activation changes.
TEST_F(WidgetTest, ChangeActivation) {
  Widget* top1 = CreateTopLevelPlatformWidget();
  // CreateInputMethod before activated
  top1->GetInputMethod();
  top1->Show();
  RunPendingMessages();

  Widget* top2 = CreateTopLevelPlatformWidget();
  top2->Show();
  RunPendingMessages();

  top1->Activate();
  RunPendingMessages();

  // Create InputMethod after deactivated.
  top2->GetInputMethod();
  top2->Activate();
  RunPendingMessages();

  top1->Activate();
  RunPendingMessages();

  top1->CloseNow();
  top2->CloseNow();
}

// Tests visibility of child widgets.
TEST_F(WidgetTest, Visibility) {
  Widget* toplevel = CreateTopLevelPlatformWidget();
  gfx::NativeView parent = toplevel->GetNativeView();
  Widget* child = CreateChildPlatformWidget(parent);

  EXPECT_FALSE(toplevel->IsVisible());
  EXPECT_FALSE(child->IsVisible());

  child->Show();

  EXPECT_FALSE(toplevel->IsVisible());
  EXPECT_FALSE(child->IsVisible());

  toplevel->Show();

  EXPECT_TRUE(toplevel->IsVisible());
  EXPECT_TRUE(child->IsVisible());

  toplevel->CloseNow();
  // |child| should be automatically destroyed with |toplevel|.
}

#if defined(OS_WIN) && !defined(USE_AURA)
// On Windows, it is possible to have child window that are TYPE_POPUP.  Unlike
// regular child windows, these should be created as hidden and must be shown
// explicitly.
TEST_F(WidgetTest, Visibility_ChildPopup) {
  Widget* toplevel = CreateTopLevelPlatformWidget();
  Widget* child_popup = CreateChildPopupPlatformWidget(
      toplevel->GetNativeView());

  EXPECT_FALSE(toplevel->IsVisible());
  EXPECT_FALSE(child_popup->IsVisible());

  toplevel->Show();

  EXPECT_TRUE(toplevel->IsVisible());
  EXPECT_FALSE(child_popup->IsVisible());

  child_popup->Show();

  EXPECT_TRUE(child_popup->IsVisible());

  toplevel->CloseNow();
  // |child_popup| should be automatically destroyed with |toplevel|.
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Widget ownership tests.
//
// Tests various permutations of Widget ownership specified in the
// InitParams::Ownership param.

// A WidgetTest that supplies a toplevel widget for NativeWidget to parent to.
class WidgetOwnershipTest : public WidgetTest {
 public:
  WidgetOwnershipTest() {}
  virtual ~WidgetOwnershipTest() {}

  virtual void SetUp() {
    WidgetTest::SetUp();
    desktop_widget_ = CreateTopLevelPlatformWidget();
  }

  virtual void TearDown() {
    desktop_widget_->CloseNow();
    WidgetTest::TearDown();
  }

 private:
  Widget* desktop_widget_;

  DISALLOW_COPY_AND_ASSIGN(WidgetOwnershipTest);
};

// A bag of state to monitor destructions.
struct OwnershipTestState {
  OwnershipTestState() : widget_deleted(false), native_widget_deleted(false) {}

  bool widget_deleted;
  bool native_widget_deleted;
};

// A platform NativeWidget subclass that updates a bag of state when it is
// destroyed.
class OwnershipTestNativeWidget : public NativeWidgetPlatform {
 public:
  OwnershipTestNativeWidget(internal::NativeWidgetDelegate* delegate,
                            OwnershipTestState* state)
      : NativeWidgetPlatform(delegate),
        state_(state) {
  }
  virtual ~OwnershipTestNativeWidget() {
    state_->native_widget_deleted = true;
  }

 private:
  OwnershipTestState* state_;

  DISALLOW_COPY_AND_ASSIGN(OwnershipTestNativeWidget);
};

// A views NativeWidget subclass that updates a bag of state when it is
// destroyed.
class OwnershipTestNativeWidgetPlatform : public NativeWidgetPlatformForTest {
 public:
  OwnershipTestNativeWidgetPlatform(internal::NativeWidgetDelegate* delegate,
                                    OwnershipTestState* state)
      : NativeWidgetPlatformForTest(delegate),
        state_(state) {
  }
  virtual ~OwnershipTestNativeWidgetPlatform() {
    state_->native_widget_deleted = true;
  }

 private:
  OwnershipTestState* state_;

  DISALLOW_COPY_AND_ASSIGN(OwnershipTestNativeWidgetPlatform);
};

// A Widget subclass that updates a bag of state when it is destroyed.
class OwnershipTestWidget : public Widget {
 public:
  explicit OwnershipTestWidget(OwnershipTestState* state) : state_(state) {}
  virtual ~OwnershipTestWidget() {
    state_->widget_deleted = true;
  }

 private:
  OwnershipTestState* state_;

  DISALLOW_COPY_AND_ASSIGN(OwnershipTestWidget);
};

// Widget owns its NativeWidget, part 1: NativeWidget is a platform-native
// widget.
TEST_F(WidgetOwnershipTest, Ownership_WidgetOwnsPlatformNativeWidget) {
  OwnershipTestState state;

  scoped_ptr<Widget> widget(new OwnershipTestWidget(&state));
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget.get(), &state);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);

  // Now delete the Widget, which should delete the NativeWidget.
  widget.reset();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);

  // TODO(beng): write test for this ownership scenario and the NativeWidget
  //             being deleted out from under the Widget.
}

// Widget owns its NativeWidget, part 2: NativeWidget is a NativeWidget.
TEST_F(WidgetOwnershipTest, Ownership_WidgetOwnsViewsNativeWidget) {
  OwnershipTestState state;

  scoped_ptr<Widget> widget(new OwnershipTestWidget(&state));
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget.get(), &state);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);

  // Now delete the Widget, which should delete the NativeWidget.
  widget.reset();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);

  // TODO(beng): write test for this ownership scenario and the NativeWidget
  //             being deleted out from under the Widget.
}

// Widget owns its NativeWidget, part 3: NativeWidget is a NativeWidget,
// destroy the parent view.
TEST_F(WidgetOwnershipTest,
       Ownership_WidgetOwnsViewsNativeWidget_DestroyParentView) {
  OwnershipTestState state;

  Widget* toplevel = CreateTopLevelPlatformWidget();

  scoped_ptr<Widget> widget(new OwnershipTestWidget(&state));
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget.get(), &state);
  params.parent = toplevel->GetNativeView();
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);

  // Now close the toplevel, which deletes the view hierarchy.
  toplevel->CloseNow();

  RunPendingMessages();

  // This shouldn't delete the widget because it shouldn't be deleted
  // from the native side.
  EXPECT_FALSE(state.widget_deleted);
  EXPECT_FALSE(state.native_widget_deleted);

  // Now delete it explicitly.
  widget.reset();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// NativeWidget owns its Widget, part 1: NativeWidget is a platform-native
// widget.
TEST_F(WidgetOwnershipTest, Ownership_PlatformNativeWidgetOwnsWidget) {
  OwnershipTestState state;

  Widget* widget = new OwnershipTestWidget(&state);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget, &state);
  widget->Init(params);

  // Now destroy the native widget.
  widget->CloseNow();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// NativeWidget owns its Widget, part 2: NativeWidget is a NativeWidget.
TEST_F(WidgetOwnershipTest, Ownership_ViewsNativeWidgetOwnsWidget) {
  OwnershipTestState state;

  Widget* toplevel = CreateTopLevelPlatformWidget();

  Widget* widget = new OwnershipTestWidget(&state);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget, &state);
  params.parent = toplevel->GetNativeView();
  widget->Init(params);

  // Now destroy the native widget. This is achieved by closing the toplevel.
  toplevel->CloseNow();

  // The NativeWidget won't be deleted until after a return to the message loop
  // so we have to run pending messages before testing the destruction status.
  RunPendingMessages();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// NativeWidget owns its Widget, part 3: NativeWidget is a platform-native
// widget, destroyed out from under it by the OS.
TEST_F(WidgetOwnershipTest,
       Ownership_PlatformNativeWidgetOwnsWidget_NativeDestroy) {
  OwnershipTestState state;

  Widget* widget = new OwnershipTestWidget(&state);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget, &state);
  widget->Init(params);

  // Now simulate a destroy of the platform native widget from the OS:
#if defined(USE_AURA)
  delete widget->GetNativeView();
#elif defined(OS_WIN)
  DestroyWindow(widget->GetNativeView());
#endif

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// NativeWidget owns its Widget, part 4: NativeWidget is a NativeWidget,
// destroyed by the view hierarchy that contains it.
TEST_F(WidgetOwnershipTest,
       Ownership_ViewsNativeWidgetOwnsWidget_NativeDestroy) {
  OwnershipTestState state;

  Widget* toplevel = CreateTopLevelPlatformWidget();

  Widget* widget = new OwnershipTestWidget(&state);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget, &state);
  params.parent = toplevel->GetNativeView();
  widget->Init(params);

  // Destroy the widget (achieved by closing the toplevel).
  toplevel->CloseNow();

  // The NativeWidget won't be deleted until after a return to the message loop
  // so we have to run pending messages before testing the destruction status.
  RunPendingMessages();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// NativeWidget owns its Widget, part 5: NativeWidget is a NativeWidget,
// we close it directly.
TEST_F(WidgetOwnershipTest,
       Ownership_ViewsNativeWidgetOwnsWidget_Close) {
  OwnershipTestState state;

  Widget* toplevel = CreateTopLevelPlatformWidget();

  Widget* widget = new OwnershipTestWidget(&state);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget, &state);
  params.parent = toplevel->GetNativeView();
  widget->Init(params);

  // Destroy the widget.
  widget->Close();
  toplevel->CloseNow();

  // The NativeWidget won't be deleted until after a return to the message loop
  // so we have to run pending messages before testing the destruction status.
  RunPendingMessages();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

// Widget owns its NativeWidget and has a WidgetDelegateView as its contents.
TEST_F(WidgetOwnershipTest,
       Ownership_WidgetOwnsNativeWidgetWithWithWidgetDelegateView) {
  OwnershipTestState state;

  WidgetDelegateView* delegate_view = new WidgetDelegateView;

  scoped_ptr<Widget> widget(new OwnershipTestWidget(&state));
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.native_widget =
      new OwnershipTestNativeWidgetPlatform(widget.get(), &state);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.delegate = delegate_view;
  widget->Init(params);
  widget->SetContentsView(delegate_view);

  // Now delete the Widget. There should be no crash or use-after-free.
  widget.reset();

  EXPECT_TRUE(state.widget_deleted);
  EXPECT_TRUE(state.native_widget_deleted);
}

////////////////////////////////////////////////////////////////////////////////
// Widget observer tests.
//

class WidgetObserverTest : public WidgetTest, public WidgetObserver {
 public:
  WidgetObserverTest()
      : active_(NULL),
        widget_closed_(NULL),
        widget_activated_(NULL),
        widget_shown_(NULL),
        widget_hidden_(NULL),
        widget_bounds_changed_(NULL) {
  }

  virtual ~WidgetObserverTest() {}

  // Overridden from WidgetObserver:
  virtual void OnWidgetDestroying(Widget* widget) OVERRIDE {
    if (active_ == widget)
      active_ = NULL;
    widget_closed_ = widget;
  }

  virtual void OnWidgetActivationChanged(Widget* widget,
                                         bool active) OVERRIDE {
    if (active) {
      if (widget_activated_)
        widget_activated_->Deactivate();
      widget_activated_ = widget;
      active_ = widget;
    } else {
      if (widget_activated_ == widget)
        widget_activated_ = NULL;
      widget_deactivated_ = widget;
    }
  }

  virtual void OnWidgetVisibilityChanged(Widget* widget,
                                         bool visible) OVERRIDE {
    if (visible)
      widget_shown_ = widget;
    else
      widget_hidden_ = widget;
  }

  virtual void OnWidgetBoundsChanged(Widget* widget,
                                     const gfx::Rect& new_bounds) OVERRIDE {
    widget_bounds_changed_ = widget;
  }

  void reset() {
    active_ = NULL;
    widget_closed_ = NULL;
    widget_activated_ = NULL;
    widget_deactivated_ = NULL;
    widget_shown_ = NULL;
    widget_hidden_ = NULL;
    widget_bounds_changed_ = NULL;
  }

  Widget* NewWidget() {
    Widget* widget = CreateTopLevelNativeWidget();
    widget->AddObserver(this);
    return widget;
  }

  const Widget* active() const { return active_; }
  const Widget* widget_closed() const { return widget_closed_; }
  const Widget* widget_activated() const { return widget_activated_; }
  const Widget* widget_deactivated() const { return widget_deactivated_; }
  const Widget* widget_shown() const { return widget_shown_; }
  const Widget* widget_hidden() const { return widget_hidden_; }
  const Widget* widget_bounds_changed() const { return widget_bounds_changed_; }

 private:
  Widget* active_;

  Widget* widget_closed_;
  Widget* widget_activated_;
  Widget* widget_deactivated_;
  Widget* widget_shown_;
  Widget* widget_hidden_;
  Widget* widget_bounds_changed_;
};

TEST_F(WidgetObserverTest, DISABLED_ActivationChange) {
  Widget* toplevel = CreateTopLevelPlatformWidget();

  Widget* toplevel1 = NewWidget();
  Widget* toplevel2 = NewWidget();

  toplevel1->Show();
  toplevel2->Show();

  reset();

  toplevel1->Activate();

  RunPendingMessages();
  EXPECT_EQ(toplevel1, widget_activated());

  toplevel2->Activate();
  RunPendingMessages();
  EXPECT_EQ(toplevel1, widget_deactivated());
  EXPECT_EQ(toplevel2, widget_activated());
  EXPECT_EQ(toplevel2, active());

  toplevel->CloseNow();
}

TEST_F(WidgetObserverTest, DISABLED_VisibilityChange) {
  Widget* toplevel = CreateTopLevelPlatformWidget();

  Widget* child1 = NewWidget();
  Widget* child2 = NewWidget();

  toplevel->Show();
  child1->Show();
  child2->Show();

  reset();

  child1->Hide();
  EXPECT_EQ(child1, widget_hidden());

  child2->Hide();
  EXPECT_EQ(child2, widget_hidden());

  child1->Show();
  EXPECT_EQ(child1, widget_shown());

  child2->Show();
  EXPECT_EQ(child2, widget_shown());

  toplevel->CloseNow();
}

TEST_F(WidgetObserverTest, DestroyBubble) {
  Widget* anchor = CreateTopLevelPlatformWidget();
  anchor->Show();

  BubbleDelegateView* bubble_delegate =
      new BubbleDelegateView(anchor->client_view(), BubbleBorder::NONE);
  Widget* bubble_widget(BubbleDelegateView::CreateBubble(bubble_delegate));
  bubble_widget->Show();
  bubble_widget->CloseNow();

  anchor->Hide();
  anchor->CloseNow();
}

TEST_F(WidgetObserverTest, WidgetBoundsChanged) {
  Widget* child1 = NewWidget();
  Widget* child2 = NewWidget();

  child1->OnNativeWidgetMove();
  EXPECT_EQ(child1, widget_bounds_changed());

  child2->OnNativeWidgetMove();
  EXPECT_EQ(child2, widget_bounds_changed());

  child1->OnNativeWidgetSizeChanged(gfx::Size());
  EXPECT_EQ(child1, widget_bounds_changed());

  child2->OnNativeWidgetSizeChanged(gfx::Size());
  EXPECT_EQ(child2, widget_bounds_changed());
}

#if !defined(USE_AURA) && defined(OS_WIN)
// Aura needs shell to maximize/fullscreen window.
// NativeWidgetGtk doesn't implement GetRestoredBounds.
TEST_F(WidgetTest, GetRestoredBounds) {
  Widget* toplevel = CreateTopLevelPlatformWidget();
  EXPECT_EQ(toplevel->GetWindowBoundsInScreen().ToString(),
            toplevel->GetRestoredBounds().ToString());
  toplevel->Show();
  toplevel->Maximize();
  RunPendingMessages();
  EXPECT_NE(toplevel->GetWindowBoundsInScreen().ToString(),
            toplevel->GetRestoredBounds().ToString());
  EXPECT_GT(toplevel->GetRestoredBounds().width(), 0);
  EXPECT_GT(toplevel->GetRestoredBounds().height(), 0);

  toplevel->Restore();
  RunPendingMessages();
  EXPECT_EQ(toplevel->GetWindowBoundsInScreen().ToString(),
            toplevel->GetRestoredBounds().ToString());

  toplevel->SetFullscreen(true);
  RunPendingMessages();
  EXPECT_NE(toplevel->GetWindowBoundsInScreen().ToString(),
            toplevel->GetRestoredBounds().ToString());
  EXPECT_GT(toplevel->GetRestoredBounds().width(), 0);
  EXPECT_GT(toplevel->GetRestoredBounds().height(), 0);
}
#endif

// Test that window state is not changed after getting out of full screen.
TEST_F(WidgetTest, ExitFullscreenRestoreState) {
  Widget* toplevel = CreateTopLevelPlatformWidget();

  toplevel->Show();
  RunPendingMessages();

  // This should be a normal state window.
  EXPECT_EQ(ui::SHOW_STATE_NORMAL, GetWidgetShowState(toplevel));

  toplevel->SetFullscreen(true);
  while (GetWidgetShowState(toplevel) != ui::SHOW_STATE_FULLSCREEN)
    RunPendingMessages();
  toplevel->SetFullscreen(false);
  while (GetWidgetShowState(toplevel) == ui::SHOW_STATE_FULLSCREEN)
    RunPendingMessages();

  // And it should still be in normal state after getting out of full screen.
  EXPECT_EQ(ui::SHOW_STATE_NORMAL, GetWidgetShowState(toplevel));

  // Now, make it maximized.
  toplevel->Maximize();
  while (GetWidgetShowState(toplevel) != ui::SHOW_STATE_MAXIMIZED)
    RunPendingMessages();

  toplevel->SetFullscreen(true);
  while (GetWidgetShowState(toplevel) != ui::SHOW_STATE_FULLSCREEN)
    RunPendingMessages();
  toplevel->SetFullscreen(false);
  while (GetWidgetShowState(toplevel) == ui::SHOW_STATE_FULLSCREEN)
    RunPendingMessages();

  // And it stays maximized after getting out of full screen.
  EXPECT_EQ(ui::SHOW_STATE_MAXIMIZED, GetWidgetShowState(toplevel));

  // Clean up.
  toplevel->Close();
  RunPendingMessages();
}

TEST_F(WidgetTest, ResetCaptureOnGestureEnd) {
  Widget* toplevel = CreateTopLevelFramelessPlatformWidget();
  View* container = new View;
  toplevel->SetContentsView(container);

  View* gesture = new GestureCaptureView;
  gesture->SetBounds(0, 0, 30, 30);
  container->AddChildView(gesture);

  MouseView* mouse = new MouseView;
  mouse->SetBounds(30, 0, 30, 30);
  container->AddChildView(mouse);

  toplevel->SetSize(gfx::Size(100, 100));
  toplevel->Show();

  // Start a gesture on |gesture|.
  ui::GestureEvent begin(ui::ET_GESTURE_BEGIN,
      15, 15, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_BEGIN, 0, 0), 1);
  ui::GestureEvent end(ui::ET_GESTURE_END,
      15, 15, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_END, 0, 0), 1);
  toplevel->OnGestureEvent(&begin);

  // Now try to click on |mouse|. Since |gesture| will have capture, |mouse|
  // will not receive the event.
  gfx::Point click_location(45, 15);

  ui::MouseEvent press(ui::ET_MOUSE_PRESSED, click_location, click_location,
      ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent release(ui::ET_MOUSE_RELEASED, click_location, click_location,
      ui::EF_LEFT_MOUSE_BUTTON);

  toplevel->OnMouseEvent(&press);
  toplevel->OnMouseEvent(&release);
  EXPECT_EQ(0, mouse->pressed());

  // The end of the gesture should release the capture, and pressing on |mouse|
  // should now reach |mouse|.
  toplevel->OnGestureEvent(&end);
  toplevel->OnMouseEvent(&press);
  toplevel->OnMouseEvent(&release);
  EXPECT_EQ(1, mouse->pressed());

  toplevel->Close();
  RunPendingMessages();
}

#if defined(USE_AURA)
// The key-event propagation from Widget happens differently on aura and
// non-aura systems because of the difference in IME. So this test works only on
// aura.
TEST_F(WidgetTest, KeyboardInputEvent) {
  Widget* toplevel = CreateTopLevelPlatformWidget();
  View* container = toplevel->client_view();

  Textfield* textfield = new Textfield();
  textfield->SetText(ASCIIToUTF16("some text"));
  container->AddChildView(textfield);
  toplevel->Show();
  textfield->RequestFocus();

  // The press gets handled. The release doesn't have an effect.
  ui::KeyEvent backspace_p(ui::ET_KEY_PRESSED, ui::VKEY_DELETE, 0, false);
  toplevel->OnKeyEvent(&backspace_p);
  EXPECT_TRUE(backspace_p.stopped_propagation());
  ui::KeyEvent backspace_r(ui::ET_KEY_RELEASED, ui::VKEY_DELETE, 0, false);
  toplevel->OnKeyEvent(&backspace_r);
  EXPECT_FALSE(backspace_r.handled());

  toplevel->Close();
}

// Verifies bubbles result in a focus lost when shown.
// TODO(msw): this tests relies on focus, it needs to be in
// interactive_ui_tests.
TEST_F(WidgetTest, DISABLED_FocusChangesOnBubble) {
  // Create a widget, show and activate it and focus the contents view.
  View* contents_view = new View;
  contents_view->set_focusable(true);
  Widget widget;
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  init_params.bounds = gfx::Rect(0, 0, 200, 200);
  init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
#if !defined(OS_CHROMEOS)
  init_params.native_widget = new DesktopNativeWidgetAura(&widget);
#endif
  widget.Init(init_params);
  widget.SetContentsView(contents_view);
  widget.Show();
  widget.Activate();
  contents_view->RequestFocus();
  EXPECT_TRUE(contents_view->HasFocus());

  // Show a bubble.
  BubbleDelegateView* bubble_delegate_view =
      new BubbleDelegateView(contents_view, BubbleBorder::TOP_LEFT);
  bubble_delegate_view->set_focusable(true);
  BubbleDelegateView::CreateBubble(bubble_delegate_view)->Show();
  bubble_delegate_view->RequestFocus();

  // |contents_view_| should no longer have focus.
  EXPECT_FALSE(contents_view->HasFocus());
  EXPECT_TRUE(bubble_delegate_view->HasFocus());

  bubble_delegate_view->GetWidget()->CloseNow();

  // Closing the bubble should result in focus going back to the contents view.
  EXPECT_TRUE(contents_view->HasFocus());
}

// Desktop native widget Aura tests are for non Chrome OS platforms.
#if !defined(OS_CHROMEOS)
// Test to ensure that after minimize, view width is set to zero.
TEST_F(WidgetTest, TestViewWidthAfterMinimizingWidget) {
  // Create a widget.
  Widget widget;
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW);
  init_params.show_state = ui::SHOW_STATE_NORMAL;
  gfx::Rect initial_bounds(0, 0, 300, 400);
  init_params.bounds = initial_bounds;
  init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  init_params.native_widget = new DesktopNativeWidgetAura(&widget);
  widget.Init(init_params);
  NonClientView* non_client_view = widget.non_client_view();
  NonClientFrameView* frame_view = new MinimumSizeFrameView(&widget);
  non_client_view->SetFrameView(frame_view);
  widget.Show();
  widget.Minimize();
  EXPECT_EQ(0, frame_view->width());
}

// This class validates whether paints are received for a visible Widget.
// To achieve this it overrides the Show and Close methods on the Widget class
// and sets state whether subsequent paints are expected.
class DesktopAuraTestValidPaintWidget : public views::Widget {
 public:
  DesktopAuraTestValidPaintWidget()
    : expect_paint_(true),
      received_paint_while_hidden_(false) {
  }

  virtual ~DesktopAuraTestValidPaintWidget() {
  }

  virtual void Show() OVERRIDE {
    expect_paint_ = true;
    views::Widget::Show();
  }

  virtual void Close() OVERRIDE {
    expect_paint_ = false;
    views::Widget::Close();
  }

  void Hide() {
    expect_paint_ = false;
    views::Widget::Hide();
  }

  virtual void OnNativeWidgetPaint(gfx::Canvas* canvas) OVERRIDE {
    EXPECT_TRUE(expect_paint_);
    if (!expect_paint_)
      received_paint_while_hidden_ = true;
    views::Widget::OnNativeWidgetPaint(canvas);
  }

  bool received_paint_while_hidden() const {
    return received_paint_while_hidden_;
  }

 private:
  bool expect_paint_;
  bool received_paint_while_hidden_;
};

TEST_F(WidgetTest, DesktopNativeWidgetAuraNoPaintAfterCloseTest) {
  View* contents_view = new View;
  contents_view->set_focusable(true);
  DesktopAuraTestValidPaintWidget widget;
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  init_params.bounds = gfx::Rect(0, 0, 200, 200);
  init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  init_params.native_widget = new DesktopNativeWidgetAura(&widget);
  widget.Init(init_params);
  widget.SetContentsView(contents_view);
  widget.Show();
  widget.Activate();
  RunPendingMessages();
  widget.SchedulePaintInRect(init_params.bounds);
  widget.Close();
  RunPendingMessages();
  EXPECT_FALSE(widget.received_paint_while_hidden());
}

TEST_F(WidgetTest, DesktopNativeWidgetAuraNoPaintAfterHideTest) {
  View* contents_view = new View;
  contents_view->set_focusable(true);
  DesktopAuraTestValidPaintWidget widget;
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  init_params.bounds = gfx::Rect(0, 0, 200, 200);
  init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  init_params.native_widget = new DesktopNativeWidgetAura(&widget);
  widget.Init(init_params);
  widget.SetContentsView(contents_view);
  widget.Show();
  widget.Activate();
  RunPendingMessages();
  widget.SchedulePaintInRect(init_params.bounds);
  widget.Hide();
  RunPendingMessages();
  EXPECT_FALSE(widget.received_paint_while_hidden());
  widget.Close();
}

// This class provides functionality to test whether the destruction of full
// screen child windows occurs correctly in desktop AURA without crashing.
// It provides facilities to test the following cases:-
// 1. Child window destroyed which should lead to the destruction of the
//    parent.
// 2. Parent window destroyed which should lead to the child being destroyed.
class DesktopAuraFullscreenChildWindowDestructionTest
    : public views::TestViewsDelegate,
      public aura::WindowObserver {
 public:
  DesktopAuraFullscreenChildWindowDestructionTest()
      : full_screen_widget_(NULL),
        child_window_(NULL),
        parent_destroyed_(false),
        child_destroyed_(false) {}

  ~DesktopAuraFullscreenChildWindowDestructionTest() {
    EXPECT_TRUE(parent_destroyed_);
    EXPECT_TRUE(child_destroyed_);
    full_screen_widget_ = NULL;
    child_window_ = NULL;
  }

  // views::TestViewsDelegate overrides.
  virtual void OnBeforeWidgetInit(
      Widget::InitParams* params,
      internal::NativeWidgetDelegate* delegate) OVERRIDE {
    if (!params->native_widget)
      params->native_widget = new views::DesktopNativeWidgetAura(delegate);
  }

  void CreateFullscreenChildWindow(const gfx::Rect& bounds) {
    Widget::InitParams init_params;
    init_params.type = Widget::InitParams::TYPE_WINDOW;
    init_params.bounds = bounds;
    init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
    init_params.layer_type = ui::LAYER_NOT_DRAWN;

    widget_.Init(init_params);

    child_window_ = new aura::Window(&child_window_delegate_);
    child_window_->SetType(aura::client::WINDOW_TYPE_NORMAL);
    child_window_->Init(ui::LAYER_TEXTURED);
    child_window_->SetName("TestFullscreenChildWindow");
    child_window_->SetProperty(aura::client::kShowStateKey,
                              ui::SHOW_STATE_FULLSCREEN);
    child_window_->SetDefaultParentByRootWindow(
        widget_.GetNativeView()->GetRootWindow(), gfx::Rect(0, 0, 1900, 1600));
    child_window_->Show();
    child_window_->AddObserver(this);

    ASSERT_TRUE(child_window_->parent() != NULL);
    child_window_->parent()->AddObserver(this);

    full_screen_widget_ =
        views::Widget::GetWidgetForNativeView(child_window_->parent());
    ASSERT_TRUE(full_screen_widget_ != NULL);
  }

  void DestroyChildWindow() {
    ASSERT_TRUE(child_window_ != NULL);
    delete child_window_;
  }

  void DestroyParentWindow() {
    ASSERT_TRUE(full_screen_widget_ != NULL);
    full_screen_widget_->CloseNow();
  }

  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE {
    window->RemoveObserver(this);
    if (window == child_window_) {
      child_destroyed_ = true;
    } else if (window == full_screen_widget_->GetNativeView()) {
      parent_destroyed_ = true;
    } else {
      ADD_FAILURE() << "Unexpected window destroyed callback: " << window;
    }
  }

 private:
  views::Widget widget_;
  views::Widget* full_screen_widget_;
  aura::Window* child_window_;
  bool parent_destroyed_;
  bool child_destroyed_;
  aura::test::TestWindowDelegate child_window_delegate_;

  DISALLOW_COPY_AND_ASSIGN(DesktopAuraFullscreenChildWindowDestructionTest);
};

TEST_F(WidgetTest, DesktopAuraFullscreenChildDestroyedBeforeParentTest) {
  ViewsDelegate::views_delegate = NULL;
  DesktopAuraFullscreenChildWindowDestructionTest full_screen_child_test;
  ASSERT_NO_FATAL_FAILURE(full_screen_child_test.CreateFullscreenChildWindow(
      gfx::Rect(0, 0, 200, 200)));

  RunPendingMessages();
  ASSERT_NO_FATAL_FAILURE(full_screen_child_test.DestroyChildWindow());
  RunPendingMessages();
}

TEST_F(WidgetTest, DesktopAuraFullscreenChildParentDestroyed) {
  ViewsDelegate::views_delegate = NULL;

  DesktopAuraFullscreenChildWindowDestructionTest full_screen_child_test;
  ASSERT_NO_FATAL_FAILURE(full_screen_child_test.CreateFullscreenChildWindow(
      gfx::Rect(0, 0, 200, 200)));

  RunPendingMessages();
  ASSERT_NO_FATAL_FAILURE(full_screen_child_test.DestroyParentWindow());
  RunPendingMessages();
}

// Test to ensure that the aura Window's visiblity state is set to visible if
// the underlying widget is hidden and then shown.
TEST_F(WidgetTest, TestWindowVisibilityAfterHide) {
  // Create a widget.
  Widget widget;
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW);
  init_params.show_state = ui::SHOW_STATE_NORMAL;
  gfx::Rect initial_bounds(0, 0, 300, 400);
  init_params.bounds = initial_bounds;
  init_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  init_params.native_widget = new DesktopNativeWidgetAura(&widget);
  widget.Init(init_params);
  NonClientView* non_client_view = widget.non_client_view();
  NonClientFrameView* frame_view = new MinimumSizeFrameView(&widget);
  non_client_view->SetFrameView(frame_view);

  widget.Hide();
  EXPECT_FALSE(widget.GetNativeView()->IsVisible());
  widget.Show();
  EXPECT_TRUE(widget.GetNativeView()->IsVisible());
}

#endif  // !defined(OS_CHROMEOS)

// Tests that wheel events generated from scroll events are targetted to the
// views under the cursor when the focused view does not processed them.
TEST_F(WidgetTest, WheelEventsFromScrollEventTarget) {
  EventCountView* cursor_view = new EventCountView;
  cursor_view->SetBounds(60, 0, 50, 40);

  Widget* widget = CreateTopLevelPlatformWidget();
  widget->GetRootView()->AddChildView(cursor_view);

  // Generate a scroll event on the cursor view.
  ui::ScrollEvent scroll(ui::ET_SCROLL,
                         gfx::Point(65, 5),
                         ui::EventTimeForNow(),
                         0,
                         0, 20,
                         0, 20,
                         2);
  widget->OnScrollEvent(&scroll);

  EXPECT_EQ(1, cursor_view->GetEventCount(ui::ET_SCROLL));
  EXPECT_EQ(1, cursor_view->GetEventCount(ui::ET_MOUSEWHEEL));

  cursor_view->ResetCounts();

  ui::ScrollEvent scroll2(ui::ET_SCROLL,
                          gfx::Point(5, 5),
                          ui::EventTimeForNow(),
                          0,
                          0, 20,
                          0, 20,
                          2);
  widget->OnScrollEvent(&scroll2);

  EXPECT_EQ(0, cursor_view->GetEventCount(ui::ET_SCROLL));
  EXPECT_EQ(0, cursor_view->GetEventCount(ui::ET_MOUSEWHEEL));

  widget->CloseNow();
}

#endif  // defined(USE_AURA)

// Tests that if a scroll-begin gesture is not handled, then subsequent scroll
// events are not dispatched to any view.
TEST_F(WidgetTest, GestureScrollEventDispatching) {
  EventCountView* noscroll_view = new EventCountView;
  EventCountView* scroll_view = new ScrollableEventCountView;

  noscroll_view->SetBounds(0, 0, 50, 40);
  scroll_view->SetBounds(60, 0, 40, 40);

  Widget* widget = CreateTopLevelPlatformWidget();
  widget->GetRootView()->AddChildView(noscroll_view);
  widget->GetRootView()->AddChildView(scroll_view);

  {
    ui::GestureEvent begin(ui::ET_GESTURE_SCROLL_BEGIN,
        5, 5, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_BEGIN, 0, 0),
        1);
    widget->OnGestureEvent(&begin);
    ui::GestureEvent update(ui::ET_GESTURE_SCROLL_UPDATE,
        25, 15, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_UPDATE, 20, 10),
        1);
    widget->OnGestureEvent(&update);
    ui::GestureEvent end(ui::ET_GESTURE_SCROLL_END,
        25, 15, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_END, 0, 0),
        1);
    widget->OnGestureEvent(&end);

    EXPECT_EQ(1, noscroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_BEGIN));
    EXPECT_EQ(0, noscroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_UPDATE));
    EXPECT_EQ(0, noscroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_END));
  }

  {
    ui::GestureEvent begin(ui::ET_GESTURE_SCROLL_BEGIN,
        65, 5, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_BEGIN, 0, 0),
        1);
    widget->OnGestureEvent(&begin);
    ui::GestureEvent update(ui::ET_GESTURE_SCROLL_UPDATE,
        85, 15, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_UPDATE, 20, 10),
        1);
    widget->OnGestureEvent(&update);
    ui::GestureEvent end(ui::ET_GESTURE_SCROLL_END,
        85, 15, 0, base::TimeDelta(),
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_END, 0, 0),
        1);
    widget->OnGestureEvent(&end);

    EXPECT_EQ(1, scroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_BEGIN));
    EXPECT_EQ(1, scroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_UPDATE));
    EXPECT_EQ(1, scroll_view->GetEventCount(ui::ET_GESTURE_SCROLL_END));
  }

  widget->CloseNow();
}

// Tests that event-handlers installed on the RootView get triggered correctly.
TEST_F(WidgetTest, EventHandlersOnRootView) {
  Widget* widget = CreateTopLevelNativeWidget();
  View* root_view = widget->GetRootView();

  EventCountView* view = new EventCountView;
  view->SetBounds(0, 0, 20, 20);
  root_view->AddChildView(view);

  EventCountHandler h1;
  root_view->AddPreTargetHandler(&h1);

  EventCountHandler h2;
  root_view->AddPostTargetHandler(&h2);

  widget->SetBounds(gfx::Rect(0, 0, 100, 100));
  widget->Show();

  ui::TouchEvent pressed(ui::ET_TOUCH_PRESSED,
                         gfx::Point(10, 10),
                         0, 0,
                         ui::EventTimeForNow(),
                         1.0, 0.0, 1.0, 0.0);
  widget->OnTouchEvent(&pressed);
  EXPECT_EQ(1, h1.GetEventCount(ui::ET_TOUCH_PRESSED));
  EXPECT_EQ(1, view->GetEventCount(ui::ET_TOUCH_PRESSED));
  EXPECT_EQ(1, h2.GetEventCount(ui::ET_TOUCH_PRESSED));

  ui::GestureEvent begin(ui::ET_GESTURE_BEGIN,
      5, 5, 0, ui::EventTimeForNow(),
      ui::GestureEventDetails(ui::ET_GESTURE_BEGIN, 0, 0), 1);
  ui::GestureEvent end(ui::ET_GESTURE_END,
      5, 5, 0, ui::EventTimeForNow(),
      ui::GestureEventDetails(ui::ET_GESTURE_END, 0, 0), 1);
  widget->OnGestureEvent(&begin);
  EXPECT_EQ(1, h1.GetEventCount(ui::ET_GESTURE_BEGIN));
  EXPECT_EQ(1, view->GetEventCount(ui::ET_GESTURE_BEGIN));
  EXPECT_EQ(1, h2.GetEventCount(ui::ET_GESTURE_BEGIN));

  ui::TouchEvent released(ui::ET_TOUCH_RELEASED,
                          gfx::Point(10, 10),
                          0, 0,
                          ui::EventTimeForNow(),
                          1.0, 0.0, 1.0, 0.0);
  widget->OnTouchEvent(&released);
  EXPECT_EQ(1, h1.GetEventCount(ui::ET_TOUCH_RELEASED));
  EXPECT_EQ(1, view->GetEventCount(ui::ET_TOUCH_RELEASED));
  EXPECT_EQ(1, h2.GetEventCount(ui::ET_TOUCH_RELEASED));

  widget->OnGestureEvent(&end);
  EXPECT_EQ(1, h1.GetEventCount(ui::ET_GESTURE_END));
  EXPECT_EQ(1, view->GetEventCount(ui::ET_GESTURE_END));
  EXPECT_EQ(1, h2.GetEventCount(ui::ET_GESTURE_END));

  ui::ScrollEvent scroll(ui::ET_SCROLL,
                         gfx::Point(5, 5),
                         ui::EventTimeForNow(),
                         0,
                         0, 20,
                         0, 20,
                         2);
  widget->OnScrollEvent(&scroll);
  EXPECT_EQ(1, h1.GetEventCount(ui::ET_SCROLL));
  EXPECT_EQ(1, view->GetEventCount(ui::ET_SCROLL));
  EXPECT_EQ(1, h2.GetEventCount(ui::ET_SCROLL));

  widget->CloseNow();
}

TEST_F(WidgetTest, SynthesizeMouseMoveEvent) {
  Widget* widget = CreateTopLevelNativeWidget();
  View* root_view = widget->GetRootView();

  EventCountView* v1 = new EventCountView();
  v1->SetBounds(0, 0, 10, 10);
  root_view->AddChildView(v1);
  EventCountView* v2 = new EventCountView();
  v2->SetBounds(0, 10, 10, 10);
  root_view->AddChildView(v2);

  gfx::Point cursor_location(5, 5);
  ui::MouseEvent move(ui::ET_MOUSE_MOVED, cursor_location, cursor_location,
                      ui::EF_NONE);
  widget->OnMouseEvent(&move);

  EXPECT_EQ(1, v1->GetEventCount(ui::ET_MOUSE_ENTERED));
  EXPECT_EQ(0, v2->GetEventCount(ui::ET_MOUSE_ENTERED));

  delete v1;
  v2->SetBounds(0, 0, 10, 10);
  EXPECT_EQ(0, v2->GetEventCount(ui::ET_MOUSE_ENTERED));

  widget->SynthesizeMouseMoveEvent();
  EXPECT_EQ(1, v2->GetEventCount(ui::ET_MOUSE_ENTERED));
}

// Used by SingleWindowClosing to count number of times WindowClosing() has
// been invoked.
class ClosingDelegate : public WidgetDelegate {
 public:
  ClosingDelegate() : count_(0), widget_(NULL) {}

  int count() const { return count_; }

  void set_widget(views::Widget* widget) { widget_ = widget; }

  // WidgetDelegate overrides:
  virtual Widget* GetWidget() OVERRIDE { return widget_; }
  virtual const Widget* GetWidget() const OVERRIDE { return widget_; }
  virtual void WindowClosing() OVERRIDE {
    count_++;
  }

 private:
  int count_;
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(ClosingDelegate);
};

// Verifies WindowClosing() is invoked correctly on the delegate when a Widget
// is closed.
TEST_F(WidgetTest, SingleWindowClosing) {
  scoped_ptr<ClosingDelegate> delegate(new ClosingDelegate());
  Widget* widget = new Widget();  // Destroyed by CloseNow() below.
  Widget::InitParams init_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW);
  init_params.bounds = gfx::Rect(0, 0, 200, 200);
  init_params.delegate = delegate.get();
#if defined(USE_AURA) && !defined(OS_CHROMEOS)
  init_params.native_widget = new DesktopNativeWidgetAura(widget);
#endif
  widget->Init(init_params);
  EXPECT_EQ(0, delegate->count());
  widget->CloseNow();
  EXPECT_EQ(1, delegate->count());
}

// Used by SetTopLevelCorrectly to track calls to OnBeforeWidgetInit().
class VerifyTopLevelDelegate : public TestViewsDelegate {
 public:
  VerifyTopLevelDelegate()
      : on_before_init_called_(false),
        is_top_level_(false) {
  }

  bool on_before_init_called() const { return on_before_init_called_; }
  bool is_top_level() const { return is_top_level_; }

  virtual void OnBeforeWidgetInit(
      Widget::InitParams* params,
      internal::NativeWidgetDelegate* delegate) OVERRIDE {
    on_before_init_called_ = true;
    is_top_level_ = params->top_level;
  }

 private:
  bool on_before_init_called_;
  bool is_top_level_;

  DISALLOW_COPY_AND_ASSIGN(VerifyTopLevelDelegate);
};

// Verifies |top_level| is correctly passed to
// ViewsDelegate::OnBeforeWidgetInit().
TEST_F(WidgetTest, SetTopLevelCorrectly) {
  set_views_delegate(NULL);
  VerifyTopLevelDelegate* delegate = new VerifyTopLevelDelegate;
  set_views_delegate(delegate);  // ViewsTestBase takes ownership.
  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params =
      CreateParams(views::Widget::InitParams::TYPE_POPUP);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);
  EXPECT_TRUE(delegate->on_before_init_called());
  EXPECT_TRUE(delegate->is_top_level());
}

// A scumbag View that deletes its owning widget OnMousePressed.
class WidgetDeleterView : public View {
 public:
  WidgetDeleterView() : View() {}

  // Overridden from View.
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE {
    delete GetWidget();
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(WidgetDeleterView);
};

TEST_F(WidgetTest, TestWidgetDeletedInOnMousePressed) {
  Widget* widget = new Widget;
  Widget::InitParams params =
      CreateParams(views::Widget::InitParams::TYPE_POPUP);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);

  widget->SetContentsView(new WidgetDeleterView);

  widget->SetSize(gfx::Size(100, 100));
  widget->Show();

  gfx::Point click_location(45, 15);
  ui::MouseEvent press(ui::ET_MOUSE_PRESSED, click_location, click_location,
      ui::EF_LEFT_MOUSE_BUTTON);
  widget->OnMouseEvent(&press);

  // Yay we did not crash!
}

}  // namespace
}  // namespace views
