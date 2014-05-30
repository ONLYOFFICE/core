// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_POPUP_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_POPUP_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include <string>

#import "base/memory/scoped_nsobject.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class Notification;
}

@class MCNotificationController;

// A window controller that hosts a notification as a popup balloon on the
// user's desktop. Unlike most window controllers, this does not own itself and
// its lifetime must be managed manually.
MESSAGE_CENTER_EXPORT
@interface MCPopupController : NSWindowController {
 @private
  scoped_nsobject<MCNotificationController> notificationController_;
}

// Designated initializer.
- (id)initWithNotification:(const message_center::Notification*)notification
    messageCenter:(message_center::MessageCenter*)messageCenter;

// Accessor for the notification.
- (const message_center::Notification*)notification;

// Gets the notification ID. This string is owned by the NotificationController
// rather than the model object, so it's safe to use after the Notification has
// been deleted.
- (const std::string&)notificationID;

@end

#endif  // UI_MESSAGE_CENTER_COCOA_POPUP_CONTROLLER_H_
