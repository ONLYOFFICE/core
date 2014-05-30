// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/message_center/cocoa/popup_controller.h"

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/sys_string_conversions.h"
#include "base/utf_string_conversions.h"
#import "ui/base/test/ui_cocoa_test_helper.h"
#include "ui/message_center/notification.h"

class PopupControllerTest : public ui::CocoaTest {
};

TEST_F(PopupControllerTest, Creation) {
  scoped_ptr<message_center::Notification> notification(
      new message_center::Notification(
          message_center::NOTIFICATION_TYPE_SIMPLE,
          "",
          ASCIIToUTF16("Added to circles"),
          ASCIIToUTF16("Jonathan and 5 others"),
          string16(),
          std::string(),
          NULL));

  scoped_nsobject<MCPopupController> controller(
      [[MCPopupController alloc] initWithNotification:notification.get()
                                        messageCenter:NULL]);

  EXPECT_TRUE([controller window]);
  EXPECT_EQ(notification.get(), [controller notification]);

  [controller showWindow:nil];
  [controller close];
}
