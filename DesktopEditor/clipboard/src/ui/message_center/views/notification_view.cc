// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/notification_view.h"

#include "base/command_line.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_resources.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/text/text_elider.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/size.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/message_center_switches.h"
#include "ui/message_center/message_center_util.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"
#include "ui/message_center/views/bounded_label.h"
#include "ui/message_center/views/message_simple_view.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/widget/widget.h"

namespace {

// Dimensions.
const int kIconSize = message_center::kNotificationIconSize;
const int kLegacyIconSize = 40;
const int kIconBottomPadding = 16;
const int kTextLeftPadding = kIconSize + message_center::kIconToTextPadding;
const int kTextBottomPadding = 12;
const int kTextRightPadding = 23;
const int kItemTitleToMessagePadding = 3;
const int kButtonHeight = 38;
const int kButtonHorizontalPadding = 16;
const int kButtonVecticalPadding = 0;
const int kButtonIconTopPadding = 11;
const int kButtonIconToTitlePadding = 16;
const int kButtonTitleTopPadding = 0;

// Line limits.
const int kTitleLineLimit = 3;
const int kMessageCollapsedLineLimit = 3;
const int kMessageExpandedLineLimit = 7;

// Character limits: Displayed text will be subject to the line limits above,
// but we also remove trailing characters from text to reduce processing cost.
// Character limit = pixels per line * line limit / min. pixels per character.
const size_t kTitleCharacterLimit =
    message_center::kNotificationWidth * kTitleLineLimit / 4;
const size_t kMessageCharacterLimit =
    message_center::kNotificationWidth * kMessageExpandedLineLimit / 3;

// Notification colors. The text background colors below are used only to keep
// view::Label from modifying the text color and will not actually be drawn.
// See view::Label's RecalculateColors() for details.
const SkColor kRegularTextBackgroundColor = SK_ColorWHITE;
const SkColor kDimTextColor = SkColorSetRGB(102, 102, 102);
const SkColor kDimTextBackgroundColor = SK_ColorWHITE;
const SkColor kButtonSeparatorColor = SkColorSetRGB(234, 234, 234);
const SkColor kHoveredButtonBackgroundColor = SkColorSetRGB(243, 243, 243);

// static
views::Background* MakeBackground(
    SkColor color = message_center::kNotificationBackgroundColor) {
  return views::Background::CreateSolidBackground(color);
}

// static
views::Border* MakeEmptyBorder(int top, int left, int bottom, int right) {
  return views::Border::CreateEmptyBorder(top, left, bottom, right);
}

// static
views::Border* MakeTextBorder(int padding, int top, int bottom) {
  // Split the padding between the top and the bottom, then add the extra space.
  return MakeEmptyBorder(padding / 2 + top, kTextLeftPadding,
                         (padding + 1) / 2 + bottom, kTextRightPadding);
}

// static
views::Border* MakeSeparatorBorder(int top, int left, SkColor color) {
  return views::Border::CreateSolidSidedBorder(top, left, 0, 0, color);
}

// static
// Return true if and only if the image is null or has alpha.
bool HasAlpha(gfx::ImageSkia& image, views::Widget* widget) {
  // Determine which bitmap to use.
  ui::ScaleFactor factor = ui::SCALE_FACTOR_100P;
  if (widget) {
    factor = ui::GetScaleFactorForNativeView(widget->GetNativeView());
    if (factor == ui::SCALE_FACTOR_NONE)
      factor = ui::SCALE_FACTOR_100P;
  }

  // Extract that bitmap's alpha and look for a non-opaque pixel there.
  SkBitmap bitmap = image.GetRepresentation(factor).sk_bitmap();
  if (!bitmap.isNull()) {
    SkBitmap alpha;
    alpha.setConfig(SkBitmap::kA1_Config, bitmap.width(), bitmap.height(), 0);
    bitmap.extractAlpha(&alpha);
    for (int y = 0; y < bitmap.height(); ++y) {
      for (int x = 0; x < bitmap.width(); ++x) {
        if (alpha.getColor(x, y) != SK_ColorBLACK) {
          return true;
        }
      }
    }
  }

  // If no opaque pixel was found, return false unless the bitmap is empty.
  return bitmap.isNull();
}

// ItemView ////////////////////////////////////////////////////////////////////

// ItemViews are responsible for drawing each list notification item's title and
// message next to each other within a single column.
class ItemView : public views::View {
 public:
  ItemView(const message_center::NotificationItem& item);
  virtual ~ItemView();

  virtual void SetVisible(bool visible) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ItemView);
};

ItemView::ItemView(const message_center::NotificationItem& item) {
  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kHorizontal,
                                        0, 0, kItemTitleToMessagePadding));

  views::Label* title = new views::Label(item.title);
  title->set_collapse_when_hidden(true);
  title->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  title->SetElideBehavior(views::Label::ELIDE_AT_END);
  title->SetEnabledColor(message_center::kRegularTextColor);
  title->SetBackgroundColor(kRegularTextBackgroundColor);
  AddChildView(title);

  views::Label* message = new views::Label(item.message);
  message->set_collapse_when_hidden(true);
  message->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  message->SetElideBehavior(views::Label::ELIDE_AT_END);
  message->SetEnabledColor(kDimTextColor);
  message->SetBackgroundColor(kDimTextBackgroundColor);
  AddChildView(message);

  PreferredSizeChanged();
  SchedulePaint();
}

ItemView::~ItemView() {
}

void ItemView::SetVisible(bool visible) {
  views::View::SetVisible(visible);
  for (int i = 0; i < child_count(); ++i)
    child_at(i)->SetVisible(visible);
}

// ProportionalImageView ///////////////////////////////////////////////////////

// ProportionalImageViews center their images to preserve their proportion.
class ProportionalImageView : public views::View {
 public:
  ProportionalImageView(const gfx::ImageSkia& image);
  virtual ~ProportionalImageView();

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  gfx::Size GetImageSizeForWidth(int width);

  gfx::ImageSkia image_;

  DISALLOW_COPY_AND_ASSIGN(ProportionalImageView);
};

ProportionalImageView::ProportionalImageView(const gfx::ImageSkia& image)
    : image_(image) {
}

ProportionalImageView::~ProportionalImageView() {
}

gfx::Size ProportionalImageView::GetPreferredSize() {
  gfx::Size size = GetImageSizeForWidth(image_.width());
  return gfx::Size(size.width() + GetInsets().width(),
                   size.height() + GetInsets().height());
}

int ProportionalImageView::GetHeightForWidth(int width) {
  return GetImageSizeForWidth(width).height();
}

void ProportionalImageView::OnPaint(gfx::Canvas* canvas) {
  views::View::OnPaint(canvas);

  gfx::Size draw_size(GetImageSizeForWidth(width()));
  if (!draw_size.IsEmpty()) {
    gfx::Rect draw_bounds = GetContentsBounds();
    draw_bounds.ClampToCenteredSize(draw_size);

    gfx::Size image_size(image_.size());
    if (image_size == draw_size) {
      canvas->DrawImageInt(image_, draw_bounds.x(), draw_bounds.y());
    } else {
      // Resize case
      SkPaint paint;
      paint.setFilterBitmap(true);
      canvas->DrawImageInt(image_, 0, 0,
                           image_size.width(), image_size.height(),
                           draw_bounds.x(), draw_bounds.y(),
                           draw_size.width(), draw_size.height(),
                           true, paint);
    }
  }
}

gfx::Size ProportionalImageView::GetImageSizeForWidth(int width) {
  gfx::Size size = visible() ? image_.size() : gfx::Size();
  if (width > 0 && !size.IsEmpty()) {
    double proportion = size.height() / (double) size.width();
    size.SetSize(width, std::max(0.5 + width * proportion, 1.0));
    if (size.height() > message_center::kNotificationMaximumImageHeight) {
      int height = message_center::kNotificationMaximumImageHeight;
      size.SetSize(std::max(0.5 + height / proportion, 1.0), height);
    }
  }
  return size;
}

// NotificationButton //////////////////////////////////////////////////////////

// NotificationButtons render the action buttons of notifications.
class NotificationButton : public views::CustomButton {
 public:
  NotificationButton(views::ButtonListener* listener);
  virtual ~NotificationButton();

  void SetIcon(const gfx::ImageSkia& icon);
  void SetTitle(const string16& title);

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;

  // Overridden from views::CustomButton:
  virtual void StateChanged() OVERRIDE;

 private:
  views::ImageView* icon_;
  views::Label* title_;
};

NotificationButton::NotificationButton(views::ButtonListener* listener)
    : views::CustomButton(listener),
      icon_(NULL),
      title_(NULL) {
  set_focusable(true);
  set_request_focus_on_press(false);
  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kHorizontal,
                                        kButtonHorizontalPadding,
                                        kButtonVecticalPadding,
                                        kButtonIconToTitlePadding));
}

NotificationButton::~NotificationButton() {
}

void NotificationButton::SetIcon(const gfx::ImageSkia& image) {
  if (icon_ != NULL)
    delete icon_;  // This removes the icon from this view's children.
  if (image.isNull()) {
    icon_ = NULL;
  } else {
    icon_ = new views::ImageView();
    icon_->SetImageSize(gfx::Size(message_center::kNotificationButtonIconSize,
                                  message_center::kNotificationButtonIconSize));
    icon_->SetImage(image);
    icon_->SetHorizontalAlignment(views::ImageView::LEADING);
    icon_->SetVerticalAlignment(views::ImageView::LEADING);
    icon_->set_border(MakeEmptyBorder(kButtonIconTopPadding, 0, 0, 0));
    AddChildViewAt(icon_, 0);
  }
}

void NotificationButton::SetTitle(const string16& title) {
  if (title_ != NULL)
    delete title_;  // This removes the title from this view's children.
  if (title.empty()) {
    title_ = NULL;
  } else {
    title_ = new views::Label(title);
    title_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    title_->SetElideBehavior(views::Label::ELIDE_AT_END);
    title_->SetEnabledColor(message_center::kRegularTextColor);
    title_->SetBackgroundColor(kRegularTextBackgroundColor);
    title_->set_border(MakeEmptyBorder(kButtonTitleTopPadding, 0, 0, 0));
    AddChildView(title_);
  }
}

gfx::Size NotificationButton::GetPreferredSize() {
  return gfx::Size(message_center::kNotificationWidth, kButtonHeight);
}

int NotificationButton::GetHeightForWidth(int width) {
  return kButtonHeight;
}

void NotificationButton::OnFocus() {
  ScrollRectToVisible(GetLocalBounds());
}

void NotificationButton::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    canvas->DrawRect(gfx::Rect(2, 1, width() - 4, height() - 3),
                     message_center::kFocusBorderColor);
  }
}

void NotificationButton::StateChanged() {
  if (state() == STATE_HOVERED || state() == STATE_PRESSED)
    set_background(MakeBackground(kHoveredButtonBackgroundColor));
  else
    set_background(NULL);
}

}  // namespace

namespace message_center {

// NotificationView ////////////////////////////////////////////////////////////

// static
MessageView* NotificationView::Create(const Notification& notification,
                                      MessageCenter* message_center,
                                      bool expanded) {
  // Use MessageSimpleView for simple notifications unless rich style
  // notifications are enabled. This preserves the appearance of notifications
  // created by existing code that uses webkitNotifications.
  if (!IsRichNotificationEnabled() &&
      notification.type() == NOTIFICATION_TYPE_SIMPLE)
    return new MessageSimpleView(notification, message_center);

  switch (notification.type()) {
    case NOTIFICATION_TYPE_BASE_FORMAT:
    case NOTIFICATION_TYPE_IMAGE:
    case NOTIFICATION_TYPE_MULTIPLE:
    case NOTIFICATION_TYPE_SIMPLE:
      break;
    default:
      // If the caller asks for an unrecognized kind of view (entirely possible
      // if an application is running on an older version of this code that
      // doesn't have the requested kind of notification template), we'll fall
      // back to a notification instance that will provide at least basic
      // functionality.
      LOG(WARNING) << "Unable to fulfill request for unrecognized "
                   << "notification type " << notification.type() << ". "
                   << "Falling back to simple notification type.";
  }

  // Currently all roads lead to the generic NotificationView.
  return new NotificationView(notification, message_center, expanded);
}

NotificationView::NotificationView(const Notification& notification,
                                   MessageCenter* message_center,
                                   bool expanded)
    : MessageView(notification, message_center, expanded) {
  // Create the opaque background that's above the view's shadow.
  background_view_ = new views::View();
  background_view_->set_background(MakeBackground());

  // Create the top_view_, which collects into a vertical box all content
  // at the top of the notification (to the right of the icon) except for the
  // close button.
  top_view_ = new views::View();
  top_view_->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0));
  top_view_->set_border(MakeEmptyBorder(
      kTextTopPadding - 8, 0, kTextBottomPadding - 5, 0));

  // Create the title view if appropriate.
  title_view_ = NULL;
  if (!notification.title().empty()) {
    gfx::Font font = views::Label().font().DeriveFont(2);
    int padding = kTitleLineHeight - font.GetHeight();
    title_view_ = new BoundedLabel(
        ui::TruncateString(notification.title(), kTitleCharacterLimit), font);
    title_view_->SetLineHeight(kTitleLineHeight);
    title_view_->SetLineLimit(kTitleLineLimit);
    title_view_->SetColors(message_center::kRegularTextColor,
                           kRegularTextBackgroundColor);
    title_view_->set_border(MakeTextBorder(padding, 3, 0));
    top_view_->AddChildView(title_view_);
  }

  // Create the message view if appropriate.
  message_view_ = NULL;
  if (!notification.message().empty()) {
    int padding = kMessageLineHeight - views::Label().font().GetHeight();
    message_view_ = new BoundedLabel(
        ui::TruncateString(notification.message(), kMessageCharacterLimit));
    message_view_->SetLineHeight(kMessageLineHeight);
    message_view_->SetVisible(!is_expanded() || !notification.items().size());
    message_view_->SetColors(kDimTextColor, kDimTextBackgroundColor);
    message_view_->set_border(MakeTextBorder(padding, 4, 0));
    top_view_->AddChildView(message_view_);
  }

  // Create the list item views (up to a maximum).
  int padding = kMessageLineHeight - views::Label().font().GetHeight();
  std::vector<NotificationItem> items = notification.items();
  for (size_t i = 0; i < items.size() && i < kNotificationMaximumItems; ++i) {
    ItemView* item_view = new ItemView(items[i]);
    item_view->SetVisible(is_expanded());
    item_view->set_border(MakeTextBorder(padding, i ? 0 : 4, 0));
    item_views_.push_back(item_view);
    top_view_->AddChildView(item_view);
  }

  // Create the notification icon view.
  gfx::ImageSkia icon = notification.icon().AsImageSkia();
  if (notification.type() == NOTIFICATION_TYPE_SIMPLE &&
      (icon.width() != kIconSize ||
       icon.height() != kIconSize ||
       HasAlpha(icon, GetWidget()))) {
    views::ImageView* icon_view = new views::ImageView();
    icon_view->SetImage(icon);
    icon_view->SetImageSize(gfx::Size(kLegacyIconSize, kLegacyIconSize));
    icon_view->SetHorizontalAlignment(views::ImageView::CENTER);
    icon_view->SetVerticalAlignment(views::ImageView::CENTER);
    icon_view->set_background(MakeBackground(kLegacyIconBackgroundColor));
    icon_view_ = icon_view;
  } else {
    icon_view_ = new ProportionalImageView(icon);
  }

  // Create the bottom_view_, which collects into a vertical box all content
  // below the notification icon except for the expand button.
  bottom_view_ = new views::View();
  bottom_view_->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0));

  // Create the image view if appropriate.
  image_view_ = NULL;
  if (!notification.image().IsEmpty()) {
    image_view_ = new ProportionalImageView(notification.image().AsImageSkia());
    image_view_->SetVisible(is_expanded());
    bottom_view_->AddChildView(image_view_);
  }

  // Create action buttons if appropriate.
  std::vector<ButtonInfo> buttons = notification.buttons();
  for (size_t i = 0; i < buttons.size(); ++i) {
    views::View* separator = new views::ImageView();
    separator->set_border(MakeSeparatorBorder(1, 0, kButtonSeparatorColor));
    bottom_view_->AddChildView(separator);
    NotificationButton* button = new NotificationButton(this);
    ButtonInfo button_info = buttons[i];
    button->SetTitle(button_info.title);
    button->SetIcon(button_info.icon.AsImageSkia());
    action_buttons_.push_back(button);
    bottom_view_->AddChildView(button);
  }

  // Put together the different content and control views. Layering those allows
  // for proper layout logic and it also allows the close and expand buttons to
  // overlap the content as needed to provide large enough click and touch areas
  // (<http://crbug.com/168822> and <http://crbug.com/168856>).
  AddChildView(background_view_);
  AddChildView(top_view_);
  AddChildView(icon_view_);
  AddChildView(bottom_view_);
  AddChildView(close_button());
  AddChildView(expand_button());
}

NotificationView::~NotificationView() {
}

gfx::Size NotificationView::GetPreferredSize() {
  int top_width = top_view_->GetPreferredSize().width();
  int bottom_width = bottom_view_->GetPreferredSize().width();
  int preferred_width = std::max(top_width, bottom_width) + GetInsets().width();
  return gfx::Size(preferred_width, GetHeightForWidth(preferred_width));
}

int NotificationView::GetHeightForWidth(int width) {
  // Get the height assuming no line limit changes.
  int content_width = width - GetInsets().width();
  int top_height = top_view_->GetHeightForWidth(content_width);
  int bottom_height = bottom_view_->GetHeightForWidth(content_width);
  int content_height = std::max(top_height, kIconSize) + bottom_height;

  // <http://crbug.com/230448> Fix: Adjust the height when the message_view's
  // line limit would be different for the specified width than it currently is.
  // TODO(dharcourt): Avoid BoxLayout and directly compute the correct height.
  if (message_view_) {
    int used_limit = message_view_->GetLineLimit();
    int correct_limit = GetMessageLineLimit(width);
    if (used_limit != correct_limit) {
      content_height -= GetMessageHeight(content_width, used_limit);
      content_height += GetMessageHeight(content_width, correct_limit);
    }
  }

  // Adjust the height to make sure there is at least 16px of space below the
  // icon if there is any space there (<http://crbug.com/232966>).
  if (content_height > kIconSize)
    content_height = std::max(content_height, kIconSize + kIconBottomPadding);

  return content_height + GetInsets().height();
}

void NotificationView::Layout() {
  gfx::Insets insets = GetInsets();
  int content_width = width() - insets.width();
  int content_right = width() - insets.right();

  // Before any resizing, set or adjust the number of message lines.
  if (message_view_)
    message_view_->SetLineLimit(GetMessageLineLimit(width()));

  // Background.
  background_view_->SetBounds(insets.left(), insets.top(),
                              content_width, height() - insets.height());

  // Top views.
  int top_height = top_view_->GetHeightForWidth(content_width);
  top_view_->SetBounds(insets.left(), insets.top(), content_width, top_height);

  // Icon.
  icon_view_->SetBounds(insets.left(), insets.top(), kIconSize, kIconSize);

  // Bottom views.
  int bottom_y = insets.top() + std::max(top_height, kIconSize);
  int bottom_height = bottom_view_->GetHeightForWidth(content_width);
  bottom_view_->SetBounds(insets.left(), bottom_y,
                          content_width, bottom_height);

  // Close button.
  gfx::Size close_size(close_button()->GetPreferredSize());
  close_button()->SetBounds(content_right - close_size.width(), insets.top(),
                            close_size.width(), close_size.height());

  // Expand button.
  gfx::Size expand_size(expand_button()->GetPreferredSize());
  int expand_y = bottom_y - expand_size.height();
  expand_button()->SetVisible(IsExpansionNeeded(width()));
  expand_button()->SetBounds(content_right - expand_size.width(), expand_y,
                             expand_size.width(), expand_size.height());
}

void NotificationView::ScrollRectToVisible(const gfx::Rect& rect) {
  // Notification want to show the whole notification when a part of it (like
  // a button) gets focused.
  views::View::ScrollRectToVisible(GetLocalBounds());
}

void NotificationView::ButtonPressed(views::Button* sender,
                                     const ui::Event& event) {
  // See if the button pressed was an action button.
  for (size_t i = 0; i < action_buttons_.size(); ++i) {
    if (sender == action_buttons_[i]) {
      message_center()->ClickOnNotificationButton(notification_id(), i);
      return;
    }
  }

  // Adjust notification subviews for expansion.
  if (sender == expand_button()) {
    if (message_view_ && item_views_.size())
      message_view_->SetVisible(false);
    for (size_t i = 0; i < item_views_.size(); ++i)
      item_views_[i]->SetVisible(true);
    if (image_view_)
      image_view_->SetVisible(true);
    expand_button()->SetVisible(false);
    PreferredSizeChanged();
    SchedulePaint();

    return;
  }

  // Let the superclass handled anything other than action buttons.
  // Warning: This may cause the NotificationView itself to be deleted,
  // so don't do anything afterwards.
  MessageView::ButtonPressed(sender, event);
}

bool NotificationView::IsExpansionNeeded(int width) {
  return (!is_expanded() &&
              (image_view_ ||
                  item_views_.size() ||
                  IsMessageExpansionNeeded(width)));
}

bool NotificationView::IsMessageExpansionNeeded(int width) {
  int current = GetMessageLines(width, GetMessageLineLimit(width));
  int expanded = GetMessageLines(width, kMessageExpandedLineLimit);
  return current < expanded;
}

int NotificationView::GetMessageLineLimit(int width) {
  // Expanded notifications get a larger limit, except for image notifications,
  // whose images must be kept flush against their icons.
  if (is_expanded() && !image_view_)
    return kMessageExpandedLineLimit;

  // If there's a title ensure title + message lines <= collapsed line limit.
  if (title_view_) {
    int title_lines = title_view_->GetLinesForWidthAndLimit(width, -1);
    return std::max(kMessageCollapsedLineLimit - title_lines, 0);
  }

  return kMessageCollapsedLineLimit;
}

int NotificationView::GetMessageLines(int width, int limit) {
  return message_view_ ?
         message_view_->GetLinesForWidthAndLimit(width, limit) : 0;
}

int NotificationView::GetMessageHeight(int width, int limit) {
  return message_view_ ?
         message_view_->GetSizeForWidthAndLines(width, limit).height() : 0;
}

}  // namespace message_center
