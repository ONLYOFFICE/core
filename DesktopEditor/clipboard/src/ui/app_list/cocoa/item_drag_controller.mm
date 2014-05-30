// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/app_list/cocoa/item_drag_controller.h"

#include "base/logging.h"
#import "ui/app_list/cocoa/apps_grid_view_item.h"
#include "ui/base/cocoa/window_size_constants.h"

namespace {

// Scale to transform the grid cell when a drag starts. Note that 1.5 ensures
// that integers are used for the layer bounds when the grid cell dimensions
// are even.
const CGFloat kDraggingIconScale = 1.5;

const NSTimeInterval kAnimationDuration = 0.2;
NSString* const kGrowAnimationKey = @"growAnimation";

}  // namespace

@interface ItemDragController ()

- (void)animateTransformFrom:(CATransform3D)fromValue
                 useDelegate:(BOOL)useDelegate;

- (void)clearAnimations;

@end

@implementation ItemDragController

- (id)initWithGridCellSize:(NSSize)size {
  if ((self = [super init])) {
    NSRect frameRect = NSMakeRect(0,
                                  0,
                                  size.width * kDraggingIconScale,
                                  size.height * kDraggingIconScale);
    scoped_nsobject<NSView> dragView([[NSView alloc] initWithFrame:frameRect]);
    [dragView setWantsLayer:YES];
    [dragView setHidden:YES];

    dragLayer_.reset([[CALayer layer] retain]);
    [dragLayer_ setFrame:NSRectToCGRect(frameRect)];
    [[dragView layer] addSublayer:dragLayer_];

    [self setView:dragView];
  }
  return self;
}

- (void)initiate:(AppsGridViewItem*)item
    mouseDownLocation:(NSPoint)mouseDownLocation
      currentLocation:(NSPoint)currentLocation
            timestamp:(NSTimeInterval)eventTimestamp {
  [self clearAnimations];
  [item setSelected:NO];
  NSView* itemView = [item view];
  NSPoint pointInGridCell = [itemView convertPoint:mouseDownLocation
                                          fromView:nil];
  mouseOffset_ = NSMakePoint(pointInGridCell.x - NSMidX([itemView bounds]),
                             NSMidY([itemView bounds]) - pointInGridCell.y);

  // Take a snapshot of the grid cell without the text label and hide the cell.
  // Also remove the cell highlight on the image, added when it was clicked.
  NSButton* button = [item button];
  scoped_nsobject<NSString> oldTitle([[button title] retain]);
  [button setTitle:[NSString string]];
  [[button cell] setHighlighted:NO];
  NSBitmapImageRep* imageRep =
      [itemView bitmapImageRepForCachingDisplayInRect:[itemView visibleRect]];
  [itemView cacheDisplayInRect:[itemView visibleRect]
              toBitmapImageRep:imageRep];
  [button setHidden:YES];
  [button setTitle:oldTitle];

  [dragLayer_ setContents:reinterpret_cast<id>([imageRep CGImage])];
  [dragLayer_ setTransform:CATransform3DIdentity];

  // Add a grow animation to the layer.
  CATransform3D growFrom = CATransform3DScale(CATransform3DIdentity,
                                              1.0 / kDraggingIconScale,
                                              1.0 / kDraggingIconScale,
                                              1.0);
  [self animateTransformFrom:growFrom
                 useDelegate:NO];

  growStart_ = eventTimestamp;
  [[self view] setHidden:NO];
}

- (void)update:(NSPoint)currentLocation
     timestamp:(NSTimeInterval)eventTimestamp {
  NSPoint pointInSuperview = [[[self view] superview]
      convertPoint:currentLocation
          fromView:nil];
  NSRect rect = [[self view] bounds];
  NSPoint anchor = NSMakePoint(NSMidX(rect), NSMidY(rect));

  // If the grow animation is still in progress, make the point of the image
  // that was clicked appear stuck to the mouse cursor.
  CGFloat progress = (eventTimestamp - growStart_) / kAnimationDuration;
  CGFloat currentIconScale = progress < 1.0 ?
      1.0 + (kDraggingIconScale - 1.0) * progress :
      kDraggingIconScale;

  pointInSuperview.x -= (mouseOffset_.x * currentIconScale + anchor.x);
  pointInSuperview.y -= (mouseOffset_.y * currentIconScale + anchor.y);
  [[self view] setFrameOrigin:pointInSuperview];
}

- (void)complete:(AppsGridViewItem*)item
    targetOrigin:(NSPoint)targetOrigin {
  [self clearAnimations];

  NSView* itemView = [item view];

  // Take another snapshot of the grid cell, after restoring the label.
  NSButton* button = [item button];
  [button setHidden:NO];
  NSBitmapImageRep* imageRep =
      [itemView bitmapImageRepForCachingDisplayInRect:[itemView visibleRect]];
  [itemView cacheDisplayInRect:[itemView visibleRect]
              toBitmapImageRep:imageRep];
  [button setHidden:YES];

  [dragLayer_ setContents:reinterpret_cast<id>([imageRep CGImage])];
  [dragLayer_ setTransform:CATransform3DScale(CATransform3DIdentity,
                                              1.0 / kDraggingIconScale,
                                              1.0 / kDraggingIconScale,
                                              1.0)];

  // Retain the button so it can be unhidden when the animation completes. Note
  // that the |item| and corresponding button can differ from the |item| passed
  // to initiate(), if it moved to a new page during the drag. At this point the
  // destination page is known, so retain the button.
  buttonToRestore_.reset([button retain]);

  // Add the shrink animation for the layer.
  [self animateTransformFrom:CATransform3DIdentity
                 useDelegate:YES];
  shrinking_ = YES;

  // Also animate the translation, on the view.
  // TODO(tapted): This should be merged into the scale transform, instead of
  // using a separate NSViewAnimation.
  NSRect startRect = [[self view] frame];

  // The final position needs to be adjusted since it shrinks from each side.
  NSRect targetRect = NSMakeRect(
      targetOrigin.x - NSMidX([itemView bounds]) * (kDraggingIconScale - 1),
      targetOrigin.y - NSMidY([itemView bounds]) * (kDraggingIconScale - 1),
      startRect.size.width,
      startRect.size.height);

  NSDictionary* animationDict = @{
      NSViewAnimationTargetKey:     [self view],
      NSViewAnimationStartFrameKey: [NSValue valueWithRect:startRect],
      NSViewAnimationEndFrameKey:   [NSValue valueWithRect:targetRect]
  };

  scoped_nsobject<NSViewAnimation> translate([[NSViewAnimation alloc]
      initWithViewAnimations:[NSArray arrayWithObject:animationDict]]);
  [translate setDuration:kAnimationDuration];
  [translate startAnimation];
}

- (void)animateTransformFrom:(CATransform3D)fromValue
                 useDelegate:(BOOL)useDelegate {
  CABasicAnimation* animation =
      [CABasicAnimation animationWithKeyPath:@"transform"];
  [animation setFromValue:[NSValue valueWithCATransform3D:fromValue]];
  if (useDelegate)
    [animation setDelegate:self];

  [animation setDuration:kAnimationDuration];
  [CATransaction begin];
  [CATransaction setValue:[NSNumber numberWithFloat:kAnimationDuration]
                   forKey:kCATransactionAnimationDuration];
  [dragLayer_ addAnimation:animation
                    forKey:@"transform"];
  [CATransaction commit];
}

- (void)clearAnimations {
  [dragLayer_ removeAllAnimations];
  if (!shrinking_)
    return;

  DCHECK(buttonToRestore_);
  [buttonToRestore_ setHidden:NO];
  buttonToRestore_.reset();
  shrinking_ = NO;
}

- (void)animationDidStop:(CAAnimation*)anim
                finished:(BOOL)finished {
  if (!finished)
    return;

  DCHECK(shrinking_);
  [self clearAnimations];
  [dragLayer_ setContents:nil];
  [[self view] setHidden:YES];
}

@end
