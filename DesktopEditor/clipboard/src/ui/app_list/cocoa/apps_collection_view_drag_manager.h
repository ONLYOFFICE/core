// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_COLLECTION_VIEW_DRAG_MANAGER_H_
#define UI_APP_LIST_COCOA_APPS_COLLECTION_VIEW_DRAG_MANAGER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "ui/app_list/app_list_export.h"

@class AppsGridController;
@class ItemDragController;

// Manager for the state associated with dragging an NSCollectionViewItem in the
// AppsGridController. It is also a factory for the NSCollectionView pages in
// the grid, allowing items to be dragged between pages.
APP_LIST_EXPORT
@interface AppsCollectionViewDragManager : NSObject {
 @private
  scoped_nsobject<ItemDragController> itemDragController_;
  AppsGridController* gridController_;  // Weak. Owns us.

  NSSize cellSize_;
  size_t rows_;
  size_t columns_;

  // Index of the last known position of the item currently being dragged.
  size_t itemDragIndex_;

  // Model index of the item being dragged, or NSNotFound if nothing was hit on
  // the last mouseDown.
  size_t itemHitIndex_;

  // Location in the window of the last mouseDown event.
  NSPoint mouseDownLocation_;

  // Whether the current mouse action has converted into an item drag.
  BOOL dragging_;
}

- (id)initWithCellSize:(NSSize)cellSize
                  rows:(size_t)rows
               columns:(size_t)columns
        gridController:(AppsGridController*)gridController;

// Make an empty NSCollectionView with draggable items in the given |pageFrame|.
- (NSCollectionView*)makePageWithFrame:(NSRect)pageFrame;

- (void)cancelDrag;

@end

@interface AppsCollectionViewDragManager (TestingAPI)

- (void)onMouseDownInPage:(NSCollectionView*)page
                withEvent:(NSEvent*)theEvent;
- (void)onMouseDragged:(NSEvent*)theEvent;
- (void)onMouseUp:(NSEvent*)theEvent;

@end

#endif  // UI_APP_LIST_COCOA_APPS_COLLECTION_VIEW_DRAG_MANAGER_H_
