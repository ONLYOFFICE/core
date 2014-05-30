// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_NOTIFICATION_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_NOTIFICATION_VIEW_H_

#include <vector>

#include "ui/message_center/message_center_export.h"
#include "ui/message_center/views/message_view.h"

namespace message_center {

class BoundedLabel;
class MessageCenter;

// View that displays all current types of notification (web, basic, image, and
// list). Future notification types may be handled by other classes, in which
// case instances of those classes would be returned by the Create() factory
// method below.
class MESSAGE_CENTER_EXPORT NotificationView : public MessageView {
 public:
  // Creates appropriate MessageViews for notifications. Those currently are
  // always NotificationView or MessageSimpleView instances but in the future
  // may be instances of other classes, with the class depending on the
  // notification type.
  static MessageView* Create(const Notification& notification,
                             MessageCenter* message_center,
                             bool expanded);

  virtual ~NotificationView();

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ScrollRectToVisible(const gfx::Rect& rect) OVERRIDE;

  // Overridden from MessageView:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 protected:
  NotificationView(const Notification& notification,
                   MessageCenter* message_center,
                   bool expanded);

 private:
  bool IsExpansionNeeded(int width);
  bool IsMessageExpansionNeeded(int width);
  int GetMessageLineLimit(int width);
  int GetMessageLines(int width, int limit);
  int GetMessageHeight(int width, int limit);

  // Weak references to NotificationView descendants owned by their parents.
  views::View* background_view_;
  views::View* top_view_;
  BoundedLabel* title_view_;
  BoundedLabel* message_view_;
  std::vector<views::View*> item_views_;
  views::View* icon_view_;
  views::View* bottom_view_;
  views::View* image_view_;
  std::vector<views::View*> action_buttons_;

  DISALLOW_COPY_AND_ASSIGN(NotificationView);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_VIEWS_NOTIFICATION_VIEW_H_
