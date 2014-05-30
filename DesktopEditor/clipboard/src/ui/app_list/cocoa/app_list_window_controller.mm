// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/app_list/cocoa/app_list_window_controller.h"

#include "ui/app_list/app_list_view_delegate.h"
#import "ui/app_list/cocoa/app_list_view_controller.h"
#import "ui/app_list/cocoa/apps_grid_controller.h"
#include "ui/base/cocoa/window_size_constants.h"

@interface AppListWindow : NSWindow;
@end

@implementation AppListWindow

// If we initialize a window with NSBorderlessWindowMask, it will not accept key
// events (among other things) unless canBecomeKeyWindow is overridden.
- (BOOL)canBecomeKeyWindow {
  return YES;
}

@end

@implementation AppListWindowController;

- (id)init {
  scoped_nsobject<NSWindow> controlledWindow(
      [[AppListWindow alloc] initWithContentRect:ui::kWindowSizeDeterminedLater
                                       styleMask:NSBorderlessWindowMask
                                         backing:NSBackingStoreBuffered
                                           defer:NO]);
  [controlledWindow setReleasedWhenClosed:NO];
  [controlledWindow setBackgroundColor:[NSColor clearColor]];
  [controlledWindow setOpaque:NO];
  [controlledWindow setHasShadow:NO];

  if ((self = [super initWithWindow:controlledWindow])) {
    appListViewController_.reset([[AppListViewController alloc] init]);
    [[self window] setFrame:[[appListViewController_ view] bounds]
                    display:NO];
    [[self window] setContentView:[appListViewController_ view]];
    [[self window] setDelegate:self];
    [[self window] makeFirstResponder:
        [[appListViewController_ appsGridController]
            collectionViewAtPageIndex:0]];
  }
  return self;
}

- (AppListViewController*)appListViewController {
  return appListViewController_;
}

- (void)doCommandBySelector:(SEL)command {
  if (command == @selector(cancel:)) {
    if ([appListViewController_ delegate])
      [appListViewController_ delegate]->Dismiss();
  } else if (command == @selector(insertNewline:) ||
             command == @selector(insertLineBreak:)) {
    [[appListViewController_ appsGridController] activateSelection];
  }
}

@end
