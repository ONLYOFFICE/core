// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_TEXT_INPUT_TEST_SUPPORT_H_
#define UI_BASE_IME_TEXT_INPUT_TEST_SUPPORT_H_

#include "base/basictypes.h"

namespace ui {

class TextInputTestSupport {
 public:
  // Initialize DBusThreadManager for text input testing if necessary.
  static void Initialize();

  // Shutdown DBusThreadManager if necessary.
  static void Shutdown();
};

}  // namespace ui

#endif  // UI_BASE_IME_TEXT_INPUT_TEST_SUPPORT_H_
