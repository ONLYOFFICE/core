// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/app_list/cocoa/apps_collection_view_drag_manager.h"

#include "base/logging.h"
#include "base/mac/foundation_util.h"
#import "ui/app_list/cocoa/apps_grid_controller.h"
#import "ui/app_list/cocoa/apps_grid_view_item.h"
#import "ui/app_list/cocoa/item_drag_controller.h"

namespace {

// Distance cursor must travel in either x or y direction to start a drag.
const CGFloat kDragThreshold = 5;

}  // namespace

@interface AppsCollectionViewDragManager ()

// Returns the item index that |theEvent| would hit in the page at |pageIndex|
// or NSNotFound if no item is hit.
- (size_t)itemIndexForPage:(size_t)pageIndex
              hitWithEvent:(NSEvent*)theEvent;

- (void)initiateDrag:(NSEvent*)theEvent;
- (void)updateDrag:(NSEvent*)theEvent;
- (void)completeDrag;

@end

// An NSCollectionView that forwards mouse events to the factory they share.
@interface GridCollectionView : NSCollectionView {
 @private
  AppsCollectionViewDragManager* factory_;
}

@property(assign, nonatomic) AppsCollectionViewDragManager* factory;

@end

@implementation AppsCollectionViewDragManager

- (id)initWithCellSize:(NSSize)cellSize
                  rows:(size_t)rows
               columns:(size_t)columns
        gridController:(AppsGridController*)gridController {
  if ((self = [super init])) {
    cellSize_ = cellSize;
    rows_ = rows;
    columns_ = columns;
    gridController_ = gridController;
  }
  return self;
}

- (NSCollectionView*)makePageWithFrame:(NSRect)pageFrame {
  scoped_nsobject<GridCollectionView> itemCollectionView(
      [[GridCollectionView alloc] initWithFrame:pageFrame]);
  [itemCollectionView setFactory:self];
  [itemCollectionView setMaxNumberOfRows:rows_];
  [itemCollectionView setMinItemSize:cellSize_];
  [itemCollectionView setMaxItemSize:cellSize_];
  [itemCollectionView setSelectable:YES];
  [itemCollectionView setFocusRingType:NSFocusRingTypeNone];
  [itemCollectionView setBackgroundColors:
      [NSArray arrayWithObject:[NSColor clearColor]]];

  scoped_nsobject<AppsGridViewItem> itemPrototype(
      [[AppsGridViewItem alloc] initWithSize:cellSize_]);
  [[itemPrototype button] setTarget:gridController_];
  [[itemPrototype button] setAction:@selector(onItemClicked:)];

  [itemCollectionView setItemPrototype:itemPrototype];
  return itemCollectionView.autorelease();
}

- (void)onMouseDownInPage:(NSCollectionView*)page
                withEvent:(NSEvent*)theEvent {
  size_t pageIndex = [gridController_ pageIndexForCollectionView:page];
  itemHitIndex_ = [self itemIndexForPage:pageIndex
                            hitWithEvent:theEvent];
  if (itemHitIndex_ == NSNotFound)
    return;

  mouseDownLocation_ = [theEvent locationInWindow];
  [[[gridController_ itemAtIndex:itemHitIndex_] view] mouseDown:theEvent];
}

- (void)onMouseDragged:(NSEvent*)theEvent {
  if (itemHitIndex_ == NSNotFound)
    return;

  if (dragging_) {
    [self updateDrag:theEvent];
    return;
  }

  NSPoint mouseLocation = [theEvent locationInWindow];
  CGFloat deltaX = mouseLocation.x - mouseDownLocation_.x;
  CGFloat deltaY = mouseLocation.y - mouseDownLocation_.y;
  if (deltaX * deltaX + deltaY * deltaY > kDragThreshold * kDragThreshold) {
    [self initiateDrag:theEvent];
    return;
  }

  [[[gridController_ itemAtIndex:itemHitIndex_] view] mouseDragged:theEvent];
}

- (void)onMouseUp:(NSEvent*)theEvent {
  if (itemHitIndex_ == NSNotFound)
    return;

  if (dragging_) {
    [self completeDrag];
    return;
  }

  [[[gridController_ itemAtIndex:itemHitIndex_] view] mouseUp:theEvent];
}

- (size_t)itemIndexForPage:(size_t)pageIndex
              hitWithEvent:(NSEvent*)theEvent {
  NSCollectionView* page =
      [gridController_ collectionViewAtPageIndex:pageIndex];
  NSPoint pointInView = [page convertPoint:[theEvent locationInWindow]
                                  fromView:nil];

  const size_t itemsInPage = [[page content] count];
  for (size_t indexInPage = 0; indexInPage < itemsInPage; ++indexInPage) {
    if ([page mouse:pointInView
             inRect:[page frameForItemAtIndex:indexInPage]]) {
      return indexInPage + pageIndex * rows_ * columns_;
    }
  }

  return NSNotFound;
}

- (void)initiateDrag:(NSEvent*)theEvent {
  DCHECK_LT(itemHitIndex_, [gridController_ itemCount]);
  dragging_ = YES;

  if (!itemDragController_) {
    itemDragController_.reset(
        [[ItemDragController alloc] initWithGridCellSize:cellSize_]);
    [[gridController_ view] addSubview:[itemDragController_ view]];
  }

  [itemDragController_ initiate:[gridController_ itemAtIndex:itemHitIndex_]
              mouseDownLocation:mouseDownLocation_
                currentLocation:[theEvent locationInWindow]
                      timestamp:[theEvent timestamp]];

  itemDragIndex_ = itemHitIndex_;
  [self updateDrag:theEvent];
}

- (void)updateDrag:(NSEvent*)theEvent {
  [itemDragController_ update:[theEvent locationInWindow]
                    timestamp:[theEvent timestamp]];
  size_t visiblePage = [gridController_ visiblePage];
  size_t itemIndexOver = [self itemIndexForPage:visiblePage
                                   hitWithEvent:theEvent];
  DCHECK_NE(0u, [gridController_ itemCount]);
  if (itemIndexOver == NSNotFound) {
    if (visiblePage != [gridController_ pageCount] - 1)
      return;

    // If nothing was hit, but the last page is shown, move to the end.
    itemIndexOver = [gridController_ itemCount] - 1;
  }

  if (itemDragIndex_ == itemIndexOver)
    return;

  [gridController_ moveItemForDrag:itemDragIndex_
                       toItemIndex:itemIndexOver];
  itemDragIndex_ = itemIndexOver;
}

- (void)cancelDrag {
  if (!dragging_)
    return;

  [gridController_ moveItemForDrag:itemDragIndex_
                       toItemIndex:itemHitIndex_];
  itemDragIndex_ = itemHitIndex_;
  [self completeDrag];
  itemHitIndex_ = NSNotFound;  // Ignore future mouse events for this drag.
}

- (void)completeDrag {
  DCHECK_GE(itemDragIndex_, 0u);
  AppsGridViewItem* item = [gridController_ itemAtIndex:itemDragIndex_];

  // The item could still be animating in the NSCollectionView, so ask it where
  // it would place the item.
  NSCollectionView* pageView = base::mac::ObjCCastStrict<NSCollectionView>(
      [[item view] superview]);
  size_t indexInPage = itemDragIndex_ % (rows_ * columns_);
  NSPoint targetOrigin = [[gridController_ view]
      convertPoint:[pageView frameForItemAtIndex:indexInPage].origin
          fromView:pageView];

  [itemDragController_ complete:item
                   targetOrigin:targetOrigin];
  [gridController_ moveItemWithIndex:itemHitIndex_
                        toModelIndex:itemDragIndex_];

  dragging_ = NO;
}

@end

@implementation GridCollectionView

@synthesize factory = factory_;

- (void)mouseDown:(NSEvent*)theEvent {
  [factory_ onMouseDownInPage:self
                    withEvent:theEvent];
}

- (void)mouseDragged:(NSEvent*)theEvent {
  [factory_ onMouseDragged:theEvent];
}

- (void)mouseUp:(NSEvent*)theEvent {
  [factory_ onMouseUp:theEvent];
}

@end
