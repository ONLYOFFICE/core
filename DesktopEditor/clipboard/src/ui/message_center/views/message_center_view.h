// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_VIEWS_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_VIEWS_H_

#include "ui/views/view.h"

#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/notification_list.h"

namespace views {
class Button;
}  // namespace views

namespace message_center {

class MessageCenter;
class MessageCenterBubble;
class MessageView;
class MessageListView;

// MessageCenterButtonBar //////////////////////////////////////////////////////

// If you know how to better hide this implementation class please do so, and
// otherwise please refrain from using it :-).
class MessageCenterButtonBar : public views::View {
 public:
  explicit MessageCenterButtonBar(MessageCenter* message_center);
  virtual ~MessageCenterButtonBar();

  void SetCloseAllVisible(bool visible);

 protected:
  MessageCenter* message_center() { return message_center_; }
  views::Button* close_all_button() { return close_all_button_; }
  void set_close_all_button(views::Button* button) {
    close_all_button_ = button;
  }

 private:
  MessageCenter* message_center_;  // Weak reference.
  views::Button* close_all_button_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterButtonBar);
};

// MessageCenterView ///////////////////////////////////////////////////////////

class MESSAGE_CENTER_EXPORT MessageCenterView : public views::View,
                                                public MessageCenterObserver {
 public:
  MessageCenterView(MessageCenter* message_center, int max_height);
  virtual ~MessageCenterView();

  void SetNotifications(const NotificationList::Notifications& notifications);

  size_t NumMessageViewsForTest() const;

 protected:
  // Overridden from views::View:
  virtual void Layout() OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  // Overridden from MessageCenterObserver:
  virtual void OnNotificationAdded(const std::string& id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(const std::string& id) OVERRIDE;

 private:
  friend class MessageCenterViewTest;

  void AddNotificationAt(const Notification& notification, int index);
  void NotificationsChanged();
  void SetNotificationViewForTest(views::View* view);

  MessageCenter* message_center_;  // Weak reference.
  std::vector<MessageView*> message_views_;
  views::ScrollView* scroller_;
  MessageListView* message_list_view_;
  MessageCenterButtonBar* button_bar_;
  views::View* no_notifications_message_view_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterView);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_VIEWS_H_
