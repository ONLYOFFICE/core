// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_MAC_H_
#define UI_AURA_ROOT_WINDOW_MAC_H_

#import <Cocoa/Cocoa.h>

namespace aura {
class RootWindowHostMacDelegate;
}  // aura

// RootWindow routes NSWindow events back to the RootWindowHost for dispatch
// to the Aura event handling system.
@interface RootWindowMac : NSWindow {
 @private
  // Weak.  May be NULL.  The host delegate acts as a conduit for event routing
  // back to the host.
  aura::RootWindowHostMacDelegate* hostDelegate_;
}

// Designated initializer.
- (id)initWithContentRect:(NSRect)contentRect
                styleMask:(NSUInteger)windowStyle
                  backing:(NSBackingStoreType)bufferingType
                    defer:(BOOL)deferCreation;

// Sets the |hostDelegate_|
- (void)setHostDelegate:(aura::RootWindowHostMacDelegate*)hostDelegate;

// Overrides main event dispatch to route NSWindow events to host delegate.
- (void)sendEvent:(NSEvent*)event;

@end

#endif  // UI_AURA_ROOT_WINDOW_MAC_H_
