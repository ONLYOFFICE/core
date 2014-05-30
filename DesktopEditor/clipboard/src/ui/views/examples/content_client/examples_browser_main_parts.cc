// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/content_client/examples_browser_main_parts.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/message_loop.h"
#include "base/run_loop.h"
#include "base/string_number_conversions.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "content/public/common/content_switches.h"
#include "content/shell/shell_browser_context.h"
#include "googleurl/src/gurl.h"
#include "ui/views/examples/examples_window_with_content.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/test/desktop_test_views_delegate.h"

#if defined(USE_AURA)
#include "ui/aura/env.h"
#include "ui/gfx/screen.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/views/widget/native_widget_aura.h"
#endif

namespace views {
namespace examples {

ExamplesBrowserMainParts::ExamplesBrowserMainParts(
    const content::MainFunctionParams& parameters) {
}

ExamplesBrowserMainParts::~ExamplesBrowserMainParts() {
}

void ExamplesBrowserMainParts::PreMainMessageLoopRun() {
  browser_context_.reset(new content::ShellBrowserContext(false));

#if !defined(OS_CHROMEOS) && defined(USE_AURA)
  gfx::Screen::SetScreenInstance(
      gfx::SCREEN_TYPE_NATIVE, CreateDesktopScreen());
#endif
  views_delegate_.reset(new DesktopTestViewsDelegate);

  ShowExamplesWindowWithContent(QUIT_ON_CLOSE, browser_context_.get());
}

void ExamplesBrowserMainParts::PostMainMessageLoopRun() {
  browser_context_.reset();
  views_delegate_.reset();
#if defined(USE_AURA)
  aura::Env::DeleteInstance();
#endif
}

bool ExamplesBrowserMainParts::MainMessageLoopRun(int* result_code) {
  // xxx: Hax here because this kills event handling.
#if !defined(USE_AURA)
  AcceleratorHandler accelerator_handler;
  base::RunLoop run_loop(&accelerator_handler);
#else
  base::RunLoop run_loop;
#endif
  run_loop.Run();
  return true;
}

}  // namespace examples
}  // namespace views
