// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/test_aura_initializer.h"

#include "base/base_paths.h"
#include "base/path_service.h"
#include "ui/base/resource/resource_bundle.h"

namespace aura {
namespace test {

TestAuraInitializer::TestAuraInitializer() {
#if defined(OS_LINUX)
  base::FilePath pak_file;
  PathService::Get(base::DIR_MODULE, &pak_file);
  pak_file = pak_file.Append(FILE_PATH_LITERAL(
      "aura_test_support_resources.pak"));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
#endif
}

TestAuraInitializer::~TestAuraInitializer() {
#if defined(OS_LINUX)
  ui::ResourceBundle::CleanupSharedInstance();
#endif
}

}  // namespace test
}  // namespace aura
