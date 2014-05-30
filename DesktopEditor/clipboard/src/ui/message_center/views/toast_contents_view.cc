// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/toast_contents_view.h"

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time.h"
#include "base/timer.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/views/message_popup_collection.h"
#include "ui/message_center/views/message_view.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace message_center {
namespace {

// The width of a toast before animated reveal and after closing.
const int kClosedToastWidth = 5;

// FadeIn/Out look a bit better if they are slightly longer then default slide.
const int kFadeInOutDuration = 200;

}  // namespace.

// static
gfx::Size ToastContentsView::GetToastSizeForView(views::View* view) {
  int width = kNotificationWidth + view->GetInsets().width();
  return gfx::Size(width, view->GetHeightForWidth(width));
}

ToastContentsView::ToastContentsView(
  const Notification* notification,
  base::WeakPtr<MessagePopupCollection> collection,
  MessageCenter* message_center)
    : collection_(collection),
      message_center_(message_center),
      id_(notification->id()),
      is_animating_bounds_(false),
      is_closing_(false),
      closing_animation_(NULL) {
  DCHECK(collection_);

  set_notify_enter_exit_on_child(true);
  // Sets the transparent background. Then, when the message view is slid out,
  // the whole toast seems to slide although the actual bound of the widget
  // remains. This is hacky but easier to keep the consistency.
  set_background(views::Background::CreateSolidBackground(0, 0, 0, 0));

  // Creates the timer only when it does the timeout (i.e. not never-timeout).
  if (!notification->never_timeout()) {
    timer_.reset(new base::OneShotTimer<ToastContentsView>);
    ResetTimeout(notification->priority());
    StartTimer();
  }

  fade_animation_.reset(new ui::SlideAnimation(this));
  fade_animation_->SetSlideDuration(kFadeInOutDuration);
}

// This is destroyed when the toast window closes.
ToastContentsView::~ToastContentsView() {
}

views::Widget* ToastContentsView::CreateWidget(gfx::NativeView parent) {
  views::Widget::InitParams params(
      views::Widget::InitParams::TYPE_POPUP);
  params.keep_on_top = true;
  params.transparent = true;
  if (parent)
    params.parent = parent;
  else
    params.top_level = true;
  params.transparent = true;
  params.delegate = this;
  views::Widget* widget = new views::Widget();
  widget->set_focus_on_creation(false);
  widget->Init(params);
  return widget;
}

void ToastContentsView::SetContents(MessageView* view) {
  RemoveAllChildViews(true);
  AddChildView(view);
  preferred_size_ = GetToastSizeForView(view);
  Layout();
}

void ToastContentsView::ResetTimeout(int priority) {
  int seconds = kAutocloseDefaultDelaySeconds;
  if (priority > DEFAULT_PRIORITY)
    seconds = kAutocloseHighPriorityDelaySeconds;
  timeout_ = base::TimeDelta::FromSeconds(seconds);
  // If timer exists and is not suspended, re-start it with new timeout.
  if (timer_.get() && timer_->IsRunning())
    StartTimer();
}

void ToastContentsView::SuspendTimer() {
  if (!timer_.get() || !timer_->IsRunning())
    return;
  timer_->Stop();
  passed_ += base::Time::Now() - start_time_;
}

void ToastContentsView::StartTimer() {
  if (!timer_.get())
    return;

  base::TimeDelta timeout_to_close =
      timeout_ <= passed_ ? base::TimeDelta() : timeout_ - passed_;
  start_time_ = base::Time::Now();
  timer_->Start(FROM_HERE,
                timeout_to_close,
                base::Bind(&ToastContentsView::CloseWithAnimation,
                           base::Unretained(this),
                           true));
}

void ToastContentsView::RevealWithAnimation(gfx::Point origin) {
  // Place/move the toast widgets. Currently it stacks the widgets from the
  // right-bottom of the work area.
  // TODO(mukai): allow to specify the placement policy from outside of this
  // class. The policy should be specified from preference on Windows, or
  // the launcher alignment on ChromeOS.
  origin_ = gfx::Point(origin.x() - preferred_size_.width(),
                       origin.y() - preferred_size_.height());

  gfx::Rect stable_bounds(origin_, preferred_size_);

  SetBoundsInstantly(GetClosedToastBounds(stable_bounds));
  StartFadeIn();
  SetBoundsWithAnimation(stable_bounds);
}

void ToastContentsView::CloseWithAnimation(bool mark_as_shown) {
  if (is_closing_)
    return;
  is_closing_ = true;
  timer_.reset();
  if (collection_)
    collection_->RemoveToast(this);
  if (mark_as_shown)
    message_center_->MarkSinglePopupAsShown(id(), false);
  StartFadeOut();
}

void ToastContentsView::SetBoundsInstantly(gfx::Rect new_bounds) {
  if (new_bounds == bounds())
    return;

  origin_ = new_bounds.origin();
  if (!GetWidget())
    return;
  GetWidget()->SetBounds(new_bounds);
}

void ToastContentsView::SetBoundsWithAnimation(gfx::Rect new_bounds) {
  if (new_bounds == bounds())
    return;

  origin_ = new_bounds.origin();
  if (!GetWidget())
    return;

  // This picks up the current bounds, so if there was a previous animation
  // half-done, the next one will pick up from the current location.
  // This is the only place that should query current location of the Widget
  // on screen, the rest should refer to the bounds_.
  animated_bounds_start_ = GetWidget()->GetWindowBoundsInScreen();
  animated_bounds_end_ = new_bounds;

  if (collection_)
      collection_->IncrementDeferCounter();

  if (bounds_animation_.get())
    bounds_animation_->Stop();

  bounds_animation_.reset(new ui::SlideAnimation(this));
  bounds_animation_->Show();
}

void ToastContentsView::StartFadeIn() {
  // The decrement is done in OnBoundsAnimationEndedOrCancelled callback.
  if (collection_)
    collection_->IncrementDeferCounter();
  fade_animation_->Stop();

  GetWidget()->SetOpacity(0);
  GetWidget()->Show();
  fade_animation_->Reset(0);
  fade_animation_->Show();
}

  void ToastContentsView::StartFadeOut() {
    // The decrement is done in OnBoundsAnimationEndedOrCancelled callback.
    if (collection_)
      collection_->IncrementDeferCounter();
  fade_animation_->Stop();

  closing_animation_ = (is_closing_ ? fade_animation_.get() : NULL);
  fade_animation_->Reset(1);
  fade_animation_->Hide();
}

void ToastContentsView::OnBoundsAnimationEndedOrCancelled(
    const ui::Animation* animation) {
  if (is_closing_ && closing_animation_ == animation && GetWidget())
    GetWidget()->Close();

  // This cannot be called before GetWidget()->Close(). Decrementing defer count
  // will invoke update, which may invoke another close animation with
  // incrementing defer counter. Close() after such process will cause a
  // mismatch between increment/decrement. See crbug.com/238477
  if (collection_)
    collection_->DecrementDeferCounter();
}

// ui::AnimationDelegate
void ToastContentsView::AnimationProgressed(const ui::Animation* animation) {
  if (animation == bounds_animation_.get()) {
    gfx::Rect current(animation->CurrentValueBetween(
        animated_bounds_start_, animated_bounds_end_));
    GetWidget()->SetBounds(current);
  } else if (animation == fade_animation_.get()) {
    unsigned char opacity =
        static_cast<unsigned char>(fade_animation_->GetCurrentValue() * 255);
    GetWidget()->SetOpacity(opacity);
  }
}

void ToastContentsView::AnimationEnded(const ui::Animation* animation) {
  OnBoundsAnimationEndedOrCancelled(animation);
}

void ToastContentsView::AnimationCanceled(
    const ui::Animation* animation) {
  OnBoundsAnimationEndedOrCancelled(animation);
}

// views::WidgetDelegate
views::View* ToastContentsView::GetContentsView() {
  return this;
}

void ToastContentsView::WindowClosing() {
  SuspendTimer();
  if (!is_closing_ && collection_)
    collection_->RemoveToast(this);
}

bool ToastContentsView::CanActivate() const {
#if defined(OS_WIN) && defined(USE_AURA)
  return true;
#else
  return false;
#endif
}

// views::View
void ToastContentsView::OnMouseEntered(const ui::MouseEvent& event) {
  if (collection_)
    collection_->OnMouseEntered(this);
}

void ToastContentsView::OnMouseExited(const ui::MouseEvent& event) {
  if (collection_)
    collection_->OnMouseExited(this);
}

void ToastContentsView::Layout() {
  if (child_count() > 0) {
    child_at(0)->SetBounds(
        0, 0, preferred_size_.width(), preferred_size_.height());
  }
}

gfx::Size ToastContentsView::GetPreferredSize() {
  return child_count() ? GetToastSizeForView(child_at(0)) : gfx::Size();
}

gfx::Rect ToastContentsView::GetClosedToastBounds(gfx::Rect bounds) {
  return gfx::Rect(bounds.x() + bounds.width() - kClosedToastWidth,
                   bounds.y(),
                   kClosedToastWidth,
                   bounds.height());
}

}  // namespace message_center
