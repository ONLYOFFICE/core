// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_popup_collection.h"

#include <set>

#include "base/bind.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/memory/weak_ptr.h"
#include "base/run_loop.h"
#include "base/time.h"
#include "base/timer.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/gfx/screen.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_list.h"
#include "ui/message_center/views/notification_view.h"
#include "ui/message_center/views/toast_contents_view.h"
#include "ui/views/background.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace message_center {
namespace {

// Timeout between the last user-initiated close of the toast and the moment
// when normal layout/update of the toast stack continues. If the last toast was
// just closed, the timeout is shorter.
const int kMouseExitedDeferTimeoutMs = 200;

const int kToastMargin = kMarginBetweenItems;

}  // namespace.

MessagePopupCollection::MessagePopupCollection(gfx::NativeView parent,
                                               MessageCenter* message_center)
    : parent_(parent),
      message_center_(message_center),
      defer_counter_(0),
      latest_toast_entered_(NULL),
      user_is_closing_toasts_by_clicking_(false) {
  DCHECK(message_center_);
  defer_timer_.reset(new base::OneShotTimer<MessagePopupCollection>);
  DoUpdateIfPossible();
  message_center_->AddObserver(this);
}

MessagePopupCollection::~MessagePopupCollection() {
  message_center_->RemoveObserver(this);
  CloseAllWidgets();
}

void MessagePopupCollection::RemoveToast(ToastContentsView* toast) {
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    if ((*iter) == toast) {
      toasts_.erase(iter);
      break;
    }
  }
}

void MessagePopupCollection::UpdateWidgets() {
  NotificationList::PopupNotifications popups =
      message_center_->GetPopupNotifications();

  if (popups.empty()) {
    CloseAllWidgets();
    return;
  }

  gfx::Point base_position = GetWorkAreaBottomRight();
#if defined(OS_CHROMEOS)
  // In ChromeOS, RTL UI language mirrors the whole desktop layout, so the toast
  // widgets should be at the bottom-left instead of bottom right.
  if (base::i18n::IsRTL())
    base_position.set_x(work_area_.x() + kToastMargin);
#endif
  int bottom = toasts_.empty() ?
      base_position.y() : toasts_.back()->origin().y();
  bottom -= kToastMargin;
  // Iterate in the reverse order to keep the oldest toasts on screen. Newer
  // items may be ignored if there are no room to place them.
  for (NotificationList::PopupNotifications::const_reverse_iterator iter =
           popups.rbegin(); iter != popups.rend(); ++iter) {
    if (HasToast((*iter)->id()))
      continue;

    MessageView* view =
        NotificationView::Create(*(*iter), message_center_, true);
    int view_height = ToastContentsView::GetToastSizeForView(view).height();
    if (bottom - view_height - kToastMargin < 0) {
      delete view;
      break;
    }

    ToastContentsView* toast = new ToastContentsView(
        *iter, AsWeakPtr(), message_center_);
    toast->CreateWidget(parent_);
    toast->SetContents(view);
    toasts_.push_back(toast);

    gfx::Point origin(base_position.x() - kToastMargin, bottom);
#if defined(OS_CHROMEOS)
    if (base::i18n::IsRTL())
      origin.set_x(base_position.x() + toast->GetPreferredSize().width());
#endif
    toast->RevealWithAnimation(origin);
    bottom -= view_height + kToastMargin;

    message_center_->DisplayedNotification((*iter)->id());
  }
}

void MessagePopupCollection::OnMouseEntered(ToastContentsView* toast_entered) {
  // Sometimes we can get two MouseEntered/MouseExited in a row when animating
  // toasts.  So we need to keep track of which one is the currently active one.
  latest_toast_entered_ = toast_entered;

  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    (*iter)->SuspendTimer();
  }

  if (user_is_closing_toasts_by_clicking_)
    defer_timer_->Stop();
}

void MessagePopupCollection::OnMouseExited(ToastContentsView* toast_exited) {
  // If we're exiting a toast after entering a different toast, then ignore
  // this mouse event.
  if (toast_exited != latest_toast_entered_)
    return;
  latest_toast_entered_ = NULL;

  if (user_is_closing_toasts_by_clicking_) {
    defer_timer_->Start(
        FROM_HERE,
        base::TimeDelta::FromMilliseconds(kMouseExitedDeferTimeoutMs),
        this,
        &MessagePopupCollection::OnDeferTimerExpired);
  } else {
    for (Toasts::iterator iter = toasts_.begin();
         iter != toasts_.end();
         ++iter) {
      (*iter)->StartTimer();
    }
  }
}

void MessagePopupCollection::CloseAllWidgets() {
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end();) {
    // the toast can be removed from toasts_ during CloseWithAnimation().
    Toasts::iterator curiter = iter++;
    (*curiter)->CloseWithAnimation(true);
  }
  DCHECK(toasts_.empty());
}

gfx::Point MessagePopupCollection::GetWorkAreaBottomRight() {
  if (!work_area_.IsEmpty())
    return work_area_.bottom_right();

  if (!parent_) {
    // On Win+Aura, we don't have a parent since the popups currently show up
    // on the Windows desktop, not in the Aura/Ash desktop.  This code will
    // display the popups on the primary display.
    gfx::Screen* screen = gfx::Screen::GetNativeScreen();
    work_area_ = screen->GetPrimaryDisplay().work_area();
  } else {
    gfx::Screen* screen = gfx::Screen::GetScreenFor(parent_);
    work_area_ = screen->GetDisplayNearestWindow(parent_).work_area();
  }

  return work_area_.bottom_right();
}

void MessagePopupCollection::RepositionWidgets() {
  int bottom = GetWorkAreaBottomRight().y() - kToastMargin;
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end();) {
    Toasts::iterator curr = iter++;
    gfx::Rect bounds((*curr)->bounds());
    bounds.set_y(bottom - bounds.height());
    // The notification may scrolls the top boundary of the screen due to image
    // load and such notifications should disappear. Do not call
    // CloseWithAnimation, we don't want to show the closing animation, and we
    // don't want to mark such notifications as shown. See crbug.com/233424
    if (bounds.y() >= 0)
      (*curr)->SetBoundsWithAnimation(bounds);
    else
      (*curr)->CloseWithAnimation(false);
    bottom -= bounds.height() + kToastMargin;
  }
}

void MessagePopupCollection::RepositionWidgetsWithTarget() {
  if (toasts_.empty())
    return;

  if (toasts_.back()->origin().y() > target_top_edge_) {
    // No widgets are above, thus slides up the widgets.
    int slide_length =
        toasts_.back()->origin().y() - target_top_edge_;
    for (Toasts::iterator iter = toasts_.begin();
         iter != toasts_.end(); ++iter) {
      gfx::Rect bounds((*iter)->bounds());
      bounds.set_y(bounds.y() - slide_length);
      (*iter)->SetBoundsWithAnimation(bounds);
    }
  } else {
    Toasts::reverse_iterator iter = toasts_.rbegin();
    for (; iter != toasts_.rend(); ++iter) {
      if ((*iter)->origin().y() > target_top_edge_)
        break;
    }
    --iter;
    int slide_length = target_top_edge_ - (*iter)->origin().y();
    for (; ; --iter) {
      gfx::Rect bounds((*iter)->bounds());
      bounds.set_y(bounds.y() + slide_length);
      (*iter)->SetBoundsWithAnimation(bounds);

      if (iter == toasts_.rbegin())
        break;
    }
  }
}

void MessagePopupCollection::OnNotificationAdded(
    const std::string& notification_id) {
  DoUpdateIfPossible();
}

void MessagePopupCollection::OnNotificationRemoved(
    const std::string& notification_id,
    bool by_user) {
  // Find a toast.
  Toasts::iterator iter = toasts_.begin();
  for (; iter != toasts_.end(); ++iter) {
    if ((*iter)->id() == notification_id)
      break;
  }
  if (iter == toasts_.end())
    return;

  target_top_edge_ = (*iter)->bounds().y();
  (*iter)->CloseWithAnimation(true);
  if (by_user) {
    RepositionWidgetsWithTarget();
    // [Re] start a timeout after which the toasts re-position to their
    // normal locations after tracking the mouse pointer for easy deletion.
    // This provides a period of time when toasts are easy to remove because
    // they re-position themselves to have Close button right under the mouse
    // pointer. If the user continue to remove the toasts, the delay is reset.
    // Once user stopped removing the toasts, the toasts re-populate/rearrange
    // after the specified delay.
    if (!user_is_closing_toasts_by_clicking_) {
      user_is_closing_toasts_by_clicking_ = true;
      IncrementDeferCounter();
    }
  }
}

void MessagePopupCollection::OnDeferTimerExpired() {
  user_is_closing_toasts_by_clicking_ = false;
  DecrementDeferCounter();

  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    (*iter)->StartTimer();
  }
}

void MessagePopupCollection::OnNotificationUpdated(
    const std::string& notification_id) {
  // Find a toast.
  Toasts::iterator toast_iter = toasts_.begin();
  for (; toast_iter != toasts_.end(); ++toast_iter) {
    if ((*toast_iter)->id() == notification_id)
      break;
  }
  if (toast_iter == toasts_.end())
    return;

  NotificationList::PopupNotifications notifications =
      message_center_->GetPopupNotifications();
  bool updated = false;

  for (NotificationList::PopupNotifications::iterator iter =
           notifications.begin(); iter != notifications.end(); ++iter) {
    if ((*iter)->id() != notification_id)
      continue;

    MessageView* view = NotificationView::Create(
        *(*iter), message_center_, true);
    (*toast_iter)->SetContents(view);
    (*toast_iter)->ResetTimeout((*iter)->priority());
    updated = true;
  }

  // OnNotificationUpdated() can be called when a notification is excluded from
  // the popup notification list but still remains in the full notification
  // list. In that case the widget for the notification has to be closed here.
  if (!updated)
    (*toast_iter)->CloseWithAnimation(true);

  if (user_is_closing_toasts_by_clicking_)
    RepositionWidgetsWithTarget();
  else
    DoUpdateIfPossible();
}

void MessagePopupCollection::SetWorkAreaForTest(const gfx::Rect& work_area) {
  work_area_ = work_area;
}

bool MessagePopupCollection::HasToast(const std::string& notification_id) {
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    if ((*iter)->id() == notification_id)
      return true;
  }
  return false;
}

void MessagePopupCollection::IncrementDeferCounter() {
  defer_counter_++;
}

void MessagePopupCollection::DecrementDeferCounter() {
  defer_counter_--;
  DCHECK(defer_counter_ >= 0);
  DoUpdateIfPossible();
}

// This is the main sequencer of tasks. It does a step, then waits for
// all started transitions to play out before doing the next step.
// First, remove all expired toasts.
// Then, reposition widgets (the reposition on close happens before all
// deferred tasks are even able to run)
// Then, see if there is vacant space for new toasts.
void MessagePopupCollection::DoUpdateIfPossible() {
  if (defer_counter_ > 0)
    return;

  RepositionWidgets();

  if (defer_counter_ > 0)
    return;

  // Reposition could create extra space which allows additional widgets.
  UpdateWidgets();

  if (defer_counter_ > 0)
    return;

  // Test support. Quit the test run loop when no more updates are deferred,
  // meaining th echeck for updates did not cause anything to change so no new
  // transition animations were started.
  if (run_loop_for_test_.get())
    run_loop_for_test_->Quit();
}

views::Widget* MessagePopupCollection::GetWidgetForTest(const std::string& id) {
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    if ((*iter)->id() == id)
      return (*iter)->GetWidget();
  }
  return NULL;
}

void MessagePopupCollection::RunLoopForTest() {
  run_loop_for_test_.reset(new base::RunLoop());
  run_loop_for_test_->Run();
  run_loop_for_test_.reset();
}

gfx::Rect MessagePopupCollection::GetToastRectAt(size_t index) {
  DCHECK(defer_counter_ == 0) << "Fetching the bounds with animations active.";
  size_t i = 0;
  for (Toasts::iterator iter = toasts_.begin(); iter != toasts_.end(); ++iter) {
    if (i++ == index) {
      views::Widget* widget = (*iter)->GetWidget();
      if (widget)
        return widget->GetWindowBoundsInScreen();
      break;
    }
  }
  return gfx::Rect();
}

}  // namespace message_center
