// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_TEST_WIDGET_OBSERVER_H_
#define UI_VIEWS_TEST_TEST_WIDGET_OBSERVER_H_

#include <stddef.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/widget/widget_observer.h"

namespace views {
namespace test {

// A Widget observer class used in the tests below to observe bubbles closing.
class TestWidgetObserver : public WidgetObserver {
 public:
  explicit TestWidgetObserver(Widget* widget);
  virtual ~TestWidgetObserver();

  bool widget_closed() const { return widget_ == NULL; }

 private:
  // WidgetObserver overrides:
  virtual void OnWidgetDestroying(Widget* widget) OVERRIDE;

  Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(TestWidgetObserver);
};

}  // namespace test
}  // namespace views

#endif  // UI_VIEWS_TEST_TEST_WIDGET_OBSERVER_H_
