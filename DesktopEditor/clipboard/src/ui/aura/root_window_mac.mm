// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/aura/root_window_mac.h"

#include "ui/aura/root_window_host_mac.h"

@implementation RootWindowMac

- (id)initWithContentRect:(NSRect)contentRect
                styleMask:(NSUInteger)windowStyle
                  backing:(NSBackingStoreType)bufferingType
                    defer:(BOOL)deferCreation {
  if ((self = [super initWithContentRect:contentRect
                               styleMask:windowStyle
                                 backing:bufferingType
                                   defer:deferCreation])) {
    hostDelegate_ = NULL;
  }
  return self;
}

- (void)setHostDelegate:(aura::RootWindowHostMacDelegate*)hostDelegate {
  hostDelegate_ = hostDelegate;
}

- (void)sendEvent:(NSEvent*)event {
  // Allow both the Cocoa machinery and the Aura machinery to handle the event.
  [super sendEvent:event];
  if (hostDelegate_)
    hostDelegate_->SendEvent(event);
}

@end
