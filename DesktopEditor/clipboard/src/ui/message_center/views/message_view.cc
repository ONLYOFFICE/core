// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_view.h"

#include "grit/ui_resources.h"
#include "grit/ui_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/compositor/scoped_layer_animation_settings.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/skia_util.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/message_center/message_center_util.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/menu/menu_runner.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/widget/widget.h"

namespace {

const int kCloseIconTopPadding = 5;
const int kCloseIconRightPadding = 5;
const int kExpandIconBottomPadding = 8;
const int kExpandIconRightPadding = 11;

const int kShadowOffset = 1;
const int kShadowBlur = 4;

const SkColor kShadowColor = SkColorSetARGB(0.3 * 255, 0, 0, 0);
const SkColor kTransparentColor = SkColorSetARGB(0, 0, 0, 0);

// Menu constants
const int kTogglePermissionCommand = 0;
const int kToggleExtensionCommand = 1;
const int kShowSettingsCommand = 2;

// ControlButtons are ImageButtons whose image can be padded within the button.
// This allows the creation of buttons like the notification close and expand
// buttons whose clickable areas extends beyond their image areas
// (<http://crbug.com/168822>) without the need to create and maintain
// corresponding resource images with alpha padding. In the future, this class
// will also allow for buttons whose touch areas extend beyond their clickable
// area (<http://crbug.com/168856>).
class ControlButton : public views::ImageButton {
 public:
  ControlButton(views::ButtonListener* listener);
  virtual ~ControlButton();

  // Overridden from views::ImageButton:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;

  // The SetPadding() method also sets the button's image alignment (positive
  // values yield left/top alignments, negative values yield right/bottom ones,
  // and zero values center/middle ones). ImageButton::SetImageAlignment() calls
  // will not affect ControlButton image alignments.
  void SetPadding(int horizontal_padding, int vertical_padding);

  void SetNormalImage(int resource_id);
  void SetHoveredImage(int resource_id);
  void SetPressedImage(int resource_id);

 protected:
  gfx::Point ComputePaddedImagePaintPosition(const gfx::ImageSkia& image);

 private:
  gfx::Insets padding_;

  DISALLOW_COPY_AND_ASSIGN(ControlButton);
};

ControlButton::ControlButton(views::ButtonListener* listener)
  : views::ImageButton(listener) {
  set_focusable(true);
  set_request_focus_on_press(false);
}

ControlButton::~ControlButton() {
}

void ControlButton::SetPadding(int horizontal_padding, int vertical_padding) {
  padding_.Set(std::max(vertical_padding, 0),
               std::max(horizontal_padding, 0),
               std::max(-vertical_padding, 0),
               std::max(-horizontal_padding, 0));
}

void ControlButton::SetNormalImage(int resource_id) {
  SetImage(views::CustomButton::STATE_NORMAL,
           ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
               resource_id));
}

void ControlButton::SetHoveredImage(int resource_id) {
  SetImage(views::CustomButton::STATE_HOVERED,
           ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
               resource_id));
}

void ControlButton::SetPressedImage(int resource_id) {
  SetImage(views::CustomButton::STATE_PRESSED,
           ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
               resource_id));
}

gfx::Size ControlButton::GetPreferredSize() {
  return gfx::Size(message_center::kControlButtonSize,
                   message_center::kControlButtonSize);
}

void ControlButton::OnPaint(gfx::Canvas* canvas) {
  // This is the same implementation as ImageButton::OnPaint except
  // that it calls ComputePaddedImagePaintPosition() instead of
  // ComputeImagePaintPosition(), in effect overriding that private method.
  View::OnPaint(canvas);
  gfx::ImageSkia image = GetImageToPaint();
  if (!image.isNull()) {
    gfx::Point position = ComputePaddedImagePaintPosition(image);
    if (!background_image_.isNull())
      canvas->DrawImageInt(background_image_, position.x(), position.y());
    canvas->DrawImageInt(image, position.x(), position.y());
    if (!overlay_image_.isNull())
      canvas->DrawImageInt(overlay_image_, position.x(), position.y());
  }
  OnPaintFocusBorder(canvas);
}

void ControlButton::OnFocus() {
  ScrollRectToVisible(GetLocalBounds());
}

void ControlButton::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    canvas->DrawRect(gfx::Rect(2, 1, width() - 4, height() - 3),
                     message_center::kFocusBorderColor);
  }
}

gfx::Point ControlButton::ComputePaddedImagePaintPosition(
    const gfx::ImageSkia& image) {
  gfx::Vector2d offset;
  gfx::Rect bounds = GetContentsBounds();
  bounds.Inset(padding_);

  if (padding_.left() == 0 && padding_.right() == 0)
    offset.set_x((bounds.width() - image.width()) / 2);  // Center align.
  else if (padding_.right() > 0)
    offset.set_x(bounds.width() - image.width());  // Right align.

  if (padding_.top() == 0 && padding_.bottom() == 0)
    offset.set_y((bounds.height() - image.height()) / 2);  // Middle align.
  else if (padding_.bottom() > 0)
    offset.set_y(bounds.height() - image.height());  // Bottom align.

  return bounds.origin() + offset;
}

// A border to provide the shadow for each card.
// Current shadow should look like css box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3)
class ShadowBorder : public views::Border {
 public:
  ShadowBorder() : views::Border() {}
  virtual ~ShadowBorder() {}

 protected:
  // Overridden from views::Border:
  virtual void Paint(const views::View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ShadowBorder);
};

void ShadowBorder::Paint(const views::View& view, gfx::Canvas* canvas) {
  SkPaint paint;
  std::vector<gfx::ShadowValue> shadows;
  shadows.push_back(gfx::ShadowValue(gfx::Point(), kShadowBlur, kShadowColor));
  skia::RefPtr<SkDrawLooper> looper = gfx::CreateShadowDrawLooper(shadows);
  paint.setLooper(looper.get());
  paint.setColor(kTransparentColor);
  paint.setStrokeJoin(SkPaint::kRound_Join);
  gfx::Rect bounds(view.size());
  bounds.Inset(gfx::Insets(kShadowBlur / 2, kShadowBlur / 2,
                           kShadowBlur / 2, kShadowBlur / 2));
  canvas->DrawRect(bounds, paint);
}

gfx::Insets ShadowBorder::GetInsets() const {
  return message_center::MessageView::GetShadowInsets();
}

// A dropdown menu for notifications.
class MenuModel : public ui::SimpleMenuModel,
                  public ui::SimpleMenuModel::Delegate {
 public:
  MenuModel(message_center::MessageCenter* message_center,
            const std::string& notification_id,
            const string16& display_source,
            const std::string& extension_id);
  virtual ~MenuModel();

  // Overridden from ui::SimpleMenuModel::Delegate:
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  message_center::MessageCenter* message_center_;  // Weak reference.
  std::string notification_id_;

  DISALLOW_COPY_AND_ASSIGN(MenuModel);
};

MenuModel::MenuModel(message_center::MessageCenter* message_center,
                     const std::string& notification_id,
                     const string16& display_source,
                     const std::string& extension_id)
    : ui::SimpleMenuModel(this),
      message_center_(message_center),
      notification_id_(notification_id) {
  // Add 'disable notifications' menu item.
  if (!extension_id.empty() && !display_source.empty()) {
    AddItem(kToggleExtensionCommand,
            l10n_util::GetStringFUTF16(IDS_MESSAGE_CENTER_EXTENSIONS_DISABLE,
                                       display_source));
  } else if (!display_source.empty()) {
    AddItem(kTogglePermissionCommand,
            l10n_util::GetStringFUTF16(IDS_MESSAGE_CENTER_SITE_DISABLE,
                                       display_source));
  }
  // Add settings menu item.
  if (message_center::IsRichNotificationEnabled() || !display_source.empty()) {
    AddItem(kShowSettingsCommand,
            l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_SETTINGS));
  }
}

MenuModel::~MenuModel() {
}

bool MenuModel::IsItemForCommandIdDynamic(int command_id) const {
  return false;
}

bool MenuModel::IsCommandIdChecked(int command_id) const {
  return false;
}

bool MenuModel::IsCommandIdEnabled(int command_id) const {
  return true;
}

bool MenuModel::GetAcceleratorForCommandId(int command_id,
                                           ui::Accelerator* accelerator) {
  return false;
}

void MenuModel::ExecuteCommand(int command_id, int event_flags) {
  switch (command_id) {
    case kToggleExtensionCommand:
      message_center_->DisableNotificationsByExtension(notification_id_);
      break;
    case kTogglePermissionCommand:
      message_center_->DisableNotificationsByUrl(notification_id_);
      break;
    case kShowSettingsCommand:
      if (message_center::IsRichNotificationEnabled())
        message_center_->ShowNotificationSettingsDialog(NULL);
      else
        message_center_->ShowNotificationSettings(notification_id_);
      break;
    default:
      NOTREACHED();
  }
}

} // namespace

namespace message_center {

MessageView::MessageView(const Notification& notification,
                         MessageCenter* message_center,
                         bool expanded)
    : message_center_(message_center),
      notification_id_(notification.id()),
      display_source_(notification.display_source()),
      extension_id_(notification.extension_id()),
      scroller_(NULL),
      is_expanded_(expanded) {
  ControlButton *close = new ControlButton(this);
  close->SetPadding(-kCloseIconRightPadding, kCloseIconTopPadding);
  close->SetNormalImage(IDR_NOTIFICATION_CLOSE);
  close->SetHoveredImage(IDR_NOTIFICATION_CLOSE_HOVER);
  close->SetPressedImage(IDR_NOTIFICATION_CLOSE_PRESSED);
  close->set_owned_by_client();
  close_button_.reset(close);

  ControlButton *expand = new ControlButton(this);
  expand->SetPadding(-kExpandIconRightPadding, -kExpandIconBottomPadding);
  expand->SetNormalImage(IDR_NOTIFICATION_EXPAND);
  expand->SetHoveredImage(IDR_NOTIFICATION_EXPAND_HOVER);
  expand->SetPressedImage(IDR_NOTIFICATION_EXPAND_PRESSED);
  expand->set_owned_by_client();
  expand_button_.reset(expand);

  if (IsRichNotificationEnabled())
    set_border(new ShadowBorder());
}

MessageView::MessageView() {
}

MessageView::~MessageView() {
}

// static
gfx::Insets MessageView::GetShadowInsets() {
  return gfx::Insets(kShadowBlur / 2 - kShadowOffset,
                     kShadowBlur / 2,
                     kShadowBlur / 2 + kShadowOffset,
                     kShadowBlur / 2);
}

bool MessageView::IsCloseButtonFocused() {
  views::FocusManager* focus_manager = GetFocusManager();
  return focus_manager && focus_manager->GetFocusedView() == close_button();
}

void MessageView::RequestFocusOnCloseButton() {
  close_button_->RequestFocus();
}

bool MessageView::OnMousePressed(const ui::MouseEvent& event) {
  if (event.flags() & ui::EF_RIGHT_MOUSE_BUTTON) {
    ShowMenu(event.location());
    return true;
  }
  message_center_->ClickOnNotification(notification_id_);
  return true;
}

void MessageView::OnGestureEvent(ui::GestureEvent* event) {
  if (event->type() == ui::ET_GESTURE_TAP) {
    message_center_->ClickOnNotification(notification_id_);
    event->SetHandled();
    return;
  }

  if (event->type() == ui::ET_GESTURE_LONG_PRESS) {
    ShowMenu(event->location());
    event->SetHandled();
    return;
  }

  SlideOutView::OnGestureEvent(event);
  // Do not return here by checking handled(). SlideOutView calls SetHandled()
  // even though the scroll gesture doesn't make no (or little) effects on the
  // slide-out behavior. See http://crbug.com/172991

  if (!event->IsScrollGestureEvent())
    return;

  if (scroller_)
    scroller_->OnGestureEvent(event);
  event->SetHandled();
}

void MessageView::ButtonPressed(views::Button* sender,
                                const ui::Event& event) {
  if (sender == close_button()) {
    message_center_->RemoveNotification(notification_id_, true);  // By user.
  } else if (sender == expand_button()) {
    is_expanded_ = true;
    message_center_->ExpandNotification(notification_id_);
  }
}

void MessageView::ShowMenu(gfx::Point screen_location) {
  MenuModel menu_model(message_center_, notification_id_,
                       display_source_, extension_id_);
  if (menu_model.GetItemCount() == 0)
    return;

  views::MenuRunner menu_runner(&menu_model);

  views::View::ConvertPointToScreen(this, &screen_location);
  ignore_result(menu_runner.RunMenuAt(
      GetWidget()->GetTopLevelWidget(),
      NULL,
      gfx::Rect(screen_location, gfx::Size()),
      views::MenuItemView::TOPRIGHT,
      views::MenuRunner::HAS_MNEMONICS));
}

void MessageView::OnSlideOut() {
  message_center_->RemoveNotification(notification_id_, true);  // By user.
}

}  // namespace message_center
