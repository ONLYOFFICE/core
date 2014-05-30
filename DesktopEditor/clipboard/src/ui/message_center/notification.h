// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFICATION_H_
#define UI_MESSAGE_CENTER_NOTIFICATION_H_

#include <string>
#include <vector>

#include "base/string16.h"
#include "base/time.h"
#include "base/values.h"
#include "ui/gfx/image/image.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notification_types.h"

namespace message_center {

struct MESSAGE_CENTER_EXPORT NotificationItem {
  string16 title;
  string16 message;

  NotificationItem(const string16& title, const string16& message);
};

struct MESSAGE_CENTER_EXPORT ButtonInfo {
  string16 title;
  gfx::Image icon;

  ButtonInfo(const string16& title);
};

class MESSAGE_CENTER_EXPORT Notification {
 public:
  Notification(NotificationType type,
               const std::string& id,
               const string16& title,
               const string16& message,
               const string16& display_source,
               const std::string& extension_id,
               const DictionaryValue* optional_fields);  // May be NULL.
  virtual ~Notification();

  // Copies the internal on-memory state from |base|, i.e. shown_as_popup,
  // is_read, is_expanded, and never_timeout.
  void CopyState(Notification* base);

  NotificationType type() const { return type_; }
  const std::string& id() const { return id_; }
  const string16& title() const { return title_; }
  const string16& message() const { return message_; }
  const string16& display_source() const { return display_source_; }
  const std::string& extension_id() const { return extension_id_; }

  // Begin unpacked values from optional_fields.
  int priority() const { return priority_; }
  base::Time timestamp() const { return timestamp_; }
  const string16& expanded_message() const { return expanded_message_; }
  const std::vector<NotificationItem>& items() const { return items_; }
  // End unpacked values.

  // Images fetched asynchronously.
  const gfx::Image& icon() const { return icon_; }
  void set_icon(const gfx::Image& icon) { icon_ = icon; }

  const gfx::Image& image() const { return image_; }
  void set_image(const gfx::Image& image) { image_ = image; }

  // Buttons, with icons fetched asynchronously.
  const std::vector<ButtonInfo>& buttons() const { return buttons_; }
  bool SetButtonIcon(size_t index, const gfx::Image& icon);

  bool shown_as_popup() const { return shown_as_popup_; }
  void set_shown_as_popup(bool shown_as_popup) {
    shown_as_popup_ = shown_as_popup;
  }

  // Read status in the message center.
  bool is_read() const { return is_read_; }
  void set_is_read(bool read) { is_read_ = read; }

  // Expanded status in the message center (not the popups).
  bool is_expanded() const { return is_expanded_; }
  void set_is_expanded(bool expanded) { is_expanded_ = expanded; }

  // Used to keep the order of notifications with the same timestamp.
  // The notification with lesser serial_number is considered 'older'.
  unsigned serial_number() { return serial_number_; }

  bool never_timeout() const { return never_timeout_; }

 private:
  // Unpacks the provided |optional_fields| and applies the values to override
  // the notification's data members.
  void ApplyOptionalFields(const DictionaryValue* optional_fields);

  NotificationType type_;
  std::string id_;
  string16 title_;
  string16 message_;
  string16 display_source_;
  std::string extension_id_;
  int priority_;
  base::Time timestamp_;
  unsigned serial_number_;
  string16 expanded_message_;
  std::vector<NotificationItem> items_;
  gfx::Image icon_;
  gfx::Image image_;
  std::vector<ButtonInfo> buttons_;
  bool shown_as_popup_;  // True if this has been shown as a popup.
  bool is_read_;  // True if this has been seen in the message center.
  bool is_expanded_;  // True if this has been expanded in the message center.
  bool never_timeout_; // True if it doesn't timeout when it appears as a toast.

  DISALLOW_COPY_AND_ASSIGN(Notification);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_NOTIFICATION_H_
