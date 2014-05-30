// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_SIMPLE_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_SIMPLE_VIEW_H_

#include "ui/message_center/views/message_view.h"

namespace message_center {

class MessageCenter;
class Notification;

// A simple view for a notification entry (icon + message + buttons).
class MessageSimpleView : public MessageView {
 public:
  MessageSimpleView(const Notification& notification,
                    MessageCenter* message_center);
  virtual ~MessageSimpleView();

  // Overridden from MessageView:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 protected:
  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;

  MessageSimpleView();

 private:
  void SetUpView(const Notification& notification);

  scoped_ptr<views::ImageButton> old_style_close_button_;
  scoped_ptr<views::View> content_view_;

  DISALLOW_COPY_AND_ASSIGN(MessageSimpleView);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_VIEWS_MESSAGE_SIMPLE_VIEW_H_
