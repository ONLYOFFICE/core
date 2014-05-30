// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/message_center/cocoa/popup_controller.h"

#import "ui/base/cocoa/window_size_constants.h"
#import "ui/message_center/cocoa/notification_controller.h"
#include "ui/message_center/message_center.h"

#if !defined(MAC_OS_X_VERSION_10_7) || \
    MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_7
enum {
  NSWindowCollectionBehaviorFullScreenAuxiliary = 1 << 8
};
#endif  // MAC_OS_X_VERSION_10_7

@implementation MCPopupController

- (id)initWithNotification:(const message_center::Notification*)notification
    messageCenter:(message_center::MessageCenter*)messageCenter {
  scoped_nsobject<NSWindow> window(
      [[NSWindow alloc] initWithContentRect:ui::kWindowSizeDeterminedLater
                                  styleMask:NSBorderlessWindowMask
                                    backing:NSBackingStoreBuffered
                                      defer:YES]);
  if ((self = [super initWithWindow:window])) {
    notificationController_.reset(
        [[MCNotificationController alloc] initWithNotification:notification
                                                 messageCenter:messageCenter]);
    [window setReleasedWhenClosed:NO];

    [window setLevel:NSFloatingWindowLevel];
    [window setExcludedFromWindowsMenu:YES];
    [window setCollectionBehavior:
        NSWindowCollectionBehaviorIgnoresCycle |
        NSWindowCollectionBehaviorFullScreenAuxiliary];

    [window setHasShadow:YES];
    [window setFrame:[[notificationController_ view] frame] display:NO];
    [window setContentView:[notificationController_ view]];
  }
  return self;
}

- (const message_center::Notification*)notification {
  return [notificationController_ notification];
}

- (const std::string&)notificationID {
  return [notificationController_ notificationID];
}

@end
