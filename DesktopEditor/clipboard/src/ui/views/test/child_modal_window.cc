// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/test/child_modal_window.h"

#include "base/utf_string_conversions.h"  // ASCIIToUTF16
#include "ui/aura/window.h"
#include "ui/gfx/canvas.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/corewm/window_modality_controller.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {
namespace test {

namespace {

// Parent window size and position.
const int kWindowLeft = 170;
const int kWindowTop = 200;
const int kWindowWidth = 400;
const int kWindowHeight = 400;

// Parent window layout.
const int kButtonHeight = 35;
const int kTextfieldHeight = 35;

// Child window size.
const int kChildWindowWidth = 330;
const int kChildWindowHeight = 200;

// Child window layout.
const int kChildTextfieldLeft = 20;
const int kChildTextfieldTop = 50;
const int kChildTextfieldWidth = 290;
const int kChildTextfieldHeight = 35;

const SkColor kModalParentColor = SK_ColorWHITE;
const SkColor kChildColor = SK_ColorWHITE;

}  // namespace

void CreateChildModalParent(gfx::NativeView context) {
  Widget::CreateWindowWithContextAndBounds(
      new ChildModalParent(context),
      context,
      gfx::Rect(kWindowLeft, kWindowTop, kWindowWidth, kWindowHeight))->Show();
}


class ChildModalWindow : public WidgetDelegateView {
 public:
  ChildModalWindow();
  virtual ~ChildModalWindow();

 private:
  // Overridden from View:
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  // Overridden from WidgetDelegate:
  virtual View* GetContentsView() OVERRIDE;
  virtual string16 GetWindowTitle() const OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ChildModalWindow);
};

ChildModalWindow::ChildModalWindow() {
  Textfield* textfield = new Textfield;
  AddChildView(textfield);
  textfield->SetBounds(
      kChildTextfieldLeft, kChildTextfieldTop,
      kChildTextfieldWidth, kChildTextfieldHeight);
}

ChildModalWindow::~ChildModalWindow() {
}

void ChildModalWindow::OnPaint(gfx::Canvas* canvas) {
  canvas->FillRect(GetLocalBounds(), kChildColor);
}

gfx::Size ChildModalWindow::GetPreferredSize() {
  return gfx::Size(kChildWindowWidth, kChildWindowHeight);
}

View* ChildModalWindow::GetContentsView() {
  return this;
}

string16 ChildModalWindow::GetWindowTitle() const {
  return ASCIIToUTF16("Examples: Child Modal Window");
}

bool ChildModalWindow::CanResize() const {
  return false;
}

ui::ModalType ChildModalWindow::GetModalType() const {
  return ui::MODAL_TYPE_CHILD;
}

ChildModalParent::ChildModalParent(gfx::NativeView context)
    : button_(new LabelButton(this,
                              ASCIIToUTF16("Show/Hide Child Modal Window"))),
      textfield_(new Textfield),
      host_(new NativeViewHost),
      modal_parent_(NULL),
      child_(NULL) {
  Widget* widget = new Widget;
  Widget::InitParams params(Widget::InitParams::TYPE_CONTROL);
  params.context = context;
  widget->Init(params);
  widget->GetRootView()->set_background(
      Background::CreateSolidBackground(kModalParentColor));
  modal_parent_ = widget->GetNativeView();
  widget->GetNativeView()->SetName("ModalParent");
  AddChildView(button_);
  AddChildView(textfield_);
  AddChildView(host_);
}

ChildModalParent::~ChildModalParent() {
}

void ChildModalParent::ShowChild() {
  if (!child_)
    child_ = CreateChild();
  child_->Show();
}

gfx::NativeWindow ChildModalParent::GetModalParent() const {
  return modal_parent_;
}

gfx::NativeWindow ChildModalParent::GetChild() const {
  if (child_)
    return child_->GetNativeView();
  return NULL;
}

Widget* ChildModalParent::CreateChild() {
  Widget* child = Widget::CreateWindowWithParent(
      new ChildModalWindow, GetWidget()->GetNativeView());
  corewm::SetModalParent(child->GetNativeView(), GetModalParent());
  child->AddObserver(this);
  child->GetNativeView()->SetName("ChildModalWindow");
  return child;
}

View* ChildModalParent::GetContentsView() {
  return this;
}

string16 ChildModalParent::GetWindowTitle() const {
  return ASCIIToUTF16("Examples: Child Modal Parent");
}

bool ChildModalParent::CanResize() const {
  return false;
}

void ChildModalParent::DeleteDelegate() {
  if (child_) {
    child_->RemoveObserver(this);
    child_->Close();
    child_ = NULL;
  }

  delete this;
}

void ChildModalParent::Layout() {
  int running_y = y();
  button_->SetBounds(x(), running_y, width(), kButtonHeight);
  running_y += kButtonHeight;
  textfield_->SetBounds(x(), running_y, width(), kTextfieldHeight);
  running_y += kTextfieldHeight;
  host_->SetBounds(x(), running_y, width(), height() - running_y);
}

void ChildModalParent::ViewHierarchyChanged(bool is_add,
                                            View* parent,
                                            View* child) {
  if (is_add && child == this) {
    host_->Attach(modal_parent_);
    GetWidget()->GetNativeView()->SetName("Parent");
  }
}

void ChildModalParent::ButtonPressed(Button* sender,
                                     const ui::Event& event) {
  if (sender == button_) {
    if (!child_)
      child_ = CreateChild();
    if (child_->IsVisible())
      child_->Hide();
    else
      child_->Show();
  }
}

void ChildModalParent::OnWidgetDestroying(Widget* widget) {
  if (child_) {
    DCHECK_EQ(child_, widget);
    child_ = NULL;
  }
}

}  // namespace test
}  // namespace views
