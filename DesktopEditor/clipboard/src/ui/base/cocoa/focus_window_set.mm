// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "ui/base/cocoa/focus_window_set.h"

namespace ui {

void FocusWindowSet(std::set<NSWindow*> windows) {
  NSArray* ordered_windows = [NSApp orderedWindows];
  [NSApp activateIgnoringOtherApps:YES];
  NSWindow* frontmost_window = nil;
  for (int i = [ordered_windows count] - 1; i >= 0; i--) {
    NSWindow* win = [ordered_windows objectAtIndex:i];
    if (windows.find(win) != windows.end()) {
      [win orderFront:nil];
      frontmost_window = win;
    }
  }
  [frontmost_window makeMainWindow];
  [frontmost_window makeKeyWindow];
}

}  // namespace ui
