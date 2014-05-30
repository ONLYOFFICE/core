// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/notification.h"

#include "base/logging.h"
#include "ui/message_center/notification_types.h"

namespace {
unsigned g_next_serial_number_ = 0;
}

namespace message_center {

NotificationItem::NotificationItem(const string16& title,
                                   const string16& message)
 : title(title),
   message(message) {
}

ButtonInfo::ButtonInfo(const string16& title)
 : title(title) {
}

Notification::Notification(NotificationType type,
                           const std::string& id,
                           const string16& title,
                           const string16& message,
                           const string16& display_source,
                           const std::string& extension_id,
                           const DictionaryValue* optional_fields)
 : type_(type),
   id_(id),
   title_(title),
   message_(message),
   display_source_(display_source),
   extension_id_(extension_id),
   priority_(DEFAULT_PRIORITY),
   timestamp_(base::Time::Now()),
   serial_number_(g_next_serial_number_++),
   shown_as_popup_(false),
   is_read_(false),
   is_expanded_(false),
   never_timeout_(false) {
  // This can override some data members initialized to deafule values above.
  ApplyOptionalFields(optional_fields);
}

Notification::~Notification() {
}

void Notification::CopyState(Notification* base) {
  shown_as_popup_ = base->shown_as_popup();
  is_read_ = base->is_read();
  is_expanded_ = base->is_expanded();
  never_timeout_ = base->never_timeout();
}

bool Notification::SetButtonIcon(size_t index, const gfx::Image& icon) {
  if (index >= buttons_.size())
    return false;
  buttons_[index].icon = icon;
  return true;
}

void Notification::ApplyOptionalFields(const DictionaryValue* fields) {
  if (!fields)
    return;

  fields->GetInteger(kPriorityKey, &priority_);
  if (fields->HasKey(kTimestampKey)) {
    std::string time_string;
    fields->GetString(kTimestampKey, &time_string);
    base::Time::FromString(time_string.c_str(), &timestamp_);
  }
  if (fields->HasKey(kButtonOneTitleKey) ||
      fields->HasKey(kButtonOneIconUrlKey)) {
    string16 title;
    string16 icon;
    if (fields->GetString(kButtonOneTitleKey, &title) ||
        fields->GetString(kButtonOneIconUrlKey, &icon)) {
      buttons_.push_back(ButtonInfo(title));
      if (fields->GetString(kButtonTwoTitleKey, &title) ||
          fields->GetString(kButtonTwoIconUrlKey, &icon)) {
        buttons_.push_back(ButtonInfo(title));
      }
    }
  }
  fields->GetString(kExpandedMessageKey, &expanded_message_);
  if (fields->HasKey(kItemsKey)) {
    const ListValue* items;
    CHECK(fields->GetList(kItemsKey, &items));
    for (size_t i = 0; i < items->GetSize(); ++i) {
      string16 title;
      string16 message;
      const base::DictionaryValue* item;
      items->GetDictionary(i, &item);
      item->GetString(kItemTitleKey, &title);
      item->GetString(kItemMessageKey, &message);
      items_.push_back(NotificationItem(title, message));
    }
  }

  fields->GetBoolean(kPrivateNeverTimeoutKey, &never_timeout_);
}

}  // namespace message_center
