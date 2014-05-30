// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/test/test_suite.h"

#include "base/files/file_path.h"
#include "base/path_service.h"
#include "build/build_config.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/resource/resource_handle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/gfx/gfx_paths.h"

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#include "ui/android/ui_jni_registrar.h"
#endif

#if defined(OS_MACOSX) && !defined(OS_IOS)
#include "base/mac/bundle_locations.h"
#endif

namespace ui {
namespace test {

UITestSuite::UITestSuite(int argc, char** argv) : base::TestSuite(argc, argv) {}

void UITestSuite::Initialize() {
  base::TestSuite::Initialize();

#if defined(OS_ANDROID)
  // Register JNI bindings for android.
  ui::android::RegisterJni(base::android::AttachCurrentThread());
#endif

  ui::RegisterPathProvider();
  gfx::RegisterPathProvider();

#if defined(OS_MACOSX) && !defined(OS_IOS)
  // Look in the framework bundle for resources.
  // TODO(port): make a resource bundle for non-app exes.  What's done here
  // isn't really right because this code needs to depend on chrome_dll
  // being built.  This is inappropriate in app.
  base::FilePath path;
  PathService::Get(base::DIR_EXE, &path);
#if defined(GOOGLE_CHROME_BUILD)
  path = path.AppendASCII("Google Chrome Framework.framework");
#elif defined(CHROMIUM_BUILD)
  path = path.AppendASCII("Chromium Framework.framework");
#else
#error Unknown branding
#endif
  base::mac::SetOverrideFrameworkBundlePath(path);
#elif defined(OS_POSIX)
  base::FilePath pak_dir;
#if defined(OS_ANDROID)
  PathService::Get(ui::DIR_RESOURCE_PAKS_ANDROID, &pak_dir);
#else
  PathService::Get(base::DIR_MODULE, &pak_dir);
  pak_dir = pak_dir.AppendASCII("ui_unittests_strings");
  PathService::Override(ui::DIR_LOCALES, pak_dir);
#endif  // defined(OS_ANDROID)
#endif  // defined(OS_MACOSX) && !defined(OS_IOS)

  // Force unittests to run using en-US so if we test against string
  // output, it'll pass regardless of the system language.
  ui::ResourceBundle::InitSharedInstanceWithLocale("en-US", NULL);

#if !defined(OS_MACOSX) && defined(OS_POSIX)
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      pak_dir.AppendASCII("chrome_100_percent.pak"),
      ui::SCALE_FACTOR_100P);
#endif
}

void UITestSuite::Shutdown() {
  ui::ResourceBundle::CleanupSharedInstance();

#if defined(OS_MACOSX) && !defined(OS_IOS)
  base::mac::SetOverrideFrameworkBundle(NULL);
#endif
  base::TestSuite::Shutdown();
}

}  // namespace test
}  // namespace ui
