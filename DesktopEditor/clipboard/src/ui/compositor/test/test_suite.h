// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TEST_TEST_SUITE_H_
#define UI_COMPOSITOR_TEST_TEST_SUITE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/test/test_suite.h"

namespace base {
class MessageLoop;
}

namespace ui {
namespace test {

class CompositorTestSuite : public base::TestSuite {
 public:
  CompositorTestSuite(int argc, char** argv);
  virtual ~CompositorTestSuite();

 protected:
  // Overridden from base::TestSuite:
  virtual void Initialize() OVERRIDE;
  virtual void Shutdown() OVERRIDE;

 private:
  scoped_ptr<base::MessageLoop> message_loop_;

  DISALLOW_COPY_AND_ASSIGN(CompositorTestSuite);
};

}  // namespace test
}  // namespace ui

#endif  // UI_COMPOSITOR_TEST_TEST_SUITE_H_
