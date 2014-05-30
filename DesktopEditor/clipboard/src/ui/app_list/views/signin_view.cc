// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/signin_view.h"

#include "ui/app_list/signin_delegate.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/link.h"
#include "ui/views/layout/grid_layout.h"

namespace {

const int kTopPadding = 10;
const int kBottomPadding = 10;
const int kLeftPadding = 20;
const int kRightPadding = 20;
const int kHeadingPadding = 30;
const int kButtonPadding = 100;

}  // namespace

namespace app_list {

SigninView::SigninView(SigninDelegate* delegate, int width)
    : delegate_(delegate) {
  if (!delegate_)
    return;

  views::GridLayout* layout = new views::GridLayout(this);
  layout->SetInsets(kTopPadding, kLeftPadding, kBottomPadding, kRightPadding);
  SetLayoutManager(layout);

  const int kTopSetId = 0;
  views::ColumnSet* columns = layout->AddColumnSet(kTopSetId);
  columns->AddColumn(views::GridLayout::FILL,
                     views::GridLayout::FILL,
                     1,
                     views::GridLayout::USE_PREF,
                     0,
                     0);

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();

  views::Label* heading = new views::Label(delegate_->GetSigninHeading());
  heading->SetFont(rb.GetFont(ui::ResourceBundle::LargeFont));
  heading->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  layout->StartRow(0, kTopSetId);
  layout->AddView(heading);

  views::Label* text = new views::Label(delegate_->GetSigninText());
  text->SetFont(rb.GetFont(ui::ResourceBundle::SmallFont));
  text->SetMultiLine(true);
  text->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout->StartRowWithPadding(0, kTopSetId, 0, kHeadingPadding);
  layout->AddView(text);

  views::LabelButton* signin_button = new views::LabelButton(
      this,
      delegate_->GetSigninButtonText());
  signin_button->SetStyle(views::Button::STYLE_NATIVE_TEXTBUTTON);
  layout->StartRowWithPadding(0, kTopSetId, 0, kButtonPadding);
  layout->AddView(signin_button);

  const int kBottomSetId = 1;
  columns = layout->AddColumnSet(kBottomSetId);
  columns->AddColumn(views::GridLayout::FILL,
                     views::GridLayout::FILL,
                     0,
                     views::GridLayout::USE_PREF,
                     0,
                     0);
  columns->AddColumn(views::GridLayout::FILL,
                     views::GridLayout::FILL,
                     1,
                     views::GridLayout::USE_PREF,
                     0,
                     0);

  layout->StartRow(1, kBottomSetId);
  learn_more_link_ = new views::Link(delegate_->GetLearnMoreLinkText());
  learn_more_link_->set_listener(this);
  layout->AddView(learn_more_link_,
                  1,
                  1,
                  views::GridLayout::LEADING,
                  views::GridLayout::TRAILING);

  settings_link_ = new views::Link(delegate_->GetSettingsLinkText());
  settings_link_->set_listener(this);
  settings_link_->SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  layout->AddView(settings_link_,
                  1,
                  1,
                  views::GridLayout::TRAILING,
                  views::GridLayout::TRAILING);
}

SigninView::~SigninView() {
}

void SigninView::ButtonPressed(views::Button* sender, const ui::Event& event) {
  if (delegate_)
    delegate_->ShowSignin();
}

void SigninView::LinkClicked(views::Link* source, int event_flags) {
  if (delegate_) {
    if (source == learn_more_link_)
      delegate_->OpenLearnMore();
    else if (source == settings_link_)
      delegate_->OpenSettings();
    else
      NOTREACHED();
  }
}

}  // namespace app_list
