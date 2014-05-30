// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/app/content_main.h"
#include "sandbox/win/src/sandbox_types.h"
#include "ui/views/examples/content_client/examples_main_delegate.h"

#if defined(OS_WIN)
#include "content/public/app/startup_helper_win.h"
#endif

#if defined(OS_WIN)
int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
  sandbox::SandboxInterfaceInfo sandbox_info = {0};
  content::InitializeSandboxInfo(&sandbox_info);
  views::examples::ExamplesMainDelegate delegate;
  return content::ContentMain(instance, &sandbox_info, &delegate);
}
#else
int main(int argc, const char** argv) {
  views::examples::ExamplesMainDelegate delegate;
  return content::ContentMain(argc, argv, &delegate);
}
#endif
