// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/test/views_test_base.h"

#include "base/run_loop.h"
#include "ui/base/clipboard/clipboard.h"

#if defined(USE_AURA)
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_helper.h"
#endif

#if defined(OS_WIN)
#include "base/win/metro.h"
#include "ui/base/ime/win/tsf_bridge.h"
#endif

namespace views {

ViewsTestBase::ViewsTestBase()
    : setup_called_(false),
      teardown_called_(false) {
}

ViewsTestBase::~ViewsTestBase() {
  CHECK(setup_called_)
      << "You have overridden SetUp but never called super class's SetUp";
  CHECK(teardown_called_)
      << "You have overrideen TearDown but never called super class's TearDown";
}

void ViewsTestBase::SetUp() {
  testing::Test::SetUp();
  setup_called_ = true;
  if (!views_delegate_.get())
    views_delegate_.reset(new TestViewsDelegate());
#if defined(USE_AURA)
  aura_test_helper_.reset(new aura::test::AuraTestHelper(&message_loop_));
  aura_test_helper_->SetUp();
#endif  // USE_AURA
#if defined(OS_WIN)
    if (base::win::IsTSFAwareRequired())
      ui::TSFBridge::Initialize();
#endif
}

void ViewsTestBase::TearDown() {
  ui::Clipboard::DestroyClipboardForCurrentThread();

  // Flush the message loop because we have pending release tasks
  // and these tasks if un-executed would upset Valgrind.
  RunPendingMessages();
  teardown_called_ = true;
  views_delegate_.reset();
  testing::Test::TearDown();
#if defined(USE_AURA)
  aura_test_helper_->TearDown();
#endif
}

void ViewsTestBase::RunPendingMessages() {
  base::RunLoop run_loop;
#if defined(USE_AURA)
  run_loop.set_dispatcher(aura::Env::GetInstance()->GetDispatcher());
#endif
  run_loop.RunUntilIdle();
}

Widget::InitParams ViewsTestBase::CreateParams(
    Widget::InitParams::Type type) {
  Widget::InitParams params(type);
#if defined(USE_AURA)
  params.context = aura_test_helper_->root_window();
#endif
  return params;
}

gfx::NativeView ViewsTestBase::GetContext() {
#if defined(USE_AURA)
  return aura_test_helper_->root_window();
#else
  return NULL;
#endif
}

}  // namespace views
