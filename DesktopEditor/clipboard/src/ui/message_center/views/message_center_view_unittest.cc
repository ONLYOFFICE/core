// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/message_center/fake_message_center.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_list.h"
#include "ui/message_center/notification_types.h"
#include "ui/message_center/views/message_center_view.h"
#include "ui/message_center/views/notification_view.h"

namespace message_center {

/* Types **********************************************************************/

enum CallType {
  GET_PREFERRED_SIZE,
  GET_HEIGHT_FOR_WIDTH,
  LAYOUT
};

/* Instrumented/Mock NotificationView subclass ********************************/

class MockNotificationView : public NotificationView {
 public:
  class Test {
   public:
    virtual void RegisterCall(int receiver_id, CallType type) = 0;
  };

  explicit MockNotificationView(const Notification& notification,
                                MessageCenter* message_center,
                                Test* test,
                                int view_id);
  virtual ~MockNotificationView();

  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int w) OVERRIDE;
  virtual void Layout() OVERRIDE;

  int get_id() { return id_; };

 private:
  void RegisterCall(CallType type);

  Test* test_;
  int id_;

  DISALLOW_COPY_AND_ASSIGN(MockNotificationView);
};

MockNotificationView::MockNotificationView(const Notification& notification,
                                           MessageCenter* message_center,
                                           Test* test,
                                           int view_id)
    : NotificationView(notification, message_center, true),
      test_(test),
      id_(view_id) {
}

MockNotificationView::~MockNotificationView() {
}

gfx::Size MockNotificationView::GetPreferredSize() {
  RegisterCall(GET_PREFERRED_SIZE);
  return child_count() ? NotificationView::GetPreferredSize() :
                         gfx::Size(id_, id_);
}

int MockNotificationView::GetHeightForWidth(int width) {
  RegisterCall(GET_HEIGHT_FOR_WIDTH);
  return child_count() ? NotificationView::GetHeightForWidth(width) : (id_);
}

void MockNotificationView::Layout() {
  RegisterCall(LAYOUT);
  if (child_count())
    NotificationView::Layout();
}

void MockNotificationView::RegisterCall(CallType type) {
  if (test_)
    test_->RegisterCall(id_, type);
}

/* Test fixture ***************************************************************/

class MessageCenterViewTest : public testing::Test,
                              public MockNotificationView::Test {
 public:
  MessageCenterViewTest();
  virtual ~MessageCenterViewTest();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  MessageCenterView* GetMessageCenterView();
  int GetNotificationCount();
  int GetCallCount(CallType type);

  virtual void RegisterCall(int receiver_id, CallType type) OVERRIDE;

  void LogBounds(int depth, views::View* view);

 private:
  views::View* MakeParent(views::View* child1, views::View* child2);


  scoped_ptr<MessageCenterView> message_center_view_;
  FakeMessageCenter message_center_;
  std::map<CallType,int> callCounts_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterViewTest);
};

MessageCenterViewTest::MessageCenterViewTest() {
}

MessageCenterViewTest::~MessageCenterViewTest() {
}

void MessageCenterViewTest::SetUp() {
  // Create a dummy notification.
  Notification notification(
        NOTIFICATION_TYPE_SIMPLE,
        std::string("notification id"),
        UTF8ToUTF16("title"),
        UTF8ToUTF16("message"),
        UTF8ToUTF16("display source"),
        std::string("extension id"),
        NULL);

  // ...and a list for it.
  NotificationList::Notifications notifications;
  notifications.insert(&notification);

  // Then create a new MessageCenterView with that single notification.
  message_center_view_.reset(new MessageCenterView(&message_center_, 100));
  message_center_view_->SetNotifications(notifications);

  // Remove and delete the NotificationView now owned by the MessageCenterView's
  // MessageListView and replace it with an instrumented MockNotificationView
  // that will become owned by the MessageListView.
  MockNotificationView* mock;
  mock = new MockNotificationView(notification, &message_center_, this, 42);
  message_center_view_->message_views_.push_back(mock);
  message_center_view_->SetNotificationViewForTest(mock);
}

void MessageCenterViewTest::TearDown() {
  message_center_view_.reset();
}

MessageCenterView* MessageCenterViewTest::GetMessageCenterView() {
  return message_center_view_.get();
}

int MessageCenterViewTest::GetNotificationCount() {
  return 1;
}

int MessageCenterViewTest::GetCallCount(CallType type) {
  return callCounts_[type];
}

void MessageCenterViewTest::RegisterCall(int receiver_id, CallType type) {
  callCounts_[type] += 1;
}

void MessageCenterViewTest::LogBounds(int depth, views::View* view) {
  string16 inset;
  for (int i = 0; i < depth; ++i)
    inset.append(UTF8ToUTF16("  "));
  gfx::Rect bounds = view->bounds();
  DVLOG(0) << inset << bounds.width() << " x " << bounds.height()
           << " @ " << bounds.x() << ", " << bounds.y();
  for (int i = 0; i < view->child_count(); ++i)
    LogBounds(depth + 1, view->child_at(i));
}

/* Unit tests *****************************************************************/

TEST_F(MessageCenterViewTest, CallTest) {
  // Exercise (with size values that just need to be large enough).
  GetMessageCenterView()->SetBounds(0, 0, 100, 100);

  // Verify that this didn't generate more than 2 Layout() call per descendant
  // NotificationView or more than a total of 20 GetPreferredSize() and
  // GetHeightForWidth() calls per descendant NotificationView. 20 is a very
  // large number corresponding to the current reality. That number will be
  // ratcheted down over time as the code improves.
  EXPECT_LE(GetCallCount(LAYOUT), GetNotificationCount() * 2);
  EXPECT_LE(GetCallCount(GET_PREFERRED_SIZE) +
            GetCallCount(GET_HEIGHT_FOR_WIDTH),
            GetNotificationCount() * 20);
}

}  // namespace message_center
