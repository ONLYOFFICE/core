// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_center_bubble.h"

#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/message_center_util.h"
#include "ui/message_center/views/message_center_view.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/widget/widget.h"

namespace message_center {

// ContentsView ////////////////////////////////////////////////////////////////

// Handles changes in MessageCenterView sizes.
class ContentsView : public views::View {
 public:
  explicit ContentsView(MessageCenterBubble* bubble, views::View* contents);
  virtual ~ContentsView();

  // Overridden from views::View:
  virtual int GetHeightForWidth(int width) OVERRIDE;

 protected:
  // Overridden from views::View:
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;

 private:
  base::WeakPtr<MessageCenterBubble> bubble_;

  DISALLOW_COPY_AND_ASSIGN(ContentsView);
};

ContentsView::ContentsView(MessageCenterBubble* bubble, views::View* contents)
    : bubble_(bubble->AsWeakPtr()) {
  SetLayoutManager(new views::FillLayout());
  AddChildView(contents);
}

ContentsView::~ContentsView() {
}

int ContentsView::GetHeightForWidth(int width) {
  DCHECK_EQ(1, child_count());
  int contents_width = std::max(width - GetInsets().width(), 0);
  int contents_height = child_at(0)->GetHeightForWidth(contents_width);
  return contents_height + GetInsets().height();
}

void ContentsView::ChildPreferredSizeChanged(View* child) {
  // TODO(dharcourt): Reduce the amount of updating this requires.
  if (bubble_.get())
    bubble_->bubble_view()->UpdateBubble();
}

// MessageCenterBubble /////////////////////////////////////////////////////////

MessageCenterBubble::MessageCenterBubble(MessageCenter* message_center)
    : MessageBubbleBase(message_center),
      message_center_view_(NULL) {
}

MessageCenterBubble::~MessageCenterBubble() {
}

views::TrayBubbleView::InitParams MessageCenterBubble::GetInitParams(
    views::TrayBubbleView::AnchorAlignment anchor_alignment) {
  views::TrayBubbleView::InitParams init_params =
      GetDefaultInitParams(anchor_alignment);
  if (IsRichNotificationEnabled()) {
    init_params.min_width += kMarginBetweenItems * 2;
    init_params.max_width += kMarginBetweenItems * 2;
  }
  init_params.max_height = max_height();
  init_params.can_activate = true;
  return init_params;
}

void MessageCenterBubble::InitializeContents(
    views::TrayBubbleView* new_bubble_view) {
  set_bubble_view(new_bubble_view);
  message_center_view_ = new MessageCenterView(message_center(), max_height());
  bubble_view()->AddChildView(new ContentsView(this, message_center_view_));
  // Resize the content of the bubble view to the given bubble size. This is
  // necessary in case of the bubble border forcing a bigger size then the
  // |new_bubble_view| actually wants. See crbug.com/169390.
  bubble_view()->Layout();
  UpdateBubbleView();
}

void MessageCenterBubble::OnBubbleViewDestroyed() {
  message_center_view_ = NULL;
}

void MessageCenterBubble::UpdateBubbleView() {
  if (!bubble_view())
    return;  // Could get called after view is closed
  const NotificationList::Notifications& notifications =
      message_center()->GetNotifications();
  message_center_view_->SetNotifications(notifications);
  bubble_view()->GetWidget()->Show();
  bubble_view()->UpdateBubble();
}

void MessageCenterBubble::OnMouseEnteredView() {
}

void MessageCenterBubble::OnMouseExitedView() {
}

size_t MessageCenterBubble::NumMessageViewsForTest() const {
  return message_center_view_->NumMessageViewsForTest();
}

}  // namespace message_center
