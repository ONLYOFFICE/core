// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/notification_list.h"

#include "base/bind.h"
#include "base/logging.h"
#include "base/stl_util.h"
#include "base/time.h"
#include "base/values.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"

namespace message_center {

bool ComparePriorityTimestampSerial::operator()(Notification* n1,
                                                Notification* n2) {
  if (n1->priority() > n2->priority())  // Higher pri go first.
    return true;
  if (n1->priority() < n2->priority())
    return false;
  return CompareTimestampSerial()(n1, n2);
}

bool CompareTimestampSerial::operator()(Notification* n1, Notification* n2) {
  if (n1->timestamp() > n2->timestamp())  // Newer come first.
    return true;
  if (n1->timestamp() < n2->timestamp())
    return false;
  if (n1->serial_number() > n2->serial_number())  // Newer come first.
    return true;
  if (n1->serial_number() < n2->serial_number())
    return false;
  return false;
}

NotificationList::NotificationList()
    : message_center_visible_(false),
      unread_count_(0),
      quiet_mode_(false) {
}

NotificationList::~NotificationList() {
  STLDeleteContainerPointers(notifications_.begin(), notifications_.end());
}

void NotificationList::SetMessageCenterVisible(
    bool visible,
    std::set<std::string>* updated_ids) {
  if (message_center_visible_ == visible)
    return;

  message_center_visible_ = visible;

  if (!visible)
    return;

  unread_count_ = 0;

  for (Notifications::iterator iter = notifications_.begin();
       iter != notifications_.end(); ++iter) {
    Notification* notification = *iter;
    notification->set_shown_as_popup(true);
    notification->set_is_read(true);
    if (updated_ids &&
        !(notification->shown_as_popup() && notification->is_read())) {
      updated_ids->insert(notification->id());
    }
  }
}

void NotificationList::AddNotification(
    NotificationType type,
    const std::string& id,
    const string16& title,
    const string16& message,
    const string16& display_source,
    const std::string& extension_id,
    const DictionaryValue* optional_fields) {
  scoped_ptr<Notification> notification(
      new Notification(type, id, title, message, display_source, extension_id,
          optional_fields));
  PushNotification(notification.Pass());
}

void NotificationList::UpdateNotificationMessage(
    const std::string& old_id,
    const std::string& new_id,
    const string16& title,
    const string16& message,
    const base::DictionaryValue* optional_fields) {
  Notifications::iterator iter = GetNotification(old_id);
  if (iter == notifications_.end())
    return;

  // Copy and update a notification. It has an effect of setting a new timestamp
  // if not overridden by optional_fields
  scoped_ptr<Notification> notification(
      new Notification((*iter)->type(),
                       new_id,
                       title,
                       message,
                       (*iter)->display_source(),
                       (*iter)->extension_id(),
                       optional_fields));
  notification->CopyState(*iter);

  // Handles priority promotion. If the notification is already dismissed but
  // the updated notification has higher priority, it should re-appear as a
  // toast.
  if ((*iter)->priority() < notification->priority()) {
    notification->set_is_read(false);
    notification->set_shown_as_popup(false);
  }

  // Do not use EraseNotification and PushNotification, since we don't want to
  // change unread counts nor to update is_read/shown_as_popup states.
  Notification *old = *iter;
  notifications_.erase(iter);
  delete old;
  notifications_.insert(notification.release());
}

void NotificationList::RemoveNotification(const std::string& id) {
  EraseNotification(GetNotification(id));
}

void NotificationList::RemoveAllNotifications() {
  for (Notifications::iterator loopiter = notifications_.begin();
       loopiter != notifications_.end(); ) {
    Notifications::iterator curiter = loopiter++;
    EraseNotification(curiter);
  }
  unread_count_ = 0;
}

NotificationList::Notifications NotificationList::GetNotificationsBySource(
    const std::string& id) {
  Notifications notifications;
  Notifications::iterator source_iter = GetNotification(id);
  if (source_iter == notifications_.end())
    return notifications;

  string16 display_source = (*source_iter)->display_source();
  for (Notifications::iterator iter = notifications_.begin();
       iter != notifications_.end(); ++iter) {
    if ((*iter)->display_source() == display_source)
      notifications.insert(*iter);
  }
  return notifications;
}

NotificationList::Notifications NotificationList::GetNotificationsByExtension(
        const std::string& id) {
  Notifications notifications;
  Notifications::iterator source_iter = GetNotification(id);
  if (source_iter == notifications_.end())
    return notifications;

  std::string extension_id = (*source_iter)->extension_id();
  for (Notifications::iterator iter = notifications_.begin();
       iter != notifications_.end(); ++iter) {
    if ((*iter)->extension_id() == extension_id)
      notifications.insert(*iter);
  }
  return notifications;
}

bool NotificationList::SetNotificationIcon(const std::string& notification_id,
                                           const gfx::Image& image) {
  Notifications::iterator iter = GetNotification(notification_id);
  if (iter == notifications_.end())
    return false;
  (*iter)->set_icon(image);
  return true;
}

bool NotificationList::SetNotificationImage(const std::string& notification_id,
                                            const gfx::Image& image) {
  Notifications::iterator iter = GetNotification(notification_id);
  if (iter == notifications_.end())
    return false;
  (*iter)->set_image(image);
  return true;
}

bool NotificationList::SetNotificationButtonIcon(
    const std::string& notification_id, int button_index,
    const gfx::Image& image) {
  Notifications::iterator iter = GetNotification(notification_id);
  if (iter == notifications_.end())
    return false;
  return (*iter)->SetButtonIcon(button_index, image);
}

bool NotificationList::HasNotification(const std::string& id) {
  return GetNotification(id) != notifications_.end();
}

bool NotificationList::HasPopupNotifications() {
  for (Notifications::iterator iter = notifications_.begin();
       iter != notifications_.end(); ++iter) {
    if ((*iter)->priority() < DEFAULT_PRIORITY)
      break;
    if (!(*iter)->shown_as_popup())
      return true;
  }
  return false;
}

NotificationList::PopupNotifications NotificationList::GetPopupNotifications() {
  PopupNotifications result;
  size_t default_priority_popup_count = 0;

  // Collect notifications that should be shown as popups. Start from oldest.
  for (Notifications::const_reverse_iterator iter = notifications_.rbegin();
       iter != notifications_.rend(); iter++) {

    if ((*iter)->shown_as_popup())
      continue;

    // No popups for LOW/MIN priority.
    if ((*iter)->priority() < DEFAULT_PRIORITY)
      continue;

    // Checking limits. No limits for HIGH/MAX priority. DEFAULT priority
    // will return at most kMaxVisiblePopupNotifications entries. If the
    // popup entries are more, older entries are used. see crbug.com/165768
    if ((*iter)->priority() == DEFAULT_PRIORITY &&
        default_priority_popup_count++ >= kMaxVisiblePopupNotifications) {
      continue;
    }

    result.insert(*iter);
  }
  return result;
}

void NotificationList::MarkPopupsAsShown(int priority) {
  PopupNotifications popups = GetPopupNotifications();
  for (PopupNotifications::iterator iter = popups.begin();
       iter != popups.end(); ++iter) {
    if ((*iter)->priority() == priority)
      (*iter)->set_shown_as_popup(true);
  }
}

void NotificationList::MarkSinglePopupAsShown(
    const std::string& id, bool mark_notification_as_read) {
  Notifications::iterator iter = GetNotification(id);
  DCHECK(iter != notifications_.end());

  if ((*iter)->shown_as_popup())
    return;

  (*iter)->set_shown_as_popup(true);

  // The popup notification is already marked as read when it's displayed.
  // Set the is_read() back to false if necessary.
  if (!mark_notification_as_read) {
    (*iter)->set_is_read(false);
    ++unread_count_;
  }
}

void NotificationList::MarkSinglePopupAsDisplayed(const std::string& id) {
  Notifications::iterator iter = GetNotification(id);
  if (iter == notifications_.end())
    return;

  if ((*iter)->shown_as_popup())
    return;

  if (!(*iter)->is_read()) {
    (*iter)->set_is_read(true);
    --unread_count_;
  }
}

void NotificationList::MarkNotificationAsExpanded(const std::string& id) {
  Notifications::iterator iter = GetNotification(id);
  if (iter != notifications_.end())
    (*iter)->set_is_expanded(true);
}

void NotificationList::SetQuietMode(bool quiet_mode) {
  SetQuietModeInternal(quiet_mode);
  quiet_mode_timer_.reset();
}

void NotificationList::EnterQuietModeWithExpire(
    const base::TimeDelta& expires_in) {
  if (quiet_mode_timer_.get()) {
    // Note that the capital Reset() is the method to restart the timer, not
    // scoped_ptr::reset().
    quiet_mode_timer_->Reset();
  } else {
    SetQuietModeInternal(true);
    quiet_mode_timer_.reset(new base::OneShotTimer<NotificationList>);
    quiet_mode_timer_->Start(FROM_HERE, expires_in, base::Bind(
        &NotificationList::SetQuietMode, base::Unretained(this), false));
  }
}

const NotificationList::Notifications& NotificationList::GetNotifications() {
  return notifications_;
}

size_t NotificationList::NotificationCount() const {
  return notifications_.size();
}

void NotificationList::SetQuietModeInternal(bool quiet_mode) {
  quiet_mode_ = quiet_mode;
  if (quiet_mode_) {
    for (Notifications::iterator iter = notifications_.begin();
         iter != notifications_.end(); ++iter) {
      (*iter)->set_is_read(true);
      (*iter)->set_shown_as_popup(true);
    }
    unread_count_ = 0;
  }
}

NotificationList::Notifications::iterator
    NotificationList::GetNotification(const std::string& id) {
  for (Notifications::iterator iter = notifications_.begin();
       iter != notifications_.end(); ++iter) {
    if ((*iter)->id() == id)
      return iter;
  }
  return notifications_.end();
}

void NotificationList::EraseNotification(Notifications::iterator iter) {
  if (!(*iter)->is_read() && (*iter)->priority() > MIN_PRIORITY)
    --unread_count_;
  delete *iter;
  notifications_.erase(iter);
}

void NotificationList::PushNotification(scoped_ptr<Notification> notification) {
  // Ensure that notification.id is unique by erasing any existing
  // notification with the same id (shouldn't normally happen).
  Notifications::iterator iter = GetNotification(notification->id());
  bool state_inherited = false;
  if (iter != notifications_.end()) {
    notification->CopyState(*iter);
    state_inherited = true;
    EraseNotification(iter);
  }
  // Add the notification to the the list and mark it unread and unshown.
  if (!state_inherited) {
    // TODO(mukai): needs to distinguish if a notification is dismissed by
    // the quiet mode or user operation.
    notification->set_is_read(quiet_mode_);
    notification->set_shown_as_popup(message_center_visible_ || quiet_mode_);
    if (!quiet_mode_ && notification->priority() > MIN_PRIORITY)
        ++unread_count_;
  }
  // Take ownership. The notification can only be removed from the list
  // in EraseNotification(), which will delete it.
  notifications_.insert(notification.release());
}

}  // namespace message_center
