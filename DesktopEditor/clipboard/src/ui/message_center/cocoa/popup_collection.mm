// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/message_center/cocoa/popup_collection.h"

#include "ui/message_center/cocoa/popup_controller.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/message_center_observer.h"

@interface MCPopupCollection (Private)
// Returns the primary screen's visible frame rectangle.
- (NSRect)screenFrame;

// Shows a popup, if there is room on-screen, for the given notification.
// Returns YES if the notification was actually displayed.
- (BOOL)addNotification:(const message_center::Notification*)notification;

// Removes a popup from the screen and lays out new notifications that can
// now potentially fit on the screen.
- (void)removeNotification:(const std::string&)notificationID;

// Closes all the popups.
- (void)removeAllNotifications;
@end

namespace {

class PopupCollectionObserver : public message_center::MessageCenterObserver {
 public:
  PopupCollectionObserver(message_center::MessageCenter* message_center,
                          MCPopupCollection* popup_collection)
      : message_center_(message_center),
        popup_collection_(popup_collection) {
    message_center_->AddObserver(this);
  }

  virtual ~PopupCollectionObserver() {
    message_center_->RemoveObserver(this);
  }

  virtual void OnNotificationAdded(
      const std::string& notification_id) OVERRIDE {
    const auto& popups = message_center_->GetPopupNotifications();
    for (auto it = popups.begin(); it != popups.end(); ++it) {
      if ((*it)->id() == notification_id) {
        [popup_collection_ addNotification:*it];
        return;
      }
    }
  }

  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool user_id) OVERRIDE {
    [popup_collection_ removeNotification:notification_id];
  }

  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE {
    // TODO(rsesek): Refactor MCNotificationController to support updating.
  }

 private:
  message_center::MessageCenter* message_center_;  // Weak, global.

  MCPopupCollection* popup_collection_;  // Weak, owns this.
};

}  // namespace

@implementation MCPopupCollection

- (id)initWithMessageCenter:(message_center::MessageCenter*)messageCenter {
  if ((self = [super init])) {
    messageCenter_ = messageCenter;
    observer_.reset(new PopupCollectionObserver(messageCenter_, self));
    popups_.reset([[NSMutableArray alloc] init]);
  }
  return self;
}

- (void)dealloc {
  [self removeAllNotifications];
  [super dealloc];
}

// Testing API /////////////////////////////////////////////////////////////////

- (void)setScreenFrame:(NSRect)frame {
  testingScreenFrame_ = frame;
}

// Private /////////////////////////////////////////////////////////////////////

- (NSRect)screenFrame {
  if (!NSIsEmptyRect(testingScreenFrame_))
    return testingScreenFrame_;
  return [[[NSScreen screens] objectAtIndex:0] visibleFrame];
}

- (BOOL)addNotification:(const message_center::Notification*)notification {
  scoped_nsobject<MCPopupController> popup(
      [[MCPopupController alloc] initWithNotification:notification
                                        messageCenter:messageCenter_]);

  NSRect screenFrame = [self screenFrame];
  NSRect popupFrame = [[popup window] frame];

  CGFloat x = NSMaxX(screenFrame) - message_center::kMarginBetweenItems -
      NSWidth(popupFrame);
  CGFloat y = 0;

  MCPopupController* bottomPopup = [popups_ lastObject];
  if (!bottomPopup) {
    y = NSMaxY(screenFrame);
  } else {
    y = NSMinY([[bottomPopup window] frame]);
  }

  y -= message_center::kMarginBetweenItems + NSHeight(popupFrame);

  if (y > NSMinY(screenFrame)) {
    [[popup window] setFrameOrigin:NSMakePoint(x, y)];
    [popup showWindow:self];
    [popups_ addObject:popup];  // Transfer ownership.
    return YES;
  }

  return NO;
}

- (void)removeNotification:(const std::string&)notificationID {
  // The set of notification IDs that are currently displayed.
  std::set<std::string> onScreen;

  // Find the popup for the given ID.
  NSUInteger index = 0;
  MCPopupController* popup = nil;
  for (MCPopupController* candidate in popups_.get()) {
    if ([candidate notificationID] == notificationID) {
      popup = candidate;
    } else {
      onScreen.insert([candidate notificationID]);
    }

    // If the popup has not yet been found, increase the index.
    if (!popup)
      ++index;
  }

  if (!popup)
    return;

  // Calculate its height and then close it.
  CGFloat delta = NSHeight([[popup window] frame]) +
      message_center::kMarginBetweenItems;
  [popup close];
  [popups_ removeObjectAtIndex:index];

  // Shift all the popups up.
  for ( ; index < [popups_ count]; ++index) {
    NSWindow* window = [[popups_ objectAtIndex:index] window];
    NSPoint origin = [window frame].origin;
    origin.y += delta;
    [window setFrameOrigin:origin];
  }

  // Display any new popups that can now fit on screen.
  const auto& allPopups = messageCenter_->GetPopupNotifications();
  for (auto it = allPopups.begin(); it != allPopups.end(); ++it) {
    if (onScreen.find((*it)->id()) == onScreen.end()) {
      // If there's no room left on screen to display notifications, stop
      // trying.
      if (![self addNotification:*it])
        break;
    }
  }
}

- (void)removeAllNotifications {
  [popups_ makeObjectsPerformSelector:@selector(close)];
  [popups_ removeAllObjects];
}

@end
