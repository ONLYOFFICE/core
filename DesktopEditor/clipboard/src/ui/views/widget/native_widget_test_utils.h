// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_TEST_UTILS_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_TEST_UTILS_H_

namespace views {
namespace internal {

class NativeWidgetPrivate;

// Create dummy widgets for use in testing.  Caller owns the returned
// |NativeWidgetPrivate| object which, in turn, owns the associated Widget.
NativeWidgetPrivate* CreateNativeWidget();
NativeWidgetPrivate* CreateNativeSubWidget();

}  // namespace internal
}  // namespace views

#endif  // UI_VIEWS_WIDGET_NATIVE_WIDGET_TEST_UTILS_H_
