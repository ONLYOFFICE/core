// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_TOAST_CONTENTS_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_TOAST_CONTENTS_VIEW_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/timer.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/views/widget/widget_delegate.h"

namespace ui {
class Animation;
class SlideAnimation;
}

namespace views {
class View;
}

namespace message_center {

class MessageCenter;
class MessagePopupCollection;
class MessageView;
class Notification;


class ToastContentsView : public views::WidgetDelegateView,
                          public ui::AnimationDelegate {
 public:
  ToastContentsView(const Notification* notification,
                    base::WeakPtr<MessagePopupCollection> collection,
                    MessageCenter* message_center);
  virtual ~ToastContentsView();

  // Initialization and update.
  views::Widget* CreateWidget(gfx::NativeView parent);
  void SetContents(MessageView* view);

  // Expiration timer control.
  // (Re-)initializes the timer, initially and on notification update.
  void ResetTimeout(int priority);
  void SuspendTimer();
  // (Re-)starts the timer, initially or after it was supended.
  void StartTimer();

  // Shows the new toast for the first time, animated.
  // |origin| is the right-bottom corner of the toast.
  void RevealWithAnimation(gfx::Point origin);
  // Disconnectes the toast from the rest of the system immediately and start
  // an animation. Once animation finishes, closes the widget.
  void CloseWithAnimation(bool mark_as_shown);

  void SetBoundsInstantly(gfx::Rect new_bounds);
  void SetBoundsWithAnimation(gfx::Rect new_bounds);

  // Origin and bounds are not 'instant', but rather 'current stable values',
  // there could be animation in progress that targets these values.
  gfx::Point origin() { return origin_; }
  gfx::Rect bounds() { return gfx::Rect(origin_, preferred_size_); }

  const std::string& id() { return id_; }

  // Computes the size of the toast assuming it will host the given view.
  static gfx::Size GetToastSizeForView(views::View* view);

  // Overridden from views::View:
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 private:
  // Overridden from ui::AnimationDelegate:
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const ui::Animation* animation) OVERRIDE;

  // Overridden from views::WidgetDelegate:
  virtual views::View* GetContentsView() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;

  // Given the bounds of a toast on the screen, compute the bouds for that
  // toast in 'closed' state. The 'closed' state is used as origin/destination
  // in reveal/closing animations.
  gfx::Rect GetClosedToastBounds(gfx::Rect bounds);

  void StartFadeIn();
  void StartFadeOut();  // Will call Widget::Close() when animation ends.
  void OnBoundsAnimationEndedOrCancelled(const ui::Animation* animation);

  base::WeakPtr<MessagePopupCollection> collection_;
  MessageCenter* message_center_;

  // Id if the corresponding Notification.
  std::string id_;

  // Expiration timer values.
  base::TimeDelta timeout_;
  base::TimeDelta passed_;
  base::Time start_time_;
  scoped_ptr<base::OneShotTimer<ToastContentsView> > timer_;


  scoped_ptr<ui::SlideAnimation> bounds_animation_;
  scoped_ptr<ui::SlideAnimation> fade_animation_;

  bool is_animating_bounds_;
  gfx::Rect animated_bounds_start_;
  gfx::Rect animated_bounds_end_;
  // Started closing animation, will close at the end.
  bool is_closing_;
  // Closing animation - when it ends, close the widget. Weak, only used
  // for referential equality.
  ui::Animation* closing_animation_;

  gfx::Point origin_;
  gfx::Size preferred_size_;

  DISALLOW_COPY_AND_ASSIGN(ToastContentsView);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_VIEWS_TOAST_CONTENTS_VIEW_H_
