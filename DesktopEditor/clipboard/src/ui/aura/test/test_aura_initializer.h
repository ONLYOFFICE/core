// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_AURA_INITIALIZER_H_
#define UI_AURA_TEST_TEST_AURA_INITIALIZER_H_

#include "base/basictypes.h"

namespace aura {
namespace test {

// Initializes various objects needed to run unit tests that use aura::*
// objects.
class TestAuraInitializer {
 public:
  TestAuraInitializer();
  virtual ~TestAuraInitializer();

 private:
  DISALLOW_COPY_AND_ASSIGN(TestAuraInitializer);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_TEST_AURA_INITIALIZER_H_
