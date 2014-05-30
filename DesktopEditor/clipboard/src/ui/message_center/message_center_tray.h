// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_H_

#include "base/observer_list.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/message_center_tray_delegate.h"

namespace message_center {

class MessageCenter;
class MessageBubbleBase;
class MessagePopupBubble;
class QuietModeBubble;

// Implementation found with each supported platform's implementation of
// MessageCenterTrayDelegate.
MessageCenterTrayDelegate* CreateMessageCenterTray();

// Class that observes a MessageCenter. Manages the popup and message center
// bubbles. Tells the MessageCenterTrayHost when the tray is changed, as well
// as when bubbles are shown and hidden.
class MESSAGE_CENTER_EXPORT MessageCenterTray
    : public MessageCenterObserver,
      public ui::SimpleMenuModel::Delegate {
 public:
  MessageCenterTray(MessageCenterTrayDelegate* delegate,
                    message_center::MessageCenter* message_center);
  virtual ~MessageCenterTray();

  // Shows or updates the message center bubble and hides the popup bubble.
  // Returns whether the message center is visible after the call, whether or
  // not it was visible before.
  bool ShowMessageCenterBubble();

  // Returns whether the message center was visible before.
  bool HideMessageCenterBubble();

  void ToggleMessageCenterBubble();

  // Causes an update if the popup bubble is already shown.
  void ShowPopupBubble();

  // Returns whether the popup was visible before.
  bool HidePopupBubble();

  // Creates the menu model for quiet mode and returns it. The caller must
  // take the ownership of the return value.
  ui::MenuModel* CreateQuietModeMenu();

  bool message_center_visible() { return message_center_visible_; }
  bool popups_visible() { return popups_visible_; }
  MessageCenterTrayDelegate* delegate() { return delegate_; }
  const message_center::MessageCenter* message_center() const {
    return message_center_;
  }
  message_center::MessageCenter* message_center() { return message_center_; }

  // Overridden from MessageCenterObserver:
  virtual void OnNotificationAdded(const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationClicked(
      const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationButtonClicked(
      const std::string& notification_id,
      int button_index) OVERRIDE;
  virtual void OnNotificationDisplayed(
      const std::string& notification_id) OVERRIDE;

  // Overridden from SimpleMenuModel::Delegate.
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  void OnMessageCenterChanged();
  void NotifyMessageCenterTrayChanged();

  // |message_center_| is a weak pointer that must live longer than
  // MessageCenterTray.
  message_center::MessageCenter* message_center_;
  bool message_center_visible_;
  bool popups_visible_;
  // |delegate_| is a weak pointer that must live longer than MessageCenterTray.
  MessageCenterTrayDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterTray);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_H_
