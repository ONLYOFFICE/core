// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_simple_view.h"

#include "grit/ui_resources.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/notification.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/layout/grid_layout.h"

namespace message_center {

const SkColor kNotificationColor = SkColorSetRGB(0xfe, 0xfe, 0xfe);
const SkColor kNotificationReadColor = SkColorSetRGB(0xfa, 0xfa, 0xfa);

MessageSimpleView::MessageSimpleView(const Notification& notification,
                                     MessageCenter* message_center)
    : MessageView(notification, message_center, false) {
  set_focusable(true);
  views::ImageButton* close = new views::ImageButton(this);
  close->SetImage(
      views::CustomButton::STATE_NORMAL,
      ResourceBundle::GetSharedInstance().GetImageSkiaNamed(IDR_MESSAGE_CLOSE));
  close->SetImageAlignment(views::ImageButton::ALIGN_CENTER,
                           views::ImageButton::ALIGN_MIDDLE);
  old_style_close_button_.reset(close);
  SetUpView(notification);
}

MessageSimpleView::~MessageSimpleView() {
  // old_style_close_button_ has to be cleared before content_view_ is cleared.
  // Otherwise, resetting content_view_ will delete old_style_close_button_ so
  // a double-free happens.
  old_style_close_button_.reset();
  content_view_.reset();
}

void MessageSimpleView::Layout() {
  if (content_view_) {
    gfx::Rect contents_bounds = GetContentsBounds();
    content_view_->SetBoundsRect(contents_bounds);
  }
}

gfx::Size MessageSimpleView::GetPreferredSize() {
  if (!content_view_)
    return gfx::Size();
  gfx::Size size = content_view_->GetPreferredSize();
  if (border()) {
    gfx::Insets border_insets = border()->GetInsets();
    size.Enlarge(border_insets.width(), border_insets.height());
  }
  return size;
}

int MessageSimpleView::GetHeightForWidth(int width) {
  if (!content_view_)
    return 0;
  return content_view_->GetHeightForWidth(width);
}

void MessageSimpleView::SetUpView(const Notification& notification) {
  views::ImageView* icon = new views::ImageView;
  icon->SetImageSize(
      gfx::Size(kWebNotificationIconSize, kWebNotificationIconSize));
  icon->SetImage(notification.icon().AsImageSkia());

  views::Label* title = new views::Label(notification.title());
  title->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  title->SetFont(title->font().DeriveFont(0, gfx::Font::BOLD));
  views::Label* message = new views::Label(notification.message());
  message->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  message->SetMultiLine(true);

  SkColor bg_color = notification.is_read() ?
      kNotificationReadColor : kNotificationColor;
  content_view_.reset(new views::View);
  content_view_->set_background(
      views::Background::CreateSolidBackground(bg_color));
  AddChildView(content_view_.get());
  views::GridLayout* layout = new views::GridLayout(content_view_.get());
  content_view_->SetLayoutManager(layout);

  views::ColumnSet* columns = layout->AddColumnSet(0);

  const int padding_width = kPaddingHorizontal / 2;
  columns->AddPaddingColumn(0, padding_width);

  // Notification Icon.
  columns->AddColumn(views::GridLayout::CENTER, views::GridLayout::LEADING,
                     0, /* resize percent */
                     views::GridLayout::FIXED,
                     kWebNotificationIconSize, kWebNotificationIconSize);

  columns->AddPaddingColumn(0, padding_width);

  // Notification message text.
  const int message_width = kNotificationWidth - kWebNotificationIconSize -
      kWebNotificationButtonWidth - (padding_width * 3) -
      (scroller() ? scroller()->GetScrollBarWidth() : 0);
  columns->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                     100, /* resize percent */
                     views::GridLayout::FIXED,
                     message_width, message_width);

  columns->AddPaddingColumn(0, padding_width);
  message->SizeToFit(message_width);

  // Close button.
  columns->AddColumn(views::GridLayout::CENTER, views::GridLayout::LEADING,
                     0, /* resize percent */
                     views::GridLayout::FIXED,
                     kWebNotificationButtonWidth,
                     kWebNotificationButtonWidth);

  // Layout rows
  layout->AddPaddingRow(0, kPaddingBetweenItems);

  layout->StartRow(0, 0);
  layout->AddView(icon, 1, 2);
  layout->AddView(title, 1, 1);
  layout->AddView(old_style_close_button_.get(), 1, 1);

  layout->StartRow(0, 0);
  layout->SkipColumns(2);
  layout->AddView(message, 1, 1);
  layout->AddPaddingRow(0, kPaddingBetweenItems);
}

void MessageSimpleView::ButtonPressed(views::Button* sender,
                                      const ui::Event& event) {
  bool close = (sender == old_style_close_button_.get());
  MessageView::ButtonPressed(close ? close_button() : sender, event);
}

}  // namespace message_center
