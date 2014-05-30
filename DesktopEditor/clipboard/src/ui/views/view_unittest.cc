// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/rand_util.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_strings.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animator.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/transform.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/button_dropdown.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/focus/view_storage.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/view.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/native_widget.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/window/dialog_client_view.h"
#include "ui/views/window/dialog_delegate.h"

#if defined(OS_WIN)
#include "ui/views/test/test_views_delegate.h"
#endif
#if defined(USE_AURA)
#include "ui/aura/root_window.h"
#include "ui/base/gestures/gesture_recognizer.h"
#endif

using ::testing::_;

namespace {

// Returns true if |ancestor| is an ancestor of |layer|.
bool LayerIsAncestor(const ui::Layer* ancestor, const ui::Layer* layer) {
  while (layer && layer != ancestor)
    layer = layer->parent();
  return layer == ancestor;
}

// Convenience functions for walking a View tree.
const views::View* FirstView(const views::View* view) {
  const views::View* v = view;
  while (v->has_children())
    v = v->child_at(0);
  return v;
}

const views::View* NextView(const views::View* view) {
  const views::View* v = view;
  const views::View* parent = v->parent();
  if (!parent)
    return NULL;
  int next = parent->GetIndexOf(v) + 1;
  if (next != parent->child_count())
    return FirstView(parent->child_at(next));
  return parent;
}

// Convenience functions for walking a Layer tree.
const ui::Layer* FirstLayer(const ui::Layer* layer) {
  const ui::Layer* l = layer;
  while (l->children().size() > 0)
    l = l->children()[0];
  return l;
}

const ui::Layer* NextLayer(const ui::Layer* layer) {
  const ui::Layer* parent = layer->parent();
  if (!parent)
    return NULL;
  const std::vector<ui::Layer*> children = parent->children();
  size_t index;
  for (index = 0; index < children.size(); index++) {
    if (children[index] == layer)
      break;
  }
  size_t next = index + 1;
  if (next < children.size())
    return FirstLayer(children[next]);
  return parent;
}

// Given the root nodes of a View tree and a Layer tree, makes sure the two
// trees are in sync.
bool ViewAndLayerTreeAreConsistent(const views::View* view,
                                   const ui::Layer* layer) {
  const views::View* v = FirstView(view);
  const ui::Layer* l = FirstLayer(layer);
  while (v && l) {
    // Find the view with a layer.
    while (v && !v->layer())
      v = NextView(v);
    EXPECT_TRUE(v);
    if (!v)
      return false;

    // Check if the View tree and the Layer tree are in sync.
    EXPECT_EQ(l, v->layer());
    if (v->layer() != l)
      return false;

    // Check if the visibility states of the View and the Layer are in sync.
    EXPECT_EQ(l->IsDrawn(), v->IsDrawn());
    if (v->IsDrawn() != l->IsDrawn()) {
      for (const views::View* vv = v; vv; vv = vv->parent())
        LOG(ERROR) << "V: " << vv << " " << vv->visible() << " "
                   << vv->IsDrawn() << " " << vv->layer();
      for (const ui::Layer* ll = l; ll; ll = ll->parent())
        LOG(ERROR) << "L: " << ll << " " << ll->IsDrawn();
      return false;
    }

    // Check if the size of the View and the Layer are in sync.
    EXPECT_EQ(l->bounds(), v->bounds());
    if (v->bounds() != l->bounds())
      return false;

    if (v == view || l == layer)
      return v == view && l == layer;

    v = NextView(v);
    l = NextLayer(l);
  }

  return false;
}

// Constructs a View tree with the specified depth.
void ConstructTree(views::View* view, int depth) {
  if (depth == 0)
    return;
  int count = base::RandInt(1, 5);
  for (int i = 0; i < count; i++) {
    views::View* v = new views::View;
    view->AddChildView(v);
    if (base::RandDouble() > 0.5)
      v->SetPaintToLayer(true);
    if (base::RandDouble() < 0.2)
      v->SetVisible(false);

    ConstructTree(v, depth - 1);
  }
}

void ScrambleTree(views::View* view) {
  int count = view->child_count();
  if (count == 0)
    return;
  for (int i = 0; i < count; i++) {
    ScrambleTree(view->child_at(i));
  }

  if (count > 1) {
    int a = base::RandInt(0, count - 1);
    int b = base::RandInt(0, count - 1);

    views::View* view_a = view->child_at(a);
    views::View* view_b = view->child_at(b);
    view->ReorderChildView(view_a, b);
    view->ReorderChildView(view_b, a);
  }

  if (!view->layer() && base::RandDouble() < 0.1)
    view->SetPaintToLayer(true);

  if (base::RandDouble() < 0.1)
    view->SetVisible(!view->visible());
}

// Convenience to make constructing a GestureEvent simpler.
class GestureEventForTest : public ui::GestureEvent {
 public:
  GestureEventForTest(ui::EventType type, int x, int y, int flags)
      : GestureEvent(type, x, y, flags, base::TimeDelta(),
                     ui::GestureEventDetails(type, 0.0f, 0.0f), 0) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(GestureEventForTest);
};

}  // namespace

namespace views {

typedef ViewsTestBase ViewTest;

// A derived class for testing purpose.
class TestView : public View {
 public:
  TestView() : View(), delete_on_pressed_(false), in_touch_sequence_(false) {}
  virtual ~TestView() {}

  // Reset all test state
  void Reset() {
    did_change_bounds_ = false;
    last_mouse_event_type_ = 0;
    location_.SetPoint(0, 0);
    received_mouse_enter_ = false;
    received_mouse_exit_ = false;
    last_touch_event_type_ = 0;
    last_touch_event_was_handled_ = false;
    last_gesture_event_type_ = 0;
    last_gesture_event_was_handled_ = false;
    last_clip_.setEmpty();
    accelerator_count_map_.clear();
  }

  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  // Ignores GestureEvent by default.
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;
  virtual void SchedulePaintInRect(const gfx::Rect& rect) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  // OnBoundsChanged.
  bool did_change_bounds_;
  gfx::Rect new_bounds_;

  // MouseEvent.
  int last_mouse_event_type_;
  gfx::Point location_;
  bool received_mouse_enter_;
  bool received_mouse_exit_;
  bool delete_on_pressed_;

  // Painting.
  std::vector<gfx::Rect> scheduled_paint_rects_;

  // GestureEvent
  int last_gesture_event_type_;
  bool last_gesture_event_was_handled_;

  // TouchEvent.
  int last_touch_event_type_;
  bool last_touch_event_was_handled_;
  bool in_touch_sequence_;

  // Painting.
  SkRect last_clip_;

  // Accelerators.
  std::map<ui::Accelerator, int> accelerator_count_map_;
};

// A view subclass that ignores all touch events for testing purposes.
class TestViewIgnoreTouch : public TestView {
 public:
  TestViewIgnoreTouch() : TestView() {}
  virtual ~TestViewIgnoreTouch() {}

 private:
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
};

// A view subclass that consumes all Gesture events for testing purposes.
class TestViewConsumeGesture : public TestView {
 public:
  TestViewConsumeGesture() : TestView() {}
  virtual ~TestViewConsumeGesture() {}

 protected:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    last_gesture_event_type_ = event->type();
    location_.SetPoint(event->x(), event->y());
    event->StopPropagation();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestViewConsumeGesture);
};

// A view subclass that ignores all Gesture events.
class TestViewIgnoreGesture: public TestView {
 public:
  TestViewIgnoreGesture() : TestView() {}
  virtual ~TestViewIgnoreGesture() {}

 private:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
  }

  DISALLOW_COPY_AND_ASSIGN(TestViewIgnoreGesture);
};

// A view subclass that ignores all scroll-gesture events, but consume all other
// gesture events.
class TestViewIgnoreScrollGestures : public TestViewConsumeGesture {
 public:
  TestViewIgnoreScrollGestures() {}
  virtual ~TestViewIgnoreScrollGestures() {}

 private:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    if (event->IsScrollGestureEvent())
      return;
    TestViewConsumeGesture::OnGestureEvent(event);
  }

  DISALLOW_COPY_AND_ASSIGN(TestViewIgnoreScrollGestures);
};

////////////////////////////////////////////////////////////////////////////////
// OnBoundsChanged
////////////////////////////////////////////////////////////////////////////////

void TestView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  did_change_bounds_ = true;
  new_bounds_ = bounds();
}

TEST_F(ViewTest, OnBoundsChanged) {
  TestView v;

  gfx::Rect prev_rect(0, 0, 200, 200);
  gfx::Rect new_rect(100, 100, 250, 250);

  v.SetBoundsRect(prev_rect);
  v.Reset();
  v.SetBoundsRect(new_rect);

  EXPECT_TRUE(v.did_change_bounds_);
  EXPECT_EQ(v.new_bounds_, new_rect);
  EXPECT_EQ(v.bounds(), new_rect);
}

////////////////////////////////////////////////////////////////////////////////
// MouseEvent
////////////////////////////////////////////////////////////////////////////////

bool TestView::OnMousePressed(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  if (delete_on_pressed_)
    delete this;
  return true;
}

bool TestView::OnMouseDragged(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  return true;
}

void TestView::OnMouseReleased(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
}

void TestView::OnMouseEntered(const ui::MouseEvent& event) {
  received_mouse_enter_ = true;
}

void TestView::OnMouseExited(const ui::MouseEvent& event) {
  received_mouse_exit_ = true;
}

TEST_F(ViewTest, MouseEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);

  v1->Reset();
  v2->Reset();

  gfx::Point p1(110, 120);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1,
                         ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_PRESSED);
  EXPECT_EQ(v2->location_.x(), 10);
  EXPECT_EQ(v2->location_.y(), 20);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Drag event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  gfx::Point p2(50, 40);
  ui::MouseEvent dragged(ui::ET_MOUSE_DRAGGED, p2, p2,
                         ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMouseDragged(dragged);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_DRAGGED);
  EXPECT_EQ(v2->location_.x(), -50);
  EXPECT_EQ(v2->location_.y(), -60);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Releasted event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(), 0);
  root->OnMouseDragged(released);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_RELEASED);
  EXPECT_EQ(v2->location_.x(), -100);
  EXPECT_EQ(v2->location_.y(), -100);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  widget->CloseNow();
}

// Confirm that a view can be deleted as part of processing a mouse press.
TEST_F(ViewTest, DeleteOnPressed) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  v1->Reset();
  v2->Reset();

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  v2->delete_on_pressed_ = true;
  gfx::Point point(110, 120);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_EQ(0, v1->child_count());

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// TouchEvent
////////////////////////////////////////////////////////////////////////////////
void TestView::OnTouchEvent(ui::TouchEvent* event) {
  last_touch_event_type_ = event->type();
  location_.SetPoint(event->x(), event->y());
  if (!in_touch_sequence_) {
    if (event->type() == ui::ET_TOUCH_PRESSED) {
      in_touch_sequence_ = true;
      event->StopPropagation();
      return;
    }
  } else {
    if (event->type() == ui::ET_TOUCH_RELEASED) {
      in_touch_sequence_ = false;
      event->SetHandled();
      return;
    }
    event->StopPropagation();
    return;
  }

  if (last_touch_event_was_handled_)
   event->StopPropagation();
}

void TestViewIgnoreTouch::OnTouchEvent(ui::TouchEvent* event) {
}

TEST_F(ViewTest, TouchEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  TestView* v3 = new TestViewIgnoreTouch();
  v3->SetBoundsRect(gfx::Rect(0, 0, 100, 100));

  scoped_ptr<Widget> widget(new Widget());
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);
  v2->AddChildView(v3);

  // |v3| completely obscures |v2|, but all the touch events on |v3| should
  // reach |v2| because |v3| doesn't process any touch events.

  // Make sure if none of the views handle the touch event, the gesture manager
  // does.
  v1->Reset();
  v2->Reset();

  ui::TouchEvent unhandled(ui::ET_TOUCH_MOVED,
                           gfx::Point(400, 400),
                           0, /* no flags */
                           0, /* first finger touch */
                           base::TimeDelta(),
                           1.0, 0.0, 1.0, 0.0);
  root->DispatchTouchEvent(&unhandled);

  EXPECT_EQ(v1->last_touch_event_type_, 0);
  EXPECT_EQ(v2->last_touch_event_type_, 0);

  // Test press, drag, release touch sequence.
  v1->Reset();
  v2->Reset();

  ui::TouchEvent pressed(ui::ET_TOUCH_PRESSED,
                         gfx::Point(110, 120),
                         0, /* no flags */
                         0, /* first finger touch */
                         base::TimeDelta(),
                         1.0, 0.0, 1.0, 0.0);
  v2->last_touch_event_was_handled_ = true;
  root->DispatchTouchEvent(&pressed);

  EXPECT_EQ(v2->last_touch_event_type_, ui::ET_TOUCH_PRESSED);
  EXPECT_EQ(v2->location_.x(), 10);
  EXPECT_EQ(v2->location_.y(), 20);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_touch_event_type_, 0);

  // Drag event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  ui::TouchEvent dragged(ui::ET_TOUCH_MOVED,
                         gfx::Point(50, 40),
                         0, /* no flags */
                         0, /* first finger touch */
                         base::TimeDelta(),
                         1.0, 0.0, 1.0, 0.0);

  root->DispatchTouchEvent(&dragged);
  EXPECT_EQ(v2->last_touch_event_type_, ui::ET_TOUCH_MOVED);
  EXPECT_EQ(v2->location_.x(), -50);
  EXPECT_EQ(v2->location_.y(), -60);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_touch_event_type_, 0);

  // Released event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  ui::TouchEvent released(ui::ET_TOUCH_RELEASED, gfx::Point(),
                          0, /* no flags */
                          0, /* first finger */
                          base::TimeDelta(),
                          1.0, 0.0, 1.0, 0.0);
  v2->last_touch_event_was_handled_ = true;
  root->DispatchTouchEvent(&released);
  EXPECT_EQ(v2->last_touch_event_type_, ui::ET_TOUCH_RELEASED);
  EXPECT_EQ(v2->location_.x(), -100);
  EXPECT_EQ(v2->location_.y(), -100);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_touch_event_type_, 0);

  widget->CloseNow();
}

void TestView::OnGestureEvent(ui::GestureEvent* event) {
}

TEST_F(ViewTest, GestureEvent) {
  // Views hierarchy for non delivery of GestureEvent.
  TestView* v1 = new TestViewConsumeGesture();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestViewConsumeGesture();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  TestView* v3 = new TestViewIgnoreGesture();
  v3->SetBoundsRect(gfx::Rect(0, 0, 100, 100));

  scoped_ptr<Widget> widget(new Widget());
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);
  v2->AddChildView(v3);

  // |v3| completely obscures |v2|, but all the gesture events on |v3| should
  // reach |v2| because |v3| doesn't process any gesture events. However, since
  // |v2| does process gesture events, gesture events on |v3| or |v2| should not
  // reach |v1|.

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Gesture on |v3|
  GestureEventForTest g1(ui::ET_GESTURE_TAP, 110, 110, 0);
  root->DispatchGestureEvent(&g1);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v2->last_gesture_event_type_);
  EXPECT_EQ(gfx::Point(10, 10), v2->location_);
  EXPECT_EQ(ui::ET_UNKNOWN, v1->last_gesture_event_type_);

  // Simulate an up so that RootView is no longer targetting |v3|.
  GestureEventForTest g1_up(ui::ET_GESTURE_END, 110, 110, 0);
  root->DispatchGestureEvent(&g1_up);

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Gesture on |v1|
  GestureEventForTest g2(ui::ET_GESTURE_TAP, 80, 80, 0);
  root->DispatchGestureEvent(&g2);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v1->last_gesture_event_type_);
  EXPECT_EQ(gfx::Point(80, 80), v1->location_);
  EXPECT_EQ(ui::ET_UNKNOWN, v2->last_gesture_event_type_);

  // Send event |g1| again. Even though the coordinates target |v3| it should go
  // to |v1| as that is the view the touch was initially down on.
  v1->last_gesture_event_type_ = ui::ET_UNKNOWN;
  v3->last_gesture_event_type_ = ui::ET_UNKNOWN;
  root->DispatchGestureEvent(&g1);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v1->last_gesture_event_type_);
  EXPECT_EQ(ui::ET_UNKNOWN, v3->last_gesture_event_type_);
  EXPECT_EQ("110,110", v1->location_.ToString());

  widget->CloseNow();
}

TEST_F(ViewTest, ScrollGestureEvent) {
  // Views hierarchy for non delivery of GestureEvent.
  TestView* v1 = new TestViewConsumeGesture();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestViewIgnoreScrollGestures();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  TestView* v3 = new TestViewIgnoreGesture();
  v3->SetBoundsRect(gfx::Rect(0, 0, 100, 100));

  scoped_ptr<Widget> widget(new Widget());
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);
  v2->AddChildView(v3);

  // |v3| completely obscures |v2|, but all the gesture events on |v3| should
  // reach |v2| because |v3| doesn't process any gesture events. However, since
  // |v2| does process gesture events, gesture events on |v3| or |v2| should not
  // reach |v1|.

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Gesture on |v3|
  GestureEventForTest g1(ui::ET_GESTURE_TAP, 110, 110, 0);
  root->DispatchGestureEvent(&g1);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v2->last_gesture_event_type_);
  EXPECT_EQ(gfx::Point(10, 10), v2->location_);
  EXPECT_EQ(ui::ET_UNKNOWN, v1->last_gesture_event_type_);

  v2->Reset();

  // Send scroll gestures on |v3|. The gesture should reach |v2|, however,
  // since it does not process scroll-gesture events, these events should reach
  // |v1|.
  GestureEventForTest gscroll_begin(ui::ET_GESTURE_SCROLL_BEGIN, 115, 115, 0);
  root->DispatchGestureEvent(&gscroll_begin);
  EXPECT_EQ(ui::ET_UNKNOWN, v2->last_gesture_event_type_);
  EXPECT_EQ(ui::ET_GESTURE_SCROLL_BEGIN, v1->last_gesture_event_type_);
  v1->Reset();

  // Send a second tap on |v1|. The event should reach |v2| since it is the
  // default gesture handler, and not |v1| (even though it is the view under the
  // point, and is the scroll event handler).
  GestureEventForTest second_tap(ui::ET_GESTURE_TAP, 70, 70, 0);
  root->DispatchGestureEvent(&second_tap);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v2->last_gesture_event_type_);
  EXPECT_EQ(ui::ET_UNKNOWN, v1->last_gesture_event_type_);
  v2->Reset();

  GestureEventForTest gscroll_end(ui::ET_GESTURE_SCROLL_END, 50, 50, 0);
  root->DispatchGestureEvent(&gscroll_end);
  EXPECT_EQ(ui::ET_GESTURE_SCROLL_END, v1->last_gesture_event_type_);
  v1->Reset();

  // Simulate an up so that RootView is no longer targetting |v3|.
  GestureEventForTest g1_up(ui::ET_GESTURE_END, 110, 110, 0);
  root->DispatchGestureEvent(&g1_up);
  EXPECT_EQ(ui::ET_GESTURE_END, v2->last_gesture_event_type_);

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Gesture on |v1|
  GestureEventForTest g2(ui::ET_GESTURE_TAP, 80, 80, 0);
  root->DispatchGestureEvent(&g2);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v1->last_gesture_event_type_);
  EXPECT_EQ(gfx::Point(80, 80), v1->location_);
  EXPECT_EQ(ui::ET_UNKNOWN, v2->last_gesture_event_type_);

  // Send event |g1| again. Even though the coordinates target |v3| it should go
  // to |v1| as that is the view the touch was initially down on.
  v1->last_gesture_event_type_ = ui::ET_UNKNOWN;
  v3->last_gesture_event_type_ = ui::ET_UNKNOWN;
  root->DispatchGestureEvent(&g1);
  EXPECT_EQ(ui::ET_GESTURE_TAP, v1->last_gesture_event_type_);
  EXPECT_EQ(ui::ET_UNKNOWN, v3->last_gesture_event_type_);
  EXPECT_EQ("110,110", v1->location_.ToString());

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// Painting
////////////////////////////////////////////////////////////////////////////////

void TestView::Paint(gfx::Canvas* canvas) {
  canvas->sk_canvas()->getClipBounds(&last_clip_);
}

void TestView::SchedulePaintInRect(const gfx::Rect& rect) {
  scheduled_paint_rects_.push_back(rect);
  View::SchedulePaintInRect(rect);
}

void CheckRect(const SkRect& check_rect, const SkRect& target_rect) {
  EXPECT_EQ(target_rect.fLeft, check_rect.fLeft);
  EXPECT_EQ(target_rect.fRight, check_rect.fRight);
  EXPECT_EQ(target_rect.fTop, check_rect.fTop);
  EXPECT_EQ(target_rect.fBottom, check_rect.fBottom);
}

/* This test is disabled because it is flakey on some systems.
TEST_F(ViewTest, DISABLED_Painting) {
  // Determine if InvalidateRect generates an empty paint rectangle.
  EmptyWindow paint_window(CRect(50, 50, 650, 650));
  paint_window.RedrawWindow(CRect(0, 0, 600, 600), NULL,
                            RDW_UPDATENOW | RDW_INVALIDATE | RDW_ALLCHILDREN);
  bool empty_paint = paint_window.empty_paint();

  NativeWidgetWin window;
  window.set_delete_on_destroy(false);
  window.set_window_style(WS_OVERLAPPEDWINDOW);
  window.Init(NULL, gfx::Rect(50, 50, 650, 650), NULL);
  View* root = window.GetRootView();

  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 650, 650));
  root->AddChildView(v1);

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(10, 10, 80, 80));
  v1->AddChildView(v2);

  TestView* v3 = new TestView();
  v3->SetBoundsRect(gfx::Rect(10, 10, 60, 60));
  v2->AddChildView(v3);

  TestView* v4 = new TestView();
  v4->SetBoundsRect(gfx::Rect(10, 200, 100, 100));
  v1->AddChildView(v4);

  // Make sure to paint current rects
  PaintRootView(root, empty_paint);


  v1->Reset();
  v2->Reset();
  v3->Reset();
  v4->Reset();
  v3->SchedulePaintInRect(gfx::Rect(10, 10, 10, 10));
  PaintRootView(root, empty_paint);

  SkRect tmp_rect;

  tmp_rect.iset(10, 10, 20, 20);
  CheckRect(v3->last_clip_, tmp_rect);

  tmp_rect.iset(20, 20, 30, 30);
  CheckRect(v2->last_clip_, tmp_rect);

  tmp_rect.iset(30, 30, 40, 40);
  CheckRect(v1->last_clip_, tmp_rect);

  // Make sure v4 was not painted
  tmp_rect.setEmpty();
  CheckRect(v4->last_clip_, tmp_rect);

  window.DestroyWindow();
}
*/

TEST_F(ViewTest, RemoveNotification) {
  ViewStorage* vs = ViewStorage::GetInstance();
  Widget* widget = new Widget;
  widget->Init(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  View* v1 = new View;
  int s1 = vs->CreateStorageID();
  vs->StoreView(s1, v1);
  root_view->AddChildView(v1);
  View* v11 = new View;
  int s11 = vs->CreateStorageID();
  vs->StoreView(s11, v11);
  v1->AddChildView(v11);
  View* v111 = new View;
  int s111 = vs->CreateStorageID();
  vs->StoreView(s111, v111);
  v11->AddChildView(v111);
  View* v112 = new View;
  int s112 = vs->CreateStorageID();
  vs->StoreView(s112, v112);
  v11->AddChildView(v112);
  View* v113 = new View;
  int s113 = vs->CreateStorageID();
  vs->StoreView(s113, v113);
  v11->AddChildView(v113);
  View* v1131 = new View;
  int s1131 = vs->CreateStorageID();
  vs->StoreView(s1131, v1131);
  v113->AddChildView(v1131);
  View* v12 = new View;
  int s12 = vs->CreateStorageID();
  vs->StoreView(s12, v12);
  v1->AddChildView(v12);

  View* v2 = new View;
  int s2 = vs->CreateStorageID();
  vs->StoreView(s2, v2);
  root_view->AddChildView(v2);
  View* v21 = new View;
  int s21 = vs->CreateStorageID();
  vs->StoreView(s21, v21);
  v2->AddChildView(v21);
  View* v211 = new View;
  int s211 = vs->CreateStorageID();
  vs->StoreView(s211, v211);
  v21->AddChildView(v211);

  size_t stored_views = vs->view_count();

  // Try removing a leaf view.
  v21->RemoveChildView(v211);
  EXPECT_EQ(stored_views - 1, vs->view_count());
  EXPECT_EQ(NULL, vs->RetrieveView(s211));
  delete v211;  // We won't use this one anymore.

  // Now try removing a view with a hierarchy of depth 1.
  v11->RemoveChildView(v113);
  EXPECT_EQ(stored_views - 3, vs->view_count());
  EXPECT_EQ(NULL, vs->RetrieveView(s113));
  EXPECT_EQ(NULL, vs->RetrieveView(s1131));
  delete v113;  // We won't use this one anymore.

  // Now remove even more.
  root_view->RemoveChildView(v1);
  EXPECT_EQ(NULL, vs->RetrieveView(s1));
  EXPECT_EQ(NULL, vs->RetrieveView(s11));
  EXPECT_EQ(NULL, vs->RetrieveView(s12));
  EXPECT_EQ(NULL, vs->RetrieveView(s111));
  EXPECT_EQ(NULL, vs->RetrieveView(s112));

  // Put v1 back for more tests.
  root_view->AddChildView(v1);
  vs->StoreView(s1, v1);

  // Synchronously closing the window deletes the view hierarchy, which should
  // remove all its views from ViewStorage.
  widget->CloseNow();
  EXPECT_EQ(stored_views - 10, vs->view_count());
  EXPECT_EQ(NULL, vs->RetrieveView(s1));
  EXPECT_EQ(NULL, vs->RetrieveView(s12));
  EXPECT_EQ(NULL, vs->RetrieveView(s11));
  EXPECT_EQ(NULL, vs->RetrieveView(s12));
  EXPECT_EQ(NULL, vs->RetrieveView(s21));
  EXPECT_EQ(NULL, vs->RetrieveView(s111));
  EXPECT_EQ(NULL, vs->RetrieveView(s112));
}

namespace {
class HitTestView : public View {
 public:
  explicit HitTestView(bool has_hittest_mask)
      : has_hittest_mask_(has_hittest_mask) {
  }
  virtual ~HitTestView() {}

 protected:
  // Overridden from View:
  virtual bool HasHitTestMask() const OVERRIDE {
    return has_hittest_mask_;
  }
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE {
    DCHECK(has_hittest_mask_);
    DCHECK(mask);

    SkScalar w = SkIntToScalar(width());
    SkScalar h = SkIntToScalar(height());

    // Create a triangular mask within the bounds of this View.
    mask->moveTo(w / 2, 0);
    mask->lineTo(w, h);
    mask->lineTo(0, h);
    mask->close();
  }

 private:
  bool has_hittest_mask_;

  DISALLOW_COPY_AND_ASSIGN(HitTestView);
};

gfx::Point ConvertPointToView(View* view, const gfx::Point& p) {
  gfx::Point tmp(p);
  View::ConvertPointToTarget(view->GetWidget()->GetRootView(), view, &tmp);
  return tmp;
}

gfx::Rect ConvertRectToView(View* view, const gfx::Rect& r) {
  gfx::Rect tmp(r);
  tmp.set_origin(ConvertPointToView(view, r.origin()));
  return tmp;
}

void RotateCounterclockwise(gfx::Transform* transform) {
  transform->matrix().set3x3(0, -1, 0,
                             1,  0, 0,
                             0,  0, 1);
}

void RotateClockwise(gfx::Transform* transform) {
  transform->matrix().set3x3( 0, 1, 0,
                             -1, 0, 0,
                              0, 0, 1);
}

}  // namespace

TEST_F(ViewTest, HitTestMasks) {
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  gfx::Rect v1_bounds = gfx::Rect(0, 0, 100, 100);
  HitTestView* v1 = new HitTestView(false);
  v1->SetBoundsRect(v1_bounds);
  root_view->AddChildView(v1);

  gfx::Rect v2_bounds = gfx::Rect(105, 0, 100, 100);
  HitTestView* v2 = new HitTestView(true);
  v2->SetBoundsRect(v2_bounds);
  root_view->AddChildView(v2);

  gfx::Point v1_centerpoint = v1_bounds.CenterPoint();
  gfx::Point v2_centerpoint = v2_bounds.CenterPoint();
  gfx::Point v1_origin = v1_bounds.origin();
  gfx::Point v2_origin = v2_bounds.origin();

  gfx::Rect r1(10, 10, 110, 15);
  gfx::Rect r2(106, 1, 98, 98);
  gfx::Rect r3(0, 0, 300, 300);
  gfx::Rect r4(115, 342, 200, 10);

  // Test HitTestPoint
  EXPECT_TRUE(v1->HitTestPoint(ConvertPointToView(v1, v1_centerpoint)));
  EXPECT_TRUE(v2->HitTestPoint(ConvertPointToView(v2, v2_centerpoint)));

  EXPECT_TRUE(v1->HitTestPoint(ConvertPointToView(v1, v1_origin)));
  EXPECT_FALSE(v2->HitTestPoint(ConvertPointToView(v2, v2_origin)));

  // Test HitTestRect
  EXPECT_TRUE(v1->HitTestRect(ConvertRectToView(v1, r1)));
  EXPECT_FALSE(v2->HitTestRect(ConvertRectToView(v2, r1)));

  EXPECT_FALSE(v1->HitTestRect(ConvertRectToView(v1, r2)));
  EXPECT_TRUE(v2->HitTestRect(ConvertRectToView(v2, r2)));

  EXPECT_TRUE(v1->HitTestRect(ConvertRectToView(v1, r3)));
  EXPECT_TRUE(v2->HitTestRect(ConvertRectToView(v2, r3)));

  EXPECT_FALSE(v1->HitTestRect(ConvertRectToView(v1, r4)));
  EXPECT_FALSE(v2->HitTestRect(ConvertRectToView(v2, r4)));

  // Test GetEventHandlerForPoint
  EXPECT_EQ(v1, root_view->GetEventHandlerForPoint(v1_centerpoint));
  EXPECT_EQ(v2, root_view->GetEventHandlerForPoint(v2_centerpoint));

  EXPECT_EQ(v1, root_view->GetEventHandlerForPoint(v1_origin));
  EXPECT_EQ(root_view, root_view->GetEventHandlerForPoint(v2_origin));

  // Test GetTooltipHandlerForPoint
  EXPECT_EQ(v1, root_view->GetTooltipHandlerForPoint(v1_centerpoint));
  EXPECT_EQ(v2, root_view->GetTooltipHandlerForPoint(v2_centerpoint));

  EXPECT_EQ(v1, root_view->GetTooltipHandlerForPoint(v1_origin));
  EXPECT_EQ(root_view, root_view->GetTooltipHandlerForPoint(v2_origin));

  EXPECT_FALSE(v1->GetTooltipHandlerForPoint(v2_origin));

  widget->CloseNow();
}

TEST_F(ViewTest, NotifyEnterExitOnChild) {
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  // Have this hierarchy of views (the coords here are in root coord):
  // v1 (0, 0, 100, 100)
  //  - v11 (0, 0, 20, 30)
  //    - v111 (5, 5, 5, 15)
  //  - v12 (50, 10, 30, 90)
  //    - v121 (60, 20, 10, 10)
  // v2 (105, 0, 100, 100)
  //  - v21 (120, 10, 50, 20)

  TestView* v1 = new TestView;
  v1->SetBounds(0, 0, 100, 100);
  root_view->AddChildView(v1);
  v1->set_notify_enter_exit_on_child(true);

  TestView* v11 = new TestView;
  v11->SetBounds(0, 0, 20, 30);
  v1->AddChildView(v11);

  TestView* v111 = new TestView;
  v111->SetBounds(5, 5, 5, 15);
  v11->AddChildView(v111);

  TestView* v12 = new TestView;
  v12->SetBounds(50, 10, 30, 90);
  v1->AddChildView(v12);

  TestView* v121 = new TestView;
  v121->SetBounds(10, 10, 10, 10);
  v12->AddChildView(v121);

  TestView* v2 = new TestView;
  v2->SetBounds(105, 0, 100, 100);
  root_view->AddChildView(v2);

  TestView* v21 = new TestView;
  v21->SetBounds(15, 10, 50, 20);
  v2->AddChildView(v21);

  v1->Reset();
  v11->Reset();
  v111->Reset();
  v12->Reset();
  v121->Reset();
  v2->Reset();
  v21->Reset();

  // Move the mouse in v111.
  gfx::Point p1(6, 6);
  ui::MouseEvent move1(ui::ET_MOUSE_MOVED, p1, p1, 0);
  root_view->OnMouseMoved(move1);
  EXPECT_TRUE(v111->received_mouse_enter_);
  EXPECT_FALSE(v11->last_mouse_event_type_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v111->Reset();
  v1->Reset();

  // Now, move into v121.
  gfx::Point p2(65, 21);
  ui::MouseEvent move2(ui::ET_MOUSE_MOVED, p2, p2, 0);
  root_view->OnMouseMoved(move2);
  EXPECT_TRUE(v111->received_mouse_exit_);
  EXPECT_TRUE(v121->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v111->Reset();
  v121->Reset();

  // Now, move into v11.
  gfx::Point p3(1, 1);
  ui::MouseEvent move3(ui::ET_MOUSE_MOVED, p3, p3, 0);
  root_view->OnMouseMoved(move3);
  EXPECT_TRUE(v121->received_mouse_exit_);
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v121->Reset();
  v11->Reset();

  // Move to v21.
  gfx::Point p4(121, 15);
  ui::MouseEvent move4(ui::ET_MOUSE_MOVED, p4, p4, 0);
  root_view->OnMouseMoved(move4);
  EXPECT_TRUE(v21->received_mouse_enter_);
  EXPECT_FALSE(v2->last_mouse_event_type_);
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_exit_);

  v21->Reset();
  v11->Reset();
  v1->Reset();

  // Move to v1.
  gfx::Point p5(21, 0);
  ui::MouseEvent move5(ui::ET_MOUSE_MOVED, p5, p5, 0);
  root_view->OnMouseMoved(move5);
  EXPECT_TRUE(v21->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v21->Reset();
  v1->Reset();

  // Now, move into v11.
  gfx::Point p6(15, 15);
  ui::MouseEvent mouse6(ui::ET_MOUSE_MOVED, p6, p6, 0);
  root_view->OnMouseMoved(mouse6);
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v11->Reset();
  v1->Reset();

  // Move back into v1. Although |v1| had already received an ENTER for mouse6,
  // and the mouse remains inside |v1| the whole time, it receives another ENTER
  // when the mouse leaves v11.
  gfx::Point p7(21, 0);
  ui::MouseEvent mouse7(ui::ET_MOUSE_MOVED, p7, p7, 0);
  root_view->OnMouseMoved(mouse7);
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_FALSE(v1->received_mouse_enter_);

  widget->CloseNow();
}

TEST_F(ViewTest, Textfield) {
  const string16 kText = ASCIIToUTF16("Reality is that which, when you stop "
                                      "believing it, doesn't go away.");
  const string16 kExtraText = ASCIIToUTF16("Pretty deep, Philip!");
  const string16 kEmptyString;

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root_view = widget->GetRootView();

  Textfield* textfield = new Textfield();
  root_view->AddChildView(textfield);

  // Test setting, appending text.
  textfield->SetText(kText);
  EXPECT_EQ(kText, textfield->text());
  textfield->AppendText(kExtraText);
  EXPECT_EQ(kText + kExtraText, textfield->text());
  textfield->SetText(string16());
  EXPECT_EQ(kEmptyString, textfield->text());

  // Test selection related methods.
  textfield->SetText(kText);
  EXPECT_EQ(kEmptyString, textfield->GetSelectedText());
  textfield->SelectAll(false);
  EXPECT_EQ(kText, textfield->text());
  textfield->ClearSelection();
  EXPECT_EQ(kEmptyString, textfield->GetSelectedText());

  widget->CloseNow();
}

// Tests that the Textfield view respond appropiately to cut/copy/paste.
TEST_F(ViewTest, TextfieldCutCopyPaste) {
  const string16 kNormalText = ASCIIToUTF16("Normal");
  const string16 kReadOnlyText = ASCIIToUTF16("Read only");
  const string16 kPasswordText = ASCIIToUTF16("Password! ** Secret stuff **");

  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root_view = widget->GetRootView();

  Textfield* normal = new Textfield();
  Textfield* read_only = new Textfield();
  read_only->SetReadOnly(true);
  Textfield* password = new Textfield(Textfield::STYLE_OBSCURED);

  root_view->AddChildView(normal);
  root_view->AddChildView(read_only);
  root_view->AddChildView(password);

  normal->SetText(kNormalText);
  read_only->SetText(kReadOnlyText);
  password->SetText(kPasswordText);

  //
  // Test cut.
  //

  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_CUT);
  string16 result;
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  EXPECT_EQ(kNormalText, result);
  normal->SetText(kNormalText);  // Let's revert to the original content.

  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_CUT);
  result.clear();
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  // Cut should have failed, so the clipboard content should not have changed.
  EXPECT_EQ(kNormalText, result);

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_CUT);
  result.clear();
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  // Cut should have failed, so the clipboard content should not have changed.
  EXPECT_EQ(kNormalText, result);

  //
  // Test copy.
  //

  // Start with |read_only| to observe a change in clipboard text.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_COPY);
  result.clear();
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  EXPECT_EQ(kReadOnlyText, result);

  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_COPY);
  result.clear();
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  EXPECT_EQ(kNormalText, result);

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_COPY);
  result.clear();
  clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  // Text cannot be copied from an obscured field; the clipboard won't change.
  EXPECT_EQ(kNormalText, result);

  //
  // Test paste.
  //

  // Attempting to paste kNormalText in a read-only text-field should fail.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_PASTE);
  EXPECT_EQ(kReadOnlyText, read_only->text());

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_PASTE);
  EXPECT_EQ(kNormalText, password->text());

  // Copy from |read_only| to observe a change in the normal textfield text.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_COPY);
  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_PASTE);
  EXPECT_EQ(kReadOnlyText, normal->text());
  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// Accelerators
////////////////////////////////////////////////////////////////////////////////
bool TestView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  accelerator_count_map_[accelerator]++;
  return true;
}

#if defined(OS_WIN) && !defined(USE_AURA)
TEST_F(ViewTest, ActivateAccelerator) {
  // Register a keyboard accelerator before the view is added to a window.
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestView* view = new TestView();
  view->Reset();
  view->AddAccelerator(return_accelerator);
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 0);

  // Create a window and add the view as its child.
  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root = widget->GetRootView();
  root->AddChildView(view);
  widget->Show();

  // Get the focus manager.
  FocusManager* focus_manager = widget->GetFocusManager();
  ASSERT_TRUE(focus_manager);

  // Hit the return key and see if it takes effect.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);

  // Hit the escape key. Nothing should happen.
  ui::Accelerator escape_accelerator(ui::VKEY_ESCAPE, ui::EF_NONE);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 0);

  // Now register the escape key and hit it again.
  view->AddAccelerator(escape_accelerator);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);

  // Remove the return key accelerator.
  view->RemoveAccelerator(return_accelerator);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);

  // Add it again. Hit the return key and the escape key.
  view->AddAccelerator(return_accelerator);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);

  // Remove all the accelerators.
  view->ResetAccelerators();
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);

  widget->CloseNow();
}
#endif

#if defined(OS_WIN) && !defined(USE_AURA)
TEST_F(ViewTest, HiddenViewWithAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestView* view = new TestView();
  view->Reset();
  view->AddAccelerator(return_accelerator);
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 0);

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root = widget->GetRootView();
  root->AddChildView(view);
  widget->Show();

  FocusManager* focus_manager = widget->GetFocusManager();
  ASSERT_TRUE(focus_manager);

  view->SetVisible(false);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));

  view->SetVisible(true);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));

  widget->CloseNow();
}
#endif

#if defined(OS_WIN) && !defined(USE_AURA)
TEST_F(ViewTest, ViewInHiddenWidgetWithAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestView* view = new TestView();
  view->Reset();
  view->AddAccelerator(return_accelerator);
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 0);

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root = widget->GetRootView();
  root->AddChildView(view);

  FocusManager* focus_manager = widget->GetFocusManager();
  ASSERT_TRUE(focus_manager);

  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(0, view->accelerator_count_map_[return_accelerator]);

  widget->Show();
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);

  widget->Hide();
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);

  widget->CloseNow();
}
#endif

#if defined(OS_WIN) && !defined(USE_AURA)
////////////////////////////////////////////////////////////////////////////////
// Mouse-wheel message rerouting
////////////////////////////////////////////////////////////////////////////////
class ScrollableTestView : public View {
 public:
  ScrollableTestView() { }

  virtual gfx::Size GetPreferredSize() {
    return gfx::Size(100, 10000);
  }

  virtual void Layout() {
    SizeToPreferredSize();
  }
};

class TestViewWithControls : public View {
 public:
  TestViewWithControls() {
    text_field_ = new Textfield();
    AddChildView(text_field_);
  }

  Textfield* text_field_;
};

class SimpleWidgetDelegate : public WidgetDelegate {
 public:
  explicit SimpleWidgetDelegate(View* contents) : contents_(contents) {  }

  virtual void DeleteDelegate() { delete this; }

  virtual View* GetContentsView() { return contents_; }

  virtual Widget* GetWidget() { return contents_->GetWidget(); }
  virtual const Widget* GetWidget() const { return contents_->GetWidget(); }

 private:
  View* contents_;
};

// Tests that the mouse-wheel messages are correctly rerouted to the window
// under the mouse.
// TODO(jcampan): http://crbug.com/10572 Disabled as it fails on the Vista build
//                bot.
// Note that this fails for a variety of reasons:
// - focused view is apparently reset across window activations and never
//   properly restored
// - this test depends on you not having any other window visible open under the
//   area that it opens the test windows. --beng
TEST_F(ViewTest, DISABLED_RerouteMouseWheelTest) {
  TestViewWithControls* view_with_controls = new TestViewWithControls();
  Widget* window1 = Widget::CreateWindowWithBounds(
      new SimpleWidgetDelegate(view_with_controls),
      gfx::Rect(0, 0, 100, 100));
  window1->Show();
  ScrollView* scroll_view = new ScrollView();
  scroll_view->SetContents(new ScrollableTestView());
  Widget* window2 = Widget::CreateWindowWithBounds(
      new SimpleWidgetDelegate(scroll_view),
      gfx::Rect(200, 200, 100, 100));
  window2->Show();
  EXPECT_EQ(0, scroll_view->GetVisibleRect().y());

  // Make the window1 active, as this is what it would be in real-world.
  window1->Activate();

  // Let's send a mouse-wheel message to the different controls and check that
  // it is rerouted to the window under the mouse (effectively scrolling the
  // scroll-view).

  // First to the Window's HWND.
  ::SendMessage(view_with_controls->GetWidget()->GetNativeView(),
                WM_MOUSEWHEEL, MAKEWPARAM(0, -20), MAKELPARAM(250, 250));
  EXPECT_EQ(20, scroll_view->GetVisibleRect().y());

  // Then the text-field.
  ::SendMessage(view_with_controls->text_field_->GetTestingHandle(),
                WM_MOUSEWHEEL, MAKEWPARAM(0, -20), MAKELPARAM(250, 250));
  EXPECT_EQ(80, scroll_view->GetVisibleRect().y());

  // Ensure we don't scroll when the mouse is not over that window.
  ::SendMessage(view_with_controls->text_field_->GetTestingHandle(),
                WM_MOUSEWHEEL, MAKEWPARAM(0, -20), MAKELPARAM(50, 50));
  EXPECT_EQ(80, scroll_view->GetVisibleRect().y());

  window1->CloseNow();
  window2->CloseNow();
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Dialogs' default button
////////////////////////////////////////////////////////////////////////////////

class MockMenuModel : public ui::MenuModel {
 public:
  MOCK_CONST_METHOD0(HasIcons, bool());
  MOCK_CONST_METHOD0(GetItemCount, int());
  MOCK_CONST_METHOD1(GetTypeAt, ItemType(int index));
  MOCK_CONST_METHOD1(GetSeparatorTypeAt, ui::MenuSeparatorType(int index));
  MOCK_CONST_METHOD1(GetCommandIdAt, int(int index));
  MOCK_CONST_METHOD1(GetLabelAt, string16(int index));
  MOCK_CONST_METHOD1(IsItemDynamicAt, bool(int index));
  MOCK_CONST_METHOD1(GetLabelFontAt, const gfx::Font* (int index));
  MOCK_CONST_METHOD2(GetAcceleratorAt, bool(int index,
      ui::Accelerator* accelerator));
  MOCK_CONST_METHOD1(IsItemCheckedAt, bool(int index));
  MOCK_CONST_METHOD1(GetGroupIdAt, int(int index));
  MOCK_METHOD2(GetIconAt, bool(int index, gfx::Image* icon));
  MOCK_CONST_METHOD1(GetButtonMenuItemAt, ui::ButtonMenuItemModel*(int index));
  MOCK_CONST_METHOD1(IsEnabledAt, bool(int index));
  MOCK_CONST_METHOD1(IsVisibleAt, bool(int index));
  MOCK_CONST_METHOD1(GetSubmenuModelAt, MenuModel*(int index));
  MOCK_METHOD1(HighlightChangedTo, void(int index));
  MOCK_METHOD1(ActivatedAt, void(int index));
  MOCK_METHOD2(ActivatedAt, void(int index, int disposition));
  MOCK_METHOD0(MenuWillShow, void());
  MOCK_METHOD0(MenuClosed, void());
  MOCK_METHOD1(SetMenuModelDelegate, void(ui::MenuModelDelegate* delegate));
  MOCK_CONST_METHOD0(GetMenuModelDelegate, ui::MenuModelDelegate*());
  MOCK_METHOD3(GetModelAndIndexForCommandId, bool(int command_id,
      MenuModel** model, int* index));
};

class TestDialog : public DialogDelegate, public ButtonListener {
 public:
  explicit TestDialog(MockMenuModel* mock_menu_model)
      : contents_(NULL),
        button1_(NULL),
        button2_(NULL),
        checkbox_(NULL),
        button_drop_(NULL),
        last_pressed_button_(NULL),
        mock_menu_model_(mock_menu_model),
        canceled_(false),
        oked_(false),
        closeable_(false),
        widget_(NULL) {
  }

  void TearDown() {
    // Now we can close safely.
    closeable_ = true;
    widget_->Close();
    widget_ = NULL;
    // delegate has to be alive while shutting down.
    base::MessageLoop::current()->DeleteSoon(FROM_HERE, this);
  }

  // DialogDelegate implementation:
  virtual int GetDefaultDialogButton() const OVERRIDE {
    return ui::DIALOG_BUTTON_OK;
  }

  virtual View* GetContentsView() OVERRIDE {
    if (!contents_) {
      contents_ = new View;
      button1_ = new LabelButton(this, ASCIIToUTF16("Button1"));
      button2_ = new LabelButton(this, ASCIIToUTF16("Button2"));
      checkbox_ = new Checkbox(ASCIIToUTF16("My checkbox"));
      button_drop_ = new ButtonDropDown(this, mock_menu_model_);
      contents_->AddChildView(button1_);
      contents_->AddChildView(button2_);
      contents_->AddChildView(checkbox_);
      contents_->AddChildView(button_drop_);
    }
    return contents_;
  }

  // Prevent the dialog from really closing (so we can click the OK/Cancel
  // buttons to our heart's content).
  virtual bool Cancel() OVERRIDE {
    canceled_ = true;
    return closeable_;
  }
  virtual bool Accept() OVERRIDE {
    oked_ = true;
    return closeable_;
  }

  virtual Widget* GetWidget() OVERRIDE {
    return widget_;
  }
  virtual const Widget* GetWidget() const OVERRIDE {
    return widget_;
  }

  // ButtonListener implementation.
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE {
    last_pressed_button_ = sender;
  }

  void ResetStates() {
    oked_ = false;
    canceled_ = false;
    last_pressed_button_ = NULL;
  }

  // Set up expectations for methods that are called when an (empty) menu is
  // shown from a drop down button.
  void ExpectShowDropMenu() {
    if (mock_menu_model_) {
      EXPECT_CALL(*mock_menu_model_, HasIcons());
      EXPECT_CALL(*mock_menu_model_, GetItemCount());
      EXPECT_CALL(*mock_menu_model_, MenuClosed());
    }
  }

  View* contents_;
  LabelButton* button1_;
  LabelButton* button2_;
  Checkbox* checkbox_;
  ButtonDropDown* button_drop_;
  Button* last_pressed_button_;
  MockMenuModel* mock_menu_model_;

  bool canceled_;
  bool oked_;
  bool closeable_;
  Widget* widget_;
};

class DefaultButtonTest : public ViewTest {
 public:
  enum ButtonID {
    OK,
    CANCEL,
    BUTTON1,
    BUTTON2
  };

  DefaultButtonTest()
      : focus_manager_(NULL),
        test_dialog_(NULL),
        client_view_(NULL),
        ok_button_(NULL),
        cancel_button_(NULL) {
  }

  virtual void SetUp() OVERRIDE {
    ViewTest::SetUp();
    test_dialog_ = new TestDialog(NULL);

    Widget* window = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
    params.delegate = test_dialog_;
    params.bounds = gfx::Rect(0, 0, 100, 100);
    window->Init(params);

    test_dialog_->widget_ = window;
    window->Show();
    focus_manager_ = test_dialog_->contents_->GetFocusManager();
    ASSERT_TRUE(focus_manager_ != NULL);
    client_view_ =
        static_cast<DialogClientView*>(window->client_view());
    ok_button_ = client_view_->ok_button();
    cancel_button_ = client_view_->cancel_button();
  }

  virtual void TearDown() OVERRIDE {
    test_dialog_->TearDown();
    ViewTest::TearDown();
  }

  void SimulatePressingEnterAndCheckDefaultButton(ButtonID button_id) {
    ui::KeyEvent event(ui::ET_KEY_PRESSED, ui::VKEY_RETURN, 0, false);
    focus_manager_->OnKeyEvent(event);
    switch (button_id) {
      case OK:
        EXPECT_TRUE(test_dialog_->oked_);
        EXPECT_FALSE(test_dialog_->canceled_);
        EXPECT_FALSE(test_dialog_->last_pressed_button_);
        break;
      case CANCEL:
        EXPECT_FALSE(test_dialog_->oked_);
        EXPECT_TRUE(test_dialog_->canceled_);
        EXPECT_FALSE(test_dialog_->last_pressed_button_);
        break;
      case BUTTON1:
        EXPECT_FALSE(test_dialog_->oked_);
        EXPECT_FALSE(test_dialog_->canceled_);
        EXPECT_TRUE(test_dialog_->last_pressed_button_ ==
            test_dialog_->button1_);
        break;
      case BUTTON2:
        EXPECT_FALSE(test_dialog_->oked_);
        EXPECT_FALSE(test_dialog_->canceled_);
        EXPECT_TRUE(test_dialog_->last_pressed_button_ ==
            test_dialog_->button2_);
        break;
    }
    test_dialog_->ResetStates();
  }

  FocusManager* focus_manager_;
  TestDialog* test_dialog_;
  DialogClientView* client_view_;
  LabelButton* ok_button_;
  LabelButton* cancel_button_;
};

TEST_F(DefaultButtonTest, DialogDefaultButtonTest) {
  // Window has just been shown, we expect the default button specified in the
  // DialogDelegate.
  EXPECT_TRUE(ok_button_->is_default());

  // Simulate pressing enter, that should trigger the OK button.
  SimulatePressingEnterAndCheckDefaultButton(OK);

  // Simulate focusing another button, it should become the default button.
  client_view_->OnWillChangeFocus(ok_button_, test_dialog_->button1_);
  EXPECT_FALSE(ok_button_->is_default());
  EXPECT_TRUE(test_dialog_->button1_->is_default());
  // Simulate pressing enter, that should trigger button1.
  SimulatePressingEnterAndCheckDefaultButton(BUTTON1);

  // Now select something that is not a button, the OK should become the default
  // button again.
  client_view_->OnWillChangeFocus(test_dialog_->button1_,
                                  test_dialog_->checkbox_);
  EXPECT_TRUE(ok_button_->is_default());
  EXPECT_FALSE(test_dialog_->button1_->is_default());
  SimulatePressingEnterAndCheckDefaultButton(OK);

  // Select yet another button.
  client_view_->OnWillChangeFocus(test_dialog_->checkbox_,
                                  test_dialog_->button2_);
  EXPECT_FALSE(ok_button_->is_default());
  EXPECT_FALSE(test_dialog_->button1_->is_default());
  EXPECT_TRUE(test_dialog_->button2_->is_default());
  SimulatePressingEnterAndCheckDefaultButton(BUTTON2);

  // Focus nothing.
  client_view_->OnWillChangeFocus(test_dialog_->button2_, NULL);
  EXPECT_TRUE(ok_button_->is_default());
  EXPECT_FALSE(test_dialog_->button1_->is_default());
  EXPECT_FALSE(test_dialog_->button2_->is_default());
  SimulatePressingEnterAndCheckDefaultButton(OK);

  // Focus the cancel button.
  client_view_->OnWillChangeFocus(NULL, cancel_button_);
  EXPECT_FALSE(ok_button_->is_default());
  EXPECT_TRUE(cancel_button_->is_default());
  EXPECT_FALSE(test_dialog_->button1_->is_default());
  EXPECT_FALSE(test_dialog_->button2_->is_default());
  SimulatePressingEnterAndCheckDefaultButton(CANCEL);
}

class ButtonDropDownTest : public ViewTest {
 public:
  ButtonDropDownTest()
      : test_dialog_(NULL),
        button_as_view_(NULL) {
  }

  virtual void SetUp() OVERRIDE {
    ViewTest::SetUp();
    test_dialog_ = new TestDialog(new MockMenuModel());

    Widget* window = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
    params.delegate = test_dialog_;
    params.bounds = gfx::Rect(0, 0, 100, 100);
    window->Init(params);

    test_dialog_->widget_ = window;
    window->Show();
    test_dialog_->button_drop_->SetBoundsRect(gfx::Rect(0, 0, 100, 100));
    // We have to cast the button back into a View in order to invoke it's
    // OnMouseReleased method.
    button_as_view_ = static_cast<View*>(test_dialog_->button_drop_);
  }

  virtual void TearDown() OVERRIDE {
    test_dialog_->TearDown();
    ViewTest::TearDown();
  }

  TestDialog* test_dialog_;
  // This is owned by test_dialog_.
  View* button_as_view_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ButtonDropDownTest);
};

// Ensure that regular clicks on the drop down button still work. (i.e. - the
// click events are processed and the listener gets the click)
TEST_F(ButtonDropDownTest, RegularClickTest) {
  gfx::Point point(1, 1);
  ui::MouseEvent press_event(ui::ET_MOUSE_PRESSED, point, point,
                             ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent release_event(ui::ET_MOUSE_RELEASED, point, point,
                               ui::EF_LEFT_MOUSE_BUTTON);
  button_as_view_->OnMousePressed(press_event);
  button_as_view_->OnMouseReleased(release_event);
  EXPECT_EQ(test_dialog_->last_pressed_button_, test_dialog_->button_drop_);
}

////////////////////////////////////////////////////////////////////////////////
// Native view hierachy
////////////////////////////////////////////////////////////////////////////////
class TestNativeViewHierarchy : public View {
 public:
  TestNativeViewHierarchy() {
  }

  virtual void NativeViewHierarchyChanged(
      bool attached,
      gfx::NativeView native_view,
      internal::RootView* root_view) OVERRIDE {
    NotificationInfo info;
    info.attached = attached;
    info.native_view = native_view;
    info.root_view = root_view;
    notifications_.push_back(info);
  };
  struct NotificationInfo {
    bool attached;
    gfx::NativeView native_view;
    internal::RootView* root_view;
  };
  static const size_t kTotalViews = 2;
  std::vector<NotificationInfo> notifications_;
};

class TestChangeNativeViewHierarchy {
 public:
  explicit TestChangeNativeViewHierarchy(ViewTest *view_test) {
    view_test_ = view_test;
    native_host_ = new NativeViewHost();
    host_ = new Widget;
    Widget::InitParams params =
        view_test->CreateParams(Widget::InitParams::TYPE_POPUP);
    params.bounds = gfx::Rect(0, 0, 500, 300);
    host_->Init(params);
    host_->GetRootView()->AddChildView(native_host_);
    for (size_t i = 0; i < TestNativeViewHierarchy::kTotalViews; ++i) {
      windows_[i] = new Widget;
      Widget::InitParams params(Widget::InitParams::TYPE_CONTROL);
      params.parent = host_->GetNativeView();
      params.bounds = gfx::Rect(0, 0, 500, 300);
      windows_[i]->Init(params);
      root_views_[i] = windows_[i]->GetRootView();
      test_views_[i] = new TestNativeViewHierarchy;
      root_views_[i]->AddChildView(test_views_[i]);
    }
  }

  ~TestChangeNativeViewHierarchy() {
    for (size_t i = 0; i < TestNativeViewHierarchy::kTotalViews; ++i) {
      windows_[i]->Close();
    }
    host_->Close();
    // Will close and self-delete widgets - no need to manually delete them.
    view_test_->RunPendingMessages();
  }

  void CheckEnumeratingNativeWidgets() {
    if (!host_->GetTopLevelWidget())
      return;
    Widget::Widgets widgets;
    Widget::GetAllChildWidgets(host_->GetNativeView(), &widgets);
    EXPECT_EQ(TestNativeViewHierarchy::kTotalViews + 1, widgets.size());
    // Unfortunately there is no guarantee the sequence of views here so always
    // go through all of them.
    for (Widget::Widgets::iterator i = widgets.begin();
         i != widgets.end(); ++i) {
      View* root_view = (*i)->GetRootView();
      if (host_->GetRootView() == root_view)
        continue;
      size_t j;
      for (j = 0; j < TestNativeViewHierarchy::kTotalViews; ++j)
        if (root_views_[j] == root_view)
          break;
      // EXPECT_LT/GT/GE() fails to compile with class-defined constants
      // with gcc, with error
      // "error: undefined reference to 'TestNativeViewHierarchy::kTotalViews'"
      // so I forced to use EXPECT_TRUE() instead.
      EXPECT_TRUE(TestNativeViewHierarchy::kTotalViews > j);
    }
  }

  void CheckChangingHierarhy() {
    size_t i;
    for (i = 0; i < TestNativeViewHierarchy::kTotalViews; ++i) {
      // TODO(georgey): use actual hierarchy changes to send notifications.
      static_cast<internal::RootView*>(root_views_[i])->
          NotifyNativeViewHierarchyChanged(false, host_->GetNativeView());
      static_cast<internal::RootView*>(root_views_[i])->
          NotifyNativeViewHierarchyChanged(true, host_->GetNativeView());
    }
    for (i = 0; i < TestNativeViewHierarchy::kTotalViews; ++i) {
      ASSERT_EQ(static_cast<size_t>(2), test_views_[i]->notifications_.size());
      EXPECT_FALSE(test_views_[i]->notifications_[0].attached);
      EXPECT_EQ(host_->GetNativeView(),
          test_views_[i]->notifications_[0].native_view);
      EXPECT_EQ(root_views_[i], test_views_[i]->notifications_[0].root_view);
      EXPECT_TRUE(test_views_[i]->notifications_[1].attached);
      EXPECT_EQ(host_->GetNativeView(),
          test_views_[i]->notifications_[1].native_view);
      EXPECT_EQ(root_views_[i], test_views_[i]->notifications_[1].root_view);
    }
  }

  NativeViewHost* native_host_;
  Widget* host_;
  Widget* windows_[TestNativeViewHierarchy::kTotalViews];
  View* root_views_[TestNativeViewHierarchy::kTotalViews];
  TestNativeViewHierarchy* test_views_[TestNativeViewHierarchy::kTotalViews];
  ViewTest* view_test_;
};

TEST_F(ViewTest, ChangeNativeViewHierarchyFindRoots) {
  // TODO(georgey): Fix the test for Linux
#if defined(OS_WIN)
  TestChangeNativeViewHierarchy test(this);
  test.CheckEnumeratingNativeWidgets();
#endif
}

TEST_F(ViewTest, ChangeNativeViewHierarchyChangeHierarchy) {
  // TODO(georgey): Fix the test for Linux
#if defined(OS_WIN)
  TestChangeNativeViewHierarchy test(this);
  test.CheckChangingHierarhy();
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Transformations
////////////////////////////////////////////////////////////////////////////////

class TransformPaintView : public TestView {
 public:
  TransformPaintView() {}
  virtual ~TransformPaintView() {}

  void ClearScheduledPaintRect() {
    scheduled_paint_rect_ = gfx::Rect();
  }

  gfx::Rect scheduled_paint_rect() const { return scheduled_paint_rect_; }

  // Overridden from View:
  virtual void SchedulePaintInRect(const gfx::Rect& rect) OVERRIDE {
    gfx::Rect xrect = ConvertRectToParent(rect);
    scheduled_paint_rect_.Union(xrect);
  }

 private:
  gfx::Rect scheduled_paint_rect_;

  DISALLOW_COPY_AND_ASSIGN(TransformPaintView);
};

TEST_F(ViewTest, TransformPaint) {
  TransformPaintView* v1 = new TransformPaintView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 500, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 200, 100));

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  widget->Show();
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  // At this moment, |v2| occupies (100, 100) to (300, 200) in |root|.
  v1->ClearScheduledPaintRect();
  v2->SchedulePaint();

  EXPECT_EQ(gfx::Rect(100, 100, 200, 100), v1->scheduled_paint_rect());

  // Rotate |v1| counter-clockwise.
  gfx::Transform transform;
  RotateCounterclockwise(&transform);
  transform.matrix().set(1, 3, 500.0);
  v1->SetTransform(transform);

  // |v2| now occupies (100, 200) to (200, 400) in |root|.

  v1->ClearScheduledPaintRect();
  v2->SchedulePaint();

  EXPECT_EQ(gfx::Rect(100, 200, 100, 200), v1->scheduled_paint_rect());

  widget->CloseNow();
}

TEST_F(ViewTest, TransformEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 500, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 200, 100));

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  // At this moment, |v2| occupies (100, 100) to (300, 200) in |root|.

  // Rotate |v1| counter-clockwise.
  gfx::Transform transform(v1->GetTransform());
  RotateCounterclockwise(&transform);
  transform.matrix().set(1, 3, 500.0);
  v1->SetTransform(transform);

  // |v2| now occupies (100, 200) to (200, 400) in |root|.
  v1->Reset();
  v2->Reset();

  gfx::Point p1(110, 210);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1,
                         ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_EQ(0, v1->last_mouse_event_type_);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v2->last_mouse_event_type_);
  EXPECT_EQ(190, v2->location_.x());
  EXPECT_EQ(10, v2->location_.y());

  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(), 0);
  root->OnMouseReleased(released);

  // Now rotate |v2| inside |v1| clockwise.
  transform = v2->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().setDouble(0, 3, 100.0);
  v2->SetTransform(transform);

  // Now, |v2| occupies (100, 100) to (200, 300) in |v1|, and (100, 300) to
  // (300, 400) in |root|.

  v1->Reset();
  v2->Reset();

  gfx::Point point2(110, 320);
  ui::MouseEvent p2(ui::ET_MOUSE_PRESSED, point2, point2,
                    ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p2);
  EXPECT_EQ(0, v1->last_mouse_event_type_);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v2->last_mouse_event_type_);
  EXPECT_EQ(10, v2->location_.x());
  EXPECT_EQ(20, v2->location_.y());

  root->OnMouseReleased(released);

  v1->SetTransform(gfx::Transform());
  v2->SetTransform(gfx::Transform());

  TestView* v3 = new TestView();
  v3->SetBoundsRect(gfx::Rect(10, 10, 20, 30));
  v2->AddChildView(v3);

  // Rotate |v3| clockwise with respect to |v2|.
  transform = v1->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().setDouble(0, 3, 30.0);
  v3->SetTransform(transform);

  // Scale |v2| with respect to |v1| along both axis.
  transform = v2->GetTransform();
  transform.matrix().setDouble(0, 0, 0.8);
  transform.matrix().setDouble(1, 1, 0.5);
  v2->SetTransform(transform);

  // |v3| occupies (108, 105) to (132, 115) in |root|.

  v1->Reset();
  v2->Reset();
  v3->Reset();

  gfx::Point point(112, 110);
  ui::MouseEvent p3(ui::ET_MOUSE_PRESSED, point, point,
                    ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p3);

  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v3->last_mouse_event_type_);
  EXPECT_EQ(10, v3->location_.x());
  EXPECT_EQ(25, v3->location_.y());

  root->OnMouseReleased(released);

  v1->SetTransform(gfx::Transform());
  v2->SetTransform(gfx::Transform());
  v3->SetTransform(gfx::Transform());

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Rotate |v3| clockwise with respect to |v2|, and scale it along both axis.
  transform = v3->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().setDouble(0, 3, 30.0);
  // Rotation sets some scaling transformation. Using SetScale would overwrite
  // that and pollute the rotation. So combine the scaling with the existing
  // transforamtion.
  gfx::Transform scale;
  scale.Scale(0.8, 0.5);
  transform.ConcatTransform(scale);
  v3->SetTransform(transform);

  // Translate |v2| with respect to |v1|.
  transform = v2->GetTransform();
  transform.matrix().setDouble(0, 3, 10.0);
  transform.matrix().setDouble(1, 3, 10.0);
  v2->SetTransform(transform);

  // |v3| now occupies (120, 120) to (144, 130) in |root|.

  gfx::Point point3(124, 125);
  ui::MouseEvent p4(ui::ET_MOUSE_PRESSED, point3, point3,
                    ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p4);

  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v3->last_mouse_event_type_);
  EXPECT_EQ(10, v3->location_.x());
  EXPECT_EQ(25, v3->location_.y());

  root->OnMouseReleased(released);

  widget->CloseNow();
}

TEST_F(ViewTest, TransformVisibleBound) {
  gfx::Rect viewport_bounds(0, 0, 100, 100);

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = viewport_bounds;
  widget->Init(params);
  widget->GetRootView()->SetBoundsRect(viewport_bounds);

  View* viewport = new View;
  widget->SetContentsView(viewport);
  View* contents = new View;
  viewport->AddChildView(contents);
  viewport->SetBoundsRect(viewport_bounds);
  contents->SetBoundsRect(gfx::Rect(0, 0, 100, 200));

  View* child = new View;
  contents->AddChildView(child);
  child->SetBoundsRect(gfx::Rect(10, 90, 50, 50));
  EXPECT_EQ(gfx::Rect(0, 0, 50, 10), child->GetVisibleBounds());

  // Rotate |child| counter-clockwise
  gfx::Transform transform;
  RotateCounterclockwise(&transform);
  transform.matrix().setDouble(1, 3, 50.0);
  child->SetTransform(transform);
  EXPECT_EQ(gfx::Rect(40, 0, 10, 50), child->GetVisibleBounds());

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// OnVisibleBoundsChanged()

class VisibleBoundsView : public View {
 public:
  VisibleBoundsView() : received_notification_(false) {}
  virtual ~VisibleBoundsView() {}

  bool received_notification() const { return received_notification_; }
  void set_received_notification(bool received) {
    received_notification_ = received;
  }

 private:
  // Overridden from View:
  virtual bool NeedsNotificationWhenVisibleBoundsChange() const OVERRIDE {
     return true;
  }
  virtual void OnVisibleBoundsChanged() OVERRIDE {
    received_notification_ = true;
  }

  bool received_notification_;

  DISALLOW_COPY_AND_ASSIGN(VisibleBoundsView);
};

TEST_F(ViewTest, OnVisibleBoundsChanged) {
  gfx::Rect viewport_bounds(0, 0, 100, 100);

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = viewport_bounds;
  widget->Init(params);
  widget->GetRootView()->SetBoundsRect(viewport_bounds);

  View* viewport = new View;
  widget->SetContentsView(viewport);
  View* contents = new View;
  viewport->AddChildView(contents);
  viewport->SetBoundsRect(viewport_bounds);
  contents->SetBoundsRect(gfx::Rect(0, 0, 100, 200));

  // Create a view that cares about visible bounds notifications, and position
  // it just outside the visible bounds of the viewport.
  VisibleBoundsView* child = new VisibleBoundsView;
  contents->AddChildView(child);
  child->SetBoundsRect(gfx::Rect(10, 110, 50, 50));

  // The child bound should be fully clipped.
  EXPECT_TRUE(child->GetVisibleBounds().IsEmpty());

  // Now scroll the contents, but not enough to make the child visible.
  contents->SetY(contents->y() - 1);

  // We should have received the notification since the visible bounds may have
  // changed (even though they didn't).
  EXPECT_TRUE(child->received_notification());
  EXPECT_TRUE(child->GetVisibleBounds().IsEmpty());
  child->set_received_notification(false);

  // Now scroll the contents, this time by enough to make the child visible by
  // one pixel.
  contents->SetY(contents->y() - 10);
  EXPECT_TRUE(child->received_notification());
  EXPECT_EQ(1, child->GetVisibleBounds().height());
  child->set_received_notification(false);

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// BoundsChanged()

TEST_F(ViewTest, SetBoundsPaint) {
  TestView top_view;
  TestView* child_view = new TestView;

  top_view.SetBoundsRect(gfx::Rect(0, 0, 100, 100));
  top_view.scheduled_paint_rects_.clear();
  child_view->SetBoundsRect(gfx::Rect(10, 10, 20, 20));
  top_view.AddChildView(child_view);

  top_view.scheduled_paint_rects_.clear();
  child_view->SetBoundsRect(gfx::Rect(30, 30, 20, 20));
  EXPECT_EQ(2U, top_view.scheduled_paint_rects_.size());

  // There should be 2 rects, spanning from (10, 10) to (50, 50).
  gfx::Rect paint_rect = top_view.scheduled_paint_rects_[0];
  paint_rect.Union(top_view.scheduled_paint_rects_[1]);
  EXPECT_EQ(gfx::Rect(10, 10, 40, 40), paint_rect);
}

// Tests conversion methods with a transform.
TEST_F(ViewTest, ConvertPointToViewWithTransform) {
  TestView top_view;
  TestView* child = new TestView;
  TestView* child_child = new TestView;

  top_view.AddChildView(child);
  child->AddChildView(child_child);

  top_view.SetBoundsRect(gfx::Rect(0, 0, 1000, 1000));

  child->SetBoundsRect(gfx::Rect(7, 19, 500, 500));
  gfx::Transform transform;
  transform.Scale(3.0, 4.0);
  child->SetTransform(transform);

  child_child->SetBoundsRect(gfx::Rect(17, 13, 100, 100));
  transform.MakeIdentity();
  transform.Scale(5.0, 7.0);
  child_child->SetTransform(transform);

  // Sanity check to make sure basic transforms act as expected.
  {
    gfx::Transform transform;
    transform.Translate(110.0, -110.0);
    transform.Scale(100.0, 55.0);
    transform.Translate(1.0, 1.0);

    // convert to a 3x3 matrix.
    const SkMatrix& matrix = transform.matrix();

    EXPECT_EQ(210, matrix.getTranslateX());
    EXPECT_EQ(-55, matrix.getTranslateY());
    EXPECT_EQ(100, matrix.getScaleX());
    EXPECT_EQ(55, matrix.getScaleY());
    EXPECT_EQ(0, matrix.getSkewX());
    EXPECT_EQ(0, matrix.getSkewY());
  }

  {
    gfx::Transform transform;
    transform.Translate(1.0, 1.0);
    gfx::Transform t2;
    t2.Scale(100.0, 55.0);
    gfx::Transform t3;
    t3.Translate(110.0, -110.0);
    transform.ConcatTransform(t2);
    transform.ConcatTransform(t3);

    // convert to a 3x3 matrix
    const SkMatrix& matrix = transform.matrix();

    EXPECT_EQ(210, matrix.getTranslateX());
    EXPECT_EQ(-55, matrix.getTranslateY());
    EXPECT_EQ(100, matrix.getScaleX());
    EXPECT_EQ(55, matrix.getScaleY());
    EXPECT_EQ(0, matrix.getSkewX());
    EXPECT_EQ(0, matrix.getSkewY());
  }

  // Conversions from child->top and top->child.
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child, &top_view, &point);
    EXPECT_EQ(22, point.x());
    EXPECT_EQ(39, point.y());

    point.SetPoint(22, 39);
    View::ConvertPointToTarget(&top_view, child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());
  }

  // Conversions from child_child->top and top->child_child.
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child_child, &top_view, &point);
    EXPECT_EQ(133, point.x());
    EXPECT_EQ(211, point.y());

    point.SetPoint(133, 211);
    View::ConvertPointToTarget(&top_view, child_child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());
  }

  // Conversions from child_child->child and child->child_child
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child_child, child, &point);
    EXPECT_EQ(42, point.x());
    EXPECT_EQ(48, point.y());

    point.SetPoint(42, 48);
    View::ConvertPointToTarget(child, child_child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());
  }

  // Conversions from top_view to child with a value that should be negative.
  // This ensures we don't round up with negative numbers.
  {
    gfx::Point point(6, 18);
    View::ConvertPointToTarget(&top_view, child, &point);
    EXPECT_EQ(-1, point.x());
    EXPECT_EQ(-1, point.y());
  }
}

// Tests conversion methods for rectangles.
TEST_F(ViewTest, ConvertRectWithTransform) {
  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  TestView* v1 = new TestView;
  TestView* v2 = new TestView;
  root->AddChildView(v1);
  v1->AddChildView(v2);

  v1->SetBoundsRect(gfx::Rect(10, 10, 500, 500));
  v2->SetBoundsRect(gfx::Rect(20, 20, 100, 200));

  // |v2| now occupies (30, 30) to (130, 230) in |widget|
  gfx::Rect rect(5, 5, 15, 40);
  EXPECT_EQ(gfx::Rect(25, 25, 15, 40), v2->ConvertRectToParent(rect));
  EXPECT_EQ(gfx::Rect(35, 35, 15, 40), v2->ConvertRectToWidget(rect));

  // Rotate |v2|
  gfx::Transform t2;
  RotateCounterclockwise(&t2);
  t2.matrix().setDouble(1, 3, 100.0);
  v2->SetTransform(t2);

  // |v2| now occupies (30, 30) to (230, 130) in |widget|
  EXPECT_EQ(gfx::Rect(25, 100, 40, 15), v2->ConvertRectToParent(rect));
  EXPECT_EQ(gfx::Rect(35, 110, 40, 15), v2->ConvertRectToWidget(rect));

  // Scale down |v1|
  gfx::Transform t1;
  t1.Scale(0.5, 0.5);
  v1->SetTransform(t1);

  // The rectangle should remain the same for |v1|.
  EXPECT_EQ(gfx::Rect(25, 100, 40, 15), v2->ConvertRectToParent(rect));

  // |v2| now occupies (20, 20) to (120, 70) in |widget|
  EXPECT_EQ(gfx::Rect(22, 60, 21, 8).ToString(),
            v2->ConvertRectToWidget(rect).ToString());

  widget->CloseNow();
}

class ObserverView : public View {
 public:
  ObserverView();
  virtual ~ObserverView();

  void ResetTestState();

  bool child_added() const { return child_added_; }
  bool child_removed() const { return child_removed_; }
  const View* parent_view() const { return parent_view_; }
  const View* child_view() const { return child_view_; }

 private:
  // View:
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;

  bool child_added_;
  bool child_removed_;
  View* parent_view_;
  View* child_view_;

  DISALLOW_COPY_AND_ASSIGN(ObserverView);
};

ObserverView::ObserverView()
    : child_added_(false),
      child_removed_(false),
      parent_view_(NULL),
      child_view_(NULL) {
}

ObserverView::~ObserverView() {}

void ObserverView::ResetTestState() {
  child_added_ = false;
  child_removed_ = false;
  parent_view_ = NULL;
  child_view_ = NULL;
}

void ObserverView::ViewHierarchyChanged(bool is_add,
                                        View* parent,
                                        View* child) {
  if (is_add)
    child_added_ = true;
  else
    child_removed_ = true;

  parent_view_ = parent;
  child_view_ = child;
}

// Verifies that the ViewHierarchyChanged() notification is sent correctly when
// a child view is added or removed to all the views in the hierarchy (up and
// down).
// The tree looks like this:
// v1
// +-- v2
//     +-- v3
TEST_F(ViewTest, ViewHierarchyChanged) {
  ObserverView v1;

  ObserverView* v3 = new ObserverView();

  // Add |v3| to |v2|.
  scoped_ptr<ObserverView> v2(new ObserverView());
  v2->AddChildView(v3);

  // Make sure both |v2| and |v3| receive the ViewHierarchyChanged()
  // notification.
  EXPECT_TRUE(v2->child_added());
  EXPECT_FALSE(v2->child_removed());
  EXPECT_EQ(v2.get(), v2->parent_view());
  EXPECT_EQ(v3, v2->child_view());

  EXPECT_TRUE(v3->child_added());
  EXPECT_FALSE(v3->child_removed());
  EXPECT_EQ(v2.get(), v3->parent_view());
  EXPECT_EQ(v3, v3->child_view());

  // Reset everything to the initial state.
  v2->ResetTestState();
  v3->ResetTestState();

  // Add |v2| to v1.
  v1.AddChildView(v2.get());

  // Verifies that |v2| is the child view *added* and the parent view is |v1|.
  // Make sure all the views (v1, v2, v3) received _that_ information.
  EXPECT_TRUE(v1.child_added());
  EXPECT_FALSE(v1.child_removed());
  EXPECT_EQ(&v1, v1.parent_view());
  EXPECT_EQ(v2.get(), v1.child_view());

  EXPECT_TRUE(v2->child_added());
  EXPECT_FALSE(v2->child_removed());
  EXPECT_EQ(&v1, v2->parent_view());
  EXPECT_EQ(v2.get(), v2->child_view());

  EXPECT_TRUE(v3->child_added());
  EXPECT_FALSE(v3->child_removed());
  EXPECT_EQ(&v1, v3->parent_view());
  EXPECT_EQ(v2.get(), v3->child_view());

  // Reset everything to the initial state.
  v1.ResetTestState();
  v2->ResetTestState();
  v3->ResetTestState();

  // Remove |v2| from |v1|.
  v1.RemoveChildView(v2.get());

  // Verifies that |v2| is the child view *removed* and the parent view is |v1|.
  // Make sure all the views (v1, v2, v3) received _that_ information.
  EXPECT_FALSE(v1.child_added());
  EXPECT_TRUE(v1.child_removed());
  EXPECT_EQ(&v1, v1.parent_view());
  EXPECT_EQ(v2.get(), v1.child_view());

  EXPECT_FALSE(v2->child_added());
  EXPECT_TRUE(v2->child_removed());
  EXPECT_EQ(&v1, v2->parent_view());
  EXPECT_EQ(v2.get(), v2->child_view());

  EXPECT_FALSE(v3->child_added());
  EXPECT_TRUE(v3->child_removed());
  EXPECT_EQ(&v1, v3->parent_view());
  EXPECT_EQ(v3, v3->child_view());
}

// Verifies if the child views added under the root are all deleted when calling
// RemoveAllChildViews.
// The tree looks like this:
// root
// +-- child1
//     +-- foo
//         +-- bar0
//         +-- bar1
//         +-- bar2
// +-- child2
// +-- child3
TEST_F(ViewTest, RemoveAllChildViews) {
  View root;

  View* child1 = new View;
  root.AddChildView(child1);

  for (int i = 0; i < 2; ++i)
    root.AddChildView(new View);

  View* foo = new View;
  child1->AddChildView(foo);

  // Add some nodes to |foo|.
  for (int i = 0; i < 3; ++i)
    foo->AddChildView(new View);

  EXPECT_EQ(3, root.child_count());
  EXPECT_EQ(1, child1->child_count());
  EXPECT_EQ(3, foo->child_count());

  // Now remove all child views from root.
  root.RemoveAllChildViews(true);

  EXPECT_EQ(0, root.child_count());
  EXPECT_FALSE(root.has_children());
}

TEST_F(ViewTest, Contains) {
  View v1;
  View* v2 = new View;
  View* v3 = new View;

  v1.AddChildView(v2);
  v2->AddChildView(v3);

  EXPECT_FALSE(v1.Contains(NULL));
  EXPECT_TRUE(v1.Contains(&v1));
  EXPECT_TRUE(v1.Contains(v2));
  EXPECT_TRUE(v1.Contains(v3));

  EXPECT_FALSE(v2->Contains(NULL));
  EXPECT_TRUE(v2->Contains(v2));
  EXPECT_FALSE(v2->Contains(&v1));
  EXPECT_TRUE(v2->Contains(v3));

  EXPECT_FALSE(v3->Contains(NULL));
  EXPECT_TRUE(v3->Contains(v3));
  EXPECT_FALSE(v3->Contains(&v1));
  EXPECT_FALSE(v3->Contains(v2));
}

// Verifies if GetIndexOf() returns the correct index for the specified child
// view.
// The tree looks like this:
// root
// +-- child1
//     +-- foo1
// +-- child2
TEST_F(ViewTest, GetIndexOf) {
  View root;

  View* child1 = new View;
  root.AddChildView(child1);

  View* child2 = new View;
  root.AddChildView(child2);

  View* foo1 = new View;
  child1->AddChildView(foo1);

  EXPECT_EQ(-1, root.GetIndexOf(NULL));
  EXPECT_EQ(-1, root.GetIndexOf(&root));
  EXPECT_EQ(0, root.GetIndexOf(child1));
  EXPECT_EQ(1, root.GetIndexOf(child2));
  EXPECT_EQ(-1, root.GetIndexOf(foo1));

  EXPECT_EQ(-1, child1->GetIndexOf(NULL));
  EXPECT_EQ(-1, child1->GetIndexOf(&root));
  EXPECT_EQ(-1, child1->GetIndexOf(child1));
  EXPECT_EQ(-1, child1->GetIndexOf(child2));
  EXPECT_EQ(0, child1->GetIndexOf(foo1));

  EXPECT_EQ(-1, child2->GetIndexOf(NULL));
  EXPECT_EQ(-1, child2->GetIndexOf(&root));
  EXPECT_EQ(-1, child2->GetIndexOf(child2));
  EXPECT_EQ(-1, child2->GetIndexOf(child1));
  EXPECT_EQ(-1, child2->GetIndexOf(foo1));
}

// Verifies that the child views can be reordered correctly.
TEST_F(ViewTest, ReorderChildren) {
  View root;

  View* child = new View();
  root.AddChildView(child);

  View* foo1 = new View();
  child->AddChildView(foo1);
  View* foo2 = new View();
  child->AddChildView(foo2);
  View* foo3 = new View();
  child->AddChildView(foo3);
  foo1->set_focusable(true);
  foo2->set_focusable(true);
  foo3->set_focusable(true);

  ASSERT_EQ(0, child->GetIndexOf(foo1));
  ASSERT_EQ(1, child->GetIndexOf(foo2));
  ASSERT_EQ(2, child->GetIndexOf(foo3));
  ASSERT_EQ(foo2, foo1->GetNextFocusableView());
  ASSERT_EQ(foo3, foo2->GetNextFocusableView());
  ASSERT_EQ(NULL, foo3->GetNextFocusableView());

  // Move |foo2| at the end.
  child->ReorderChildView(foo2, -1);
  ASSERT_EQ(0, child->GetIndexOf(foo1));
  ASSERT_EQ(1, child->GetIndexOf(foo3));
  ASSERT_EQ(2, child->GetIndexOf(foo2));
  ASSERT_EQ(foo3, foo1->GetNextFocusableView());
  ASSERT_EQ(foo2, foo3->GetNextFocusableView());
  ASSERT_EQ(NULL, foo2->GetNextFocusableView());

  // Move |foo1| at the end.
  child->ReorderChildView(foo1, -1);
  ASSERT_EQ(0, child->GetIndexOf(foo3));
  ASSERT_EQ(1, child->GetIndexOf(foo2));
  ASSERT_EQ(2, child->GetIndexOf(foo1));
  ASSERT_EQ(NULL, foo1->GetNextFocusableView());
  ASSERT_EQ(foo2, foo1->GetPreviousFocusableView());
  ASSERT_EQ(foo2, foo3->GetNextFocusableView());
  ASSERT_EQ(foo1, foo2->GetNextFocusableView());

  // Move |foo2| to the front.
  child->ReorderChildView(foo2, 0);
  ASSERT_EQ(0, child->GetIndexOf(foo2));
  ASSERT_EQ(1, child->GetIndexOf(foo3));
  ASSERT_EQ(2, child->GetIndexOf(foo1));
  ASSERT_EQ(NULL, foo1->GetNextFocusableView());
  ASSERT_EQ(foo3, foo1->GetPreviousFocusableView());
  ASSERT_EQ(foo3, foo2->GetNextFocusableView());
  ASSERT_EQ(foo1, foo3->GetNextFocusableView());
}

// Verifies that GetViewByID returns the correctly child view from the specified
// ID.
// The tree looks like this:
// v1
// +-- v2
//     +-- v3
//     +-- v4
TEST_F(ViewTest, GetViewByID) {
  View v1;
  const int kV1ID = 1;
  v1.set_id(kV1ID);

  View v2;
  const int kV2ID = 2;
  v2.set_id(kV2ID);

  View v3;
  const int kV3ID = 3;
  v3.set_id(kV3ID);

  View v4;
  const int kV4ID = 4;
  v4.set_id(kV4ID);

  const int kV5ID = 5;

  v1.AddChildView(&v2);
  v2.AddChildView(&v3);
  v2.AddChildView(&v4);

  EXPECT_EQ(&v1, v1.GetViewByID(kV1ID));
  EXPECT_EQ(&v2, v1.GetViewByID(kV2ID));
  EXPECT_EQ(&v4, v1.GetViewByID(kV4ID));

  EXPECT_EQ(NULL, v1.GetViewByID(kV5ID));  // No V5 exists.
  EXPECT_EQ(NULL, v2.GetViewByID(kV1ID));  // It can get only from child views.

  const int kGroup = 1;
  v3.SetGroup(kGroup);
  v4.SetGroup(kGroup);

  View::Views views;
  v1.GetViewsInGroup(kGroup, &views);
  EXPECT_EQ(2U, views.size());

  View::Views::const_iterator i(std::find(views.begin(), views.end(), &v3));
  EXPECT_NE(views.end(), i);

  i = std::find(views.begin(), views.end(), &v4);
  EXPECT_NE(views.end(), i);
}

TEST_F(ViewTest, AddExistingChild) {
  View v1, v2, v3;

  v1.AddChildView(&v2);
  v1.AddChildView(&v3);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Check that there's no change in order when adding at same index.
  v1.AddChildViewAt(&v2, 0);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));
  v1.AddChildViewAt(&v3, 1);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Add it at a different index and check for change in order.
  v1.AddChildViewAt(&v2, 1);
  EXPECT_EQ(1, v1.GetIndexOf(&v2));
  EXPECT_EQ(0, v1.GetIndexOf(&v3));
  v1.AddChildViewAt(&v2, 0);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Check that calling |AddChildView()| does not change the order.
  v1.AddChildView(&v2);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));
  v1.AddChildView(&v3);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));
}

////////////////////////////////////////////////////////////////////////////////
// Layers
////////////////////////////////////////////////////////////////////////////////

#if defined(USE_AURA)

namespace {

// Test implementation of LayerAnimator.
class TestLayerAnimator : public ui::LayerAnimator {
 public:
  TestLayerAnimator();

  const gfx::Rect& last_bounds() const { return last_bounds_; }

  // LayerAnimator.
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;

 protected:
  virtual ~TestLayerAnimator() { }

 private:
  gfx::Rect last_bounds_;

  DISALLOW_COPY_AND_ASSIGN(TestLayerAnimator);
};

TestLayerAnimator::TestLayerAnimator()
    : ui::LayerAnimator(base::TimeDelta::FromMilliseconds(0)) {
}

void TestLayerAnimator::SetBounds(const gfx::Rect& bounds) {
  last_bounds_ = bounds;
}

}  // namespace

class ViewLayerTest : public ViewsTestBase {
 public:
  ViewLayerTest() : widget_(NULL), old_use_acceleration_(false) {}

  virtual ~ViewLayerTest() {
  }

  // Returns the Layer used by the RootView.
  ui::Layer* GetRootLayer() {
    ui::Layer* root_layer = NULL;
    widget()->CalculateOffsetToAncestorWithLayer(&root_layer);
    return root_layer;
  }

  virtual void SetUp() OVERRIDE {
    ViewTest::SetUp();
    old_use_acceleration_ = View::get_use_acceleration_when_possible();
    View::set_use_acceleration_when_possible(true);

    widget_ = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
    params.bounds = gfx::Rect(50, 50, 200, 200);
    widget_->Init(params);
    widget_->Show();
    widget_->GetRootView()->SetBounds(0, 0, 200, 200);
  }

  virtual void TearDown() OVERRIDE {
    View::set_use_acceleration_when_possible(old_use_acceleration_);
    widget_->CloseNow();
    ViewsTestBase::TearDown();
  }

  Widget* widget() { return widget_; }

 private:
  Widget* widget_;
  bool old_use_acceleration_;
};


TEST_F(ViewLayerTest, LayerToggling) {
  // Because we lazily create textures the calls to DrawTree are necessary to
  // ensure we trigger creation of textures.
  ui::Layer* root_layer = NULL;
  widget()->CalculateOffsetToAncestorWithLayer(&root_layer);
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // Create v1, give it a bounds and verify everything is set up correctly.
  View* v1 = new View;
  v1->SetPaintToLayer(true);
  EXPECT_TRUE(v1->layer() != NULL);
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));
  content_view->AddChildView(v1);
  ASSERT_TRUE(v1->layer() != NULL);
  EXPECT_EQ(root_layer, v1->layer()->parent());
  EXPECT_EQ(gfx::Rect(20, 30, 140, 150), v1->layer()->bounds());

  // Create v2 as a child of v1 and do basic assertion testing.
  View* v2 = new View;
  v1->AddChildView(v2);
  EXPECT_TRUE(v2->layer() == NULL);
  v2->SetBoundsRect(gfx::Rect(10, 20, 30, 40));
  v2->SetPaintToLayer(true);
  ASSERT_TRUE(v2->layer() != NULL);
  EXPECT_EQ(v1->layer(), v2->layer()->parent());
  EXPECT_EQ(gfx::Rect(10, 20, 30, 40), v2->layer()->bounds());

  // Turn off v1s layer. v2 should still have a layer but its parent should have
  // changed.
  v1->SetPaintToLayer(false);
  EXPECT_TRUE(v1->layer() == NULL);
  EXPECT_TRUE(v2->layer() != NULL);
  EXPECT_EQ(root_layer, v2->layer()->parent());
  ASSERT_EQ(1u, root_layer->children().size());
  EXPECT_EQ(root_layer->children()[0], v2->layer());
  // The bounds of the layer should have changed to be relative to the root view
  // now.
  EXPECT_EQ(gfx::Rect(30, 50, 30, 40), v2->layer()->bounds());

  // Make v1 have a layer again and verify v2s layer is wired up correctly.
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  v1->SetTransform(transform);
  EXPECT_TRUE(v1->layer() != NULL);
  EXPECT_TRUE(v2->layer() != NULL);
  EXPECT_EQ(root_layer, v1->layer()->parent());
  EXPECT_EQ(v1->layer(), v2->layer()->parent());
  ASSERT_EQ(1u, root_layer->children().size());
  EXPECT_EQ(root_layer->children()[0], v1->layer());
  ASSERT_EQ(1u, v1->layer()->children().size());
  EXPECT_EQ(v1->layer()->children()[0], v2->layer());
  EXPECT_EQ(gfx::Rect(10, 20, 30, 40), v2->layer()->bounds());
}

// Verifies turning on a layer wires up children correctly.
TEST_F(ViewLayerTest, NestedLayerToggling) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // Create v1, give it a bounds and verify everything is set up correctly.
  View* v1 = new View;
  content_view->AddChildView(v1);
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));

  View* v2 = new View;
  v1->AddChildView(v2);

  View* v3 = new View;
  v3->SetPaintToLayer(true);
  v2->AddChildView(v3);
  ASSERT_TRUE(v3->layer() != NULL);

  // At this point we have v1-v2-v3. v3 has a layer, v1 and v2 don't.

  v1->SetPaintToLayer(true);
  EXPECT_EQ(v1->layer(), v3->layer()->parent());
}

TEST_F(ViewLayerTest, LayerAnimator) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = new View;
  content_view->AddChildView(v1);
  v1->SetPaintToLayer(true);
  EXPECT_TRUE(v1->layer() != NULL);

  TestLayerAnimator* animator = new TestLayerAnimator();
  v1->layer()->SetAnimator(animator);

  gfx::Rect bounds(1, 2, 3, 4);
  v1->SetBoundsRect(bounds);
  EXPECT_EQ(bounds, animator->last_bounds());
  // TestLayerAnimator doesn't update the layer.
  EXPECT_NE(bounds, v1->layer()->bounds());
}

// Verifies the bounds of a layer are updated if the bounds of ancestor that
// doesn't have a layer change.
TEST_F(ViewLayerTest, BoundsChangeWithLayer) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = new View;
  content_view->AddChildView(v1);
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));

  View* v2 = new View;
  v2->SetBoundsRect(gfx::Rect(10, 11, 40, 50));
  v1->AddChildView(v2);
  v2->SetPaintToLayer(true);
  ASSERT_TRUE(v2->layer() != NULL);
  EXPECT_EQ(gfx::Rect(30, 41, 40, 50), v2->layer()->bounds());

  v1->SetPosition(gfx::Point(25, 36));
  EXPECT_EQ(gfx::Rect(35, 47, 40, 50), v2->layer()->bounds());

  v2->SetPosition(gfx::Point(11, 12));
  EXPECT_EQ(gfx::Rect(36, 48, 40, 50), v2->layer()->bounds());

  // Bounds of the layer should change even if the view is not invisible.
  v1->SetVisible(false);
  v1->SetPosition(gfx::Point(20, 30));
  EXPECT_EQ(gfx::Rect(31, 42, 40, 50), v2->layer()->bounds());

  v2->SetVisible(false);
  v2->SetBoundsRect(gfx::Rect(10, 11, 20, 30));
  EXPECT_EQ(gfx::Rect(30, 41, 20, 30), v2->layer()->bounds());
}

// Make sure layers are positioned correctly in RTL.
TEST_F(ViewLayerTest, BoundInRTL) {
  std::string locale = l10n_util::GetApplicationLocale(std::string());
  base::i18n::SetICUDefaultLocale("he");

  View* view = new View;
  widget()->SetContentsView(view);

  int content_width = view->width();

  // |v1| is initially not attached to anything. So its layer will have the same
  // bounds as the view.
  View* v1 = new View;
  v1->SetPaintToLayer(true);
  v1->SetBounds(10, 10, 20, 10);
  EXPECT_EQ(gfx::Rect(10, 10, 20, 10),
            v1->layer()->bounds());

  // Once |v1| is attached to the widget, its layer will get RTL-appropriate
  // bounds.
  view->AddChildView(v1);
  EXPECT_EQ(gfx::Rect(content_width - 30, 10, 20, 10),
            v1->layer()->bounds());
  gfx::Rect l1bounds = v1->layer()->bounds();

  // Now attach a View to the widget first, then create a layer for it. Make
  // sure the bounds are correct.
  View* v2 = new View;
  v2->SetBounds(50, 10, 30, 10);
  EXPECT_FALSE(v2->layer());
  view->AddChildView(v2);
  v2->SetPaintToLayer(true);
  EXPECT_EQ(gfx::Rect(content_width - 80, 10, 30, 10),
            v2->layer()->bounds());
  gfx::Rect l2bounds = v2->layer()->bounds();

  view->SetPaintToLayer(true);
  EXPECT_EQ(l1bounds, v1->layer()->bounds());
  EXPECT_EQ(l2bounds, v2->layer()->bounds());

  // Move one of the views. Make sure the layer is positioned correctly
  // afterwards.
  v1->SetBounds(v1->x() - 5, v1->y(), v1->width(), v1->height());
  l1bounds.set_x(l1bounds.x() + 5);
  EXPECT_EQ(l1bounds, v1->layer()->bounds());

  view->SetPaintToLayer(false);
  EXPECT_EQ(l1bounds, v1->layer()->bounds());
  EXPECT_EQ(l2bounds, v2->layer()->bounds());

  // Move a view again.
  v2->SetBounds(v2->x() + 5, v2->y(), v2->width(), v2->height());
  l2bounds.set_x(l2bounds.x() - 5);
  EXPECT_EQ(l2bounds, v2->layer()->bounds());

  // Reset locale.
  base::i18n::SetICUDefaultLocale(locale);
}

// Makes sure a transform persists after toggling the visibility.
TEST_F(ViewLayerTest, ToggleVisibilityWithTransform) {
  View* view = new View;
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  view->SetTransform(transform);
  widget()->SetContentsView(view);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));

  view->SetVisible(false);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));

  view->SetVisible(true);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
}

// Verifies a transform persists after removing/adding a view with a transform.
TEST_F(ViewLayerTest, ResetTransformOnLayerAfterAdd) {
  View* view = new View;
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  view->SetTransform(transform);
  widget()->SetContentsView(view);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
  ASSERT_TRUE(view->layer() != NULL);
  EXPECT_EQ(2.0f, view->layer()->transform().matrix().get(0, 0));

  View* parent = view->parent();
  parent->RemoveChildView(view);
  parent->AddChildView(view);

  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
  ASSERT_TRUE(view->layer() != NULL);
  EXPECT_EQ(2.0f, view->layer()->transform().matrix().get(0, 0));
}

// Makes sure that layer visibility is correct after toggling View visibility.
TEST_F(ViewLayerTest, ToggleVisibilityWithLayer) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // The view isn't attached to a widget or a parent view yet. But it should
  // still have a layer, but the layer should not be attached to the root
  // layer.
  View* v1 = new View;
  v1->SetPaintToLayer(true);
  EXPECT_TRUE(v1->layer());
  EXPECT_FALSE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                               v1->layer()));

  // Once the view is attached to a widget, its layer should be attached to the
  // root layer and visible.
  content_view->AddChildView(v1);
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v1->layer()));
  EXPECT_TRUE(v1->layer()->IsDrawn());

  v1->SetVisible(false);
  EXPECT_FALSE(v1->layer()->IsDrawn());

  v1->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());

  widget()->Hide();
  EXPECT_FALSE(v1->layer()->IsDrawn());

  widget()->Show();
  EXPECT_TRUE(v1->layer()->IsDrawn());
}

// Tests that the layers in the subtree are orphaned after a View is removed
// from the parent.
TEST_F(ViewLayerTest, OrphanLayerAfterViewRemove) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = new View;
  content_view->AddChildView(v1);

  View* v2 = new View;
  v1->AddChildView(v2);
  v2->SetPaintToLayer(true);
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v2->layer()));
  EXPECT_TRUE(v2->layer()->IsDrawn());

  content_view->RemoveChildView(v1);

  EXPECT_FALSE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                               v2->layer()));

  // Reparent |v2|.
  content_view->AddChildView(v2);
  delete v1;
  v1 = NULL;
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v2->layer()));
  EXPECT_TRUE(v2->layer()->IsDrawn());
}

class PaintTrackingView : public View {
 public:
  PaintTrackingView() : painted_(false) {
  }

  bool painted() const { return painted_; }
  void set_painted(bool value) { painted_ = value; }

  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {
    painted_ = true;
  }

 private:
  bool painted_;

  DISALLOW_COPY_AND_ASSIGN(PaintTrackingView);
};

// Makes sure child views with layers aren't painted when paint starts at an
// ancestor.
TEST_F(ViewLayerTest, DontPaintChildrenWithLayers) {
  PaintTrackingView* content_view = new PaintTrackingView;
  widget()->SetContentsView(content_view);
  content_view->SetPaintToLayer(true);
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::Wait(GetRootLayer()->GetCompositor());
  GetRootLayer()->SchedulePaint(gfx::Rect(0, 0, 10, 10));
  content_view->set_painted(false);
  // content_view no longer has a dirty rect. Paint from the root and make sure
  // PaintTrackingView isn't painted.
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::Wait(GetRootLayer()->GetCompositor());
  EXPECT_FALSE(content_view->painted());

  // Make content_view have a dirty rect, paint the layers and make sure
  // PaintTrackingView is painted.
  content_view->layer()->SchedulePaint(gfx::Rect(0, 0, 10, 10));
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::Wait(GetRootLayer()->GetCompositor());
  EXPECT_TRUE(content_view->painted());
}

// Tests that the visibility of child layers are updated correctly when a View's
// visibility changes.
TEST_F(ViewLayerTest, VisibilityChildLayers) {
  View* v1 = new View;
  v1->SetPaintToLayer(true);
  widget()->SetContentsView(v1);

  View* v2 = new View;
  v1->AddChildView(v2);

  View* v3 = new View;
  v2->AddChildView(v3);
  v3->SetVisible(false);

  View* v4 = new View;
  v4->SetPaintToLayer(true);
  v3->AddChildView(v4);

  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(false);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(false);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));

  v3->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));

  // Reparent |v3| to |v1|.
  v1->AddChildView(v3);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_TRUE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));
}

// This test creates a random View tree, and then randomly reorders child views,
// reparents views etc. Unrelated changes can appear to break this test. So
// marking this as FLAKY.
TEST_F(ViewLayerTest, DISABLED_ViewLayerTreesInSync) {
  View* content = new View;
  content->SetPaintToLayer(true);
  widget()->SetContentsView(content);
  widget()->Show();

  ConstructTree(content, 5);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));
}

// Verifies when views are reordered the layer is also reordered. The widget is
// providing the parent layer.
TEST_F(ViewLayerTest, ReorderUnderWidget) {
  View* content = new View;
  widget()->SetContentsView(content);
  View* c1 = new View;
  c1->SetPaintToLayer(true);
  content->AddChildView(c1);
  View* c2 = new View;
  c2->SetPaintToLayer(true);
  content->AddChildView(c2);

  ui::Layer* parent_layer = c1->layer()->parent();
  ASSERT_TRUE(parent_layer);
  ASSERT_EQ(2u, parent_layer->children().size());
  EXPECT_EQ(c1->layer(), parent_layer->children()[0]);
  EXPECT_EQ(c2->layer(), parent_layer->children()[1]);

  // Move c1 to the front. The layers should have moved too.
  content->ReorderChildView(c1, -1);
  EXPECT_EQ(c1->layer(), parent_layer->children()[1]);
  EXPECT_EQ(c2->layer(), parent_layer->children()[0]);
}

// Verifies that the layer of a view can be acquired properly.
TEST_F(ViewLayerTest, AcquireLayer) {
  View* content = new View;
  widget()->SetContentsView(content);
  scoped_ptr<View> c1(new View);
  c1->SetPaintToLayer(true);
  EXPECT_TRUE(c1->layer());
  content->AddChildView(c1.get());

  scoped_ptr<ui::Layer> layer(c1->AcquireLayer());
  EXPECT_EQ(layer.get(), c1->layer());

  scoped_ptr<ui::Layer> layer2(c1->RecreateLayer());
  EXPECT_NE(c1->layer(), layer2.get());

  // Destroy view before destroying layer.
  c1.reset();
}

// Verify that new layer scales content only if the old layer does.
TEST_F(ViewLayerTest, RecreateLayer) {
  scoped_ptr<View> v(new View());
  v->SetPaintToLayer(true);
  // Set to non default value.
  v->layer()->set_scale_content(false);
  scoped_ptr<ui::Layer> old_layer(v->RecreateLayer());
  ui::Layer* new_layer = v->layer();
  EXPECT_FALSE(new_layer->scale_content());
}

#endif  // USE_AURA

}  // namespace views
