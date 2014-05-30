// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TEST_COMPOSITOR_TEST_SUPPORT_H_
#define UI_COMPOSITOR_TEST_COMPOSITOR_TEST_SUPPORT_H_

namespace ui {

class CompositorTestSupport {
 public:
  // Called by programs that want to use a real compositor,
  // but skip the typical initialization of browser_main.cc.
  static void Initialize();
  static void Terminate();
};

}  // namespace ui

#endif  // UI_COMPOSITOR_TEST_COMPOSITOR_TEST_SUPPORT_H_
