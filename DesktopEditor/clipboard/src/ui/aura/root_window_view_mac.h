// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_VIEW_MAC_H_
#define UI_AURA_ROOT_WINDOW_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>

#include "ui/compositor/compositor.h"

// RootWindowView provides an NSView class that delegates drawing to a
// ui::Compositor delegate, setting up the NSOpenGLContext as required.
@interface RootWindowView : NSView {
 @private
  ui::Compositor* compositor_;
}
-(void)setCompositor:(ui::Compositor*)compositor;
@end

#endif  // UI_AURA_ROOT_WINDOW_VIEW_MAC_H_
