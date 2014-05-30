// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_BUBBLE_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_BUBBLE_H_

#include <map>
#include <set>
#include <string>

#include "base/timer.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/views/message_bubble_base.h"

namespace message_center {

class PopupBubbleContentsView;

// Bubble for popup notifications.
class MESSAGE_CENTER_EXPORT MessagePopupBubble : public MessageBubbleBase {
 public:
  explicit MessagePopupBubble(MessageCenter* message_center);

  virtual ~MessagePopupBubble();

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
   class AutocloseTimer;

  void OnAutoClose(const std::string& id);

  void DeleteTimer(const std::string& id);

  std::map<std::string, AutocloseTimer*> autoclose_timers_;
  PopupBubbleContentsView* contents_view_;
  std::set<std::string> popup_ids_;

  DISALLOW_COPY_AND_ASSIGN(MessagePopupBubble);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_BUBBLE_H_
