// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/app_list/cocoa/app_list_view_controller.h"

#include "base/mac/foundation_util.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/app_list_view_delegate.h"
#import "ui/app_list/cocoa/apps_grid_controller.h"

namespace {

// The roundedness of the corners of the bubble.
const CGFloat kBubbleCornerRadius = 3;

// Padding between the bottom of the pager and the bottom of the view.
const CGFloat kViewPagerOffsetY = 12;
// Padding between the bottom of the grid and the bottom of the view.
const CGFloat kViewGridOffsetY = 38;

// Minimum margin on either side of the pager. If the pager grows beyond this,
// the segment size is reduced.
const CGFloat kMinPagerMargin = 40;
// Maximum width of a single segment.
const CGFloat kMaxSegmentWidth = 80;

}  // namespace

@interface BackgroundView : NSView;
@end

@implementation BackgroundView

- (void)drawRect:(NSRect)dirtyRect {
  [NSGraphicsContext saveGraphicsState];
  [gfx::SkColorToCalibratedNSColor(app_list::kContentsBackgroundColor) set];
  [[NSBezierPath bezierPathWithRoundedRect:[self bounds]
                                   xRadius:kBubbleCornerRadius
                                   yRadius:kBubbleCornerRadius] addClip];
  NSRectFill([self bounds]);
  [NSGraphicsContext restoreGraphicsState];
}

@end

@interface AppListViewController ()

- (void)loadAndSetView;

- (void)onPagerClicked:(id)sender;

@end

@implementation AppListViewController

- (id)init {
  if ((self = [super init])) {
    appsGridController_.reset([[AppsGridController alloc] init]);
    [self loadAndSetView];

    [self totalPagesChanged];
    [self selectedPageChanged:0];
    [appsGridController_ setPaginationObserver:self];
  }
  return self;
}

- (void)dealloc {
  [appsGridController_ setPaginationObserver:nil];
  [appsGridController_ setDelegate:NULL];
  [super dealloc];
}

- (AppsGridController*)appsGridController {
  return appsGridController_;
}

- (NSSegmentedControl*)pagerControl {
  return pagerControl_;
}

- (app_list::AppListViewDelegate*)delegate {
  return delegate_.get();
}

- (void)setDelegate:(scoped_ptr<app_list::AppListViewDelegate>)newDelegate {
  delegate_.reset(newDelegate.release());
  [appsGridController_ setDelegate:delegate_.get()];
}

-(void)loadAndSetView {
  pagerControl_.reset([[NSSegmentedControl alloc] initWithFrame:NSZeroRect]);
  [pagerControl_ setSegmentStyle:NSSegmentStyleRounded];
  [pagerControl_ setTarget:self];
  [pagerControl_ setAction:@selector(onPagerClicked:)];

  [[appsGridController_ view] setFrameOrigin:NSMakePoint(0, kViewGridOffsetY)];

  NSRect backgroundRect = [[appsGridController_ view] bounds];
  backgroundRect.size.height += kViewGridOffsetY;
  scoped_nsobject<BackgroundView> backgroundView(
      [[BackgroundView alloc] initWithFrame:backgroundRect]);

  [backgroundView addSubview:pagerControl_];
  [backgroundView addSubview:[appsGridController_ view]];
  [self setView:backgroundView];
}

- (void)onPagerClicked:(id)sender {
  int selectedSegment = [sender selectedSegment];
  if (selectedSegment < 0)
    return;  // No selection.

  int pageIndex = [[sender cell] tagForSegment:selectedSegment];
  if (pageIndex >= 0)
    [appsGridController_ scrollToPage:pageIndex];
}

- (void)totalPagesChanged {
  size_t pageCount = [appsGridController_ pageCount];
  [pagerControl_ setSegmentCount:pageCount];

  NSRect viewFrame = [[self view] bounds];
  CGFloat segmentWidth = std::min(
      kMaxSegmentWidth,
      (viewFrame.size.width - 2 * kMinPagerMargin) / pageCount);

  for (size_t i = 0; i < pageCount; ++i) {
    [pagerControl_ setWidth:segmentWidth
                 forSegment:i];
    [[pagerControl_ cell] setTag:i
                      forSegment:i];
  }

  // Center in view.
  [pagerControl_ sizeToFit];
  [pagerControl_ setFrameOrigin:
      NSMakePoint(NSMidX(viewFrame) - NSMidX([pagerControl_ bounds]),
                  kViewPagerOffsetY)];
}

- (void)selectedPageChanged:(int)newSelected {
  [pagerControl_ selectSegmentWithTag:newSelected];
}

@end
