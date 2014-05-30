// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_ITEM_DRAG_WINDOW_CONTROLLER_H_
#define UI_APP_LIST_COCOA_ITEM_DRAG_WINDOW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "base/memory/scoped_nsobject.h"

@class AppsGridViewItem;

// Controller to manage the animations and transient views that are used when
// dragging an app list item around the app list grid. When initiated, the item
// image (only) is grown in an animation, and sticks to the mouse cursor. When
// released, the label is added to the image and it shrinks and moves to the
// item location in the grid.
@interface ItemDragController : NSViewController {
 @private
  scoped_nsobject<CALayer> dragLayer_;
  scoped_nsobject<NSButton> buttonToRestore_;
  NSPoint mouseOffset_;
  NSTimeInterval growStart_;
  BOOL shrinking_;
}

- (id)initWithGridCellSize:(NSSize)size;

- (void)initiate:(AppsGridViewItem*)item
    mouseDownLocation:(NSPoint)mouseDownLocation
      currentLocation:(NSPoint)currentLocation
            timestamp:(NSTimeInterval)eventTimestamp;

- (void)update:(NSPoint)currentLocation
     timestamp:(NSTimeInterval)eventTimestamp;

- (void)complete:(AppsGridViewItem*)item
    targetOrigin:(NSPoint)targetOrigin;

@end

#endif  // UI_APP_LIST_COCOA_ITEM_DRAG_WINDOW_CONTROLLER_H_
