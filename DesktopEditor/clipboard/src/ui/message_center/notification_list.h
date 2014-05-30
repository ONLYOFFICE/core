// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFICATION_LIST_H_
#define UI_MESSAGE_CENTER_NOTIFICATION_LIST_H_

#include <set>
#include <string>

#include "base/string16.h"
#include "base/time.h"
#include "base/timer.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"

namespace base {
class DictionaryValue;
}

namespace message_center {
namespace test {
class NotificationListTest;
}

// Comparers used to auto-sort the lists of Notifications.
struct MESSAGE_CENTER_EXPORT ComparePriorityTimestampSerial {
  bool operator()(Notification* n1, Notification* n2);
};

struct CompareTimestampSerial {
  bool operator()(Notification* n1, Notification* n2);
};

// A helper class to manage the list of notifications.
class MESSAGE_CENTER_EXPORT NotificationList {
 public:
  // Auto-sorted set. Matches the order in which Notifications are shown in
  // Notification Center.
  typedef std::set<Notification*, ComparePriorityTimestampSerial> Notifications;

  // Auto-sorted set used to return the Notifications to be shown as popup
  // toasts.
  typedef std::set<Notification*, CompareTimestampSerial> PopupNotifications;

  explicit NotificationList();
  virtual ~NotificationList();

  // Affects whether or not a message has been "read". Collects the set of
  // ids whose state have changed and set to |udpated_ids|. NULL if updated
  // ids don't matter.
  void SetMessageCenterVisible(bool visible,
                               std::set<std::string>* updated_ids);

  void AddNotification(NotificationType type,
                       const std::string& id,
                       const string16& title,
                       const string16& message,
                       const string16& display_source,
                       const std::string& extension_id,
                       const base::DictionaryValue* optional_fields);

  void UpdateNotificationMessage(const std::string& old_id,
                                 const std::string& new_id,
                                 const string16& title,
                                 const string16& message,
                                 const base::DictionaryValue* optional_fields);

  void RemoveNotification(const std::string& id);

  void RemoveAllNotifications();

  Notifications GetNotificationsBySource(const std::string& id);
  Notifications GetNotificationsByExtension(const std::string& id);

  // Returns true if the notification exists and was updated.
  bool SetNotificationIcon(const std::string& notification_id,
                           const gfx::Image& image);

  // Returns true if the notification exists and was updated.
  bool SetNotificationImage(const std::string& notification_id,
                            const gfx::Image& image);

  // Returns true if the notification and button exist and were updated.
  bool SetNotificationButtonIcon(const std::string& notification_id,
                                 int button_index,
                                 const gfx::Image& image);

  bool HasNotification(const std::string& id);

  // Returns false if the first notification has been shown as a popup (which
  // means that all notifications have been shown).
  bool HasPopupNotifications();

  // Returns the recent notifications of the priority higher then LOW,
  // that have not been shown as a popup. kMaxVisiblePopupNotifications are
  // used to limit the number of notifications for the DEFAULT priority.
  // The returned list is sorted by timestamp, newer first.
  PopupNotifications GetPopupNotifications();

  // Marks the popups for the |priority| as shown.
  void MarkPopupsAsShown(int priority);

  // Marks a specific popup item as shown. Set |mark_notification_as_read| to
  // true in case marking the notification as read too.
  void MarkSinglePopupAsShown(const std::string& id,
                              bool mark_notification_as_read);

  // Marks a specific popup item as displayed.
  void MarkSinglePopupAsDisplayed(const std::string& id);

  // Marks the specified notification as expanded in the notification center.
  void MarkNotificationAsExpanded(const std::string& id);

  bool quiet_mode() const { return quiet_mode_; }

  // Sets the current quiet mode status to |quiet_mode|. The new status is not
  // expired.
  void SetQuietMode(bool quiet_mode);

  // Sets the current quiet mode to true. The quiet mode will expire in the
  // specified time-delta from now.
  void EnterQuietModeWithExpire(const base::TimeDelta& expires_in);

  // Returns all notifications, in a (priority-timestamp) order. Suitable for
  // rendering notifications in a NotificationCenter.
  const Notifications& GetNotifications();
  size_t NotificationCount() const;
  size_t unread_count() const { return unread_count_; }

 private:
  friend class test::NotificationListTest;

  // Iterates through the list and returns the first notification matching |id|.
  Notifications::iterator GetNotification(const std::string& id);

  void EraseNotification(Notifications::iterator iter);

  void PushNotification(scoped_ptr<Notification> notification);

  // Sets the current quiet mode status to |quiet_mode|.
  void SetQuietModeInternal(bool quiet_mode);

  Notifications notifications_;
  bool message_center_visible_;
  size_t unread_count_;
  bool quiet_mode_;
  scoped_ptr<base::OneShotTimer<NotificationList> > quiet_mode_timer_;

  DISALLOW_COPY_AND_ASSIGN(NotificationList);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_NOTIFICATION_LIST_H_
