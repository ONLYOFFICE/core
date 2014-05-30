// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/native_widget_test_utils.h"

#include "ui/views/view.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/widget/widget.h"

namespace views {
namespace internal {

namespace {

NativeWidgetPrivate* CreateNativeWidgetOfType(Widget::InitParams::Type type) {
  Widget* widget = new Widget;
  Widget::InitParams params(type);
  params.ownership = views::Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET;
  params.child = false; // Implicitly set to true by ctor with TYPE_CONTROL.
  params.bounds = gfx::Rect(10, 10, 200, 200);
  widget->Init(params);
  return widget->native_widget_private();
}

}  // namespace

NativeWidgetPrivate* CreateNativeWidget() {
  return CreateNativeWidgetOfType(Widget::InitParams::TYPE_POPUP);
}

NativeWidgetPrivate* CreateNativeSubWidget() {
  return CreateNativeWidgetOfType(Widget::InitParams::TYPE_CONTROL);
}

}  // namespace internal
}  // namespace ui
