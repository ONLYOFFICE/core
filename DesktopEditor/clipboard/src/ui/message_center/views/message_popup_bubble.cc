// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_popup_bubble.h"

#include "base/bind.h"
#include "base/stl_util.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"
#include "ui/message_center/views/message_view.h"
#include "ui/message_center/views/notification_view.h"
#include "ui/views/bubble/tray_bubble_view.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"

namespace message_center {

// Popup notifications contents.
class PopupBubbleContentsView : public views::View {
 public:
  explicit PopupBubbleContentsView(MessageCenter* message_center);

  void Update(const NotificationList::PopupNotifications& popup_notifications);

  size_t NumMessageViews() const {
    return content_->child_count();
  }

 private:
  MessageCenter* message_center_; // Weak reference.
  views::View* content_;

  DISALLOW_COPY_AND_ASSIGN(PopupBubbleContentsView);
};

PopupBubbleContentsView::PopupBubbleContentsView(
    MessageCenter* message_center)
    : message_center_(message_center) {
  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 1));

  content_ = new views::View;
  content_->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 1));
  AddChildView(content_);

  if (get_use_acceleration_when_possible()) {
    content_->SetPaintToLayer(true);
    content_->SetFillsBoundsOpaquely(false);
    content_->layer()->SetMasksToBounds(true);
  }
}

void PopupBubbleContentsView::Update(
    const NotificationList::PopupNotifications& popup_notifications) {
  content_->RemoveAllChildViews(true);
  for (NotificationList::PopupNotifications::const_iterator iter =
           popup_notifications.begin();
       iter != popup_notifications.end(); ++iter) {
    // NotificationViews are expanded by default here because MessagePopupBubble
    // hasn't been tested yet with changing subview sizes, and such changes
    // could come if those subviews were initially collapsed and allowed to be
    // expanded by users. TODO(dharcourt): Fix.
    content_->AddChildView(
        NotificationView::Create(*(*iter), message_center_, true));
  }
  content_->SizeToPreferredSize();
  content_->InvalidateLayout();
  Layout();
  if (GetWidget())
    GetWidget()->GetRootView()->SchedulePaint();
}

// The timer to call OnAutoClose for |notification|.
class MessagePopupBubble::AutocloseTimer {
 public:
  AutocloseTimer(Notification* notification, MessagePopupBubble* bubble);

  void Start();
  void Restart();
  void Suspend();

 private:
  const std::string id_;
  base::TimeDelta delay_;
  base::Time start_time_;
  MessagePopupBubble* bubble_;
  base::OneShotTimer<MessagePopupBubble> timer_;

  DISALLOW_COPY_AND_ASSIGN(AutocloseTimer);
};

MessagePopupBubble::AutocloseTimer::AutocloseTimer(
    Notification* notification,
    MessagePopupBubble* bubble)
    : id_(notification->id()),
      bubble_(bubble) {
  int seconds = kAutocloseDefaultDelaySeconds;
  if (notification->priority() > DEFAULT_PRIORITY)
    seconds = kAutocloseHighPriorityDelaySeconds;
  delay_ = base::TimeDelta::FromSeconds(seconds);
}

void MessagePopupBubble::AutocloseTimer::Start() {
  start_time_ = base::Time::Now();
  timer_.Start(FROM_HERE,
               delay_,
               base::Bind(&MessagePopupBubble::OnAutoClose,
                          base::Unretained(bubble_), id_));
}

void MessagePopupBubble::AutocloseTimer::Restart() {
  delay_ -= base::Time::Now() - start_time_;
  if (delay_ < base::TimeDelta())
    bubble_->OnAutoClose(id_);
  else
    Start();
}

void MessagePopupBubble::AutocloseTimer::Suspend() {
  timer_.Stop();
}

// MessagePopupBubble
MessagePopupBubble::MessagePopupBubble(MessageCenter* message_center)
    : MessageBubbleBase(message_center),
      contents_view_(NULL) {
}

MessagePopupBubble::~MessagePopupBubble() {
  STLDeleteContainerPairSecondPointers(autoclose_timers_.begin(),
                                       autoclose_timers_.end());
}

views::TrayBubbleView::InitParams MessagePopupBubble::GetInitParams(
    views::TrayBubbleView::AnchorAlignment anchor_alignment) {
  views::TrayBubbleView::InitParams init_params =
      GetDefaultInitParams(anchor_alignment);
  init_params.arrow_color = kBackgroundColor;
  init_params.close_on_deactivate = false;
  return init_params;
}

void MessagePopupBubble::InitializeContents(
    views::TrayBubbleView* new_bubble_view) {
  set_bubble_view(new_bubble_view);
  contents_view_ = new PopupBubbleContentsView(message_center());
  bubble_view()->AddChildView(contents_view_);
  bubble_view()->set_notify_enter_exit_on_child(true);
  UpdateBubbleView();
}

void MessagePopupBubble::OnBubbleViewDestroyed() {
  contents_view_ = NULL;
}

void MessagePopupBubble::UpdateBubbleView() {
  NotificationList::PopupNotifications popups =
      message_center()->GetPopupNotifications();

  if (popups.size() == 0) {
    if (bubble_view())
      bubble_view()->delegate()->HideBubble(bubble_view());  // deletes |this|
    return;
  }

  contents_view_->Update(popups);
  bubble_view()->GetWidget()->Show();
  bubble_view()->UpdateBubble();

  std::set<std::string> old_popup_ids;
  old_popup_ids.swap(popup_ids_);

  // Start autoclose timers.
  for (NotificationList::PopupNotifications::const_iterator iter =
           popups.begin();
       iter != popups.end(); ++iter) {
    std::string id = (*iter)->id();
    std::map<std::string, AutocloseTimer*>::const_iterator timer_iter =
        autoclose_timers_.find(id);
    if (timer_iter == autoclose_timers_.end()) {
      AutocloseTimer *timer = new AutocloseTimer(*iter, this);
      autoclose_timers_[id] = timer;
      timer->Start();
    }
    popup_ids_.insert(id);
    old_popup_ids.erase(id);
  }

  // Stops timers whose notifications are gone.
  for (std::set<std::string>::const_iterator iter = old_popup_ids.begin();
       iter != old_popup_ids.end(); ++iter) {
    DeleteTimer(*iter);
  }
}

void MessagePopupBubble::OnMouseEnteredView() {
  for (std::map<std::string, AutocloseTimer*>::iterator iter =
           autoclose_timers_.begin(); iter != autoclose_timers_.end(); ++iter) {
    iter->second->Suspend();
  }
}

void MessagePopupBubble::OnMouseExitedView() {
  for (std::map<std::string, AutocloseTimer*>::iterator iter =
           autoclose_timers_.begin(); iter != autoclose_timers_.end();) {
    // |iter| can be deleted if timer is already over.
    AutocloseTimer* timer = iter->second;
    ++iter;
    timer->Restart();
  }
}

void MessagePopupBubble::OnAutoClose(const std::string& id) {
  if (!message_center()->HasNotification(id))
    return;

  message_center()->MarkSinglePopupAsShown(id, false);
}

void MessagePopupBubble::DeleteTimer(const std::string& id) {
  std::map<std::string, AutocloseTimer*>::iterator iter =
      autoclose_timers_.find(id);
  if (iter != autoclose_timers_.end()) {
    scoped_ptr<AutocloseTimer> release_timer(iter->second);
    autoclose_timers_.erase(iter);
  }
}

size_t MessagePopupBubble::NumMessageViewsForTest() const {
  return contents_view_->NumMessageViews();
}

}  // namespace message_center
