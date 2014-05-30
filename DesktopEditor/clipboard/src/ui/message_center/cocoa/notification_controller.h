// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_NOTIFICATION_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_NOTIFICATION_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include <string>

#import "base/memory/scoped_nsobject.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class Notification;
}

@class HoverImageButton;

// The base view controller class for notifications. A notification at minimum
// has an image, title, body, and close button. This controller can be used as
// the content for both a popup bubble and a view in the notification tray.
MESSAGE_CENTER_EXPORT
@interface MCNotificationController : NSViewController {
 @protected
  // The message object. Weak.
  const message_center::Notification* notification_;

  // A copy of the notification ID.
  std::string notificationID_;

  // Controller of the notifications, where action messages are forwarded. Weak.
  message_center::MessageCenter* messageCenter_;

  // The button that invokes |-close:|, in the upper-right corner.
  scoped_nsobject<HoverImageButton> closeButton_;

  // The large icon associated with the notification, on the left side.
  scoped_nsobject<NSImageView> icon_;

  // The title of the message.
  scoped_nsobject<NSTextField> title_;

  // Body text of the message.
  scoped_nsobject<NSTextField> message_;
}

// Creates a new controller for a given notification.
- (id)initWithNotification:(const message_center::Notification*)notification
    messageCenter:(message_center::MessageCenter*)messageCenter;

// Action for clicking on the notification's |closeButton_|.
- (void)close:(id)sender;

// Accessor for the notification.
- (const message_center::Notification*)notification;

// Gets the notification ID. This string is owned by the NotificationController
// rather than the model object, so it's safe to use after the Notification has
// been deleted.
- (const std::string&)notificationID;

@end

#endif  // UI_MESSAGE_CENTER_COCOA_NOTIFICATION_CONTROLLER_H_
