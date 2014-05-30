// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_FAKE_MESSAGE_CENTER_H_
#define UI_MESSAGE_CENTER_FAKE_MESSAGE_CENTER_H_

#include "ui/message_center/message_center.h"

namespace message_center {

// MessageCenter implementation of doing nothing. Useful for tests.
class FakeMessageCenter : public MessageCenter {
 public:
  FakeMessageCenter();
  virtual ~FakeMessageCenter();

  // Overridden from FakeMessageCenter.
  virtual void SetDelegate(Delegate* delegate) OVERRIDE;
  virtual void AddObserver(MessageCenterObserver* observer) OVERRIDE;
  virtual void RemoveObserver(MessageCenterObserver* observer) OVERRIDE;
  virtual size_t NotificationCount() const OVERRIDE;
  virtual size_t UnreadNotificationCount() const OVERRIDE;
  virtual bool HasPopupNotifications() const OVERRIDE;
  virtual bool HasNotification(const std::string& id) OVERRIDE;
  virtual bool IsQuietMode() const OVERRIDE;
  virtual const NotificationList::Notifications& GetNotifications() OVERRIDE;
  virtual NotificationList::PopupNotifications GetPopupNotifications() OVERRIDE;
  virtual void AddNotification(
      NotificationType type,
      const std::string& id,
      const string16& title,
      const string16& message,
      const string16& display_source,
      const std::string& extension_id,
      const base::DictionaryValue* optional_fields) OVERRIDE;
  virtual void UpdateNotification(
      const std::string& old_id,
      const std::string& new_id,
      const string16& title,
      const string16& message,
      const base::DictionaryValue* optional_fields) OVERRIDE;
  virtual void RemoveNotification(const std::string& id, bool by_user) OVERRIDE;
  virtual void RemoveAllNotifications(bool by_user) OVERRIDE;
  virtual void SetNotificationIcon(const std::string& notification_id,
                                   const gfx::Image& image) OVERRIDE;

  virtual void SetNotificationImage(const std::string& notification_id,
                                    const gfx::Image& image) OVERRIDE;

  virtual void SetNotificationButtonIcon(const std::string& notification_id,
                                         int button_index,
                                         const gfx::Image& image) OVERRIDE;
  virtual void DisableNotificationsByExtension(const std::string& id) OVERRIDE;
  virtual void DisableNotificationsByUrl(const std::string& id) OVERRIDE;
  virtual void ShowNotificationSettings(const std::string& id) OVERRIDE;
  virtual void ShowNotificationSettingsDialog(gfx::NativeView context) OVERRIDE;
  virtual void ExpandNotification(const std::string& id) OVERRIDE;
  virtual void ClickOnNotification(const std::string& id) OVERRIDE;
  virtual void ClickOnNotificationButton(const std::string& id,
                                         int button_index) OVERRIDE;
  virtual void MarkSinglePopupAsShown(const std::string& id,
                                      bool mark_notification_as_read) OVERRIDE;
  virtual void DisplayedNotification(const std::string& id) OVERRIDE;
  virtual void SetQuietMode(bool in_quiet_mode) OVERRIDE;
  virtual void EnterQuietModeWithExpire(
      const base::TimeDelta& expires_in) OVERRIDE;
  virtual void SetMessageCenterVisible(bool visible) OVERRIDE;

 private:
  const NotificationList::Notifications empty_notifications_;

  DISALLOW_COPY_AND_ASSIGN(FakeMessageCenter);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_FAKE_MESSAGE_CENTER_H_
