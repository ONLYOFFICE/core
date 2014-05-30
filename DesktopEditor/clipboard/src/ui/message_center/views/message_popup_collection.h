// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_COLLECTION_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_COLLECTION_H_

#include <list>
#include <map>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
//#include "base/run_loop.h"
#include "base/timer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/views/widget/widget_observer.h"

namespace base {
class RunLoop;
}

namespace views {
class Widget;
}

namespace ash {
FORWARD_DECLARE_TEST(WebNotificationTrayTest, ManyPopupNotifications);
}

namespace message_center {
namespace test {
class MessagePopupCollectionTest;
class MessagePopupCollectionWidgetsTest;
}

class MessageCenter;
class ToastContentsView;

// Container for popup toasts. Because each toast is a frameless window rather
// than a view in a bubble, now the container just manages all of those toasts.
// This is similar to chrome/browser/notifications/balloon_collection, but the
// contents of each toast are for the message center and layout strategy would
// be slightly different.
class MESSAGE_CENTER_EXPORT MessagePopupCollection
    : public MessageCenterObserver,
      public base::SupportsWeakPtr<MessagePopupCollection> {
 public:
  // |parent| specifies the parent widget of the toast windows. The default
  // parent will be used for NULL.
  MessagePopupCollection(gfx::NativeView parent,
                         MessageCenter* message_center);
  virtual ~MessagePopupCollection();

  // Called by ToastContentsView when its window is closed.
  void RemoveToast(ToastContentsView* toast);

  // Since these events are really coming from individual toast widgets,
  // it helps to be able to keep track of the sender.
  void OnMouseEntered(ToastContentsView* toast_entered);
  void OnMouseExited(ToastContentsView* toast_exited);

  // Invoked by toasts when they start/finish their animations.
  // While "defer counter" is greater then zero, the popup collection does
  // not perform updates. It is used to wait for various animations and user
  // actions like serial closing of the toasts, when the remaining toasts "flow
  // under the mouse".
  void IncrementDeferCounter();
  void DecrementDeferCounter();

  // Runs the next step in update/animate sequence, if the defer counter is not
  // zero. Otherwise, simply waits when it becomes zero.
  void DoUpdateIfPossible();

 private:
  FRIEND_TEST_ALL_PREFIXES(ash::WebNotificationTrayTest,
                           ManyPopupNotifications);
  friend class test::MessagePopupCollectionTest;
  friend class test::MessagePopupCollectionWidgetsTest;
  typedef std::list<ToastContentsView*> Toasts;

  void CloseAllWidgets();

  // Returns the bottom-right position of the current work area.
  gfx::Point GetWorkAreaBottomRight();

  // Iterates toasts and starts closing the expired ones.
  void CloseToasts();

  // Creates new widgets for new toast notifications, and updates |toasts_| and
  // |widgets_| correctly.
  void UpdateWidgets();

  // Repositions all of the widgets based on the current work area.
  void RepositionWidgets();

  // Repositions widgets to the top edge of the notification toast that was
  // just removed, so that the user can click close button without mouse moves.
  // See crbug.com/224089
  void RepositionWidgetsWithTarget();

  // Overridden from MessageCenterObserver:
  virtual void OnNotificationAdded(const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE;

  bool HasToast(const std::string& notification_id);

  // While the toasts are animated, avoid updating the collection, to reduce
  // user confusion. Instead, update the collection when all animations are
  // done. This method is run when defer counter is zero, may initiate next
  // update/animation step.
  void PerformDeferredTasks();
  void OnDeferTimerExpired();

  // "ForTest" methods.
  void SetWorkAreaForTest(const gfx::Rect& work_area);
  views::Widget* GetWidgetForTest(const std::string& id);
  void RunLoopForTest();
  gfx::Rect GetToastRectAt(size_t index);

  gfx::NativeView parent_;
  MessageCenter* message_center_;
  Toasts toasts_;
  gfx::Rect work_area_;

  int defer_counter_;

  // This is only used to compare with incoming events, do not assume that
  // the toast will be valid if this pointer is non-NULL.
  ToastContentsView* latest_toast_entered_;

  // Denotes a mode when user is clicking the Close button of toasts in a
  // sequence, w/o moving the mouse. We reposition the toasts so the next one
  // happens to be right under the mouse, and the user can just dispose of
  // multipel toasts by clicking. The mode ends when defer_timer_ expires.
  bool user_is_closing_toasts_by_clicking_;
  scoped_ptr<base::OneShotTimer<MessagePopupCollection> > defer_timer_;
  // The top edge to align the position of the next toast during 'close by
  // clicking" mode.
  // Only to be used when user_is_closing_toasts_by_clicking_ is true.
  int target_top_edge_;

  // Weak, only exists temporarily in tests.
  scoped_ptr<base::RunLoop> run_loop_for_test_;

  DISALLOW_COPY_AND_ASSIGN(MessagePopupCollection);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_COLLECTION_H_
