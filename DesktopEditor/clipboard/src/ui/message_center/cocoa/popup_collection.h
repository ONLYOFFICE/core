// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_POPUP_COLLECTION_H_
#define UI_MESSAGE_CENTER_COCOA_POPUP_COLLECTION_H_

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#import "base/memory/scoped_ptr.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class MessageCenterObserver;
}

// A popup collection interfaces with the MessageCenter as an observer. It will
// arrange notifications on the screen as popups, starting in the upper-right
// corner, going to the bottom of the screen. This class maintains ownership of
// the Cocoa controllers and windows of the notifications.
MESSAGE_CENTER_EXPORT
@interface MCPopupCollection : NSObject {
 @private
  // The message center that is responsible for the notifications. Weak, global.
  message_center::MessageCenter* messageCenter_;

  // MessageCenterObserver implementation.
  scoped_ptr<message_center::MessageCenterObserver> observer_;

  // Array of all on-screen popup notifications.
  scoped_nsobject<NSMutableArray> popups_;

  // For testing only. If not a zero rect, this is the screen size to use
  // for laying out popups.
  NSRect testingScreenFrame_;
}

// Designated initializer that construct an instance to observe |messageCenter|.
- (id)initWithMessageCenter:(message_center::MessageCenter*)messageCenter;

@end

@interface MCPopupCollection (ExposedForTesting)
// Setter for the testingScreenFrame_.
- (void)setScreenFrame:(NSRect)frame;
@end

#endif  // UI_MESSAGE_CENTER_COCOA_POPUP_COLLECTION_H_
