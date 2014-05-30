// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_SUITE_H_
#define UI_AURA_TEST_TEST_SUITE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/test/test_suite.h"

namespace aura {
namespace test {

class AuraTestSuite : public base::TestSuite {
 public:
  AuraTestSuite(int argc, char** argv);

 protected:
  // Overridden from base::TestSuite:
  virtual void Initialize() OVERRIDE;
  virtual void Shutdown() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(AuraTestSuite);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_TEST_SUITE_H_
