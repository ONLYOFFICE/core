// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_SCROLL_VIEW_WITH_NO_SCROLLBARS_H_
#define UI_APP_LIST_COCOA_SCROLL_VIEW_WITH_NO_SCROLLBARS_H_

#include <Cocoa/Cocoa.h>

// Delegate to notify when a user interaction to scroll completes.
@protocol GestureScrollDelegate

// Called when a scroll gesture is observed, or when it completes.
- (void)userScrolling:(BOOL)isScrolling;

@end

// NSScrollView has a quirk when created programatically that causes gesture
// scrolling to fail if it does not have a scroll bar. This provides a scroll
// view using custom scrollers that are not visible.
@interface ScrollViewWithNoScrollbars : NSScrollView {
 @private
  id<GestureScrollDelegate> delegate_;
}

@property(assign, nonatomic) id<GestureScrollDelegate> delegate;

@end

#endif  // UI_APP_LIST_COCOA_SCROLL_VIEW_WITH_NO_SCROLLBARS_H_
