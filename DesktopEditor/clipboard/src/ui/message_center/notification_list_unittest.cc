// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/notification_list.h"

#include "base/basictypes.h"
#include "base/i18n/time_formatting.h"
#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"
#include "base/values.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification_types.h"

namespace message_center {
namespace test {

class NotificationListTest : public testing::Test {
 public:
  NotificationListTest() {}
  virtual ~NotificationListTest() {}

  virtual void SetUp() {
    notification_list_.reset(new NotificationList());
    counter_ = 0;
  }

 protected:
  // Currently NotificationListTest doesn't care about some fields like title or
  // message, so put a simple template on it. Returns the id of the new
  // notification.
  std::string AddNotification(const base::DictionaryValue* optional_fields) {
    std::string new_id = base::StringPrintf(kIdFormat, counter_);
    notification_list_->AddNotification(
        message_center::NOTIFICATION_TYPE_SIMPLE, new_id,
        UTF8ToUTF16(base::StringPrintf(kTitleFormat, counter_)),
        UTF8ToUTF16(base::StringPrintf(kMessageFormat, counter_)),
        UTF8ToUTF16(kDisplaySource), kExtensionId,
        optional_fields);
    counter_++;
    return new_id;
  }

  // Utility methods of AddNotification.
  std::string AddPriorityNotification(NotificationPriority priority) {
    base::DictionaryValue optional;
    optional.SetInteger(message_center::kPriorityKey,
                        static_cast<int>(priority));
    return AddNotification(&optional);
  }
  void SetupTimestampKey(const base::Time& time,
                         base::DictionaryValue* optional) {
    string16 time_formatted = base::TimeFormatShortDateAndTime(time);
    optional->SetString(message_center::kTimestampKey, time_formatted);
  }

  size_t GetPopupCounts() {
    return notification_list()->GetPopupNotifications().size();
  }

  Notification* GetNotification(const std::string& id) {
    NotificationList::Notifications::iterator iter =
        notification_list()->GetNotification(id);
    if (iter == notification_list()->GetNotifications().end())
      return NULL;
    return *iter;
  }

  NotificationList* notification_list() { return notification_list_.get(); }

 private:
  static const char kIdFormat[];
  static const char kTitleFormat[];
  static const char kMessageFormat[];
  static const char kDisplaySource[];
  static const char kExtensionId[];

  scoped_ptr<NotificationList> notification_list_;
  size_t counter_;

  DISALLOW_COPY_AND_ASSIGN(NotificationListTest);
};

bool IsInNotifications(const NotificationList::Notifications& notifications,
                       const std::string& id) {
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end(); ++iter) {
    if ((*iter)->id() == id)
      return true;
  }
  return false;
}

const char NotificationListTest::kIdFormat[] = "id%ld";
const char NotificationListTest::kTitleFormat[] = "id%ld";
const char NotificationListTest::kMessageFormat[] = "message%ld";
const char NotificationListTest::kDisplaySource[] = "source";
const char NotificationListTest::kExtensionId[] = "ext";

TEST_F(NotificationListTest, Basic) {
  ASSERT_EQ(0u, notification_list()->NotificationCount());
  ASSERT_EQ(0u, notification_list()->unread_count());

  std::string id0 = AddNotification(NULL);
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  std::string id1 = AddNotification(NULL);
  EXPECT_EQ(2u, notification_list()->NotificationCount());
  EXPECT_EQ(2u, notification_list()->unread_count());

  EXPECT_TRUE(notification_list()->HasPopupNotifications());
  EXPECT_TRUE(notification_list()->HasNotification(id0));
  EXPECT_TRUE(notification_list()->HasNotification(id1));
  EXPECT_FALSE(notification_list()->HasNotification(id1 + "foo"));

  EXPECT_EQ(2u, GetPopupCounts());

  notification_list()->MarkPopupsAsShown(0);
  EXPECT_EQ(2u, notification_list()->NotificationCount());
  EXPECT_EQ(0u, GetPopupCounts());

  notification_list()->RemoveNotification(id0);
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  EXPECT_EQ(1u, notification_list()->unread_count());

  AddNotification(NULL);
  EXPECT_EQ(2u, notification_list()->NotificationCount());

  notification_list()->RemoveAllNotifications();
  EXPECT_EQ(0u, notification_list()->NotificationCount());
  EXPECT_EQ(0u, notification_list()->unread_count());
}

TEST_F(NotificationListTest, MessageCenterVisible) {
  AddNotification(NULL);
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  ASSERT_EQ(1u, notification_list()->unread_count());
  ASSERT_EQ(1u, GetPopupCounts());

  // Make the message center visible. It resets the unread count and popup
  // counts.
  notification_list()->SetMessageCenterVisible(true, NULL);
  ASSERT_EQ(0u, notification_list()->unread_count());
  ASSERT_EQ(0u, GetPopupCounts());
}

TEST_F(NotificationListTest, UnreadCount) {
  std::string id0 = AddNotification(NULL);
  std::string id1 = AddNotification(NULL);
  ASSERT_EQ(2u, notification_list()->unread_count());

  notification_list()->MarkSinglePopupAsDisplayed(id0);
  EXPECT_EQ(1u, notification_list()->unread_count());
  notification_list()->MarkSinglePopupAsDisplayed(id0);
  EXPECT_EQ(1u, notification_list()->unread_count());
  notification_list()->MarkSinglePopupAsDisplayed(id1);
  EXPECT_EQ(0u, notification_list()->unread_count());
}

TEST_F(NotificationListTest, UpdateNotification) {
  std::string id0 = AddNotification(NULL);
  std::string replaced = id0 + "_replaced";
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  notification_list()->UpdateNotificationMessage(
      id0, replaced, UTF8ToUTF16("newtitle"), UTF8ToUTF16("newbody"), NULL);
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  const NotificationList::Notifications& notifications =
      notification_list()->GetNotifications();
  EXPECT_EQ(replaced, (*notifications.begin())->id());
  EXPECT_EQ(UTF8ToUTF16("newtitle"), (*notifications.begin())->title());
  EXPECT_EQ(UTF8ToUTF16("newbody"), (*notifications.begin())->message());
}

TEST_F(NotificationListTest, GetNotificationsBySourceOrExtensions) {
  notification_list()->AddNotification(
      message_center::NOTIFICATION_TYPE_SIMPLE, "id0", UTF8ToUTF16("title0"),
      UTF8ToUTF16("message0"), UTF8ToUTF16("source0"), "ext0", NULL);
  notification_list()->AddNotification(
      message_center::NOTIFICATION_TYPE_SIMPLE, "id1", UTF8ToUTF16("title1"),
      UTF8ToUTF16("message1"), UTF8ToUTF16("source0"), "ext0", NULL);
  notification_list()->AddNotification(
      message_center::NOTIFICATION_TYPE_SIMPLE, "id2", UTF8ToUTF16("title1"),
      UTF8ToUTF16("message1"), UTF8ToUTF16("source1"), "ext0", NULL);
  notification_list()->AddNotification(
      message_center::NOTIFICATION_TYPE_SIMPLE, "id3", UTF8ToUTF16("title1"),
      UTF8ToUTF16("message1"), UTF8ToUTF16("source2"), "ext1", NULL);

  NotificationList::Notifications by_source =
      notification_list()->GetNotificationsBySource("id0");
  EXPECT_TRUE(IsInNotifications(by_source, "id0"));
  EXPECT_TRUE(IsInNotifications(by_source, "id1"));
  EXPECT_FALSE(IsInNotifications(by_source, "id2"));
  EXPECT_FALSE(IsInNotifications(by_source, "id3"));

  NotificationList::Notifications by_extension =
      notification_list()->GetNotificationsByExtension("id0");
  EXPECT_TRUE(IsInNotifications(by_extension, "id0"));
  EXPECT_TRUE(IsInNotifications(by_extension, "id1"));
  EXPECT_TRUE(IsInNotifications(by_extension, "id2"));
  EXPECT_FALSE(IsInNotifications(by_extension, "id3"));
}

TEST_F(NotificationListTest, OldPopupShouldNotBeHidden) {
  std::vector<std::string> ids;
  for (size_t i = 0; i <= kMaxVisiblePopupNotifications; i++)
    ids.push_back(AddNotification(NULL));

  NotificationList::PopupNotifications popups =
      notification_list()->GetPopupNotifications();
  // The popup should contain the oldest kMaxVisiblePopupNotifications. Newer
  // one should come earlier in the popup list. It means, the last element
  // of |popups| should be the firstly added one, and so on.
  EXPECT_EQ(kMaxVisiblePopupNotifications, popups.size());
  NotificationList::PopupNotifications::const_reverse_iterator iter =
      popups.rbegin();
  for (size_t i = 0; i < kMaxVisiblePopupNotifications; ++i, ++iter) {
    EXPECT_EQ(ids[i], (*iter)->id()) << i;
  }

  notification_list()->MarkPopupsAsShown(message_center::DEFAULT_PRIORITY);
  popups.clear();
  popups = notification_list()->GetPopupNotifications();
  EXPECT_EQ(1u, popups.size());
  EXPECT_EQ(ids[ids.size() - 1], (*popups.begin())->id());
}

TEST_F(NotificationListTest, Priority) {
  ASSERT_EQ(0u, notification_list()->NotificationCount());
  ASSERT_EQ(0u, notification_list()->unread_count());

  // Default priority has the limit on the number of the popups.
  for (size_t i = 0; i <= kMaxVisiblePopupNotifications; ++i)
    AddNotification(NULL);
  EXPECT_EQ(kMaxVisiblePopupNotifications + 1,
            notification_list()->NotificationCount());
  EXPECT_EQ(kMaxVisiblePopupNotifications, GetPopupCounts());

  // Low priority: not visible to popups.
  notification_list()->SetMessageCenterVisible(true, NULL);
  notification_list()->SetMessageCenterVisible(false, NULL);
  EXPECT_EQ(0u, notification_list()->unread_count());
  AddPriorityNotification(LOW_PRIORITY);
  EXPECT_EQ(kMaxVisiblePopupNotifications + 2,
            notification_list()->NotificationCount());
  EXPECT_EQ(1u, notification_list()->unread_count());
  EXPECT_EQ(0u, GetPopupCounts());

  // Minimum priority: doesn't update the unread count.
  AddPriorityNotification(MIN_PRIORITY);
  EXPECT_EQ(kMaxVisiblePopupNotifications + 3,
            notification_list()->NotificationCount());
  EXPECT_EQ(1u, notification_list()->unread_count());
  EXPECT_EQ(0u, GetPopupCounts());

  notification_list()->RemoveAllNotifications();

  // Higher priority: no limits to the number of popups.
  for (size_t i = 0; i < kMaxVisiblePopupNotifications * 2; ++i)
    AddPriorityNotification(HIGH_PRIORITY);
  for (size_t i = 0; i < kMaxVisiblePopupNotifications * 2; ++i)
    AddPriorityNotification(MAX_PRIORITY);
  EXPECT_EQ(kMaxVisiblePopupNotifications * 4,
            notification_list()->NotificationCount());
  EXPECT_EQ(kMaxVisiblePopupNotifications * 4, GetPopupCounts());
}

TEST_F(NotificationListTest, HasPopupsWithPriority) {
  ASSERT_EQ(0u, notification_list()->NotificationCount());
  ASSERT_EQ(0u, notification_list()->unread_count());

  AddPriorityNotification(MIN_PRIORITY);
  AddPriorityNotification(MAX_PRIORITY);

  EXPECT_EQ(1u, GetPopupCounts());
}

TEST_F(NotificationListTest, PriorityPromotion) {
  std::string id0 = AddPriorityNotification(LOW_PRIORITY);
  std::string replaced = id0 + "_replaced";
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  EXPECT_EQ(0u, GetPopupCounts());
  base::DictionaryValue optional;
  optional.SetInteger(message_center::kPriorityKey, 1);
  notification_list()->UpdateNotificationMessage(
      id0, replaced, UTF8ToUTF16("newtitle"), UTF8ToUTF16("newbody"),
      &optional);
  EXPECT_EQ(1u, notification_list()->NotificationCount());
  EXPECT_EQ(1u, GetPopupCounts());
  const NotificationList::Notifications& notifications =
      notification_list()->GetNotifications();
  EXPECT_EQ(replaced, (*notifications.begin())->id());
  EXPECT_EQ(UTF8ToUTF16("newtitle"), (*notifications.begin())->title());
  EXPECT_EQ(UTF8ToUTF16("newbody"), (*notifications.begin())->message());
  EXPECT_EQ(1, (*notifications.begin())->priority());
}

TEST_F(NotificationListTest, PriorityPromotionWithPopups) {
  std::string id0 = AddPriorityNotification(LOW_PRIORITY);
  std::string id1 = AddPriorityNotification(DEFAULT_PRIORITY);
  EXPECT_EQ(1u, GetPopupCounts());
  notification_list()->MarkSinglePopupAsShown(id1, true);
  EXPECT_EQ(0u, GetPopupCounts());

  // id0 promoted to LOW->DEFAULT, it'll appear as toast (popup).
  base::DictionaryValue priority_default;
  priority_default.SetInteger(message_center::kPriorityKey,
                              static_cast<int>(DEFAULT_PRIORITY));
  notification_list()->UpdateNotificationMessage(
      id0, id0, UTF8ToUTF16("newtitle"), UTF8ToUTF16("newbody"),
      &priority_default);
  EXPECT_EQ(1u, GetPopupCounts());
  notification_list()->MarkSinglePopupAsShown(id0, true);
  EXPECT_EQ(0u, GetPopupCounts());

  // update with no promotion change for id0, it won't appear as a toast.
  notification_list()->UpdateNotificationMessage(
      id0, id0, UTF8ToUTF16("newtitle2"), UTF8ToUTF16("newbody2"),
      NULL);
  EXPECT_EQ(0u, GetPopupCounts());

  // id1 promoted to DEFAULT->HIGH, it'll appear as toast (popup).
  base::DictionaryValue priority_high;
  priority_high.SetInteger(message_center::kPriorityKey,
                           static_cast<int>(HIGH_PRIORITY));
  notification_list()->UpdateNotificationMessage(
      id1, id1, UTF8ToUTF16("newtitle"), UTF8ToUTF16("newbody"),
      &priority_high);
  EXPECT_EQ(1u, GetPopupCounts());
  notification_list()->MarkSinglePopupAsShown(id1, true);
  EXPECT_EQ(0u, GetPopupCounts());

  // id1 promoted to HIGH->MAX, it'll appear as toast again.
  base::DictionaryValue priority_max;
  priority_max.SetInteger(message_center::kPriorityKey,
                          static_cast<int>(MAX_PRIORITY));
  notification_list()->UpdateNotificationMessage(
      id1, id1, UTF8ToUTF16("newtitle2"), UTF8ToUTF16("newbody2"),
      &priority_max);
  EXPECT_EQ(1u, GetPopupCounts());
  notification_list()->MarkSinglePopupAsShown(id1, true);
  EXPECT_EQ(0u, GetPopupCounts());

  // id1 demoted to MAX->DEFAULT, no appearing as toast.
  notification_list()->UpdateNotificationMessage(
      id1, id1, UTF8ToUTF16("newtitle3"), UTF8ToUTF16("newbody3"),
      &priority_default);
  EXPECT_EQ(0u, GetPopupCounts());
}

TEST_F(NotificationListTest, NotificationOrderAndPriority) {
  base::Time now = base::Time::Now();
  base::DictionaryValue optional;
  SetupTimestampKey(now, &optional);
  optional.SetInteger(message_center::kPriorityKey, 2);
  std::string max_id = AddNotification(&optional);
  now += base::TimeDelta::FromSeconds(1);
  SetupTimestampKey(now, &optional);
  optional.SetInteger(message_center::kPriorityKey, 1);
  std::string high_id = AddNotification(&optional);
  now += base::TimeDelta::FromSeconds(1);
  SetupTimestampKey(now, &optional);
  optional.SetInteger(message_center::kPriorityKey, 0);
  std::string default_id = AddNotification(&optional);

  {
    // Popups: latest comes first.
    NotificationList::PopupNotifications popups =
        notification_list()->GetPopupNotifications();
    EXPECT_EQ(3u, popups.size());
    NotificationList::PopupNotifications::const_iterator iter = popups.begin();
    EXPECT_EQ(default_id, (*iter)->id());
    iter++;
    EXPECT_EQ(high_id, (*iter)->id());
    iter++;
    EXPECT_EQ(max_id, (*iter)->id());
  }
  {
    // Notifications: high priority comes ealier.
    const NotificationList::Notifications& notifications =
        notification_list()->GetNotifications();
    EXPECT_EQ(3u, notifications.size());
    NotificationList::Notifications::const_iterator iter =
        notifications.begin();
    EXPECT_EQ(max_id, (*iter)->id());
    iter++;
    EXPECT_EQ(high_id, (*iter)->id());
    iter++;
    EXPECT_EQ(default_id, (*iter)->id());
  }
}

TEST_F(NotificationListTest, MarkSinglePopupAsShown) {
  std::string id1 = AddNotification(NULL);
  std::string id2 = AddNotification(NULL);
  std::string id3 = AddNotification(NULL);
  ASSERT_EQ(3u, notification_list()->NotificationCount());
  ASSERT_EQ(std::min(static_cast<size_t>(3u), kMaxVisiblePopupNotifications),
            GetPopupCounts());
  notification_list()->MarkSinglePopupAsDisplayed(id1);
  notification_list()->MarkSinglePopupAsDisplayed(id2);
  notification_list()->MarkSinglePopupAsDisplayed(id3);

  notification_list()->MarkSinglePopupAsShown(id2, true);
  notification_list()->MarkSinglePopupAsShown(id3, false);
  EXPECT_EQ(3u, notification_list()->NotificationCount());
  EXPECT_EQ(1u, notification_list()->unread_count());
  EXPECT_EQ(1u, GetPopupCounts());
  NotificationList::PopupNotifications popups =
      notification_list()->GetPopupNotifications();
  EXPECT_EQ(id1, (*popups.begin())->id());

  // The notifications in the NotificationCenter are unaffected by popups shown.
  NotificationList::Notifications notifications =
      notification_list()->GetNotifications();
  NotificationList::Notifications::const_iterator iter = notifications.begin();
  EXPECT_EQ(id3, (*iter)->id());
  iter++;
  EXPECT_EQ(id2, (*iter)->id());
  iter++;
  EXPECT_EQ(id1, (*iter)->id());

  // Trickier scenario.
  notification_list()->MarkPopupsAsShown(message_center::DEFAULT_PRIORITY);
  std::string id4 = AddNotification(NULL);
  std::string id5 = AddNotification(NULL);
  std::string id6 = AddNotification(NULL);
  notification_list()->MarkSinglePopupAsShown(id5, true);

  {
    popups.clear();
    popups = notification_list()->GetPopupNotifications();
    EXPECT_EQ(2u, popups.size());
    NotificationList::PopupNotifications::const_iterator iter = popups.begin();
    EXPECT_EQ(id6, (*iter)->id());
    iter++;
    EXPECT_EQ(id4, (*iter)->id());
  }

  notifications.clear();
  notifications = notification_list()->GetNotifications();
  EXPECT_EQ(6u, notifications.size());
  iter = notifications.begin();
  EXPECT_EQ(id6, (*iter)->id());
  iter++;
  EXPECT_EQ(id5, (*iter)->id());
  iter++;
  EXPECT_EQ(id4, (*iter)->id());
  iter++;
  EXPECT_EQ(id3, (*iter)->id());
  iter++;
  EXPECT_EQ(id2, (*iter)->id());
  iter++;
  EXPECT_EQ(id1, (*iter)->id());
}

TEST_F(NotificationListTest, UpdateAfterMarkedAsShown) {
  std::string id1 = AddNotification(NULL);
  std::string id2 = AddNotification(NULL);
  notification_list()->MarkSinglePopupAsDisplayed(id1);
  notification_list()->MarkSinglePopupAsDisplayed(id2);

  EXPECT_EQ(2u, GetPopupCounts());

  const Notification* n1 = GetNotification(id1);
  EXPECT_FALSE(n1->shown_as_popup());
  EXPECT_TRUE(n1->is_read());

  notification_list()->MarkSinglePopupAsShown(id1, true);

  n1 = GetNotification(id1);
  EXPECT_TRUE(n1->shown_as_popup());
  EXPECT_TRUE(n1->is_read());

  const std::string replaced("test-replaced-id");
  notification_list()->UpdateNotificationMessage(
      id1, replaced, UTF8ToUTF16("newtitle"), UTF8ToUTF16("newbody"), NULL);
  n1 = GetNotification(id1);
  EXPECT_TRUE(n1 == NULL);
  const Notification* nr = GetNotification(replaced);
  EXPECT_TRUE(nr->shown_as_popup());
  EXPECT_TRUE(nr->is_read());
}

TEST_F(NotificationListTest, QuietMode) {
  notification_list()->SetQuietMode(true);
  AddNotification(NULL);
  AddPriorityNotification(HIGH_PRIORITY);
  AddPriorityNotification(MAX_PRIORITY);
  EXPECT_EQ(3u, notification_list()->NotificationCount());
  EXPECT_EQ(0u, GetPopupCounts());
  // TODO(mukai): fix here when notification_list distinguish dismiss by quiet
  // mode and by user operation.
  EXPECT_EQ(0u, notification_list()->unread_count());

  notification_list()->SetQuietMode(false);
  AddNotification(NULL);
  EXPECT_EQ(4u, notification_list()->NotificationCount());
  EXPECT_EQ(1u, GetPopupCounts());

  // TODO(mukai): Add test of quiet mode with expiration.
}

}  // namespace test
}  // namespace message_center
