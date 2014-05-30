// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/compositor/compositor_setup.h"
#include "ui/compositor/test/compositor_test_support.h"
#include "ui/test/test_suite.h"

class KeyboardTestSuite : public ui::test::UITestSuite {
 public:
  KeyboardTestSuite(int argc, char** argv)
      : ui::test::UITestSuite(argc, argv) {
  }

 protected:
  virtual void Initialize() OVERRIDE;
  virtual void Shutdown() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(KeyboardTestSuite);
};

void KeyboardTestSuite::Initialize() {
  ui::test::UITestSuite::Initialize();

  ui::CompositorTestSupport::Initialize();
  ui::SetupTestCompositor();
}

void KeyboardTestSuite::Shutdown() {
  ui::CompositorTestSupport::Terminate();
  ui::test::UITestSuite::Shutdown();
}

int main(int argc, char** argv) {
  return KeyboardTestSuite(argc, argv).Run();
}
