// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notification_list.h"
#include "ui/message_center/notification_types.h"

namespace base {
class DictionaryValue;
}

// Interface to manage the NotificationList. The client (e.g. Chrome) calls
// [Add|Remove|Update]Notification to create and update notifications in the
// list. It also sends those changes to its observers when a notification
// is shown, closed, or clicked on.
// It can also implement Delegate to ask platform-dependent features like
// disabling extensions or opening settings.

namespace message_center {

class MessageCenterObserver;
class NotificationList;

class MESSAGE_CENTER_EXPORT MessageCenter {
 public:
  // Creates the global message center object.
  static void Initialize();

  // Returns the global message center object. Initialize must be called first.
  static MessageCenter* Get();

  // Destroys the global message_center object.
  static void Shutdown();

  class MESSAGE_CENTER_EXPORT Delegate {
   public:
    virtual ~Delegate();

    // Request to disable the extension associated with |notification_id|.
    virtual void DisableExtension(const std::string& notification_id) = 0;

    // Request to disable notifications from the source of |notification_id|.
    virtual void DisableNotificationsFromSource(
        const std::string& notification_id) = 0;

    // Request to show the notification settings (|notification_id| is used
    // to identify the requesting browser context).
    virtual void ShowSettings(const std::string& notification_id) = 0;

    // Request to show the notification settings dialog. |context| is necessary
    // to create a new window.
    virtual void ShowSettingsDialog(gfx::NativeView context) = 0;
  };

  // Called to set the delegate.  Generally called only once, except in tests.
  // Changing the delegate does not affect notifications in its
  // NotificationList.
  virtual void SetDelegate(Delegate* delegate) = 0;

  // Management of the observer list.
  virtual void AddObserver(MessageCenterObserver* observer) = 0;
  virtual void RemoveObserver(MessageCenterObserver* observer) = 0;

  // Queries of current notification list status.
  virtual size_t NotificationCount() const = 0;
  virtual size_t UnreadNotificationCount() const = 0;
  virtual bool HasPopupNotifications() const = 0;
  virtual bool HasNotification(const std::string& id) = 0;
  virtual bool IsQuietMode() const = 0;

  // Getters of the current notifications.
  virtual const NotificationList::Notifications& GetNotifications() = 0;
  virtual NotificationList::PopupNotifications GetPopupNotifications() = 0;

  // Basic operations of notification: add/remove/update.

  // Adds a new notification. |id| is a unique identifier, used to update or
  // remove notifications. |title| and |meesage| describe the notification text.
  // Use SetNotificationIcon, SetNotificationImage, or SetNotificationButtonIcon
  // to set images. If |extension_id| is provided then 'Disable extension' will
  // appear in a dropdown menu and the id will be used to disable notifications
  // from the extension. Otherwise if |display_source| is provided, a menu item
  // showing the source and allowing notifications from that source to be
  // disabled will be shown. All actual disabling is handled by the Delegate.
  virtual void AddNotification(
      NotificationType type,
      const std::string& id,
      const string16& title,
      const string16& message,
      const string16& display_source,
      const std::string& extension_id,
      const base::DictionaryValue* optional_fields) = 0;

  // Updates an existing notification with id = old_id and set its id to new_id.
  // |optional_fields| can be NULL in case of no updates on those fields.
  virtual void UpdateNotification(
      const std::string& old_id,
      const std::string& new_id,
      const string16& title,
      const string16& message,
      const base::DictionaryValue* optional_fields) = 0;

  // Removes an existing notification.
  virtual void RemoveNotification(const std::string& id, bool by_user) = 0;
  virtual void RemoveAllNotifications(bool by_user) = 0;

  // Sets the icon image. Icon appears at the top-left of the notification.
  virtual void SetNotificationIcon(const std::string& notification_id,
                                   const gfx::Image& image) = 0;

  // Sets the large image for the notifications of type == TYPE_IMAGE. Specified
  // image will appear below of the notification.
  virtual void SetNotificationImage(const std::string& notification_id,
                                    const gfx::Image& image) = 0;

  // Sets the image for the icon of the specific aaction button.
  virtual void SetNotificationButtonIcon(const std::string& notification_id,
                                         int button_index,
                                         const gfx::Image& image) = 0;

  // Operations happening especially from GUIs: click, expand, disable,
  // and settings.
  // TODO(mukai): settings can be in another class?
  virtual void DisableNotificationsByExtension(const std::string& id) = 0;
  virtual void DisableNotificationsByUrl(const std::string& id) = 0;
  virtual void ShowNotificationSettings(const std::string& id) = 0;
  virtual void ShowNotificationSettingsDialog(gfx::NativeView context) = 0;
  virtual void ExpandNotification(const std::string& id) = 0;
  virtual void ClickOnNotification(const std::string& id) = 0;
  virtual void ClickOnNotificationButton(const std::string& id,
                                         int button_index) = 0;
  virtual void MarkSinglePopupAsShown(const std::string& id,
                                      bool mark_notification_as_read) = 0;
  virtual void DisplayedNotification(const std::string& id) = 0;
  virtual void SetQuietMode(bool in_quiet_mode) = 0;
  virtual void EnterQuietModeWithExpire(const base::TimeDelta& expires_in) = 0;
  // Informs the notification list whether the message center is visible.
  // This affects whether or not a message has been "read".
  virtual void SetMessageCenterVisible(bool visible) = 0;

 protected:
  MessageCenter();
  virtual ~MessageCenter();

 private:
  DISALLOW_COPY_AND_ASSIGN(MessageCenter);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_MESSAGE_CENTER_H_
