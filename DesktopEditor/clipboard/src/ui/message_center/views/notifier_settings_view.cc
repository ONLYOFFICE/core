// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/notifier_settings_view.h"

#include "grit/ui_strings.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/size.h"
#include "ui/message_center/message_center_constants.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/scrollbar/kennedy_scroll_bar.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/aura/window.h"
#endif

namespace message_center {
namespace {

const int kSpaceInButtonComponents = 16;
const int kMarginWidth = 16;
const int kMinimumWindowWidth = 320;
const int kMinimumWindowHeight = 480;
const int kEntryHeight = kMinimumWindowHeight / 10;
const SkColor kSeparatorColor = SkColorSetRGB(0xcc, 0xcc, 0xcc);

NotifierSettingsView* settings_view_ = NULL;

// The view to guarantee the 48px height and place the contents at the
// middle. It also guarantee the left margin.
class EntryView : public views::View {
 public:
  EntryView(views::View* contents);
  virtual ~EntryView();

  // Overridden from views::View:
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(EntryView);
};

EntryView::EntryView(views::View* contents) {
  AddChildView(contents);
}

EntryView::~EntryView() {
}

void EntryView::Layout() {
  DCHECK_EQ(1, child_count());
  views::View* content = child_at(0);
  int content_width = width() - kMarginWidth * 2;
  int content_height = content->GetHeightForWidth(content_width);
  int y = std::max((height() - content_height) / 2, 0);
  content->SetBounds(kMarginWidth, y, content_width, content_height);
}

gfx::Size EntryView::GetPreferredSize() {
  DCHECK_EQ(1, child_count());
  gfx::Size size = child_at(0)->GetPreferredSize();
  size.ClampToMin(gfx::Size(kMinimumWindowWidth, kEntryHeight));
  return size;
}

void EntryView::OnFocus() {
  ScrollRectToVisible(GetLocalBounds());
}

void EntryView::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    canvas->DrawRect(gfx::Rect(2, 1, width() - 4, height() - 3),
                     kFocusBorderColor);
  }
}

bool EntryView::OnKeyPressed(const ui::KeyEvent& event) {
  return child_at(0)->OnKeyPressed(event);
}

bool EntryView::OnKeyReleased(const ui::KeyEvent& event) {
  return child_at(0)->OnKeyReleased(event);
}

// The separator line between the title and the scroll view. Currently
// it is achieved as a top border of the scroll view.
class Separator : public views::Border {
 public:
  Separator();
  virtual ~Separator();

  // Overridden from views::Border:
  virtual void Paint(const views::View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(Separator);
};

Separator::Separator() {
}

Separator::~Separator() {
}

void Separator::Paint(const views::View& view, gfx::Canvas* canvas) {
  gfx::Rect bounds(view.GetLocalBounds());
  bounds.Inset(kMarginWidth, 0);
  canvas->DrawLine(bounds.origin(), bounds.top_right(), kSeparatorColor);
}

gfx::Insets Separator::GetInsets() const {
  // Do not set the insets for the separator. This means that the separater will
  // overlap with the top of the scroll contents, otherwise the scroll view will
  // create a scroll bar if the contents height is exactly same as the height of
  // the window.
  return gfx::Insets();
}

}  // namespace

NotifierSettingsDelegate* ShowSettings(NotifierSettingsProvider* provider,
                                       gfx::NativeView context) {
  if (!settings_view_) {
    settings_view_ = NotifierSettingsView::Create(provider, context);
  } else {
    settings_view_->GetWidget()->StackAtTop();
    settings_view_->GetWidget()->Activate();
  }
  return settings_view_;
}

// We do not use views::Checkbox class directly because it doesn't support
// showing 'icon'.
class NotifierSettingsView::NotifierButton : public views::CustomButton,
                                             public views::ButtonListener {
 public:
  NotifierButton(Notifier* notifier, views::ButtonListener* listener)
      : views::CustomButton(listener),
        notifier_(notifier),
        icon_view_(NULL),
        checkbox_(new views::Checkbox(string16())) {
    DCHECK(notifier);
    SetLayoutManager(new views::BoxLayout(
        views::BoxLayout::kHorizontal, 0, 0, kSpaceInButtonComponents));
    checkbox_->SetChecked(notifier_->enabled);
    checkbox_->set_listener(this);
    checkbox_->set_focusable(false);
    AddChildView(checkbox_);
    UpdateIconImage(notifier_->icon);
    AddChildView(new views::Label(notifier_->name));
  }

  void UpdateIconImage(const gfx::Image& icon) {
    notifier_->icon = icon;
    if (icon.IsEmpty()) {
      delete icon_view_;
      icon_view_ = NULL;
    } else {
      if (!icon_view_) {
        icon_view_ = new views::ImageView();
        AddChildViewAt(icon_view_, 1);
      }
      icon_view_->SetImage(icon.ToImageSkia());
      icon_view_->SetImageSize(gfx::Size(kSettingsIconSize, kSettingsIconSize));
    }
    Layout();
    SchedulePaint();
  }

  void SetChecked(bool checked) {
    checkbox_->SetChecked(checked);
    notifier_->enabled = checked;
  }

  bool checked() const {
    return checkbox_->checked();
  }

  const Notifier& notifier() const {
    return *notifier_.get();
  }

 private:
  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* button,
                             const ui::Event& event) OVERRIDE {
    DCHECK(button == checkbox_);
    // The checkbox state has already changed at this point, but we'll update
    // the state on NotifierSettingsView::ButtonPressed() too, so here change
    // back to the previous state.
    checkbox_->SetChecked(!checkbox_->checked());
    CustomButton::NotifyClick(event);
  }

  scoped_ptr<Notifier> notifier_;
  views::ImageView* icon_view_;
  views::Checkbox* checkbox_;

  DISALLOW_COPY_AND_ASSIGN(NotifierButton);
};

// static
NotifierSettingsView* NotifierSettingsView::Create(
    NotifierSettingsProvider* delegate,
    gfx::NativeView context) {
  NotifierSettingsView* view = new NotifierSettingsView(delegate);
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams params(views::Widget::InitParams::TYPE_WINDOW);
  params.delegate = view;
  params.context = context;
  widget->Init(params);
  widget->CenterWindow(widget->GetWindowBoundsInScreen().size());
  widget->Show();

  return view;
}

void NotifierSettingsView::UpdateIconImage(const std::string& id,
                                           const gfx::Image& icon) {
  for (std::set<NotifierButton*>::iterator iter = buttons_.begin();
       iter != buttons_.end(); ++iter) {
    if ((*iter)->notifier().id == id) {
      (*iter)->UpdateIconImage(icon);
      return;
    }
  }
}

void NotifierSettingsView::UpdateFavicon(const GURL& url,
                                         const gfx::Image& icon) {
  for (std::set<NotifierButton*>::iterator iter = buttons_.begin();
       iter != buttons_.end(); ++iter) {
    if ((*iter)->notifier().url == url) {
      (*iter)->UpdateIconImage(icon);
      return;
    }
  }
}

NotifierSettingsView::NotifierSettingsView(
    NotifierSettingsProvider* delegate)
    : delegate_(delegate) {
  DCHECK(delegate_);

  set_background(views::Background::CreateSolidBackground(SK_ColorWHITE));
  set_focusable(true);
  set_focus_border(NULL);

  gfx::Font title_font =
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::MediumFont);
  views::Label* title_label = new views::Label(
      l10n_util::GetStringUTF16(IDS_MESSAGE_CENTER_SETTINGS_BUTTON_LABEL),
      title_font);
  title_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  title_label->SetMultiLine(true);
  title_entry_ = new EntryView(title_label);
  AddChildView(title_entry_);

  scroller_ = new views::ScrollView();
  scroller_->set_border(new Separator());
  scroller_->SetVerticalScrollBar(new views::KennedyScrollBar(false));
  AddChildView(scroller_);

  views::View* contents_view = new views::View();
  contents_view->SetLayoutManager(new views::BoxLayout(
      views::BoxLayout::kVertical, 0, 0, 0));

  views::Label* top_label = new views::Label(l10n_util::GetStringUTF16(
      IDS_MESSAGE_CENTER_SETTINGS_DIALOG_DESCRIPTION));
  top_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  top_label->SetMultiLine(true);
  top_label->SizeToFit(kMinimumWindowWidth - kMarginWidth * 2);
  contents_view->AddChildView(new EntryView(top_label));

  std::vector<Notifier*> notifiers;
  delegate_->GetNotifierList(&notifiers);
  views::View* first_view = NULL;
  views::View* last_view = NULL;
  for (size_t i = 0; i < notifiers.size(); ++i) {
    NotifierButton* button = new NotifierButton(notifiers[i], this);
    EntryView* entry = new EntryView(button);
    entry->set_focusable(true);
    contents_view->AddChildView(entry);
    buttons_.insert(button);
    if (i == 0)
      first_view = entry;
    last_view = entry;
  }
  if (last_view)
    last_view->SetNextFocusableView(first_view);
  scroller_->SetContents(contents_view);

  contents_view->SetBoundsRect(gfx::Rect(contents_view->GetPreferredSize()));
}

NotifierSettingsView::~NotifierSettingsView() {
  settings_view_ = NULL;
}

views::View* NotifierSettingsView::GetInitiallyFocusedView() {
  return this;
}

void NotifierSettingsView::WindowClosing() {
  if (delegate_)
    delegate_->OnNotifierSettingsClosing();
}

views::View* NotifierSettingsView::GetContentsView() {
  return this;
}

bool NotifierSettingsView::CanResize() const {
  return true;
}

void NotifierSettingsView::Layout() {
  int title_height = title_entry_->GetPreferredSize().height();
  title_entry_->SetBounds(0, 0, width(), title_height);
  views::View* contents_view = scroller_->contents();
  int content_width = width();
  int content_height = contents_view->GetHeightForWidth(content_width);
  if (title_height + content_height > kMinimumWindowHeight) {
    content_width -= scroller_->GetScrollBarWidth();
    content_height = contents_view->GetHeightForWidth(content_width);
  }
  contents_view->SetBounds(0, 0, content_width, content_height);
  scroller_->SetBounds(0, title_height, width(), height() - title_height);
}

gfx::Size NotifierSettingsView::GetMinimumSize() {
  gfx::Size size(kMinimumWindowWidth, kMinimumWindowHeight);
  int total_height = title_entry_->GetPreferredSize().height() +
      scroller_->contents()->GetPreferredSize().height();
  if (total_height > kMinimumWindowHeight)
    size.Enlarge(scroller_->GetScrollBarWidth(), 0);
  return size;
}

gfx::Size NotifierSettingsView::GetPreferredSize() {
  return GetMinimumSize();
}

bool NotifierSettingsView::OnKeyPressed(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_ESCAPE) {
    GetWidget()->Close();
    return true;
  }

  return scroller_->OnKeyPressed(event);
}

bool NotifierSettingsView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  return scroller_->OnMouseWheel(event);
}

void NotifierSettingsView::ButtonPressed(views::Button* sender,
                                         const ui::Event& event) {
  std::set<NotifierButton*>::iterator iter = buttons_.find(
      static_cast<NotifierButton*>(sender));
  DCHECK(iter != buttons_.end());

  (*iter)->SetChecked(!(*iter)->checked());
  if (delegate_)
    delegate_->SetNotifierEnabled((*iter)->notifier(), (*iter)->checked());
}

}  // namespace message_center
