// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/message_center_tray.h"

#include "base/observer_list.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_tray_delegate.h"

namespace message_center {
namespace {

// Menu commands
const int kToggleQuietMode = 0;
const int kEnableQuietModeHour = 1;
const int kEnableQuietModeDay = 2;

}

MessageCenterTray::MessageCenterTray(
    MessageCenterTrayDelegate* delegate,
    message_center::MessageCenter* message_center)
    : message_center_(message_center),
      message_center_visible_(false),
      popups_visible_(false),
      delegate_(delegate) {
  message_center_->AddObserver(this);
}

MessageCenterTray::~MessageCenterTray() {
  message_center_->RemoveObserver(this);
}

bool MessageCenterTray::ShowMessageCenterBubble() {
  if (message_center_visible_)
    return true;

  HidePopupBubble();

  message_center_visible_ = delegate_->ShowMessageCenter();
  message_center_->SetMessageCenterVisible(message_center_visible_);
  return message_center_visible_;
}

bool MessageCenterTray::HideMessageCenterBubble() {
  if (!message_center_visible_)
    return false;
  delegate_->HideMessageCenter();
  message_center_visible_ = false;
  message_center_->SetMessageCenterVisible(false);
  NotifyMessageCenterTrayChanged();
  return true;
}

void MessageCenterTray::ToggleMessageCenterBubble() {
  if (message_center_visible_)
    HideMessageCenterBubble();
  else
    ShowMessageCenterBubble();
}

void MessageCenterTray::ShowPopupBubble() {
  if (message_center_visible_)
    return;

  if (popups_visible_) {
    delegate_->UpdatePopups();
    NotifyMessageCenterTrayChanged();
    return;
  }

  if (!message_center_->HasPopupNotifications())
    return;

  popups_visible_ = delegate_->ShowPopups();
  NotifyMessageCenterTrayChanged();
}

bool MessageCenterTray::HidePopupBubble() {
  if (!popups_visible_)
    return false;

  delegate_->HidePopups();
  popups_visible_ = false;
  NotifyMessageCenterTrayChanged();

  return true;
}

ui::MenuModel* MessageCenterTray::CreateQuietModeMenu() {
  ui::SimpleMenuModel* menu = new ui::SimpleMenuModel(this);

  menu->AddCheckItem(kToggleQuietMode,
                     l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_QUIET_MODE));
  menu->AddItem(kEnableQuietModeHour,
                l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_QUIET_MODE_1HOUR));
  menu->AddItem(kEnableQuietModeDay,
                l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_QUIET_MODE_1DAY));
  return menu;
}

void MessageCenterTray::OnNotificationAdded(
    const std::string& notification_id) {
  OnMessageCenterChanged();
}

void MessageCenterTray::OnNotificationRemoved(
    const std::string& notification_id,
    bool by_user) {
  OnMessageCenterChanged();
}

void MessageCenterTray::OnNotificationUpdated(
    const std::string& notification_id) {
  OnMessageCenterChanged();
}

void MessageCenterTray::OnNotificationClicked(
    const std::string& notification_id) {
  if (popups_visible_)
    OnMessageCenterChanged();
}

void MessageCenterTray::OnNotificationButtonClicked(
    const std::string& notification_id,
    int button_index) {
  if (popups_visible_)
    OnMessageCenterChanged();
}

void MessageCenterTray::OnNotificationDisplayed(
    const std::string& notification_id) {
  NotifyMessageCenterTrayChanged();
}

void MessageCenterTray::OnMessageCenterChanged() {
  if (message_center_visible_) {
    if (message_center_->NotificationCount() == 0)
      HideMessageCenterBubble();
  }
  if (popups_visible_) {
    if (message_center_->HasPopupNotifications())
      delegate_->UpdatePopups();
    else
      HidePopupBubble();
  } else if (message_center_->HasPopupNotifications()) {
    ShowPopupBubble();
  }

  NotifyMessageCenterTrayChanged();
}

bool MessageCenterTray::IsCommandIdChecked(int command_id) const {
  if (command_id != kToggleQuietMode)
    return false;
  return message_center()->IsQuietMode();
}

bool MessageCenterTray::IsCommandIdEnabled(int command_id) const {
  return true;
}

bool MessageCenterTray::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) {
  return false;
}

void MessageCenterTray::ExecuteCommand(int command_id, int event_flags) {
  if (command_id == kToggleQuietMode) {
    bool in_quiet_mode = message_center()->IsQuietMode();
    message_center()->SetQuietMode(!in_quiet_mode);
    NotifyMessageCenterTrayChanged();
    return;
  }
  base::TimeDelta expires_in = command_id == kEnableQuietModeDay ?
      base::TimeDelta::FromDays(1):
      base::TimeDelta::FromHours(1);
  message_center()->EnterQuietModeWithExpire(expires_in);
  NotifyMessageCenterTrayChanged();
}

void MessageCenterTray::NotifyMessageCenterTrayChanged() {
  delegate_->OnMessageCenterTrayChanged();
}

}  // namespace message_center
