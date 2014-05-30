// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_center_view.h"

#include <map>

#include "base/stl_util.h"
#include "grit/ui_strings.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/gfx/text_constants.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/message_center_util.h"
#include "ui/message_center/views/message_view.h"
#include "ui/message_center/views/notification_view.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/animation/bounds_animator_observer.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/scrollbar/kennedy_scroll_bar.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/painter.h"
#include "ui/views/widget/widget.h"


namespace message_center {

namespace {

const int kMinScrollViewHeight = 100;
const int kFooterMargin = 16;
const int kFooterHeight = 24;
const SkColor kNoNotificationsTextColor = SkColorSetRGB(0xb4, 0xb4, 0xb4);
const SkColor kMessageCenterBackgroundColor = SkColorSetRGB(0xf1, 0xf1, 0xf1);
const SkColor kBorderDarkColor = SkColorSetRGB(0xaa, 0xaa, 0xaa);
const SkColor kTransparentColor = SkColorSetARGB(0, 0, 0, 0);
const SkColor kFooterDelimiterColor = SkColorSetRGB(0xcc, 0xcc, 0xcc);
const SkColor kFooterTextColor = SkColorSetRGB(0x7b, 0x7b, 0x7b);
const SkColor kButtonTextHighlightColor = SkColorSetRGB(0x2a, 0x2a, 0x2a);
const SkColor kButtonTextHoverColor = SkColorSetRGB(0x2a, 0x2a, 0x2a);

// PoorMessageCenterButtonBar //////////////////////////////////////////////////

// The view for the buttons at the bottom of the message center window used
// when kEnableRichNotifications is false (hence the "poor" in the name :-).
class PoorMessageCenterButtonBar : public MessageCenterButtonBar,
                                   public views::ButtonListener {
 public:
  explicit PoorMessageCenterButtonBar(MessageCenter* message_center);

  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PoorMessageCenterButtonBar);
};

PoorMessageCenterButtonBar::PoorMessageCenterButtonBar(
    MessageCenter* message_center)
    : MessageCenterButtonBar(message_center) {
  set_background(views::Background::CreateBackgroundPainter(
      true,
      views::Painter::CreateVerticalGradient(kBackgroundLightColor,
                                             kBackgroundDarkColor)));
  set_border(views::Border::CreateSolidSidedBorder(
      2, 0, 0, 0, kBorderDarkColor));

  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);
  views::ColumnSet* columns = layout->AddColumnSet(0);
  columns->AddPaddingColumn(100, 0);
  columns->AddColumn(views::GridLayout::TRAILING, views::GridLayout::CENTER,
                     0, /* resize percent */
                     views::GridLayout::USE_PREF, 0, 0);
  columns->AddPaddingColumn(0, 4);

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  views::LabelButton* close_all_button = new views::LabelButton(
      this, rb.GetLocalizedString(IDS_MESSAGE_CENTER_CLEAR_ALL));
  close_all_button->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  close_all_button->set_request_focus_on_press(false);
  close_all_button->SetTextColor(views::Button::STATE_NORMAL, kFooterTextColor);
  close_all_button->SetTextColor(views::Button::STATE_HOVERED,
                                 kButtonTextHoverColor);

  layout->AddPaddingRow(0, 4);
  layout->StartRow(0, 0);
  layout->AddView(close_all_button);
  set_close_all_button(close_all_button);
}

void PoorMessageCenterButtonBar::ButtonPressed(views::Button* sender,
                                               const ui::Event& event) {
  if (sender == close_all_button())
    message_center()->RemoveAllNotifications(true);  // Action by user.
}

// NotificationCenterButton ////////////////////////////////////////////////////

class NotificationCenterButton : public views::LabelButton {
 public:
  NotificationCenterButton(views::ButtonListener* listener,
                           const string16& text);

  // Overridden from views::LabelButton:
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 protected:
  // Overridden from views::View:
  virtual void OnPaintBorder(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NotificationCenterButton);
};

NotificationCenterButton::NotificationCenterButton(
    views::ButtonListener* listener,
    const string16& text)
    : views::LabelButton(listener, text) {
  set_border(views::Border::CreateEmptyBorder(0, 16, 0, 16));
  set_min_size(gfx::Size(0, kFooterHeight));
  SetTextColor(STATE_NORMAL, kFooterTextColor);
  SetTextColor(STATE_HOVERED, kButtonTextHoverColor);
}

gfx::Size NotificationCenterButton::GetPreferredSize() {
  // Returns an empty size when invisible, to trim its space in the GridLayout.
  return visible() ? views::LabelButton::GetPreferredSize() : gfx::Size();
}

void NotificationCenterButton::OnPaintBorder(gfx::Canvas* canvas) {
  // Just paint the left border.
  canvas->DrawLine(gfx::Point(0, 0), gfx::Point(0, height()),
                   kFooterDelimiterColor);
}

void NotificationCenterButton::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    canvas->DrawRect(gfx::Rect(2, 1, width() - 4, height() - 3),
                     kFocusBorderColor);
  }
}

// RichMessageCenterButtonBar //////////////////////////////////////////////////

// TODO(mukai): Merge this into MessageCenterButtonBar and get rid of
// PoorMessageCenterButtonBar when the kEnableRichNotifications flag disappears.
class RichMessageCenterButtonBar : public MessageCenterButtonBar,
                                   public views::ButtonListener {
 public:
  explicit RichMessageCenterButtonBar(MessageCenter* message_center);

 private:
  // Overridden from views::View:
  virtual void ChildVisibilityChanged(views::View* child) OVERRIDE;

  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  views::Label* notification_label_;
  NotificationCenterButton* settings_button_;

  DISALLOW_COPY_AND_ASSIGN(RichMessageCenterButtonBar);
};

RichMessageCenterButtonBar::RichMessageCenterButtonBar(
    MessageCenter* message_center)
  : MessageCenterButtonBar(message_center) {
  set_background(views::Background::CreateSolidBackground(
      kMessageCenterBackgroundColor));
  set_border(views::Border::CreateSolidSidedBorder(
      1, 0, 0, 0, kFooterDelimiterColor));


  notification_label_ = new views::Label(l10n_util::GetStringUTF16(
      IDS_MESSAGE_CENTER_FOOTER_TITLE));
  notification_label_->SetAutoColorReadabilityEnabled(false);
  notification_label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  notification_label_->SetElideBehavior(views::Label::ELIDE_AT_END);
  notification_label_->SetEnabledColor(kFooterTextColor);
  AddChildView(notification_label_);
  settings_button_ = new NotificationCenterButton(
      this, l10n_util::GetStringUTF16(
          IDS_MESSAGE_CENTER_SETTINGS_BUTTON_LABEL));
  settings_button_->set_focusable(true);
  settings_button_->set_request_focus_on_press(false);
  AddChildView(settings_button_);
  NotificationCenterButton* close_all_button = new NotificationCenterButton(
      this, l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_CLEAR_ALL));
  close_all_button->set_focusable(true);
  close_all_button->set_request_focus_on_press(false);
  AddChildView(close_all_button);

  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);
  layout->SetInsets(
      kMarginBetweenItems, kFooterMargin, kMarginBetweenItems, 0);
  views::ColumnSet* column = layout->AddColumnSet(0);
  column->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                    1.0f, views::GridLayout::USE_PREF, 0, 0);
  column->AddColumn(views::GridLayout::LEADING, views::GridLayout::FILL,
                    0, views::GridLayout::FIXED,
                    settings_button_->GetPreferredSize().width(), 0);
  column->AddColumn(views::GridLayout::LEADING, views::GridLayout::FILL,
                    0, views::GridLayout::USE_PREF, 0, 0);
  layout->StartRow(0, 0);
  layout->AddView(notification_label_);
  layout->AddView(settings_button_);
  layout->AddView(close_all_button);
  set_close_all_button(close_all_button);
}

// Overridden from views::View:
void RichMessageCenterButtonBar::ChildVisibilityChanged(views::View* child) {
  InvalidateLayout();
}

// Overridden from views::ButtonListener:
void RichMessageCenterButtonBar::ButtonPressed(views::Button* sender,
                                               const ui::Event& event) {
  if (sender == close_all_button())
    message_center()->RemoveAllNotifications(true);  // Action by user.
  else if (sender == settings_button_)
    message_center()->ShowNotificationSettingsDialog(
        GetWidget()->GetNativeView());
  else
    NOTREACHED();
}

// BoundedScrollView ///////////////////////////////////////////////////////////

// A custom scroll view whose height has a minimum and maximum value and whose
// scroll bar disappears when not needed.
class BoundedScrollView : public views::ScrollView {
 public:
  BoundedScrollView(int min_height, int max_height);

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  int min_height_;
  int max_height_;

  DISALLOW_COPY_AND_ASSIGN(BoundedScrollView);
};

BoundedScrollView::BoundedScrollView(int min_height, int max_height)
    : min_height_(min_height),
      max_height_(max_height) {
  set_notify_enter_exit_on_child(true);
  // Cancels the default dashed focus border.
  set_focus_border(NULL);
  if (IsRichNotificationEnabled()) {
    set_background(views::Background::CreateSolidBackground(
        kMessageCenterBackgroundColor));
    SetVerticalScrollBar(new views::KennedyScrollBar(false));
  }
}

gfx::Size BoundedScrollView::GetPreferredSize() {
  gfx::Size size = contents()->GetPreferredSize();
  size.ClampToMin(gfx::Size(size.width(), min_height_));
  size.ClampToMax(gfx::Size(size.width(), max_height_));
  gfx::Insets insets = GetInsets();
  size.Enlarge(insets.width(), insets.height());
  return size;
}

int BoundedScrollView::GetHeightForWidth(int width) {
  gfx::Insets insets = GetInsets();
  width = std::max(0, width - insets.width());
  int height = contents()->GetHeightForWidth(width) + insets.height();
  return std::min(std::max(height, min_height_), max_height_);
}

void BoundedScrollView::Layout() {
  int content_width = width();
  int content_height = contents()->GetHeightForWidth(content_width);
  if (content_height > height()) {
    content_width = std::max(content_width - GetScrollBarWidth(), 0);
    content_height = contents()->GetHeightForWidth(content_width);
  }
  if (contents()->bounds().size() != gfx::Size(content_width, content_height))
    contents()->SetBounds(0, 0, content_width, content_height);
  views::ScrollView::Layout();
}

class NoNotificationMessageView : public views::View {
 public:
  NoNotificationMessageView();
  virtual ~NoNotificationMessageView();

  // Overridden from views::View.
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  views::Label* label_;

  DISALLOW_COPY_AND_ASSIGN(NoNotificationMessageView);
};

NoNotificationMessageView::NoNotificationMessageView() {
  label_ = new views::Label(l10n_util::GetStringUTF16(
      IDS_MESSAGE_CENTER_NO_MESSAGES));
  label_->SetAutoColorReadabilityEnabled(false);
  label_->SetEnabledColor(kNoNotificationsTextColor);
  // Set transparent background to ensure that subpixel rendering
  // is disabled. See crbug.com/169056
#if defined(OS_LINUX) && defined(OS_CHROMEOS)
  label_->SetBackgroundColor(kTransparentColor);
#endif
  AddChildView(label_);
}

NoNotificationMessageView::~NoNotificationMessageView() {
}

gfx::Size NoNotificationMessageView::GetPreferredSize() {
  return gfx::Size(kMinScrollViewHeight, label_->GetPreferredSize().width());
}

int NoNotificationMessageView::GetHeightForWidth(int width) {
  return kMinScrollViewHeight;
}

void NoNotificationMessageView::Layout() {
  DCHECK_EQ(100, height());
  int text_height = label_->GetHeightForWidth(width());
  int margin = (height() - text_height) / 2;
  label_->SetBounds(0, margin, width(), text_height);
}

}  // namespace

// MessageListView /////////////////////////////////////////////////////////////

// Displays a list of messages.
class MessageListView : public views::View {
 public:
  MessageListView();

  // The interface for repositioning.
  virtual void AddNotificationAt(views::View* view, int i);
  virtual void RemoveNotificationAt(int i);
  virtual void UpdateNotificationAt(views::View* view, int i);
  virtual void SetRepositionTarget(const gfx::Rect& target_rect) {}
  virtual void ResetRepositionSession() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MessageListView);
};

MessageListView::MessageListView() {
  views::BoxLayout* layout =
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 1);
  layout->set_spread_blank_space(true);
  SetLayoutManager(layout);
}

void MessageListView::AddNotificationAt(views::View* view, int i) {
  AddChildViewAt(view, i);
}

void MessageListView::RemoveNotificationAt(int i) {
  delete child_at(i);
}

void MessageListView::UpdateNotificationAt(views::View* view, int i) {
  delete child_at(i);
  AddChildViewAt(view, i);
}

// Displays a list of messages for rich notifications. It also supports
// repositioning.
class RichMessageListView : public MessageListView,
                            public views::BoundsAnimatorObserver {
 public:
  RichMessageListView();
  virtual ~RichMessageListView();

 protected:
  // Overridden from views::View.
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;

  // Overridden from MessageListView.
  virtual void AddNotificationAt(views::View* view, int i) OVERRIDE;
  virtual void RemoveNotificationAt(int i) OVERRIDE;
  virtual void UpdateNotificationAt(views::View* view, int i) OVERRIDE;
  virtual void SetRepositionTarget(const gfx::Rect& target_rect) OVERRIDE;
  virtual void ResetRepositionSession() OVERRIDE;

  // Overridden from views::BoundsAnimatorObserver.
  virtual void OnBoundsAnimatorProgressed(
      views::BoundsAnimator* animator) OVERRIDE;
  virtual void OnBoundsAnimatorDone(views::BoundsAnimator* animator) OVERRIDE;

 private:
  int GetActualIndex(int index);
  bool IsValidChild(views::View* child);
  void DoUpdateIfPossible();

  // Schedules animation for a child to the specified position.
  void AnimateChild(views::View* child, int top, int height);

  // The top position of the reposition target rectangle.
  int reposition_top_;

  int fixed_height_;

  bool has_deferred_task_;
  std::set<views::View*> adding_views_;
  std::set<views::View*> deleting_views_;
  std::set<views::View*> deleted_when_done_;
  scoped_ptr<views::BoundsAnimator> animator_;

  DISALLOW_COPY_AND_ASSIGN(RichMessageListView);
};

RichMessageListView::RichMessageListView()
    : reposition_top_(-1),
      fixed_height_(0),
      has_deferred_task_(false) {
  // Set the margin to 0 for the layout. BoxLayout assumes the same margin
  // for top and bottom, but the bottom margin here should be smaller
  // because of the shadow of message view. Use an empty border instead
  // to provide this margin.
  gfx::Insets shadow_insets = MessageView::GetShadowInsets();
  set_background(views::Background::CreateSolidBackground(
      kMessageCenterBackgroundColor));
  set_border(views::Border::CreateEmptyBorder(
      kMarginBetweenItems - shadow_insets.top(), /* top */
      kMarginBetweenItems - shadow_insets.left(), /* left */
      kMarginBetweenItems - shadow_insets.bottom(),  /* bottom */
      kMarginBetweenItems - shadow_insets.right() /* right */ ));
}

RichMessageListView::~RichMessageListView() {
  if (animator_.get())
    animator_->RemoveObserver(this);
}

void RichMessageListView::Layout() {
  if (animator_.get())
    return;

  gfx::Rect child_area = GetContentsBounds();
  int top = child_area.y();
  int between_items =
      kMarginBetweenItems - MessageView::GetShadowInsets().bottom();

  for (int i = 0; i < child_count(); ++i) {
    views::View* child = child_at(i);
    if (!child->visible())
      continue;
    int height = child->GetHeightForWidth(child_area.width());
    child->SetBounds(child_area.x(), top, child_area.width(), height);
    top += height + between_items;
  }
}

void RichMessageListView::AddNotificationAt(views::View* view, int i) {
  AddChildViewAt(view, GetActualIndex(i));
  if (GetContentsBounds().IsEmpty())
    return;

  adding_views_.insert(view);
  DoUpdateIfPossible();
}

void RichMessageListView::RemoveNotificationAt(int i) {
  views::View* child = child_at(GetActualIndex(i));
  if (GetContentsBounds().IsEmpty()) {
    delete child;
  } else {
    if (child->layer()) {
      deleting_views_.insert(child);
    } else {
      if (animator_.get())
        animator_->StopAnimatingView(child);
      delete child;
    }
    DoUpdateIfPossible();
  }
}

void RichMessageListView::UpdateNotificationAt(views::View* view, int i) {
  views::View* child = child_at(GetActualIndex(i));
  if (animator_.get())
    animator_->StopAnimatingView(child);
  gfx::Rect old_bounds = child->bounds();
  if (deleting_views_.find(child) != deleting_views_.end())
    deleting_views_.erase(child);
  if (deleted_when_done_.find(child) != deleted_when_done_.end())
    deleted_when_done_.erase(child);
  delete child;
  AddChildViewAt(view, i);
  view->SetBounds(old_bounds.x(), old_bounds.y(), old_bounds.width(),
                  view->GetHeightForWidth(old_bounds.width()));
  DoUpdateIfPossible();
}

gfx::Size RichMessageListView::GetPreferredSize() {
  int width = 0;
  for (int i = 0; i < child_count(); i++) {
    views::View* child = child_at(i);
    if (IsValidChild(child))
      width = std::max(width, child->GetPreferredSize().width());
  }

  return gfx::Size(width + GetInsets().width(),
                   GetHeightForWidth(width + GetInsets().width()));
}

int RichMessageListView::GetHeightForWidth(int width) {
  if (fixed_height_ > 0)
    return fixed_height_;

  width -= GetInsets().width();
  int height = 0;
  int padding = 0;
  for (int i = 0; i < child_count(); ++i) {
    views::View* child = child_at(i);
    if (!IsValidChild(child))
      continue;
    height += child->GetHeightForWidth(width) + padding;
    padding = kMarginBetweenItems - MessageView::GetShadowInsets().bottom();
  }

  return height + GetInsets().height();
}

void RichMessageListView::SetRepositionTarget(const gfx::Rect& target) {
  reposition_top_ = target.y();
  fixed_height_ = GetHeightForWidth(width());
}

void RichMessageListView::ResetRepositionSession() {
  // Don't call DoUpdateIfPossible(), but let Layout() do the task without
  // animation. Reset will cause the change of the bubble size itself, and
  // animation from the old location will look weird.
  if (reposition_top_ >= 0 && animator_.get()) {
    has_deferred_task_ = false;
    // cancel cause OnBoundsAnimatorDone which deletes |deleted_when_done_|.
    animator_->Cancel();
    STLDeleteContainerPointers(deleting_views_.begin(), deleting_views_.end());
    deleting_views_.clear();
    adding_views_.clear();
    animator_.reset();
  }

  reposition_top_ = -1;
  fixed_height_ = 0;
}

void RichMessageListView::OnBoundsAnimatorProgressed(
    views::BoundsAnimator* animator) {
  DCHECK_EQ(animator_.get(), animator);
  for (std::set<views::View*>::iterator iter = deleted_when_done_.begin();
       iter != deleted_when_done_.end(); ++iter) {
    const ui::SlideAnimation* animation = animator->GetAnimationForView(*iter);
    if (animation)
      (*iter)->layer()->SetOpacity(animation->CurrentValueBetween(1.0, 0.0));
  }
}

void RichMessageListView::OnBoundsAnimatorDone(
    views::BoundsAnimator* animator) {
  STLDeleteContainerPointers(
      deleted_when_done_.begin(), deleted_when_done_.end());
  deleted_when_done_.clear();

  if (has_deferred_task_) {
    has_deferred_task_ = false;
    DoUpdateIfPossible();
  }

  if (GetWidget())
    GetWidget()->SynthesizeMouseMoveEvent();
}

int RichMessageListView::GetActualIndex(int index) {
  for (int i = 0; i < child_count() && i <= index; ++i)
    index += IsValidChild(child_at(i)) ? 0 : 1;
  return std::min(index, child_count());
}

bool RichMessageListView::IsValidChild(views::View* child) {
  return child->visible() &&
      deleting_views_.find(child) == deleting_views_.end() &&
      deleted_when_done_.find(child) == deleted_when_done_.end();
}

void RichMessageListView::DoUpdateIfPossible() {
  gfx::Rect child_area = GetContentsBounds();
  if (child_area.IsEmpty())
    return;

  if (animator_.get() && animator_->IsAnimating()) {
    has_deferred_task_ = true;
    return;
  }

  if (!animator_.get()) {
    animator_.reset(new views::BoundsAnimator(this));
    animator_->AddObserver(this);
  }

  int between_items =
      kMarginBetweenItems - MessageView::GetShadowInsets().bottom();
  int width = child_area.width();
  views::View* last_child = NULL;
  for (int i = child_count() - 1; i >= 0; --i) {
    views::View* child = child_at(i);
    if (IsValidChild(child)) {
      last_child = child;
      break;
    }
  }

  if (!last_child || reposition_top_ < last_child->bounds().y()) {
    int top = std::max(reposition_top_, child_area.y());
    for (int i = 0; i < child_count(); ++i) {
      views::View* child = child_at(i);
      if (child->bounds().y() < top)
        continue;
      int height = child->GetHeightForWidth(width);
      AnimateChild(child, top, height);
      if (IsValidChild(child))
        top += height + between_items;
    }
  } else {
    int bottom = reposition_top_ + last_child->GetHeightForWidth(width);
    for (int i = child_count() - 1; i >= 0; --i) {
      views::View* child = child_at(i);
      int height = child->GetHeightForWidth(child_area.width());
      AnimateChild(child, bottom - height, height);
      if (IsValidChild(child))
        bottom -= height + between_items;
    }
  }
  adding_views_.clear();
  deleting_views_.clear();
}

void RichMessageListView::AnimateChild(views::View* child,
                                       int top,
                                       int height) {
  gfx::Rect child_area = GetContentsBounds();
  if (adding_views_.find(child) != adding_views_.end()) {
    child->SetBounds(child_area.right(), top, child_area.width(), height);
    animator_->AnimateViewTo(
        child, gfx::Rect(child_area.x(), top, child_area.width(), height));
  } else if (deleting_views_.find(child) != deleting_views_.end()) {
    DCHECK(child->layer());
    // No moves, but animate to fade-out.
    animator_->AnimateViewTo(child, child->bounds());
    deleted_when_done_.insert(child);
  } else {
    gfx::Rect target(child_area.x(), top, child_area.width(), height);
    if (child->bounds().origin() != target.origin())
      animator_->AnimateViewTo(child, target);
    else
      child->SetBoundsRect(target);
  }
}

// MessageCenterButtonBar //////////////////////////////////////////////////////

MessageCenterButtonBar::MessageCenterButtonBar(MessageCenter* message_center)
    : message_center_(message_center),
      close_all_button_(NULL) {
}

MessageCenterButtonBar::~MessageCenterButtonBar() {
}

void MessageCenterButtonBar::SetCloseAllVisible(bool visible) {
  if (close_all_button_)
    close_all_button_->SetVisible(visible);
}

// MessageCenterView ///////////////////////////////////////////////////////////

MessageCenterView::MessageCenterView(MessageCenter* message_center,
                                     int max_height)
    : message_center_(message_center) {
  message_center_->AddObserver(this);
  set_notify_enter_exit_on_child(true);
  int between_child = IsRichNotificationEnabled() ? 0 : 1;
  SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, between_child));


  if (IsRichNotificationEnabled())
    button_bar_ = new RichMessageCenterButtonBar(message_center);
  else
    button_bar_ = new PoorMessageCenterButtonBar(message_center);

  const int button_height = button_bar_->GetPreferredSize().height();
  scroller_ = new BoundedScrollView(kMinScrollViewHeight,
                                    max_height - button_height);

  if (get_use_acceleration_when_possible()) {
    scroller_->SetPaintToLayer(true);
    scroller_->SetFillsBoundsOpaquely(false);
    scroller_->layer()->SetMasksToBounds(true);
  }

  message_list_view_ = IsRichNotificationEnabled() ?
      new RichMessageListView() : new MessageListView();
  no_notifications_message_view_ = new NoNotificationMessageView();
  // Set the default visibility to false, otherwise the notification has slide
  // in animation when the center is shown.
  no_notifications_message_view_->SetVisible(false);
  message_list_view_->AddChildView(no_notifications_message_view_);
  scroller_->SetContents(message_list_view_);

  AddChildView(scroller_);
  AddChildView(button_bar_);
}

MessageCenterView::~MessageCenterView() {
  message_center_->RemoveObserver(this);
}

void MessageCenterView::SetNotifications(
    const NotificationList::Notifications& notifications)  {
  message_views_.clear();
  int index = 0;
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end();
       ++iter, ++index) {
    AddNotificationAt(*(*iter), index);
    if (message_views_.size() >= kMaxVisibleMessageCenterNotifications)
      break;
  }
  NotificationsChanged();
  scroller_->RequestFocus();
}

size_t MessageCenterView::NumMessageViewsForTest() const {
  return message_list_view_->child_count();
}

void MessageCenterView::Layout() {
  scroller_->SetBounds(0, 0, width(), scroller_->GetHeightForWidth(width()));
  views::View::Layout();
  if (GetWidget())
    GetWidget()->GetRootView()->SchedulePaint();
}

bool MessageCenterView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  // Do not rely on the default scroll event handler of ScrollView because
  // the scroll happens only when the focus is on the ScrollView. The
  // notification center will allow the scrolling even when the focus is on
  // the buttons.
  if (scroller_->bounds().Contains(event.location()))
    return scroller_->OnMouseWheel(event);
  return views::View::OnMouseWheel(event);
}

void MessageCenterView::OnMouseExited(const ui::MouseEvent& event) {
  message_list_view_->ResetRepositionSession();
  NotificationsChanged();
}

void MessageCenterView::OnNotificationAdded(const std::string& id) {
  int index = 0;
  const NotificationList::Notifications& notifications =
      message_center_->GetNotifications();
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin(); iter != notifications.end();
       ++iter, ++index) {
    if ((*iter)->id() == id) {
      AddNotificationAt(*(*iter), index);
      break;
    }
    if (message_views_.size() >= kMaxVisibleMessageCenterNotifications)
      break;
  }
  NotificationsChanged();
}

void MessageCenterView::OnNotificationRemoved(const std::string& id,
                                              bool by_user) {
  for (size_t i = 0; i < message_views_.size(); ++i) {
    if (message_views_[i]->notification_id() == id) {
      if (by_user) {
        message_list_view_->SetRepositionTarget(message_views_[i]->bounds());
        if (message_views_.size() > 1 &&
            message_views_[i]->IsCloseButtonFocused()) {
          size_t next_index = i + 1;
          if (next_index >= message_views_.size())
            next_index = message_views_.size() - 2;
          message_views_[next_index]->RequestFocusOnCloseButton();
        }
      }
      message_list_view_->RemoveNotificationAt(i);
      message_views_.erase(message_views_.begin() + i);
      NotificationsChanged();
      break;
    }
  }
}

void MessageCenterView::OnNotificationUpdated(const std::string& id) {
  const NotificationList::Notifications& notifications =
      message_center_->GetNotifications();
  size_t index = 0;
  for (NotificationList::Notifications::const_iterator iter =
           notifications.begin();
       iter != notifications.end() && index < message_views_.size();
       ++iter, ++index) {
    DCHECK((*iter)->id() == message_views_[index]->notification_id());
    if ((*iter)->id() == id) {
      MessageView* view = NotificationView::Create(
          *(*iter), message_center_, true);
      view->set_scroller(scroller_);
      message_list_view_->UpdateNotificationAt(view, index);
      message_views_[index] = view;
      NotificationsChanged();
      break;
    }
  }
}

void MessageCenterView::AddNotificationAt(const Notification& notification,
                                          int index) {
  // NotificationViews are expanded by default here until
  // http://crbug.com/217902 is fixed. TODO(dharcourt): Fix.
  MessageView* view = NotificationView::Create(
      notification, message_center_, true);
  view->set_scroller(scroller_);
  message_views_.insert(message_views_.begin() + index, view);
  message_list_view_->AddNotificationAt(view, index);
  message_center_->DisplayedNotification(notification.id());
}

void MessageCenterView::NotificationsChanged() {
  if (!message_views_.empty()) {
    no_notifications_message_view_->SetVisible(false);
    button_bar_->SetCloseAllVisible(true);
    scroller_->set_focusable(true);
  } else {
    no_notifications_message_view_->SetVisible(true);
    button_bar_->SetCloseAllVisible(false);
    scroller_->set_focusable(false);
  }
  scroller_->InvalidateLayout();
  PreferredSizeChanged();
  Layout();
}

void MessageCenterView::SetNotificationViewForTest(views::View* view) {
  message_list_view_->AddNotificationAt(view, 0);
}

}  // namespace message_center
