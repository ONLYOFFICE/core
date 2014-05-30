// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/message_center_impl.h"

#include "base/observer_list.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_list.h"

namespace message_center {

MessageCenterImpl::MessageCenterImpl()
    : MessageCenter(),
      delegate_(NULL) {
  notification_list_.reset(new NotificationList());
}

MessageCenterImpl::~MessageCenterImpl() {
  notification_list_.reset();
}

void MessageCenterImpl::AddObserver(MessageCenterObserver* observer) {
  observer_list_.AddObserver(observer);
}

void MessageCenterImpl::RemoveObserver(MessageCenterObserver* observer) {
  observer_list_.RemoveObserver(observer);
}

void MessageCenterImpl::SetDelegate(Delegate* delegate) {
  delegate_ = delegate;
}

void MessageCenterImpl::SetMessageCenterVisible(bool visible) {
  std::set<std::string> updated_ids;
  notification_list_->SetMessageCenterVisible(visible, &updated_ids);
  for (std::set<std::string>::const_iterator iter = updated_ids.begin();
       iter != updated_ids.end(); ++iter) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(*iter));
  }
}

size_t MessageCenterImpl::NotificationCount() const {
  return notification_list_->NotificationCount();
}

size_t MessageCenterImpl::UnreadNotificationCount() const {
  return notification_list_->unread_count();
}

bool MessageCenterImpl::HasPopupNotifications() const {
  return notification_list_->HasPopupNotifications();
}

bool MessageCenterImpl::HasNotification(const std::string& id) {
  return notification_list_->HasNotification(id);
}

bool MessageCenterImpl::IsQuietMode() const {
  return notification_list_->quiet_mode();
}

const NotificationList::Notifications& MessageCenterImpl::GetNotifications() {
  return notification_list_->GetNotifications();
}

NotificationList::PopupNotifications
    MessageCenterImpl::GetPopupNotifications() {
  return notification_list_->GetPopupNotifications();
}

//------------------------------------------------------------------------------
// Client code interface.

void MessageCenterImpl::AddNotification(
    NotificationType type,
    const std::string& id,
    const string16& title,
    const string16& message,
    const string16& display_source,
    const std::string& extension_id,
    const base::DictionaryValue* optional_fields) {
  // Sometimes the notification can be added with the same id and the
  // |notification_list| will replace the notification instead of adding new.
  // This is essentially an update rather than addition.
  bool already_exists = notification_list_->HasNotification(id);
  notification_list_->AddNotification(type, id, title, message, display_source,
                                      extension_id, optional_fields);
  if (already_exists) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(id));
  } else {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationAdded(id));
  }
}

void MessageCenterImpl::UpdateNotification(
    const std::string& old_id,
    const std::string& new_id,
    const string16& title,
    const string16& message,
    const base::DictionaryValue* optional_fields) {
  notification_list_->UpdateNotificationMessage(
      old_id, new_id, title, message, optional_fields);
  if (old_id == new_id) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(new_id));
  } else {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationRemoved(old_id, false));
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationAdded(new_id));
  }
}

void MessageCenterImpl::RemoveNotification(const std::string& id,
                                           bool by_user) {
  if (!HasNotification(id))
    return;

  // In many cases |id| is a reference to an existing notification instance
  // but the instance can be destructed in RemoveNotification(). Hence
  // copies the id explicitly here.
  std::string copied_id(id);
  notification_list_->RemoveNotification(copied_id);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationRemoved(copied_id, by_user));
}

void MessageCenterImpl::RemoveAllNotifications(bool by_user) {
  const NotificationList::Notifications& notifications =
      notification_list_->GetNotifications();
  std::set<std::string> ids;
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end(); ++iter) {
    ids.insert((*iter)->id());
  }
  notification_list_->RemoveAllNotifications();

  for (std::set<std::string>::const_iterator iter = ids.begin();
       iter != ids.end(); ++iter) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationRemoved(*iter, by_user));
  }
}

void MessageCenterImpl::SetNotificationIcon(const std::string& notification_id,
                                        const gfx::Image& image) {
  if (notification_list_->SetNotificationIcon(notification_id, image)) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(notification_id));
  }
}

void MessageCenterImpl::SetNotificationImage(const std::string& notification_id,
                                         const gfx::Image& image) {
  if (notification_list_->SetNotificationImage(notification_id, image)) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(notification_id));
  }
}

void MessageCenterImpl::SetNotificationButtonIcon(
    const std::string& notification_id, int button_index,
    const gfx::Image& image) {
  if (!HasNotification(notification_id))
    return;
  if (notification_list_->SetNotificationButtonIcon(notification_id,
                                                    button_index, image)) {
    FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                      OnNotificationUpdated(notification_id));
  }
}

void MessageCenterImpl::DisableNotificationsByExtension(
    const std::string& id) {
  if (delegate_)
    delegate_->DisableExtension(id);

  NotificationList::Notifications notifications =
      notification_list_->GetNotificationsByExtension(id);
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end();) {
    std::string id = (*iter)->id();
    iter++;
    RemoveNotification(id, false);
  }
}

void MessageCenterImpl::DisableNotificationsByUrl(const std::string& id) {
  if (delegate_)
    delegate_->DisableNotificationsFromSource(id);

  NotificationList::Notifications notifications =
      notification_list_->GetNotificationsBySource(id);
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end();) {
    std::string id = (*iter)->id();
    iter++;
    RemoveNotification(id, false);
  }
}

void MessageCenterImpl::ShowNotificationSettings(const std::string& id) {
  if (delegate_)
    delegate_->ShowSettings(id);
}

void MessageCenterImpl::ShowNotificationSettingsDialog(
    gfx::NativeView context) {
  if (delegate_)
    delegate_->ShowSettingsDialog(context);
}

void MessageCenterImpl::ExpandNotification(const std::string& id) {
  if (!HasNotification(id))
    return;
  notification_list_->MarkNotificationAsExpanded(id);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationUpdated(id));
}

void MessageCenterImpl::ClickOnNotification(const std::string& id) {
  if (!HasNotification(id))
    return;
  if (HasPopupNotifications())
    MarkSinglePopupAsShown(id, true);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationClicked(id));
}

void MessageCenterImpl::ClickOnNotificationButton(const std::string& id,
                                              int button_index) {
  if (!HasNotification(id))
    return;
  if (HasPopupNotifications())
    MarkSinglePopupAsShown(id, true);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationButtonClicked(id, button_index));
}

void MessageCenterImpl::MarkSinglePopupAsShown(const std::string& id,
                                               bool mark_notification_as_read) {
  if (!HasNotification(id))
    return;
  notification_list_->MarkSinglePopupAsShown(id, mark_notification_as_read);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationUpdated(id));
}

void MessageCenterImpl::DisplayedNotification(const std::string& id) {
  if (!HasNotification(id))
    return;

  if (HasPopupNotifications())
    notification_list_->MarkSinglePopupAsDisplayed(id);
  FOR_EACH_OBSERVER(MessageCenterObserver, observer_list_,
                    OnNotificationDisplayed(id));
}

void MessageCenterImpl::SetQuietMode(bool in_quiet_mode) {
  notification_list_->SetQuietMode(in_quiet_mode);
}

void MessageCenterImpl::EnterQuietModeWithExpire(
    const base::TimeDelta& expires_in) {
  notification_list_->EnterQuietModeWithExpire(expires_in);
}

}  // namespace message_center
