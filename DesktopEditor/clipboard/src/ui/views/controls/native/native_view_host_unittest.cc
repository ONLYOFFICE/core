// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/native/native_view_host.h"

#if defined(OS_WIN) && !defined(USE_AURA)
#include <windows.h>
#endif

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/aura/window.h"
#endif

namespace views {

typedef ViewsTestBase NativeViewHostTest;

namespace {

// View implementation used by NativeViewHierarchyChanged to count number of
// times NativeViewHierarchyChanged() is invoked.
class NativeViewHierarchyChangedTestView : public View {
 public:
  NativeViewHierarchyChangedTestView()
      : attached_count_(0),
        detached_count_(0) {
  }

  void ResetCounts() {
    attached_count_ = detached_count_ = 0;
  }

  int attached_count() const { return attached_count_; }
  int detached_count() const { return detached_count_; }

  // Overriden from View:
  virtual void NativeViewHierarchyChanged(
      bool attached,
      gfx::NativeView native_view,
      internal::RootView* root_view) OVERRIDE {
    if (attached)
      attached_count_++;
    else
      detached_count_++;
    View::NativeViewHierarchyChanged(attached, native_view, root_view);
  }

 private:
  int attached_count_;
  int detached_count_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewHierarchyChangedTestView);
};

#if defined(USE_AURA)

aura::Window* GetNativeParent(aura::Window* window) {
  return window->parent();
}

#elif defined(OS_WIN)

HWND GetNativeParent(HWND window) {
  return GetParent(window);
}

#endif

}  // namespace

// Verifies NativeViewHierarchyChanged is sent.
TEST_F(NativeViewHostTest, NativeViewHierarchyChanged) {
  // Create the top level widget.
  scoped_ptr<Widget> toplevel(new Widget);
  Widget::InitParams toplevel_params =
      CreateParams(Widget::InitParams::TYPE_WINDOW);
  toplevel_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  toplevel->Init(toplevel_params);

  // And the child widget.
  NativeViewHierarchyChangedTestView* test_view =
      new NativeViewHierarchyChangedTestView;
  scoped_ptr<Widget> child(new Widget);
  Widget::InitParams child_params(Widget::InitParams::TYPE_CONTROL);
  child_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  child_params.parent = toplevel->GetNativeView();
  child->Init(child_params);
  child->SetContentsView(test_view);

  // Owned by |toplevel|.
  NativeViewHost* host = new NativeViewHost;
  toplevel->GetRootView()->AddChildView(host);
  host->Attach(child->GetNativeView());

  EXPECT_EQ(0, test_view->attached_count());
  EXPECT_EQ(0, test_view->detached_count());
  test_view->ResetCounts();

  // Detach should send both an attach and detach as well as changing parent.
  host->Detach();
  EXPECT_EQ(1, test_view->attached_count());
  EXPECT_EQ(1, test_view->detached_count());
  EXPECT_NE(toplevel->GetNativeView(),
            GetNativeParent(child->GetNativeView()));
  test_view->ResetCounts();

  // Attaching should send both an attach and detach and reset parent.
  host->Attach(child->GetNativeView());
  EXPECT_EQ(1, test_view->attached_count());
  EXPECT_EQ(1, test_view->detached_count());
  EXPECT_EQ(toplevel->GetNativeView(),
            GetNativeParent(child->GetNativeView()));
}

}  // namespace views
