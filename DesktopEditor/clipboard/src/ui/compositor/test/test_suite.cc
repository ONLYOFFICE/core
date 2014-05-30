// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/test/test_suite.h"

#include "base/command_line.h"
#include "base/message_loop.h"
#include "ui/base/ui_base_paths.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/test/compositor_test_support.h"
#include "ui/gfx/gfx_paths.h"
#include "ui/gl/gl_implementation.h"

#if defined(USE_X11)
#include <X11/Xlib.h>
#endif

namespace ui {
namespace test {

CompositorTestSuite::CompositorTestSuite(int argc, char** argv)
    : TestSuite(argc, argv) {}

CompositorTestSuite::~CompositorTestSuite() {}

void CompositorTestSuite::Initialize() {
#if defined(USE_X11)
  XInitThreads();
#endif
#if defined(OS_LINUX)
  gfx::InitializeGLBindings(gfx::kGLImplementationOSMesaGL);
#endif
  base::TestSuite::Initialize();

  gfx::RegisterPathProvider();

  message_loop_.reset(new base::MessageLoop(base::MessageLoop::TYPE_UI));
  CompositorTestSupport::Initialize();
  Compositor::Initialize();
}

void CompositorTestSuite::Shutdown() {
  Compositor::Terminate();
  CompositorTestSupport::Terminate();
  message_loop_.reset();

  base::TestSuite::Shutdown();
}

}  // namespace test
}  // namespace ui
