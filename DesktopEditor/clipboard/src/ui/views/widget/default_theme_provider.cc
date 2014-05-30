// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/default_theme_provider.h"

#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia.h"

#if defined(OS_WIN) && !defined(USE_AURA)
#include "ui/base/win/shell.h"
#endif

namespace views {

DefaultThemeProvider::DefaultThemeProvider() {}

DefaultThemeProvider::~DefaultThemeProvider() {}

gfx::ImageSkia* DefaultThemeProvider::GetImageSkiaNamed(int id) const {
  return ResourceBundle::GetSharedInstance().GetImageSkiaNamed(id);
}

SkColor DefaultThemeProvider::GetColor(int id) const {
  // Return debugging-blue.
  return 0xff0000ff;
}

bool DefaultThemeProvider::GetDisplayProperty(int id, int* result) const {
  return false;
}

bool DefaultThemeProvider::ShouldUseNativeFrame() const {
#if defined(OS_WIN) && !defined(USE_AURA)
  return ui::win::IsAeroGlassEnabled();
#else
  return false;
#endif
}

bool DefaultThemeProvider::HasCustomImage(int id) const {
  return false;
}

base::RefCountedMemory* DefaultThemeProvider::GetRawData(
    int id,
    ui::ScaleFactor scale_factor) const {
  return NULL;
}

}  // namespace views
