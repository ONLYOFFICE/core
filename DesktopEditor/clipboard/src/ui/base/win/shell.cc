// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/win/shell.h"

#include <dwmapi.h>
#include <shlobj.h>  // Must be before propkey.
#include <propkey.h>
#include <shellapi.h>

#include "base/files/file_path.h"
#include "base/native_library.h"
#include "base/string_util.h"
#include "base/win/metro.h"
#include "base/win/scoped_comptr.h"
#include "base/win/win_util.h"
#include "base/win/windows_version.h"

namespace ui {
namespace win {

namespace {

void SetAppDetailsForWindow(const string16& app_id,
                            const string16& app_icon,
                            const string16& relaunch_command,
                            const string16& relaunch_display_name,
                            HWND hwnd) {
  // This functionality is only available on Win7+. It also doesn't make sense
  // to do this for Chrome Metro.
  if (base::win::GetVersion() < base::win::VERSION_WIN7 ||
      base::win::IsMetroProcess())
    return;
  base::win::ScopedComPtr<IPropertyStore> pps;
  HRESULT result = SHGetPropertyStoreForWindow(
      hwnd, __uuidof(*pps), reinterpret_cast<void**>(pps.Receive()));
  if (S_OK == result) {
    if (!app_id.empty())
      base::win::SetAppIdForPropertyStore(pps, app_id.c_str());
    if (!app_icon.empty()) {
      base::win::SetStringValueForPropertyStore(
          pps, PKEY_AppUserModel_RelaunchIconResource, app_icon.c_str());
    }
    if (!relaunch_command.empty()) {
      base::win::SetStringValueForPropertyStore(
          pps, PKEY_AppUserModel_RelaunchCommand, relaunch_command.c_str());
    }
    if (!relaunch_display_name.empty()) {
      base::win::SetStringValueForPropertyStore(
          pps, PKEY_AppUserModel_RelaunchDisplayNameResource,
          relaunch_display_name.c_str());
    }
  }
}

}  // namespace

// Show the Windows "Open With" dialog box to ask the user to pick an app to
// open the file with.
bool OpenItemWithExternalApp(const string16& full_path) {
  SHELLEXECUTEINFO sei = { sizeof(sei) };
  sei.fMask = SEE_MASK_FLAG_DDEWAIT;
  sei.nShow = SW_SHOWNORMAL;
  sei.lpVerb = L"openas";
  sei.lpFile = full_path.c_str();
  return (TRUE == ::ShellExecuteExW(&sei));
}

bool OpenAnyViaShell(const string16& full_path,
                     const string16& directory,
                     const string16& args,
                     DWORD mask) {
  SHELLEXECUTEINFO sei = { sizeof(sei) };
  sei.fMask = mask;
  sei.nShow = SW_SHOWNORMAL;
  sei.lpFile = full_path.c_str();
  sei.lpDirectory = directory.c_str();
  if (!args.empty())
    sei.lpParameters = args.c_str();

  if (::ShellExecuteExW(&sei))
    return true;
  if (::GetLastError() == ERROR_NO_ASSOCIATION)
    return OpenItemWithExternalApp(full_path);
  return false;
}

bool OpenItemViaShell(const base::FilePath& full_path) {
  return OpenAnyViaShell(full_path.value(), full_path.DirName().value(),
                         string16(), 0);
}

bool OpenItemViaShellNoZoneCheck(const base::FilePath& full_path) {
  return OpenAnyViaShell(full_path.value(), string16(), string16(),
                         SEE_MASK_NOZONECHECKS | SEE_MASK_FLAG_DDEWAIT);
}

void SetAppIdForWindow(const string16& app_id, HWND hwnd) {
  SetAppDetailsForWindow(app_id, string16(), string16(), string16(), hwnd);
}

void SetAppIconForWindow(const string16& app_icon, HWND hwnd) {
  SetAppDetailsForWindow(string16(), app_icon, string16(), string16(), hwnd);
}

void SetRelaunchDetailsForWindow(const string16& relaunch_command,
                                 const string16& display_name,
                                 HWND hwnd) {
  SetAppDetailsForWindow(string16(),
                         string16(),
                         relaunch_command,
                         display_name,
                         hwnd);
}

bool IsAeroGlassEnabled() {
  if (base::win::GetVersion() < base::win::VERSION_VISTA)
    return false;
  // If composition is not enabled, we behave like on XP.
  BOOL enabled = FALSE;
  return SUCCEEDED(DwmIsCompositionEnabled(&enabled)) && enabled;
}

}  // namespace win
}  // namespace ui
