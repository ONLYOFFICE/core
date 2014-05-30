// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_BUBBLE_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_BUBBLE_H_

#include "base/memory/weak_ptr.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/views/message_bubble_base.h"

namespace message_center {

class MessageCenterView;

// Bubble for message center.
class MESSAGE_CENTER_EXPORT MessageCenterBubble
    : public MessageBubbleBase,
      public base::SupportsWeakPtr<MessageCenterBubble> {
 public:
  explicit MessageCenterBubble(MessageCenter* message_center);

  virtual ~MessageCenterBubble();

  // Overridden from MessageBubbleBase:
  virtual views::TrayBubbleView::InitParams GetInitParams(
      views::TrayBubbleView::AnchorAlignment anchor_alignment) OVERRIDE;
  virtual void InitializeContents(views::TrayBubbleView* bubble_view) OVERRIDE;
  virtual void OnBubbleViewDestroyed() OVERRIDE;
  virtual void UpdateBubbleView() OVERRIDE;
  virtual void OnMouseEnteredView() OVERRIDE;
  virtual void OnMouseExitedView() OVERRIDE;

  size_t NumMessageViewsForTest() const;

 private:
  MessageCenterView* message_center_view_;

  // The maximum height
  int max_height_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterBubble);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_VIEWS_MESSAGE_CENTER_BUBBLE_H_
