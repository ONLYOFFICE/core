// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/resource/resource_bundle_win.h"

#include "base/logging.h"
#include "base/path_service.h"
#include "base/utf_string_conversions.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/resource/resource_data_dll_win.h"
#include "ui/base/win/dpi.h"

namespace ui {

namespace {

HINSTANCE resources_data_dll;

HINSTANCE GetCurrentResourceDLL() {
  if (resources_data_dll)
    return resources_data_dll;
  return GetModuleHandle(NULL);
}

base::FilePath GetResourcesPakFilePath(const std::string& pak_name) {
  base::FilePath path;
  if (PathService::Get(base::DIR_MODULE, &path))
    return path.AppendASCII(pak_name.c_str());

  // Return just the name of the pack file.
  return base::FilePath(ASCIIToUTF16(pak_name));
}

}  // namespace

void ResourceBundle::LoadCommonResources() {
  // As a convenience, add the current resource module as a data packs.
  data_packs_.push_back(new ResourceDataDLL(GetCurrentResourceDLL()));
  // Have high-DPI resources for 140% and 180% scaling on Windows based on
  // default scaling for Metro mode. If high-DPI mode is enabled, load resource
  // pak closest to the desired scale factor.  The high-DPI resources are
  // scaled up from 100% touch.
  float scale = ui::win::GetDeviceScaleFactor();
  bool force_touch_resources = false;
  switch(ui::GetScaleFactorFromScale(scale)) {
    case ui::SCALE_FACTOR_180P:
      AddDataPackFromPath(GetResourcesPakFilePath(
          "chrome_touch_180_percent.pak"),
          SCALE_FACTOR_180P);
      force_touch_resources = true;
      break;
    case ui::SCALE_FACTOR_140P:
      AddDataPackFromPath(GetResourcesPakFilePath(
          "chrome_touch_140_percent.pak"),
          SCALE_FACTOR_140P);
      force_touch_resources = true;
  }
  // TODO(kevers|girard): Remove loading of 1x resources when in high-DPI
  // mode once all resources are available at 140% and 180%.
  if (ui::GetDisplayLayout() == ui::LAYOUT_TOUCH || force_touch_resources) {
    AddDataPackFromPath(
        GetResourcesPakFilePath("chrome_touch_100_percent.pak"),
        SCALE_FACTOR_100P);
  } else {
    AddDataPackFromPath(
        GetResourcesPakFilePath("chrome_100_percent.pak"),
        SCALE_FACTOR_100P);
  }
}

gfx::Image& ResourceBundle::GetNativeImageNamed(int resource_id, ImageRTL rtl) {
  // Flipped image is not used on Windows.
  DCHECK_EQ(rtl, RTL_DISABLED);

  // Windows only uses SkBitmap for gfx::Image, so this is the same as
  // GetImageNamed.
  return GetImageNamed(resource_id);
}

void SetResourcesDataDLL(HINSTANCE handle) {
  resources_data_dll = handle;
}

HICON LoadThemeIconFromResourcesDataDLL(int icon_id) {
  return ::LoadIcon(GetCurrentResourceDLL(), MAKEINTRESOURCE(icon_id));
}

}  // namespace ui;
