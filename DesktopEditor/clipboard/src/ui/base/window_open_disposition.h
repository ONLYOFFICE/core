// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WINDOW_OPEN_DISPOSITION_H_
#define UI_BASE_WINDOW_OPEN_DISPOSITION_H_

#include "ui/base/ui_export.h"

enum WindowOpenDisposition {
  UNKNOWN,
  SUPPRESS_OPEN,
  CURRENT_TAB,
  // Indicates that only one tab with the url should exist in the same window.
  SINGLETON_TAB,
  NEW_FOREGROUND_TAB,
  NEW_BACKGROUND_TAB,
  NEW_POPUP,
  NEW_WINDOW,
  SAVE_TO_DISK,
  OFF_THE_RECORD,
  IGNORE_ACTION
};

namespace ui {

// Translates event flags from a click on a link into the user's desired
// window disposition.  For example, a middle click would mean to open
// a background tab.
UI_EXPORT WindowOpenDisposition DispositionFromClick(bool middle_button,
                                                     bool alt_key,
                                                     bool ctrl_key,
                                                     bool meta_key,
                                                     bool shift_key);

// Translates event flags into what kind of disposition they represents.
// For example, a middle click would mean to open a background tab.
// event_flags are the flags as understood by ui::MouseEvent.
UI_EXPORT WindowOpenDisposition DispositionFromEventFlags(int event_flags);

}  // namespace ui

#endif  // UI_BASE_WINDOW_OPEN_DISPOSITION_H_
