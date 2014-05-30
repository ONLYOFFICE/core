// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/app_list/cocoa/apps_grid_view_item.h"

#include "base/mac/foundation_util.h"
#include "base/memory/scoped_nsobject.h"
#include "base/strings/sys_string_conversions.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/app_list_item_model.h"
#include "ui/app_list/app_list_item_model_observer.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia_util_mac.h"
#include "ui/gfx/font.h"

namespace {

// Padding from the top of the tile to the top of the app icon.
const CGFloat kTileTopPadding = 10;

}  // namespace

namespace app_list {

class ItemModelObserverBridge : public app_list::AppListItemModelObserver {
 public:
  ItemModelObserverBridge(AppsGridViewItem* parent, AppListItemModel* model);
  virtual ~ItemModelObserverBridge();

  AppListItemModel* model() { return model_; }

  virtual void ItemIconChanged() OVERRIDE;
  virtual void ItemTitleChanged() OVERRIDE;
  virtual void ItemHighlightedChanged() OVERRIDE;
  virtual void ItemIsInstallingChanged() OVERRIDE;
  virtual void ItemPercentDownloadedChanged() OVERRIDE;

 private:
  AppsGridViewItem* parent_;  // Weak. Owns us.
  AppListItemModel* model_;  // Weak. Owned by AppListModel::Apps.

  DISALLOW_COPY_AND_ASSIGN(ItemModelObserverBridge);
};

ItemModelObserverBridge::ItemModelObserverBridge(AppsGridViewItem* parent,
                                                 AppListItemModel* model)
    : parent_(parent),
      model_(model) {
  model_->AddObserver(this);
}

ItemModelObserverBridge::~ItemModelObserverBridge() {
  model_->RemoveObserver(this);
}

void ItemModelObserverBridge::ItemIconChanged() {
  [[parent_ button] setImage:gfx::NSImageFromImageSkia(model_->icon())];
}

void ItemModelObserverBridge::ItemTitleChanged() {
  [[parent_ button] setTitle:base::SysUTF8ToNSString(model_->title())];
}

void ItemModelObserverBridge::ItemHighlightedChanged() {
  //TODO(tapted): Ensure the item view is visible (requires pagination).
}

void ItemModelObserverBridge::ItemIsInstallingChanged() {
  //TODO(tapted): Hide the title while itemModel->is_installing().
}

void ItemModelObserverBridge::ItemPercentDownloadedChanged() {
  //TODO(tapted): Update install progress bar for this item.
}

}  // namespace app_list

// Container for an NSButton to allow proper alignment of the icon in the apps
// grid, and to draw with a highlight when selected.
@interface AppsGridItemBackgroundView : NSView {
 @private
  BOOL selected_;
}

- (NSButton*)button;

- (void)setSelected:(BOOL)flag;

@end

@implementation AppsGridItemBackgroundView

- (NSButton*)button {
  return base::mac::ObjCCastStrict<NSButton>([[self subviews] objectAtIndex:0]);
}

- (void)setSelected:(BOOL)flag {
  if (selected_ == flag)
    return;

  selected_ = flag;
  [self setNeedsDisplay:YES];
}

// Ignore all hit tests. The grid controller needs to be the owner of any drags.
- (NSView*)hitTest:(NSPoint)aPoint {
  return nil;
}

- (void)drawRect:(NSRect)dirtyRect {
  if (!selected_)
    return;

  [gfx::SkColorToCalibratedNSColor(app_list::kHoverAndPushedColor) set];
  NSRectFillUsingOperation(dirtyRect, NSCompositeSourceOver);
}

- (void)mouseDown:(NSEvent*)theEvent {
  [[[self button] cell] setHighlighted:YES];
}

- (void)mouseDragged:(NSEvent*)theEvent {
  NSPoint pointInView = [self convertPoint:[theEvent locationInWindow]
                                  fromView:nil];
  BOOL isInView = [self mouse:pointInView inRect:[self bounds]];
  [[[self button] cell] setHighlighted:isInView];
}

- (void)mouseUp:(NSEvent*)theEvent {
  NSPoint pointInView = [self convertPoint:[theEvent locationInWindow]
                                  fromView:nil];
  if (![self mouse:pointInView inRect:[self bounds]])
    return;

  [[self button] performClick:self];
}

@end

@interface AppsGridViewItem ()

- (AppsGridItemBackgroundView*)itemBackgroundView;

@end

@implementation AppsGridViewItem

- (id)initWithSize:(NSSize)tileSize {
  if ((self = [super init])) {
    scoped_nsobject<NSButton> prototypeButton(
        [[NSButton alloc] initWithFrame:NSMakeRect(
            0, 0, tileSize.width, tileSize.height - kTileTopPadding)]);

    // This NSButton style always positions the icon at the very top of the
    // button frame. AppsGridViewItem uses an enclosing view so that it is
    // visually correct.
    [prototypeButton setImagePosition:NSImageAbove];
    [prototypeButton setButtonType:NSMomentaryChangeButton];
    [prototypeButton setBordered:NO];

    [[prototypeButton cell]
        setFont:ui::ResourceBundle::GetSharedInstance().GetFont(
            app_list::kItemTextFontStyle).GetNativeFont()];

    scoped_nsobject<AppsGridItemBackgroundView> prototypeButtonBackground(
        [[AppsGridItemBackgroundView alloc] initWithFrame:NSMakeRect(
            0, 0, tileSize.width, tileSize.height)]);
    [prototypeButtonBackground addSubview:prototypeButton];
    [self setView:prototypeButtonBackground];
  }
  return self;
}

- (void)setModel:(app_list::AppListItemModel*)itemModel {
  if (!itemModel) {
    observerBridge_.reset();
    return;
  }

  NSButton* button = [self button];
  [button setTitle:base::SysUTF8ToNSString(itemModel->title())];
  [button setImage:gfx::NSImageFromImageSkia(itemModel->icon())];
  observerBridge_.reset(new app_list::ItemModelObserverBridge(self, itemModel));

  if (trackingArea_.get())
    [[self view] removeTrackingArea:trackingArea_.get()];

  trackingArea_.reset(
      [[CrTrackingArea alloc] initWithRect:NSZeroRect
                                   options:NSTrackingInVisibleRect |
                                           NSTrackingMouseEnteredAndExited |
                                           NSTrackingActiveInKeyWindow
                                     owner:self
                                  userInfo:nil]);
  [[self view] addTrackingArea:trackingArea_.get()];
}

- (app_list::AppListItemModel*)model {
  return observerBridge_->model();
}

- (NSButton*)button {
  DCHECK_EQ(1u, [[[self view] subviews] count]);
  return base::mac::ObjCCastStrict<NSButton>(
      [[[self view] subviews] objectAtIndex:0]);
}

- (AppsGridItemBackgroundView*)itemBackgroundView {
  return base::mac::ObjCCastStrict<AppsGridItemBackgroundView>([self view]);
}

- (void)mouseEntered:(NSEvent*)theEvent {
  [self setSelected:YES];
}

- (void)mouseExited:(NSEvent*)theEvent {
  [self setSelected:NO];
}

- (void)setSelected:(BOOL)flag {
  [[self itemBackgroundView] setSelected:flag];
  [super setSelected:flag];
}

@end
