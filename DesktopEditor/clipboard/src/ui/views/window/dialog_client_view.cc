// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/window/dialog_client_view.h"

#include <algorithm>

#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/layout/layout_constants.h"
#include "ui/views/widget/widget.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {

namespace {

// The group used by the buttons.  This name is chosen voluntarily big not to
// conflict with other groups that could be in the dialog content.
const int kButtonGroup = 6666;

// Update |button|'s text and enabled state according to |delegate|'s state.
void UpdateButton(LabelButton* button,
                  DialogDelegate* dialog,
                  ui::DialogButton type) {
  button->SetText(dialog->GetDialogButtonLabel(type));
  button->SetEnabled(dialog->IsDialogButtonEnabled(type));
}

// Returns true if the given view should be shown (i.e. exists and is
// visible).
bool ShouldShow(View* view) {
  return view && view->visible();
}

}  // namespace

///////////////////////////////////////////////////////////////////////////////
// DialogClientView, public:

DialogClientView::DialogClientView(Widget* owner, View* contents_view)
    : ClientView(owner, contents_view),
      ok_button_(NULL),
      cancel_button_(NULL),
      default_button_(NULL),
      focus_manager_(NULL),
      extra_view_(NULL),
      footnote_view_(NULL),
      notified_delegate_(false) {
  // When using the new style, the background color is set on the bubble frame,
  // so a transparent background is fine.
  if (!DialogDelegate::UseNewStyle()) {
    const SkColor color = owner->GetNativeTheme()->GetSystemColor(
        ui::NativeTheme::kColorId_DialogBackground);
    set_background(views::Background::CreateSolidBackground(color));
  }
}

DialogClientView::~DialogClientView() {
  if (focus_manager_)
    focus_manager_->RemoveFocusChangeListener(this);
  focus_manager_ = NULL;
}

void DialogClientView::AcceptWindow() {
  // Only notify the delegate once. See |notified_delegate_|'s comment.
  if (!notified_delegate_ && GetDialogDelegate()->Accept(false)) {
    notified_delegate_ = true;
    Close();
  }
}

void DialogClientView::CancelWindow() {
  // Call the standard Close handler, which checks with the delegate before
  // proceeding. This checking _isn't_ done here, but in the WM_CLOSE handler,
  // so that the close box on the window also shares this code path.
  Close();
}

void DialogClientView::UpdateDialogButtons() {
  DialogDelegate* dialog = GetDialogDelegate();
  const int buttons = dialog->GetDialogButtons();
  ui::Accelerator escape(ui::VKEY_ESCAPE, ui::EF_NONE);

  if (buttons & ui::DIALOG_BUTTON_OK) {
    if (!ok_button_) {
      ok_button_ = CreateDialogButton(ui::DIALOG_BUTTON_OK);
      if (buttons & ui::DIALOG_BUTTON_CANCEL)
        ok_button_->AddAccelerator(escape);
      AddChildView(ok_button_);
    }

    UpdateButton(ok_button_, dialog, ui::DIALOG_BUTTON_OK);
  } else if (ok_button_) {
    delete ok_button_;
    ok_button_ = NULL;
  }

  if (buttons & ui::DIALOG_BUTTON_CANCEL) {
    if (!cancel_button_) {
      cancel_button_ = CreateDialogButton(ui::DIALOG_BUTTON_CANCEL);
      cancel_button_->AddAccelerator(escape);
      AddChildView(cancel_button_);
    }

    UpdateButton(cancel_button_, dialog, ui::DIALOG_BUTTON_CANCEL);
  } else if (cancel_button_) {
    delete cancel_button_;
    cancel_button_ = NULL;
  }

  // Use the escape key to close the window if there are no dialog buttons.
  if (!has_dialog_buttons())
    AddAccelerator(escape);
  else
    ResetAccelerators();
}

///////////////////////////////////////////////////////////////////////////////
// DialogClientView, ClientView overrides:

bool DialogClientView::CanClose() {
  if (notified_delegate_)
    return true;

  DialogDelegate* dialog = GetDialogDelegate();
  int buttons = dialog->GetDialogButtons();
  bool close = true;
  if ((buttons & ui::DIALOG_BUTTON_CANCEL) ||
      (buttons == ui::DIALOG_BUTTON_NONE))
    close = dialog->Cancel();
  else if (buttons & ui::DIALOG_BUTTON_OK)
    close = dialog->Accept(true);
  notified_delegate_ = close;
  return close;
}

DialogClientView* DialogClientView::AsDialogClientView() {
  return this;
}

const DialogClientView* DialogClientView::AsDialogClientView() const {
  return this;
}

void DialogClientView::OnWillChangeFocus(View* focused_before,
                                         View* focused_now) {
  // New style dialogs do not move the default button with the focus.
  // TODO(msw|wittman): Remove this functionality once the new style has landed.
  if (DialogDelegate::UseNewStyle())
    return;

  // Make the newly focused button default or restore the dialog's default.
  const int default_button = GetDialogDelegate()->GetDefaultDialogButton();
  LabelButton* new_default_button = NULL;
  if (focused_now &&
      (focused_now->GetClassName() == LabelButton::kViewClassName)) {
    new_default_button = static_cast<LabelButton*>(focused_now);
  } else if (default_button == ui::DIALOG_BUTTON_OK && ok_button_) {
    new_default_button = ok_button_;
  } else if (default_button == ui::DIALOG_BUTTON_CANCEL && cancel_button_) {
    new_default_button = cancel_button_;
  }

  if (default_button_ && default_button_ != new_default_button)
    default_button_->SetIsDefault(false);
  default_button_ = new_default_button;
  if (default_button_ && !default_button_->is_default())
    default_button_->SetIsDefault(true);
}

void DialogClientView::OnDidChangeFocus(View* focused_before,
                                        View* focused_now) {
}

////////////////////////////////////////////////////////////////////////////////
// DialogClientView, View overrides:

gfx::Size DialogClientView::GetPreferredSize() {
  // Initialize the size to fit the buttons and extra view row.
  gfx::Size size(
      (ok_button_ ? ok_button_->GetPreferredSize().width() : 0) +
      (cancel_button_ ? cancel_button_->GetPreferredSize().width() : 0) +
      (cancel_button_ && ok_button_ ? kRelatedButtonHSpacing : 0) +
      (ShouldShow(extra_view_) ? extra_view_->GetPreferredSize().width() : 0) +
      (ShouldShow(extra_view_) && has_dialog_buttons() ?
           kRelatedButtonHSpacing : 0),
      0);

  int buttons_height = GetButtonsAndExtraViewRowHeight();
  if (buttons_height != 0) {
    size.Enlarge(0, buttons_height + kRelatedControlVerticalSpacing);
    // Inset the buttons and extra view.
    const gfx::Insets insets = GetButtonRowInsets();
    size.Enlarge(insets.width(), insets.height());
  }

  // Increase the size as needed to fit the contents view.
  // NOTE: The contents view is not inset on the top or side client view edges.
  gfx::Size contents_size = contents_view()->GetPreferredSize();
  size.Enlarge(0, contents_size.height());
  size.set_width(std::max(size.width(), contents_size.width()));

  // Increase the size as needed to fit the footnote view.
  if (ShouldShow(footnote_view_)) {
    gfx::Size footnote_size = footnote_view_->GetPreferredSize();
    if (!footnote_size.IsEmpty())
      size.set_width(std::max(size.width(), footnote_size.width()));

    int footnote_height = footnote_view_->GetHeightForWidth(size.width());
    size.Enlarge(0, footnote_height);
  }

  return size;
}

void DialogClientView::Layout() {
  gfx::Rect bounds = GetContentsBounds();

  // Layout the footnote view.
  if (ShouldShow(footnote_view_)) {
    const int height = footnote_view_->GetHeightForWidth(bounds.width());
    footnote_view_->SetBounds(bounds.x(), bounds.bottom() - height,
                              bounds.width(), height);
    if (height != 0)
      bounds.Inset(0, 0, 0, height);
  }

  // Layout the row containing the buttons and the extra view.
  if (has_dialog_buttons() || extra_view_) {
    bounds.Inset(GetButtonRowInsets());
    const int height = GetButtonsAndExtraViewRowHeight();
    gfx::Rect row_bounds(bounds.x(), bounds.bottom() - height,
                         bounds.width(), height);
    if (cancel_button_) {
      const gfx::Size size = cancel_button_->GetPreferredSize();
      row_bounds.set_width(row_bounds.width() - size.width());
      cancel_button_->SetBounds(row_bounds.right(), row_bounds.y(),
                                size.width(), height);
      row_bounds.set_width(row_bounds.width() - kRelatedButtonHSpacing);
    }
    if (ok_button_) {
      const gfx::Size size = ok_button_->GetPreferredSize();
      row_bounds.set_width(row_bounds.width() - size.width());
      ok_button_->SetBounds(row_bounds.right(), row_bounds.y(),
                            size.width(), height);
      row_bounds.set_width(row_bounds.width() - kRelatedButtonHSpacing);
    }
    if (extra_view_) {
      row_bounds.set_width(std::min(row_bounds.width(),
          extra_view_->GetPreferredSize().width()));
      extra_view_->SetBoundsRect(row_bounds);
    }

    if (height > 0)
      bounds.Inset(0, 0, 0, height + kRelatedControlVerticalSpacing);
  }

  // Layout the contents view to the top and side edges of the contents bounds.
  // NOTE: The local insets do not apply to the contents view sides or top.
  const gfx::Rect contents_bounds = GetContentsBounds();
  contents_view()->SetBounds(contents_bounds.x(), contents_bounds.y(),
      contents_bounds.width(), bounds.bottom() - contents_bounds.y());
}

bool DialogClientView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  DCHECK_EQ(accelerator.key_code(), ui::VKEY_ESCAPE);
  Close();
  return true;
}

void DialogClientView::ViewHierarchyChanged(bool is_add,
                                            View* parent,
                                            View* child) {
  ClientView::ViewHierarchyChanged(is_add, parent, child);
  if (is_add && child == this) {
    focus_manager_ = GetFocusManager();
    if (focus_manager_)
      GetFocusManager()->AddFocusChangeListener(this);

    UpdateDialogButtons();
    CreateExtraView();
    CreateFootnoteView();
  }
}

////////////////////////////////////////////////////////////////////////////////
// DialogClientView, ButtonListener implementation:

void DialogClientView::ButtonPressed(Button* sender, const ui::Event& event) {
  // Check for a valid delegate to avoid handling events after destruction.
  if (!GetDialogDelegate())
    return;

  if (sender == ok_button_)
    AcceptWindow();
  else if (sender == cancel_button_)
    CancelWindow();
  else
    NOTREACHED();
}

////////////////////////////////////////////////////////////////////////////////
// DialogClientView, protected:

DialogClientView::DialogClientView(View* contents_view)
    : ClientView(NULL, contents_view),
      ok_button_(NULL),
      cancel_button_(NULL),
      default_button_(NULL),
      focus_manager_(NULL),
      extra_view_(NULL),
      footnote_view_(NULL),
      notified_delegate_(false) {}

DialogDelegate* DialogClientView::GetDialogDelegate() const {
  return GetWidget()->widget_delegate()->AsDialogDelegate();
}

void DialogClientView::CreateExtraView() {
  if (extra_view_)
    return;

  extra_view_ = GetDialogDelegate()->CreateExtraView();
  if (extra_view_) {
    extra_view_->SetGroup(kButtonGroup);
    AddChildView(extra_view_);
  }
}

void DialogClientView::CreateFootnoteView() {
  if (footnote_view_)
    return;

  footnote_view_ = GetDialogDelegate()->CreateFootnoteView();
  if (footnote_view_)
    AddChildView(footnote_view_);
}

////////////////////////////////////////////////////////////////////////////////
// DialogClientView, private:

LabelButton* DialogClientView::CreateDialogButton(ui::DialogButton type) {
  const string16 title = GetDialogDelegate()->GetDialogButtonLabel(type);
  LabelButton* button = new LabelButton(this, title);
  button->SetStyle(Button::STYLE_NATIVE_TEXTBUTTON);
  button->set_focusable(true);

  const int kDialogMinButtonWidth = 75;
  button->set_min_size(gfx::Size(kDialogMinButtonWidth, 0));
  button->SetGroup(kButtonGroup);
  if (type == GetDialogDelegate()->GetDefaultDialogButton()) {
    default_button_ = button;
    button->SetIsDefault(true);
  }
  return button;
}

int DialogClientView::GetButtonsAndExtraViewRowHeight() const {
  int extra_view_height = ShouldShow(extra_view_) ?
      extra_view_->GetPreferredSize().height() : 0;
  int buttons_height = std::max(
      ok_button_ ? ok_button_->GetPreferredSize().height() : 0,
      cancel_button_ ? cancel_button_->GetPreferredSize().height() : 0);
  return std::max(extra_view_height, buttons_height);
}

gfx::Insets DialogClientView::GetButtonRowInsets() const {
  if (GetButtonsAndExtraViewRowHeight() == 0)
    return gfx::Insets();

  // NOTE: The insets only apply to the buttons, extra view, and footnote view.
  return DialogDelegate::UseNewStyle() ?
      gfx::Insets(0, kButtonHEdgeMarginNew,
                  kButtonVEdgeMarginNew, kButtonHEdgeMarginNew) :
      gfx::Insets(0, kButtonHEdgeMargin,
                  kButtonVEdgeMargin, kButtonHEdgeMargin);
}

void DialogClientView::Close() {
  GetWidget()->Close();
  GetDialogDelegate()->OnClose();
}

}  // namespace views
