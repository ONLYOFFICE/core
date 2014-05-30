// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_SHELL_H_
#define UI_BASE_WIN_SHELL_H_

#include <windows.h>

#include "base/string16.h"
#include "ui/base/ui_export.h"

namespace base {
class FilePath;
}

namespace ui {
namespace win {
// Open or run a file via the Windows shell. In the event that there is no
// default application registered for the file specified by 'full_path',
// ask the user, via the Windows "Open With" dialog.
// Returns 'true' on successful open, 'false' otherwise.
UI_EXPORT bool OpenItemViaShell(const base::FilePath& full_path);

// The download manager now writes the alternate data stream with the
// zone on all downloads. This function is equivalent to OpenItemViaShell
// without showing the zone warning dialog.
UI_EXPORT bool OpenItemViaShellNoZoneCheck(const base::FilePath& full_path);

// Lower level function that allows opening of non-files like urls or GUIDs
// don't use it if one of the above will do. |mask| is a valid combination
// of SEE_MASK_FLAG_XXX as stated in msdn. If there is no default application
// registered for the item, it behaves the same as OpenItemViaShell.
UI_EXPORT bool OpenAnyViaShell(const string16& full_path,
                               const string16& directory,
                               const string16& args,
                               DWORD mask);

// Ask the user, via the Windows "Open With" dialog, for an application to use
// to open the file specified by 'full_path'.
// Returns 'true' on successful open, 'false' otherwise.
bool OpenItemWithExternalApp(const string16& full_path);

// Sets the application id given as the Application Model ID for the window
// specified.  This method is used to insure that different web applications
// do not group together on the Win7 task bar.
UI_EXPORT void SetAppIdForWindow(const string16& app_id, HWND hwnd);

// Sets the application icon for the window specified.
UI_EXPORT void SetAppIconForWindow(const string16& app_icon, HWND hwnd);

// Sets the relaunch command and relaunch display name for the window specified.
// Windows will use this information for grouping on the taskbar, and to create
// a shortcut if the window is pinned to the taskbar.
UI_EXPORT void SetRelaunchDetailsForWindow(const string16& relaunch_command,
                                           const string16& display_name,
                                           HWND hwnd);

// Returns true if composition is available and turned on on the current
// platform.
UI_EXPORT bool IsAeroGlassEnabled();

}  // namespace win
}  // namespace ui

#endif  // UI_BASE_WIN_SHELL_H_
